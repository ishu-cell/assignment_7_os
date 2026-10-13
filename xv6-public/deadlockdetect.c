#include "types.h"
#include "user.h"

#define N 4
#define R 3

int allocation[N][R];
int requestm[N][R];
int graph[N][N];
int color[N], parent[N], cycle[N+1], cycle_len;

void build_graph(void){
  int i,j,k;
  for(i=0;i<N;i++) for(j=0;j<N;j++) graph[i][j]=0;
  for(i=0;i<N;i++) for(k=0;k<R;k++) if(requestm[i][k])
    for(j=0;j<N;j++) if(allocation[j][k]) graph[i][j]=1;
}

void print_graph(void){
  int i,j;
  printf(1,"Wait-for graph edges:\n");
  for(i=0;i<N;i++) for(j=0;j<N;j++) if(graph[i][j]) printf(1,"P%d -> P%d\n",i,j);
}

int dfs(int u){
  int v,k;
  color[u]=1;
  for(v=0;v<N;v++) if(graph[u][v]){
    if(color[v]==0){ parent[v]=u; if(dfs(v)) return 1; }
    else if(color[v]==1){
      cycle_len=0; cycle[cycle_len++]=v; k=u;
      while(k!=v && cycle_len<N){ cycle[cycle_len++]=k; k=parent[k]; }
      cycle[cycle_len++]=v;
      return 1;
    }
  }
  color[u]=2; return 0;
}

int detect(void){
  int i;
  for(i=0;i<N;i++){ color[i]=0; parent[i]=-1; }
  for(i=0;i<N;i++) if(color[i]==0 && dfs(i)) return 1;
  return 0;
}

void run_case(int dead){
  int i,j;
  for(i=0;i<N;i++) for(j=0;j<R;j++) allocation[i][j]=requestm[i][j]=0;
  if(!dead){
    allocation[1][0]=1; allocation[2][1]=1;
    requestm[0][0]=1; requestm[3][1]=1;
  } else {
    allocation[0][0]=1; allocation[1][1]=1; allocation[2][2]=1;
    requestm[0][1]=1; requestm[1][2]=1; requestm[2][0]=1;
  }
  printf(1,"\n=== %s SCENARIO ===\n",dead?"DEADLOCK":"NO-DEADLOCK");
  build_graph(); print_graph();
  if(detect()){
    printf(1,"Deadlock DETECTED. Cycle: ");
    for(i=cycle_len-1;i>=0;i--) printf(1,"P%d%s",cycle[i],i==0?"\n":" -> ");
  } else printf(1,"No deadlock detected (graph is acyclic).\n");
}

int main(void){
  printf(1,"=== Q2: DEADLOCK DETECTION ===\n");
  run_case(0); run_case(1); exit();
}
