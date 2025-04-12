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

extern float clocktime;


struct distance_table 
{
  int costs[4][4];
} dt1;


/* students to write the following two routines, and maybe some others */
struct rtpkt pkt1[4];

int min_cost_1[4];

static void min_cost1(){
  //calculate the minimum cost to every other node
  
  for(int i=0;i<4;i++){
    int min_cost = INFINITY;
    for(int j=0;j<4;j++){
      if(dt1.costs[i][j] < min_cost){
          min_cost = dt1.costs[i][j];
      }
    }
    min_cost_1[i] = min_cost;
  }
}

void make_pkt1(){
  for(int i=0;i<4;i++) {
    pkt1[i].sourceid = 1;
    pkt1[i].destid = i; 
    memcpy(pkt1[i].mincost, min_cost_1, sizeof(min_cost_1));
  }  
}

int costs1[4] = { 1,  0,  1, INFINITY };
rtinit1() 
{
    printf("rtinit1() called at time %0.2f\n", clocktime);

 

  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
        if(i==j)
            dt1.costs[i][j] = costs1[i];
        else 
            dt1.costs[i][j] = INFINITY;
          
    }
  }

  printdt1(&dt1);
  min_cost1();
  make_pkt1();
    for(int i=0;i<4;i++) {
        if(i!=1 && i!=3 ) {   
            tolayer2(pkt1[i]);
            printf("Packet sent at time %0.2f from %d to %d with: (%d  %d  %d  %d)\n",
                clocktime, pkt1[i].sourceid, pkt1[i].destid, pkt1[i].mincost[0], pkt1[i].mincost[1],
                pkt1[i].mincost[2],pkt1[i].mincost[3]);     
       }
    }

}


rtupdate1(rcvdpkt)
struct rtpkt *rcvdpkt;
{
    int sourceid = rcvdpkt->sourceid;

    int mincost[4];
    for(int i= 0; i<4;i++)
        mincost[i] =  rcvdpkt->mincost[i];
  
    printf("rtupdate1() called at time %0.2f as %d sent a packet with (%d  %d  %d  %d)\n",
           clocktime, sourceid,
           mincost[0], mincost[1],
           mincost[2], mincost[3]);

    for(int i=0;i<4;i++){
            int val = dt1.costs[sourceid][sourceid] + mincost[i];
            if(val<INFINITY)
                dt1.costs[i][sourceid] = val;
            else
                dt1.costs[i][sourceid] = INFINITY;
    }

    printdt1(&dt1);
    int old_min_cost[4];
    memcpy(old_min_cost, min_cost_1, sizeof(min_cost_1));
    int updated = 0;
    min_cost1();
    for(int i=0;i<4;i++){
        if(old_min_cost[i]!=min_cost_1[i]){
            updated = 1;
        }
    }

    // If any cost changed, send updated packets to neighbors
    if (updated) {
        make_pkt1();
        for (int i = 0; i < 4; i++) {
            if (i != 1 && i!=3 ) { // Don't send to self
                tolayer2(pkt1[i]);
                printf("Packet sent at time %0.2f from %d to %d with: (%d  %d  %d  %d)\n",
                       clocktime, pkt1[i].sourceid, pkt1[i].destid, pkt1[i].mincost[0], pkt1[i].mincost[1],
                       pkt1[i].mincost[2],pkt1[i].mincost[3]);
            }
    }
}
}


printdt1(dtptr)
  struct distance_table *dtptr;
  
{
  printf("             via   \n");
  printf("   D1 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[0][2]);
  printf("dest 2|  %3d   %3d\n",dtptr->costs[2][0], dtptr->costs[2][2]);
  printf("     3|  %3d   %3d\n",dtptr->costs[3][0], dtptr->costs[3][2]);

}

printmincost1(){
    printf("Minimum cost from %d to other nodes are: %d %d %d %d\n", 1, min_cost_1[0], min_cost_1[1],
           min_cost_1[2], min_cost_1[3] );
  }


  void linkhandler1(linkid, newcost)
  int linkid, newcost;   
  /* called when cost from 1 to linkid changes from current value to newcost*/
  /* You can leave this routine empty if you're an undergrad. If you want */
  /* to use this routine, you'll need to change the value of the LINKCHANGE */
  /* constant definition in prog3.c from 0 to 1 */
      
  {

  }
