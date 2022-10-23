#include <am.h>
#include <nemu.h>

extern char _heap_start;
int main(const char *args);

Area heap = RANGE(&_heap_start, PMEM_END);//indicate heap's start and end
#ifndef MAINARGS
#define MAINARGS ""
#endif
static const char mainargs[] = MAINARGS;

void putch(char ch) {
  outb(SERIAL_PORT, ch);
}
//to print a character

void halt(int code) {
  nemu_trap(code);

  // should not reach here
  while (1);
}
//to end the excute of programm

void _trm_init() {
  int ret = main(mainargs);
  halt(ret);
}
//init TRM
