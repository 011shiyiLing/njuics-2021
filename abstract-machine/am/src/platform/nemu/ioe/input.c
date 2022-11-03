#include <am.h>
#include <nemu.h>
#include <stdio.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  kbd->keydown = (inl(KBD_ADDR+2) & KEYDOWN_MASK);
  printf("%d", inl(KBD_ADDR+2) & KEYDOWN_MASK);
  kbd->keycode = inl(KBD_ADDR+2);
}
