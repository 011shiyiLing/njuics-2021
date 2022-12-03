#include <common.h>
#include "syscall.h"
#include <unistd.h>

//functions sys_xxx() definations
extern void yield();
void sys_exit(int status)
{
  halt(status);
}

void sys_write(Context *c)
{
  int fd = (int)c->GPR2;
  char * b = (char *)c->GPR3;
  size_t count = (size_t)c->GPR4;

  if(fd == 1 || fd == 2)
  {
    for(int i=0; i<count; i++)
    {
      putch(*b);
      b++;
    }
    c->GPRx = count;
  }
}

//STRACE(system call trace)
static void strace(Context *c)
{
  Log("System call trace:\nmcause:0x%x\tGPR1:%d\tGPR2:0x%x\tGPR3:0x%x\tGPR4:0x%x\n",
  c->mcause,c->GPR1,c->GPR2,c->GPR3,c->GPR4); 
}

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;

  strace(c);

  switch (a[0]) {
    case 0: //SYS_exit 结束运行
      sys_exit((int)a[1]);
      break; 
    case 1://SYS_yield
      yield(); 
      c->GPRx = 0;
      break;
    case 4://SYS_write
      sys_write(c);
      break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
