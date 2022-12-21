#include <NDL.h>
#include <SDL.h>
#include <string.h>

#define keyname(k) #k,

static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};

static uint8_t keystate[256] = {0}; //记录按键状态

int SDL_PushEvent(SDL_Event *ev) {
  printf("SDL_PushEvent\n");
  return 0;
}

// 如果当前没有任何事件, 就会立即返回
int SDL_PollEvent(SDL_Event *ev) {
  char buf[64];
  char key_state[64];
  char key_name[64];
  if(NDL_PollEvent(buf,sizeof(buf)) == 0) return 0;
  sscanf(buf,"%s %s",key_state,key_name);

  if(strcmp("kd",key_state) == 0) ev->type = SDL_KEYDOWN;
  else if (strcmp("ku",key_state) == 0) ev->type = SDL_KEYUP;
  
  for(uint8_t i=0; i<256; i++)
  {
    if(strcmp(key_name, keyname[i]) == 0)
    {
      ev->key.keysym.sym = i;
      if (ev->type == SDL_KEYDOWN) keystate[i] = 1;
      else keystate[i] = 0;
      break;
    }
  }
  return 1;
}

//用于等待一个事件. 你需要将NDL中提供的事件封装成SDL事件返回给应用程序
int SDL_WaitEvent(SDL_Event *event) {
  char buf[64];
  char key_state[64];
  char key_name[64];
  while(NDL_PollEvent(buf,sizeof(buf)) == 0) {};
  sscanf(buf,"%s %s",key_state,key_name);

  if(strcmp("kd",key_state) == 0) event->type = SDL_KEYDOWN;
  else if (strcmp("ku",key_state) == 0) event->type = SDL_KEYUP;
  
  for(uint8_t i=0; i<256; i++)
  {
    if(strcmp(key_name, keyname[i]) == 0)
    {
      event->key.keysym.sym = i;
      if (event->type == SDL_KEYDOWN) keystate[i] = 1;
      else keystate[i] = 0;
      break;
    }
  }

 return 1;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  printf("SDL_PeepEvent\n");
  return 0;
}

//用来获取指定的虚拟键码的按键的状态
//numkeys:可用按键的数量
uint8_t* SDL_GetKeyState(int *numkeys) {
  //printf("SDL_GetKeyState\n");
  return keystate;
}
