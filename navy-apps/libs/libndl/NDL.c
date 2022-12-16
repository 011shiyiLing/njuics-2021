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

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
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

  //get screen_w and screen_height
  char buf[64];
  int fp = open("/proc/dispinfo",0);
  read(fp,buf,sizeof(buf));
  printf("%s",buf);
  printf("%s",buf);
  sscanf(buf,"WIDTH : %d\nHEIGHT : %d\n",&screen_w,&screen_h);
  printf("screnn_width : %d\nscreen_height : %d\n",screen_w,screen_h);
  return 0;
}

void NDL_Quit() {
}
