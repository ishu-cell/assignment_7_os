#include "types.h"
#include "user.h"

int l1, l2;

void delay(void){ volatile int i; for(i=0;i<2500000;i++); }

void bad_worker(int id){
  if(id==0){
    sem_wait(l1); printf(1,"[%d] Process A acquired Lock1\n",uptime());
    delay(); printf(1,"[%d] Process A waiting for Lock2\n",uptime()); sem_wait(l2);
    printf(1,"Process A acquired Lock2 (should not happen in bad run)\n");
    sem_post(l2); sem_post(l1);
  } else {
    sem_wait(l2); printf(1,"[%d] Process B acquired Lock2\n",uptime());
    delay(); printf(1,"[%d] Process B waiting for Lock1\n",uptime()); sem_wait(l1);
    printf(1,"Process B acquired Lock1 (should not happen in bad run)\n");
    sem_post(l1); sem_post(l2);
  }
  exit();
}

void fixed_worker(int id){
  printf(1,"[%d] Process %c requesting Lock1\n",uptime(),id==0?'A':'B'); sem_wait(l1);
  printf(1,"[%d] Process %c acquired Lock1\n",uptime(),id==0?'A':'B');
  delay();
  printf(1,"[%d] Process %c requesting Lock2\n",uptime(),id==0?'A':'B'); sem_wait(l2);
  printf(1,"[%d] Process %c acquired Lock2\n",uptime(),id==0?'A':'B');
  printf(1,"Process %c completed critical section\n",id==0?'A':'B');
  sem_post(l2); sem_post(l1); exit();
}

void run_bad(void){
  int a,b;
  printf(1,"\n=== Q3 BAD ORDER: deliberate deadlock ===\n");
  l1=sem_init(1); l2=sem_init(1);
  a=fork(); if(a==0) bad_worker(0);
  b=fork(); if(b==0) bad_worker(1);
  sleep(2);
  printf(1,"[timeout] Both processes are now waiting: circular wait demonstrated.\n");
  printf(1,"[timeout] Killing the two stuck children so the test can continue.\n");
  kill(a); kill(b); wait(); wait(); sem_destroy(l1); sem_destroy(l2);
}

void run_fixed(void){
  int a,b;
  printf(1,"\n=== Q3 FIXED ORDER: Lock1 -> Lock2 for everyone ===\n");
  l1=sem_init(1); l2=sem_init(1);
  a=fork(); if(a==0) fixed_worker(0);
  b=fork(); if(b==0) fixed_worker(1);
  wait(); wait();
  printf(1,"All processes completed: no deadlock.\n");
  sem_destroy(l1); sem_destroy(l2);
}

int main(void){ run_bad(); run_fixed(); exit(); }
