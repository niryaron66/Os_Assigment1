#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

extern int sched_policy;
extern struct proc proc[NPROC];

uint64
sys_exit(void) //TODO : check if need add char*
{
  char msg[MAXARG];
  int n;
  argint(0, &n);
  if(argstr(1, msg, MAXARG) < 0)
    return -1;
  exit(n,msg);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{

// wait(&status,&msg)
 
  // char msg[MAXARG];
  uint64 p;
  uint64 p2;
  argaddr(0, &p);
  argaddr(1, &p2);

  // if(argstr(1, msg, MAXARG) < 0)
  //   return -1;
  return wait(p,p2);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_memsize(void)
{
  // return size of running process memory in bytes
  return myproc()->sz;
}

uint64
sys_set_ps_priority(void)
{
  int new_ps_priority;
  argint(0, &new_ps_priority);
  myproc()->ps_priority=new_ps_priority;
  //set_ps_priority(new_ps_priority);  TODO : maybe return it 
  return 0;
}

uint64
sys_set_policy(void)
{
  int new_scheduler_policy;
  argint(0, &new_scheduler_policy);
  if (new_scheduler_policy < 0 || new_scheduler_policy > 2) {
        return -1; 
    }
  sched_policy = new_scheduler_policy;
  return 0; 
}

uint64
sys_set_cfs_priority(void)
{
  int new_cfs_priority;
  argint(0, &new_cfs_priority);
  if (new_cfs_priority < 0 || new_cfs_priority > 2) {
        return -1; 
    }
  myproc()->cfs_priority = new_cfs_priority;
  return 0; 
}

uint64   
 sys_get_cfs_stats(void) 
{
  //Explanation: we counting on getting two parameters: PID, and address for the int[4] array.
  //We will then load the the given address with the stats. 
  //Finally, we will return 0 or -1, for success or failure.
  int stats[4];
  uint64 addr;
  int pid;
  argint(0, &pid);
  argaddr(1, &addr);
  struct proc *process = 0;
  struct proc *p;
  for (p = proc; p < &proc[NPROC]; p++){
    if (p->pid == pid){
      process = p;
      break;
    }
  }
  if (process == 0)
    return 0;

  stats[0] = process->cfs_priority;
  stats[1] = process->rtime;
  stats[2] = process->stime;
  stats[3] = process->retime;
  
  copyout(p->pagetable, addr, (char *)stats, 4 * sizeof(int));
  return addr;
}

// uint64
// sys_get_cfs_stats(){
//   int pid=-1;
//   uint64 addr;
//   argint(0, &pid);
//   argaddr(1, &addr);
//   char* ans = "1111";
//   struct proc* p = getProc(pid);
//   //printf("%d\n", p->pid);
//   if(p != 0){
//     ans[0]=p->cfs_priority;
//     ans[1]=p->rtime;
//     ans[2]=p->stime;
//     ans[3]=p->retime;

//   }
//   copyout(p->pagetable, addr, ans, 4);
//   return 0;
// }