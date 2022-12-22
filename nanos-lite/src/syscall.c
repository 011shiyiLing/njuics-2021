#include <common.h>
#include "syscall.h"
#include <unistd.h>
#include <proc.h>

struct timezone
{
  int tz_minuteswest;		/* Minutes west of GMT.  */
  int tz_dsttime;		/* Nonzero if DST is ever in effect.  */
};

int fs_open(const char *pathname,int flags,int mode);
size_t fs_write(int fd,const void *buf,size_t len);
size_t fs_read(int fd,void *buf,size_t len);
size_t fs_lseek(int fd,size_t offset,int whence);
int fs_close(int fd);
void naive_uload(PCB *pcb, const char *filename);

//functions sys_xxx() definations
extern void yield();

int sys_write(int fd,void *buf,size_t count)
{
  /*if(fd == 1 || fd == 2)
  {
    for(int i=0; i<count; i++)
    {
      putch(*(char *)buf);
      buf++;
    }
    return count;
  }
  return -1;*/
  return fs_write(fd,(const void*)buf,count);
}

int sys_brk(void *addr)
{
  return 0;
}
//参数tv是保存获取时间结果的结构体，参数tz用于保存时区结果(一般传NULL),函数执行成功后返回0，失败后返回-1
int sys_gettimeofday(struct timeval *tv, struct timezone *tz)
{
  __uint64_t time = io_read(AM_TIMER_UPTIME).us;
  tv->tv_sec = (time / 1000000);
  tv->tv_usec = (time % 1000000);
  return 0;
}


void sys_execve(const char *fname, char * const argv[], char *const envp[])
{
  naive_uload(NULL, fname);
}

void sys_exit(int status)
{
  sys_execve("/bin/menu", NULL, NULL);
  //halt(status);
}

//STRACE(system call trace)
/*static void strace(Context *c)
{
  Log("System call trace:\nmcause:0x%x\tGPR1:%d\tGPR2:0x%x\tGPR3:0x%x\tGPR4:0x%x\n",
  c->mcause,c->GPR1,c->GPR2,c->GPR3,c->GPR4);
}*/

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;

  //strace(c);

  switch (a[0]) {
    case 0: //SYS_exit 结束运行
      sys_exit((int)a[1]);
      break; 
    case 1://SYS_yield
      yield(); 
      c->GPRx = 0;
      break;
    case 2://SYS_open
      c->GPRx = fs_open((const char*)a[1],(int)a[2],(int)a[3]);
      break;
    case 3://SYS_read
      c->GPRx = fs_read((int)a[1],(void *)a[2],(size_t)a[3]);
      break;
    case 4://SYS_write
      c->GPRx = sys_write((int)a[1],(void *)a[2],(size_t)a[3]);
      break;
    case 7://SYS_close
      c->GPRx = fs_close((int)a[1]);
      break;
    case 8://SYS_lseek
      c->GPRx = fs_lseek((int)a[1],(size_t)a[2],(int)a[3]);
      break;
    case 9://SYS_brk
      c->GPRx = sys_brk((void *)a[1]);
      break;
    case 13://SYS_execve
      sys_execve((const char *)a[1],(char * const*)a[2],(char * const*)a[3]);
      break;
    case 19://SYS_gettimeofday
      c->GPRx = sys_gettimeofday((struct timeval *)a[1],(struct timezone *)a[2]);
      break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
