#include <am.h>
#include <nemu.h>
#include <string.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)


void __am_gpu_init() {
  int i;
  int w = io_read(AM_GPU_CONFIG).width;//TODO:get the correct width
  int h = io_read(AM_GPU_CONFIG).height;//TODO:get the correct height
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for(i=0; i<w*h;i++) fb[i] = i;
  outl(SYNC_ADDR,1);
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  uint32_t info = inl(VGACTL_ADDR);
  uint16_t width = (uint16_t)(info >> 16);
  uint16_t height = (uint16_t)(info & 0xffff);

  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = width, 
    .height = height,
    .vmemsz = 0
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
  uint32_t *pixels = (uint32_t*)(uintptr_t)ctl->pixels;
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;

  int W = io_read(AM_GPU_CONFIG).width;

  if (w == 0 || h == 0) return;
  //int cp_bytes = w * sizeof(uint32_t);
  
  for(int i=0; i<h; i++)
  {
    for(int j=0; j<w; j++)
    {
      fb[y*W + i*W + x + j] = pixels[i*w + j];
    }
  }

  if (ctl->sync) {
    outl(SYNC_ADDR,1);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
