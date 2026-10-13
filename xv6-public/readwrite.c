#include "types.h"
#include "stat.h"
#include "user.h"

#define READERS 3
#define WRITERS 2
#define READ_ROUNDS 5
#define WRITE_ROUNDS 4

typedef struct {
  int shared_data;
  int read_count;
  int active_readers;
  int active_writer;
} rw_shared;

static void
delay(int n)
{
  volatile int x = 0;
  int i;
  for(i = 0; i < n; i++)
    x += i;
}

static void
reader(volatile rw_shared *s, int mutex, int rw_mutex, int turnstile, int id)
{
  int i, value;

  for(i = 0; i < READ_ROUNDS; i++){
    // Turnstile prevents a stream of new readers from indefinitely passing
    // a writer that is waiting. Existing readers may still finish together.
    sem_wait(turnstile);
    sem_post(turnstile);

    sem_wait(mutex);
    s->read_count++;
    if(s->read_count == 1)
      sem_wait(rw_mutex);
    s->active_readers++;
    sem_post(mutex);

    value = s->shared_data;
    printf(1, "t=%d Reader %d pid=%d READ data=%d active_readers=%d\n",
           uptime(), id, getpid(), value, s->active_readers);
    delay(10000);

    sem_wait(mutex);
    s->active_readers--;
    s->read_count--;
    if(s->read_count == 0)
      sem_post(rw_mutex);
    sem_post(mutex);

    delay(5000);
  }
}

static void
writer(volatile rw_shared *s, int rw_mutex, int turnstile, int id)
{
  int i;

  for(i = 0; i < WRITE_ROUNDS; i++){
    // A writer closes the turnstile so no new reader can overtake it.
    sem_wait(turnstile);
    sem_wait(rw_mutex);

    s->active_writer = 1;
    s->shared_data++;
    printf(1, "t=%d Writer %d pid=%d WRITE data=%d\n",
           uptime(), id, getpid(), s->shared_data);
    delay(15000);
    s->active_writer = 0;

    sem_post(rw_mutex);
    sem_post(turnstile);
    delay(7000);
  }
}

int
main(void)
{
  volatile rw_shared *s;
  int mutex, rw_mutex, turnstile;
  int i, pid;

  s = (rw_shared*)shm_get();
  if(s == 0){
    printf(1, "readwrite: shm_get failed\n");
    exit();
  }
  s->shared_data = 0;
  s->read_count = 0;
  s->active_readers = 0;
  s->active_writer = 0;

  mutex = sem_init(1);
  rw_mutex = sem_init(1);
  turnstile = sem_init(1);
  if(mutex < 0 || rw_mutex < 0 || turnstile < 0){
    printf(1, "readwrite: semaphore initialization failed\n");
    exit();
  }

  // Spawn readers.
  for(i = 0; i < READERS; i++){
    pid = fork();
    if(pid == 0){
      reader(s, mutex, rw_mutex, turnstile, i);
      exit();
    }
  }

  // Spawn writers.
  for(i = 0; i < WRITERS; i++){
    pid = fork();
    if(pid == 0){
      writer(s, rw_mutex, turnstile, i);
      exit();
    }
  }

  for(i = 0; i < READERS + WRITERS; i++)
    wait();

  printf(1, "Readers-writers complete: final shared_data=%d expected=%d\n",
         s->shared_data, WRITERS * WRITE_ROUNDS);

  sem_destroy(mutex);
  sem_destroy(rw_mutex);
  sem_destroy(turnstile);
  exit();
}
