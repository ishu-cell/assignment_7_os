#include "types.h"
#include "user.h"

#define P 5
#define RES 3
#define CYCLES 3

/* Resource order: Printer=0, Scanner=1, Disk=2.
 * Capacities: 2, 1, 2. Every process needs exactly two resource types.
 * Acquiring resources in increasing global order removes circular wait. */
int sems[RES];
int need[P][RES] = {
  {1,1,0}, {1,0,1}, {0,1,1}, {1,1,0}, {1,0,1}
};

void delay(void){ volatile int i; for(i=0;i<500000;i++); }

void worker(int id){
  int c,r;
  for(c=0;c<CYCLES;c++){
    printf(1,"P%d cycle %d requesting:",id,c+1);
    for(r=0;r<RES;r++) if(need[id][r]) printf(1," R%d",r);
    printf(1,"\n");
    /* Global resource ordering: always acquire R0, then R1, then R2. */
    for(r=0;r<RES;r++) if(need[id][r]){
      printf(1,"P%d requests R%d\n",id,r);
      sem_wait(sems[r]);
      printf(1,"P%d granted R%d\n",id,r);
    }
    printf(1,"P%d STARTS WORK (cycle %d)\n",id,c+1); delay();
    for(r=RES-1;r>=0;r--) if(need[id][r]){
      sem_post(sems[r]); printf(1,"P%d released R%d\n",id,r);
    }
    printf(1,"P%d completed cycle %d\n",id,c+1);
  }
  printf(1,"P%d finished all cycles\n",id); exit();
}

int main(void){
  int i;
  printf(1,"=== Q4: COMBINED SYNCHRONIZATION + DEADLOCK AVOIDANCE ===\n");
  printf(1,"Strategy: global resource ordering R0 < R1 < R2. Capacities: 2,1,2.\n");
  sems[0]=sem_init(2); sems[1]=sem_init(1); sems[2]=sem_init(2);
  for(i=0;i<P;i++) if(fork()==0) worker(i);
  for(i=0;i<P;i++) wait();
  printf(1,"All 5 processes completed %d cycles without deadlock.\n",CYCLES);
  sem_destroy(sems[0]); sem_destroy(sems[1]); sem_destroy(sems[2]);
  exit();
}
