#include <NDL.h>
#include <stdio.h>

int SDL_Init(uint32_t flags)
{
  return NDL_Init(flags);
}

void SDL_Quit()
{
  NDL_Quit();
}

char *SDL_GetError()
{
  return "Navy does not support SDL_GetError()";
}

int SDL_SetError(const char *fmt, ...)
{
  return -1;
}

int SDL_ShowCursor(int toggle)
{
  printf("SDL_ShowCursor\n");
  return 0;
}

//设置显示窗口的标题和图标
void SDL_WM_SetCaption(const char *title, const char *icon)
{

}
