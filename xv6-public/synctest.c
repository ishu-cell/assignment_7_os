#include "types.h"
#include "stat.h"
#include "user.h"
#define PGSIZE 4096
volatile int counter=0; int mutex;
void worker(void *arg){ int i; for(i=0;i<100000;i++){ sem_wait(mutex); counter++; sem_post(mutex); } exit(); }
int main(void){ int i; void *s[3]; mutex=sem_init(1); for(i=0;i<3;i++){ s[i]=malloc(PGSIZE); clone(worker,0,(char*)s[i]+PGSIZE); } for(i=0;i<3;i++) join(); printf(1,"Synchronized counter = %d (expected 300000)\n",counter); sem_destroy(mutex); for(i=0;i<3;i++) free(s[i]);
 { int empty=sem_init(5), full=sem_init(0), lock=sem_init(1), buf[5], in=0,out=0; for(i=1;i<=10;i++){ sem_wait(empty); sem_wait(lock); buf[in]=i; in=(in+1)%5; printf(1,"Produced %d\n",i); sem_post(lock); sem_post(full); sem_wait(full); sem_wait(lock); printf(1,"Consumed %d\n",buf[out]); out=(out+1)%5; sem_post(lock); sem_post(empty); } sem_destroy(empty); sem_destroy(full); sem_destroy(lock); }
 exit(); }
