#include "types.h"
#include "stat.h"
#include "user.h"

#define BUFFER_SIZE 5
#define ITEMS 20

typedef struct {
  int buffer[BUFFER_SIZE];
  int in;
  int out;
  int produced;
  int consumed;
} shared_buffer;

static void
delay(void)
{
  volatile int x = 0;
  int i;
  for(i = 0; i < 30000; i++)
    x += i;
}

int
main(void)
{
  shared_buffer *b;
  int empty, full, mutex;
  int pid;
  int i, item;

  b = (shared_buffer*)shm_get();
  if(b == 0){
    printf(1, "prodcons: shm_get failed\n");
    exit();
  }

  b->in = 0;
  b->out = 0;
  b->produced = 0;
  b->consumed = 0;

  empty = sem_init(BUFFER_SIZE);
  full = sem_init(0);
  mutex = sem_init(1);
  if(empty < 0 || full < 0 || mutex < 0){
    printf(1, "prodcons: semaphore initialization failed\n");
    exit();
  }

  pid = fork();
  if(pid < 0){
    printf(1, "prodcons: fork failed\n");
    exit();
  }

  if(pid == 0){
    // Consumer: full protects the empty-buffer case.
    for(i = 0; i < ITEMS; i++){
      printf(1, "Consumer waiting for an item (full semaphore)\n");
      sem_wait(full);
      sem_wait(mutex);

      item = b->buffer[b->out];
      b->out = (b->out + 1) % BUFFER_SIZE;
      b->consumed++;
      printf(1, "Consumer pid=%d removed %d (count=%d)\n",
             getpid(), item, b->consumed);

      sem_post(mutex);
      sem_post(empty);
      delay();
    }
    exit();
  }

  // Give the consumer a chance to demonstrate blocking on an empty buffer.
  sleep(1);

  // Parent is the producer. empty blocks it when all 5 slots are full.
  for(i = 1; i <= ITEMS; i++){
    printf(1, "Producer waiting for an empty slot (empty semaphore)\n");
    sem_wait(empty);
    sem_wait(mutex);

    b->buffer[b->in] = i;
    b->in = (b->in + 1) % BUFFER_SIZE;
    b->produced++;
    printf(1, "Producer pid=%d inserted %d (count=%d)\n",
           getpid(), i, b->produced);

    sem_post(mutex);
    sem_post(full);
  }

  wait();
  printf(1, "Producer-consumer complete: produced=%d consumed=%d\n",
         b->produced, b->consumed);

  sem_destroy(empty);
  sem_destroy(full);
  sem_destroy(mutex);
  exit();
}
