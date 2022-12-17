#include <common.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
  [AM_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};

static int width,height;

//串口
size_t serial_write(const void *buf, size_t offset, size_t len) {
  assert(len >= 0);
  for(int i=0; i<len; i++)
  {
      putch(*(char *)buf);
      buf++;
  }
  return len;
}
//把事件写入到buf中, 最长写入len字节, 然后返回写入的实际长度.
size_t events_read(void *buf, size_t offset, size_t len) {
  //strcpy(buf,"0");
  AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);
  //printf("%d\n",ev.keydown);
  if(ev.keycode == AM_KEY_NONE)
  {
    return 0;
  }
  else
  {
    if(ev.keydown)
    {
      strcpy(buf,"kd ");
      strcat(buf,keyname[ev.keycode]);
    }
    else
    {
      strcpy(buf,"ku ");
      strcat(buf,keyname[ev.keycode]);
    }
  }

  return strlen(buf);
}
//按照约定将文件的len字节写到buf中(我们认为这个文件不支持lseek, 可忽略offset)
//获取屏幕大小
size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  AM_GPU_CONFIG_T cfg = io_read(AM_GPU_CONFIG);
  width = cfg.width;
  height = cfg.height;
  sprintf(buf,"WIDTH : %d\nHEIGHT : %d\n",width,height);
  return strlen(buf);
}
//用于把buf中的len字节写到屏幕上offset处.
//需要先从offset计算出屏幕上的坐标, 然后调用IOE来进行绘图
size_t fb_write(const void *buf, size_t offset, size_t len) {
  AM_GPU_FBDRAW_T fbdraw;
  if(len == 0)
  {
    fbdraw.sync = 1;
    fbdraw.w = 0;
    fbdraw.h = 0;
    ioe_write(AM_GPU_FBDRAW, &fbdraw);
    return 0;
  }

  fbdraw.pixels = (void *)buf;
  fbdraw.w = len;
  fbdraw.h = 1;
  fbdraw.x = offset % width;
  fbdraw.y = offset / width;
  fbdraw.sync = 0;
  ioe_write(AM_GPU_FBDRAW, &fbdraw);
  return len;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}
