#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>
#include <assert.h>
//NJU DirectMedia Layer 

static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0;
static int canvas_w,canvas_h;

// 以毫秒为单位返回系统时间
uint32_t NDL_GetTicks() {
  struct timeval tv;
  struct timezone tz;
  gettimeofday(&tv, NULL);
  uint32_t res = tv.tv_sec * 1000 + tv.tv_usec / 1000;
  return res;
}

// 读出一条事件信息, 将其写入`buf`中, 最长写入`len`字节
// 若读出了有效的事件, 函数返回1, 否则返回0
int NDL_PollEvent(char *buf, int len) {
  int fp = open("/dev/events",NULL);
  return read(fp,buf,len);
}

// 打开一张(*w) X (*h)的画布
// 如果*w和*h均为0, 则将系统全屏幕作为画布, 并将*w和*h分别设为系统屏幕的大小
void NDL_OpenCanvas(int *w, int *h) {
  if(*w == 0) *w = screen_w;
  if(*h == 0) *h = screen_h;

  if(*w > screen_w || *h > screen_h) assert(0);
  canvas_w = *w;
  canvas_h = *h;

  if (getenv("NWM_APP")) {
    int fbctl = 4;
    fbdev = 5;
    screen_w = *w; screen_h = *h;
    char buf[64];
    int len = sprintf(buf, "%d %d", screen_w, screen_h);
    // let NWM resize the window and create the frame buffer
    write(fbctl, buf, len);
    while (1) {
      // 3 = evtdev
      int nread = read(3, buf, sizeof(buf) - 1);
      if (nread <= 0) continue;
      buf[nread] = '\0';
      if (strcmp(buf, "mmap ok") == 0) break;
    }
    close(fbctl);
  }
}

// 向画布`(x, y)`坐标处绘制`w*h`的矩形图像, 并将该绘制区域同步到屏幕上
// 图像像素按行优先方式存储在`pixels`中, 每个像素用32位整数以`00RRGGBB`的方式描述颜色
void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
  for(int i=0; i<h; i++)
  {
      lseek(fbdev, (y + i)*canvas_w + x, SEEK_SET);
      write(fbdev, pixels + i*w, w);
  }
  write(fbdev , 0, 0);
}

void NDL_OpenAudio(int freq, int channels, int samples) {
}

void NDL_CloseAudio() {
}

int NDL_PlayAudio(void *buf, int len) {
  return 0;
}

int NDL_QueryAudio() {
  return 0;
}

int NDL_Init(uint32_t flags) {
  if (getenv("NWM_APP")) {
    evtdev = 3;
  }
  fbdev = open("/dev/fp",0,0);
  //get screen_w and screen_height
  char buf[64];
  int fp = open("/proc/dispinfo",NULL);
  read(fp,buf,sizeof(buf));
  //printf("%s\n",buf);
  sscanf(buf,"WIDTH : %d\nHEIGHT : %d\n",&screen_w,&screen_h);
  //printf("screnn_width : %d\nscreen_height : %d\n",screen_w,screen_h);
  return 0;
}

void NDL_Quit() {
}
