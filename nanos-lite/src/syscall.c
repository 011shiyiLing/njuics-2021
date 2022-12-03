#include <common.h>
#include "syscall.h"

//functions sys_xxx() definations
extern void yield();
void sys_exit(int status)
{
  halt(status);
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
    /*case 2: //SYS_open
      c->GPRx = _open((const char *)c->GPR2,(int)c->GPR3,(mode_t)c->GPR4);
      break;
    case 3://SYS_read
      c->GPRx = _read((int)c->GPR2,(void *)c->GPR3,(size_t)c->GPR4);
      break;
    case 4://SYS_write
      c->GPRx = _write((int)c->GPR2,(void *)c->GPR3,(size_t)c->GPR4);
      break;
    case 5://SYS_kill
      c->GPRx = _kill((int)c->GPR2,(int)c->GPR3);
      break;
    case 6://SYS_getpid
      c->GPRx = _getpid();
      break;
    case 7://SYS_close
      c->GPRx = _close((int)c->GPR2);
      break;
    case 8://SYS_lseek
      c->GPRx = _lseek((int)c->GPR2,(off_t)c->GPR3,(int)c->GPR4);
      break;
    case 9://SYS_brk
      c->GPRx = _sbrk((intptr_t)c->GPR2);
      break;
    case 10://SYS_fstat
      c->GPRx = _fstat((int)c->GPR2,(struct stat*)c->GPR3);
      break;
    case 11://SYS_time
      break;
    case 12://SYS_signal
      break;
    case 13://SYS_execve
      c->GPRx = _execve((const char*)c->GPR2,(char * const*)c->GPR3,(char * const*)c->GPR4);
      break;
    case 14://SYS_fork
      c->GPRx = _fork();
      break;
    case 15://SYS_link
      c->GPRx = _link((const char *)c->GPR2,(const char*)c->GPR3);
      break;
    case 16://SYS_unlink
      c->GPRx = _unlink((const char*)c->GPR2);
      break;
    case 17://SYS_wait
      c->GPRx = _wait((int *)c->GPR2);
    case 18://SYS_times
      c->GPRx = _times((void *)c->GPR2);
      break;
    case 19://SYS_gettimeofday
      c->GPRx = _gettimeofday((struct timeval *)c->GPR2,(struct timezone *)c->GPR3);
      break;*/
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
