
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

int main()
{
    int pid1, pid2, pid3;
    int p1_stats[4], p2_stats[4], p3_stats[4];

    pid1 = fork();
    if (pid1 == 0)
    { // Child
        set_cfs_priority(2);
        run_loop_and_sleep();
    }

    else
    { // Parent
        pid2 = fork();
        if (pid2 == 0)
        {
            set_cfs_priority(1);
            run_loop_and_sleep();
        }
        else
        {
            pid3 = fork();
            if (pid3 == 0)
            {
                set_cfs_priority(0);
                run_loop_and_sleep();
            }
            else{
                wait(0,"");
                wait(0,"");
                wait(0,"");
               get_cfs_stats(pid1, p1_stats);
               get_cfs_stats(pid2, p2_stats);
               get_cfs_stats(pid3, p3_stats);
               printf("P1 pid %d stats are:\n cfs priority time: %d, runtime: %d, stime: %d, retime: %d",pid1,p1_stats[0], p1_stats[3], p1_stats[1], p1_stats[2]);
            }  
        }
    }

     return 0;
}