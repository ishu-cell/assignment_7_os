#include "types.h"
#include "stat.h"
#include "user.h"
#define PGSIZE 4096
volatile int counter = 0;
void worker(void *arg){ int i; for(i=0;i<100000;i++) counter++; exit(); }
int main(void){ int i; void *s[3]; printf(1,"Q1: clone threads and shared counter (race expected)\n"); for(i=0;i<3;i++){ s[i]=malloc(PGSIZE); if(clone(worker,0,(char*)s[i]+PGSIZE)<0) printf(1,"clone failed\n"); } for(i=0;i<3;i++) join(); printf(1,"Final counter = %d (target 300000; race may lower it)\n",counter); for(i=0;i<3;i++) free(s[i]); exit(); }
