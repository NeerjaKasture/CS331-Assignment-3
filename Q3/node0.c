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
} dt0;


/* students to write the following two routines, and maybe some others */
struct rtpkt pkt0[4];

int min_cost_0[4];

static void min_cost0(){
  //calculate the minimum cost to every other node
  
  for(int i=0;i<4;i++){
    int min_cost = INFINITY;
    for(int j=0;j<4;j++){
      if(dt0.costs[i][j] < min_cost){
          min_cost = dt0.costs[i][j];
      }
    }
    min_cost_0[i] = min_cost;
  }
}

void make_pkt0(){
  for(int i=0;i<4;i++) {
    pkt0[i].sourceid = 0;
    pkt0[i].destid = i; 
    memcpy(pkt0[i].mincost, min_cost_0, sizeof(min_cost_0));
  }  
}

int costs0[4] = { 0,  1,  3, 7 };
rtinit0() 
{
    printf("rtinit0() called at time %0.2f\n", clocktime);
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
        if(i==j)
            dt0.costs[i][j] = costs0[i];
        else 
            dt0.costs[i][j] = INFINITY;
          
    }
  }

  printdt0(&dt0);
  min_cost0();
  make_pkt0();
    for(int i=0;i<4;i++) {
        if(i!=0 ) {   
            tolayer2(pkt0[i]);
            printf("Packet sent at time %0.2f from %d to %d with: (%d  %d  %d  %d)\n", clocktime,
                   pkt0[i].sourceid, pkt0[i].destid, pkt0[i].mincost[0], pkt0[i].mincost[1],
                   pkt0[i].mincost[2],pkt0[i].mincost[3]);        
        }
    }

}


rtupdate0(rcvdpkt)
struct rtpkt *rcvdpkt;
{
    int sourceid = rcvdpkt->sourceid;

    int mincost[4];
    for(int i= 0; i<4;i++)
        mincost[i] =  rcvdpkt->mincost[i];
  
    printf("rtupdate0() is called at time %0.2f as %d sent a packet with (%d  %d  %d  %d)\n",
           clocktime, sourceid,
           mincost[0], mincost[1],
           mincost[2], mincost[3]);

    for(int i=0;i<4;i++){
            int val = dt0.costs[sourceid][sourceid] + mincost[i];
            if(val<INFINITY)
                dt0.costs[i][sourceid] = val;
            else
                dt0.costs[i][sourceid] = INFINITY;
    }

    printdt0(&dt0);
    int old_min_cost[4];
    memcpy(old_min_cost, min_cost_0, sizeof(min_cost_0));
    int updated = 0;
    min_cost0();
    for(int i=0;i<4;i++){
        if(old_min_cost[i]!=min_cost_0[i]){
            updated = 1;
        }
    }

    if (updated) {
        make_pkt0();
        for (int i = 0; i < 4; i++) {
            if (i != 0 ) { 
                tolayer2(pkt0[i]);
                printf("Packet sent at time %0.2f from %d to %d with: (%d  %d  %d  %d)\n", clocktime,
                   pkt0[i].sourceid, pkt0[i].destid, pkt0[i].mincost[0], pkt0[i].mincost[1],
                   pkt0[i].mincost[2],pkt0[i].mincost[3]);           
       }
        }
    }
}



printdt0(dtptr)
  struct distance_table *dtptr;
  
{
  printf("                via     \n");
  printf("   D0 |    1     2    3 \n");
  printf("  ----|-----------------\n");
  printf("     1|  %3d   %3d   %3d\n",dtptr->costs[1][1],
	 dtptr->costs[1][2],dtptr->costs[1][3]);
  printf("dest 2|  %3d   %3d   %3d\n",dtptr->costs[2][1],
	 dtptr->costs[2][2],dtptr->costs[2][3]);
  printf("     3|  %3d   %3d   %3d\n",dtptr->costs[3][1],
	 dtptr->costs[3][2],dtptr->costs[3][3]);
}

printmincost0(){
    printf("Minimum cost from %d to other nodes are: %d %d %d %d\n", 0, min_cost_0[0], min_cost_0[1],
           min_cost_0[2], min_cost_0[3] );
  }

  void linkhandler0(linkid, newcost)
  int linkid, newcost;

/* called when cost from 0 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */
	
{
}



