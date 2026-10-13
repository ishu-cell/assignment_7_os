#include "types.h"
#include "user.h"

#define N 5
#define M 3

int Allocation[N][M] = {
  {0, 1, 0},
  {2, 0, 0},
  {3, 0, 2},
  {2, 1, 1},
  {0, 0, 2}
};
int Max[N][M] = {
  {7, 5, 3},
  {3, 2, 2},
  {9, 0, 2},
  {2, 2, 2},
  {4, 3, 3}
};
int Available[M] = {3, 3, 2};

void print_state(void){
  int i,j;
  printf(1, "Allocation / Max / Need:\n");
  for(i=0;i<N;i++){
    printf(1,"P%d  A=",i);
    for(j=0;j<M;j++) printf(1,"%d ",Allocation[i][j]);
    printf(1," M=");
    for(j=0;j<M;j++) printf(1,"%d ",Max[i][j]);
    printf(1," N=");
    for(j=0;j<M;j++) printf(1,"%d ",Max[i][j]-Allocation[i][j]);
    printf(1,"\n");
  }
  printf(1,"Available = %d %d %d\n",Available[0],Available[1],Available[2]);
}

int safety(int seq[N]){
  int work[M], finish[N], i,j,count=0,found;
  for(j=0;j<M;j++) work[j]=Available[j];
  for(i=0;i<N;i++) finish[i]=0;
  while(count<N){
    found=0;
    for(i=0;i<N;i++){
      if(finish[i]) continue;
      int ok=1;
      for(j=0;j<M;j++) if(Max[i][j]-Allocation[i][j] > work[j]) ok=0;
      if(ok){
        for(j=0;j<M;j++) work[j]+=Allocation[i][j];
        finish[i]=1; seq[count++]=i; found=1;
      }
    }
    if(!found) break;
  }
  return count==N;
}

int request(int p, int req[M]){
  int j,seq[N];
  printf(1,"\nRequest by P%d = %d %d %d\n",p,req[0],req[1],req[2]);
  for(j=0;j<M;j++) if(req[j] > Max[p][j]-Allocation[p][j]){
    printf(1,"Request rejected: exceeds Need[%d].\n",p); return 0;
  }
  for(j=0;j<M;j++) if(req[j] > Available[j]){
    printf(1,"Request rejected: exceeds Available.\n"); return 0;
  }
  for(j=0;j<M;j++){ Available[j]-=req[j]; Allocation[p][j]+=req[j]; }
  if(safety(seq)){
    printf(1,"Request GRANTED safely. Safe sequence: ");
    for(j=0;j<N;j++) printf(1,"P%d%s",seq[j], j==N-1?"\n":" -> ");
    return 1;
  }
  for(j=0;j<M;j++){ Available[j]+=req[j]; Allocation[p][j]-=req[j]; }
  printf(1,"Request denied — would lead to unsafe state\n");
  return 0;
}

int main(void){
  int seq[N], granted;
  printf(1,"=== Q1: BANKER'S ALGORITHM ===\n");
  print_state();
  if(safety(seq)){
    int i;
    printf(1,"Initial state: SAFE\nSafe sequence: ");
    for(i=0;i<N;i++) printf(1,"P%d%s",seq[i],i==N-1?"\n":" -> ");
  } else printf(1,"Initial state: UNSAFE\n");

  { int r1[M]={1,0,2}; granted=request(1,r1); if(granted) printf(1,"P1 allocation committed.\n"); }
  { int r2[M]={0,0,2}; request(0,r2); }
  printf(1,"\nFinal matrices after tests:\n");
  print_state();
  exit();
}
