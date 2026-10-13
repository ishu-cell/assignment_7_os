#include "types.h"
#include "stat.h"
#include "user.h"
#define N 3
#define R 2
int allocm[N][R]={{1,0},{0,1},{0,0}};
int req[N][R]={{0,1},{1,0},{0,0}};
int avail[R]={0,0};
int detect_deadlock(void){ int work[R], finish[N]={0},i,j,progress,dead=0; for(j=0;j<R;j++) work[j]=avail[j]; do{ progress=0; for(i=0;i<N;i++) if(!finish[i] && req[i][0]<=work[0] && req[i][1]<=work[1]){ for(j=0;j<R;j++) work[j]+=allocm[i][j]; finish[i]=1; progress=1; printf(1,"P%d can finish; Work=(%d,%d)\n",i,work[0],work[1]); } }while(progress); for(i=0;i<N;i++) if(!finish[i]){ printf(1,"Deadlocked process: P%d\n",i); dead++; } return dead; }
int main(void){ printf(1,"Q3 deadlock detection: circular wait example\n"); if(detect_deadlock()) printf(1,"Deadlock detected\n"); else printf(1,"No deadlock\n"); printf(1,"Q3 safe example after releasing one resource\n"); avail[0]=1; avail[1]=0; if(detect_deadlock()) printf(1,"Deadlock detected\n"); else printf(1,"No deadlock\n"); printf(1,"Banker safety check uses Need=Max-Allocation and grants only if safe.\n"); exit(); }
