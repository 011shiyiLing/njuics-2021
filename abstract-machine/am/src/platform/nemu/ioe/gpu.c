#include <am.h>
#include <nemu.h>
#include <string.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)
# define W    400
# define H    300

void __am_gpu_init() {
  int i;
  int w = W;//TODO:get the correct width
  int h = H;//TODO:get the correct height
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for(i=0; i<w*h;i++) fb[i] = i;
  outl(SYNC_ADDR,1);
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = W, 
    .height = H,
    .vmemsz = 0
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
  if (w == 0 || h == 0) return;

  int cp_bytes = w * sizeof(uint32_t);
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for(int j=0;j<h;j++)
  {
    memcpy(&fb[(y+j)*w+x],ctl->pixels,cp_bytes);
  }
  if (ctl->sync) {
    outl(SYNC_ADDR,1);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
