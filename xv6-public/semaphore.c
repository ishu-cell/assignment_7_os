// Lightweight counting semaphores for the synchronization assignment.
// Each semaphore has its own spinlock. sem_wait() sleeps when the count is
// zero, and sem_post() wakes sleepers. The semaphore itself, rather than
// an xv6 user-space lock, protects the critical section in the user tests.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"

struct ksem {
  int used;
  int value;
  struct spinlock lock;
};

static struct {
  struct spinlock lock;
  struct ksem sem[NSEM];
} semtable;

void
seminit(void)
{
  int i;
  initlock(&semtable.lock, "semtable");
  for(i = 0; i < NSEM; i++){
    semtable.sem[i].used = 0;
    semtable.sem[i].value = 0;
    initlock(&semtable.sem[i].lock, "semaphore");
  }
}

int
sem_alloc(int value)
{
  int i;

  if(value < 0)
    return -1;

  acquire(&semtable.lock);
  for(i = 0; i < NSEM; i++){
    if(!semtable.sem[i].used){
      semtable.sem[i].used = 1;
      semtable.sem[i].value = value;
      release(&semtable.lock);
      return i;
    }
  }
  release(&semtable.lock);
  return -1;
}

int
sem_free(int id)
{
  if(id < 0 || id >= NSEM)
    return -1;

  acquire(&semtable.lock);
  if(!semtable.sem[id].used){
    release(&semtable.lock);
    return -1;
  }

  acquire(&semtable.sem[id].lock);
  semtable.sem[id].used = 0;
  semtable.sem[id].value = 0;
  wakeup(&semtable.sem[id]);
  release(&semtable.sem[id].lock);
  release(&semtable.lock);
  return 0;
}

int
sem_down(int id)
{
  struct ksem *s;

  if(id < 0 || id >= NSEM)
    return -1;
  s = &semtable.sem[id];
  if(!s->used)
    return -1;

  acquire(&s->lock);
  while(s->value == 0){
    if(myproc()->killed){
      release(&s->lock);
      return -1;
    }
    sleep(s, &s->lock);
  }
  s->value--;
  release(&s->lock);
  return 0;
}

int
sem_up(int id)
{
  struct ksem *s;

  if(id < 0 || id >= NSEM)
    return -1;
  s = &semtable.sem[id];
  if(!s->used)
    return -1;

  acquire(&s->lock);
  s->value++;
  wakeup(s);
  release(&s->lock);
  return 0;
}
