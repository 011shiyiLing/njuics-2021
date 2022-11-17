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
  //从VVGACTL_ADDR这个内存映射IO地址读到的数据高16位是宽度信息，低16位是高度信息
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

  int win_weight = io_read(AM_GPU_CONFIG).width;

  for(int i=0; i<h; ++i)
  {
    for(int j=0; j<w; ++j)
    {
      fb[y*win_weight + i*win_weight + x + j] = pixels[i*w + j];
    }
  }
  
  /*int cp_bytes;
  if(w < win_weight - x)
  {
    cp_bytes = sizeof(uint32_t)*w;
  }
  else{
    cp_bytes = sizeof(uint32_t)*(win_weight-x);
  }

  for(int j=0; j<h && (y+j) < win_height; j++)
  {
    memcpy(&fb[(y+j)*win_height + x],pixels,cp_bytes);
    pixels += w;
  }*/

  if (ctl->sync) {
    outl(SYNC_ADDR,1);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
