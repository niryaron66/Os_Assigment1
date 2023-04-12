#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define LOOP_COUNT 1000000
#define SLEEP_COUNT 100000

void run_loop_and_sleep()
{
    for (int i = 0; i < LOOP_COUNT; i++)
    {
        if (i % SLEEP_COUNT == 0)
        {
            sleep(1);
        }
    }
}

int main(int argc, char *argv[])
{
    int pid1, pid2, pid3;
    int p1_stats[4], p2_stats[4], p3_stats[4];
    pid1 = fork();
    if (pid1 == 0)
    { // Child
        sleep(1);
        set_cfs_priority(2);
        run_loop_and_sleep();
        printf("Childe 1 finished \n");
        int pid = getpid();
        get_cfs_stats(pid, p1_stats);
        sleep(1);
        printf("P1 pid %d stats are:\n cfs priority: %d, runtime: %d, stime: %d, retime: %d \n",pid,p1_stats[0], p1_stats[1], p1_stats[2], p1_stats[3]);
        exit(0,"");
    }

    else
    { // Parent
        pid2 = fork();
        if (pid2 == 0)
        {
            sleep(2);
            set_cfs_priority(1);
            run_loop_and_sleep();
            printf("Childe 2 finished \n");
            int pid = getpid();
            get_cfs_stats(pid, p2_stats);
            sleep(5);
            printf("P2 pid %d stats are:\n cfs priority: %d, runtime: %d, stime: %d, retime: %d \n",pid,p2_stats[0], p2_stats[1], p2_stats[2], p2_stats[3]);
            exit(0,"");

        }
        else
        {
            pid3 = fork();
            if (pid3 == 0)
            {
                sleep(3);
                set_cfs_priority(0);
                run_loop_and_sleep();
                printf("Childe 3 finished \n");
                int pid = getpid();
                get_cfs_stats(pid, p3_stats);
                sleep(10);
                printf("P3 pid %d stats are:\n cfs priority: %d, runtime: %d, stime: %d, retime: %d \n",pid,p3_stats[0], p3_stats[1], p3_stats[2], p3_stats[3]);
                exit(0,"");

            }
            else{
                wait(0,0);
                wait(0,0);
                wait(0,0);

            }  
        }
    }

     return 0;
}