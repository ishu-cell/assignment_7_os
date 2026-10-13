#include "types.h"
#include "stat.h"
#include "user.h"

#define NPHIL 5
#define CYCLES 5

typedef struct {
  int state[NPHIL];
} dining_shared;

#define THINKING 0
#define HUNGRY 1
#define EATING 2

static void
delay(void)
{
  volatile int x = 0;
  int i;
  for(i = 0; i < 20000; i++)
    x += i;
}

static void
philosopher(volatile dining_shared *s, int forks[NPHIL], int id)
{
  int left = id;
  int right = (id + 1) % NPHIL;
  int first, second;
  int cycle;

  // Resource ordering: always acquire the lower-numbered fork first.
  // This removes the circular-wait condition and therefore prevents deadlock.
  if(left < right){
    first = left;
    second = right;
  } else {
    first = right;
    second = left;
  }

  for(cycle = 0; cycle < CYCLES; cycle++){
    s->state[id] = THINKING;
    printf(1, "Philosopher %d: THINKING\n", id);
    delay();

    s->state[id] = HUNGRY;
    printf(1, "Philosopher %d: HUNGRY\n", id);

    sem_wait(forks[first]);
    sem_wait(forks[second]);

    s->state[id] = EATING;
    printf(1, "Philosopher %d: EATING\n", id);
    delay();

    sem_post(forks[second]);
    sem_post(forks[first]);

    s->state[id] = THINKING;
    printf(1, "Philosopher %d: THINKING\n", id);
  }

  printf(1, "Philosopher %d completed all %d cycles\n", id, CYCLES);
}

int
main(void)
{
  volatile dining_shared *s;
  int forks[NPHIL];
  int i, pid;

  s = (dining_shared*)shm_get();
  if(s == 0){
    printf(1, "dining: shm_get failed\n");
    exit();
  }

  for(i = 0; i < NPHIL; i++){
    s->state[i] = THINKING;
    forks[i] = sem_init(1);
    if(forks[i] < 0){
      printf(1, "dining: fork semaphore initialization failed\n");
      exit();
    }
  }

  // Fork exactly five philosopher processes.
  for(i = 0; i < NPHIL; i++){
    pid = fork();
    if(pid == 0){
      philosopher(s, forks, i);
      exit();
    }
  }

  for(i = 0; i < NPHIL; i++)
    wait();

  printf(1, "Dining philosophers complete: all 5 philosophers finished.\n");

  for(i = 0; i < NPHIL; i++)
    sem_destroy(forks[i]);

  exit();
}
