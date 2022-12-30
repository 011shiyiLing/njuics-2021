#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;

Context* __am_irq_handle(Context *c) {
  //printf("%x %x %x\n",c->mcause,c->mtatus,c->mepc);
  if (user_handler) {
    Event ev = {0};
    switch (c->mcause) {
      case -1: ev.event = EVENT_YIELD; break;
      case 0:case 1:case 2:case 3:case 4:case 5:case 6:
      case 7:case 8:case 9:case 10:case 11:case 12:case 13:
      case 14:case 15:case 16:case 17:case 18:case 19: ev.event = EVENT_SYSCALL; break;
      default: ev.event = EVENT_ERROR; break;
    }

    c = user_handler(ev, c);
    assert(c != NULL);
  }

  return c;
}

extern void __am_asm_trap(void);

bool cte_init(Context*(*handler)(Event, Context*)) {
  // initialize exception entry 将异常入口地址设置到mtvec寄存器中
  asm volatile("csrw mtvec, %0" : : "r"(__am_asm_trap));

  // register event handler
  user_handler = handler;

  return true;
}

// kstack是栈的范围, entry是内核线程的入口, arg则是内核线程的参数.
// 需要在kstack的底部创建一个以entry为返回地址的上下文结构
Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
  Context *c = (Context *)((uint8_t *)(kstack.end) - sizeof(Context));
  c->mepc = (uintptr_t)entry;
  c->mstatus = 0x1800;
  c->pdir = NULL;
  //c->gpr[10] = (uintptr_t)arg; //返回值储存在a0寄存器里，因此arg存储到a0里
  return c;
}

void yield() {
  asm volatile("li a7, -1; ecall");
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}
