#include <common.h>
#include <stdio.h>

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
  AM_INPUT_KEYBRD_T ev;
  ioe_read(AM_INPUT_KEYBRD,&ev);
  int key;
  key = ev.keycode;

  if(key == AM_KEY_NONE)
  {
    return 0;
  }
  else
  {
    if(ev.keydown)
    {
      len = sprintf(buf,"kd %s\n",keyname[key]);
    }
    else
    {
      len = sprintf(buf,"ku %s\n",keyname[key]);
    }
  }

  return len;
  //return 0;
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  return 0;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}
