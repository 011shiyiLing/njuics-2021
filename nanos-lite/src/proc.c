#include <proc.h>
#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;//用于指向当前运行进程的PCB.
void naive_uload(PCB *pcb,const char *filename);
void context_kload(PCB *pcb, void (*entry)(void *),void *arg);
void context_uload(PCB *pcb,const char *filename,char *const argv[],char *const envp[]);

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  int j = 1;
  while (1) {
    Log("Hello World from Nanos-lite with arg '%p' for the %dth time!", (uintptr_t)arg, j);
    j ++;
    yield();
  }
}

void init_proc() {
  context_kload(&pcb[0],hello_fun,NULL);
  //context_kload(&pcb[1],hello_fun,"two");
  //context_uload(&pcb[0],"/bin/pal", NULL, NULL);
  //context_uload(&pcb[0],"/bin/exec-test",NULL,NULL);
  switch_boot_pcb();

  Log("Initializing processes...");
  
  // load program here
  //naive_uload(NULL,NULL);
  //naive_uload(NULL, "/bin/dummy");
  //naive_uload(NULL, "/bin/hello");
  //naive_uload(NULL, "/bin/file-test");
  //naive_uload(NULL,"/bin/timer-test");
  //naive_uload(NULL,"/bin/event-test");
  //naive_uload(NULL, "/bin/bmp-test");
  //naive_uload(NULL, "/bin/nslider");
  //naive_uload(NULL, "/bin/menu");
  //naive_uload(NULL, "/bin/nterm");
  //naive_uload(NULL, "/bin/bird");
  //naive_uload(NULL, "/bin/pal");
}

// 用于返回将要调度的进程上下文
// 通过current来决定接下来要调度哪一个进程了.
Context* schedule(Context *prev) {
  // save the context pointer
  current->cp = prev;
  // always select pcb[0] as the new process
  //current = &pcb[0];
  current = (current == &pcb[0] ? &pcb[1]:&pcb[0]);

  return current->cp;

}
