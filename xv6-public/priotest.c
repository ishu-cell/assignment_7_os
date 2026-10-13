#include "types.h"
#include "stat.h"
#include "user.h"
int main(void){ int i,pid; for(i=0;i<4;i++){ pid=fork(); if(pid==0){ int k,pr=20-i*6; set_priority(getpid(),pr); for(k=0;k<10000000;k++); printf(1,"child priority %d completed, pid %d\n",get_priority(getpid()),getpid()); exit(); } } for(i=0;i<4;i++) wait(); exit(); }
