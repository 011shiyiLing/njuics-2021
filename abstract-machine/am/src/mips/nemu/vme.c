#include <am.h>
#include <nemu.h>

#define USER_SPACE RANGE(0x40000000, 0x80000000)

static void* (*pgalloc_usr)(int) = NULL;
static void (*pgfree_usr)(void*) = NULL;
static int vme_enable = 0;

bool vme_init(void* (*pgalloc_f)(int), void (*pgfree_f)(void*)) {
  pgalloc_usr = pgalloc_f;
  pgfree_usr = pgfree_f;
  vme_enable = 1;

  return true;
}

void protect(AddrSpace *as) {
  as->ptr = (PTE*)(pgalloc_usr(PGSIZE));
  as->pgsize = PGSIZE;
  as->area = USER_SPACE;
}

void unprotect(AddrSpace *as) {
}

static PTE *cur_pdir = NULL;
void __am_get_cur_as(Context *c) {
  c->pdir = cur_pdir;
}

void __am_switch(Context *c) {
  if (vme_enable && c->pdir != NULL) {
    cur_pdir = c->pdir;
  }
}

void map(AddrSpace *as, void *va, void *pa, int prot) {
}

// as:用于限制用户进程可以访问的内存
// kstack:内核栈, 用于分配上下文结构
// entry:用户进程的入口
Context *ucontext(AddrSpace *as, Area kstack, void *entry) {
  Context *c = (Context *)((uint8_t *)(kstack.end) - sizeof(Context));
  c->mepc = (uintptr_t)entry;
  c->mstatus = 0x1800;
  c->pdir = NULL;
  return c;
}
