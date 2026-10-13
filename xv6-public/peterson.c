#include "types.h"
#include "stat.h"
#include "user.h"

#define ITERATIONS 10

struct peterson_shared {
  int flag[2];
  int turn;
  int shared_counter;
  int in_cs;
};

static void
delay(int n)
{
  volatile int x = 0;
  int i;
  for(i = 0; i < n; i++)
    x += i;
}

static void
peterson_cs(volatile struct peterson_shared *s, int id)
{
  int other = 1 - id;
  int value;

  s->flag[id] = 1;
  s->turn = other;

  while(s->flag[other] && s->turn == other)
    ;

  // Peterson is the actual mutual-exclusion mechanism here: no xv6
  // semaphore/lock protects this critical section.
  if(s->in_cs != 0)
    printf(1, "ERROR: mutual exclusion violated!\n");
  s->in_cs = 1;

  value = s->shared_counter + 1;
  delay(10000);
  s->shared_counter = value;
  printf(1, "Process %d in CS, counter = %d\n", id, value);

  s->in_cs = 0;
  s->flag[id] = 0;

  delay(5000);
}

int
main(void)
{
  volatile struct peterson_shared *s;
  int pid;
  int id;
  int i;

  s = (struct peterson_shared*)shm_get();
  if(s == 0){
    printf(1, "peterson: shm_get failed\n");
    exit();
  }

  // Initialize before fork so the child inherits the same mapping.
  s->flag[0] = 0;
  s->flag[1] = 0;
  s->turn = 0;
  s->shared_counter = 0;
  s->in_cs = 0;

  pid = fork();
  if(pid < 0){
    printf(1, "peterson: fork failed\n");
    exit();
  }

  id = (pid == 0) ? 1 : 0;

  for(i = 0; i < ITERATIONS; i++)
    peterson_cs(s, id);

  if(id == 0){
    wait();
    printf(1, "Peterson final counter = %d (expected %d)\n",
           s->shared_counter, 2 * ITERATIONS);
  } else {
    exit();
  }

  exit();
}
