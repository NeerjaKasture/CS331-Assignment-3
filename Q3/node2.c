#include <stdio.h>
#include <string.h>
#define INFINITY 999
extern struct rtpkt {
  int sourceid;       /* id of sending router sending this pkt */
  int destid;         /* id of router to which pkt being sent 
                         (must be an immediate neighbor) */
  int mincost[4];    /* min cost to node 0 ... 3 */
  };

extern int TRACE;
extern int YES;
extern int NO;

struct distance_table 
{
  int costs[4][4];
} dt2;

extern float clocktime;
/* students to write the following two routines, and maybe some others */
struct rtpkt pkt2[4];

int min_cost_2[4];

void min_cost2(){
  //calculate the minimum cost to every other node
  
  for(int i=0;i<4;i++){
    int min_cost = INFINITY;
    for(int j=0;j<4;j++){
      if(dt2.costs[i][j] < min_cost){
          min_cost = dt2.costs[i][j];
      }
    }
    min_cost_2[i] = min_cost;
  }
}

static void make_pkt2(){

  for(int i=0;i<4;i++) {

    pkt2[i].sourceid = 2;
    pkt2[i].destid = i; 
    memcpy(pkt2[i].mincost, min_cost_2, sizeof(min_cost_2));
  }  
}
int costs2[4] = { 3,1,0,2 };
void rtinit2() 
{
    printf("rtinit2() called at time %0.2f\n", clocktime);

  

  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
        if(i==j)
            dt2.costs[i][j] = costs2[i];
        else 
            dt2.costs[i][j] = INFINITY;
          
    }
  }

  printdt2(&dt2);
  min_cost2();
  make_pkt2();
  for(int i=0;i<4;i++){
    if(i!=2 ){
      tolayer2(pkt2[i]);
      printf("Packet sent at time %0.2f from %d to %d with: (%d  %d  %d  %d)\n",
                       clocktime,pkt2[i].sourceid, pkt2[i].destid,
                       pkt2[i].mincost[0], pkt2[i].mincost[1],
                       pkt2[i].mincost[2],pkt2[i].mincost[3]);
    }
  }
}


rtupdate2(rcvdpkt)
struct rtpkt *rcvdpkt;
{
    int sourceid = rcvdpkt->sourceid;

    int mincost[4];
    for(int i= 0; i<4;i++)
        mincost[i] =  rcvdpkt->mincost[i];
  
    printf("rtupdate2() called at time %0.2f as %d sent a packet with (%d  %d  %d  %d)\n",
           clocktime, sourceid,
           mincost[0], mincost[1],
           mincost[2], mincost[3]);

    for(int i=0;i<4;i++){
            int val = dt2.costs[sourceid][sourceid] + mincost[i];
            if(val<INFINITY)
                dt2.costs[i][sourceid] = val;
            else
                dt2.costs[i][sourceid] = INFINITY;
    }

    printdt2(&dt2);
    int old_min_cost[4];
    memcpy(old_min_cost, min_cost_2, sizeof(min_cost_2));
    int updated = 0;
    min_cost2();
    for(int i=0;i<4;i++){
        if(old_min_cost[i]!=min_cost_2[i]){
            updated = 1;
        }
    }

    if (updated) {
        make_pkt2();
        for (int i = 0; i < 4; i++) {
            if (i != 2 ) { 
                tolayer2(pkt2[i]);
                printf("Packet sent at time %0.2f from %d to %d with: (%d  %d  %d  %d)\n",
                       clocktime,pkt2[i].sourceid, pkt2[i].destid,
                       pkt2[i].mincost[0], pkt2[i].mincost[1],
                       pkt2[i].mincost[2],pkt2[i].mincost[3]);
            }
        }
    }
}




printdt2(dtptr)
  struct distance_table *dtptr;
  
{
  printf("                via     \n");
  printf("   D2 |    0     1    3 \n");
  printf("  ----|-----------------\n");
  printf("     0|  %3d   %3d   %3d\n",dtptr->costs[0][0],
	 dtptr->costs[0][1],dtptr->costs[0][3]);
  printf("dest 1|  %3d   %3d   %3d\n",dtptr->costs[1][0],
	 dtptr->costs[1][1],dtptr->costs[1][3]);
  printf("     3|  %3d   %3d   %3d\n",dtptr->costs[3][0],
	 dtptr->costs[3][1],dtptr->costs[3][3]);
}

printmincost2(){
    printf("Minimum cost from %d to other nodes are: %d %d %d %d\n", 2, min_cost_2[0], min_cost_2[1],
           min_cost_2[2], min_cost_2[3] );
  }
