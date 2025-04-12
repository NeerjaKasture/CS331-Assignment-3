    #include <stdio.h>
    #include <string.h>
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
    } dt3;

    /* students to write the following two routines, and maybe some others */
    extern float clocktime;
    #define INFINITY 999
    struct rtpkt pkt3[4];

    int min_cost_3[4];

    void min_cost3(){
    //calculate the minimum cost to every other node
    
    for(int i=0;i<4;i++){
        int min_cost = INFINITY;
        for(int j=0;j<4;j++){
        if(dt3.costs[i][j] < min_cost){
            min_cost = dt3.costs[i][j];
        }
        }
        min_cost_3[i] = min_cost;
    }
    }

    void make_pkt3(){
   
    for(int i=0;i<4;i++) {
        pkt3[i].sourceid = 3;
        pkt3[i].destid = i; 
        memcpy(pkt3[i].mincost, min_cost_3, sizeof(min_cost_3));
    }  
    }

    int costs3[4] = { 7,INFINITY,2,0 };

    void rtinit3() 
    {
        printf("rtinit3() called at time %0.2f\n", clocktime);

    

    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            if(i==j)
                dt3.costs[i][j] = costs3[i];
            else 
                dt3.costs[i][j] = INFINITY;
            
        }
    }

    printdt3(&dt3);
    min_cost3();
    make_pkt3();
    for(int i=0;i<4;i++) {
        if(i!=3 && i!=1 ) { //not sending self and not sending to node 1
            tolayer2(pkt3[i]);
            printf("Packet sent at time %0.2f from %d to %d with: (%d  %d  %d  %d)\n", clocktime,
                   pkt3[i].sourceid, pkt3[i].destid, pkt3[i].mincost[0], pkt3[i].mincost[1],
                   pkt3[i].mincost[2],pkt3[i].mincost[3]);
        }
    }
    }


    rtupdate3(rcvdpkt)
    struct rtpkt *rcvdpkt;
    {
        int sourceid = rcvdpkt->sourceid;
    
        int mincost[4];
        for(int i= 0; i<4;i++)
            mincost[i] =  rcvdpkt->mincost[i];
      
        printf("rtupdate2() called at %0.2f as %d sent a packet with (%d  %d  %d  %d)\n",
               clocktime, sourceid,
               mincost[0], mincost[1],
               mincost[2], mincost[3]);
    
        for(int i=0;i<4;i++){
                int val = dt3.costs[sourceid][sourceid] + mincost[i];
                if(val<INFINITY)
                    dt3.costs[i][sourceid] = val;
                else
                    dt3.costs[i][sourceid] = INFINITY;
        }
    
        printdt2(&dt3);
        int old_min_cost[4];
        memcpy(old_min_cost, min_cost_3, sizeof(min_cost_3));
        int updated = 0;
        min_cost3();
        for(int i=0;i<4;i++){
            if(old_min_cost[i]!=min_cost_3[i]){
                updated = 1;
            }
        }
    
        if (updated) {
            make_pkt3();
            for (int i = 0; i < 4; i++) {
                if (i != 3 && i != 1) { 
                    tolayer2(pkt3[i]);
                    printf("Packet sent at time %0.2f from %d to %d with: (%d  %d  %d  %d)\n", clocktime,
                   pkt3[i].sourceid, pkt3[i].destid, pkt3[i].mincost[0], pkt3[i].mincost[1],
                   pkt3[i].mincost[2],pkt3[i].mincost[3]);
                }
            }
        }
    }
    
    

    printdt3(dtptr)
    struct distance_table *dtptr;
    
    {
    printf("             via     \n");
    printf("   D3 |    0     2 \n");
    printf("  ----|-----------\n");
    printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[0][2]);
    printf("dest 1|  %3d   %3d\n",dtptr->costs[1][0], dtptr->costs[1][2]);
    printf("     2|  %3d   %3d\n",dtptr->costs[2][0], dtptr->costs[2][2]);

    }

    printmincost3(){
        printf("Minimum cost from %d to other nodes are: %d %d %d %d\n", 3, min_cost_3[0], min_cost_3[1],
               min_cost_3[2], min_cost_3[3] );
      }
    