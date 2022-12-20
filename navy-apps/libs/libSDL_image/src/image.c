#define SDL_malloc  malloc
#define SDL_free    free
#define SDL_realloc realloc

#define SDL_STBIMAGE_IMPLEMENTATION
#include "SDL_stbimage.h"

SDL_Surface* IMG_Load_RW(SDL_RWops *src, int freesrc) {
  assert(src->type == RW_TYPE_MEM);
  assert(freesrc == 0);
  return NULL;
}

//接受一个图片文件的路径, 然后把图片的像素信息封装成SDL的Surface结构并返回
SDL_Surface* IMG_Load(const char *filename) {
  FILE *f = fopen(filename,"r");

  fseek(f,0L,SEEK_END);
  size_t size = ftell(f);
  fseek(f,0L,SEEK_SET);

  char *buf = (char*)malloc(size);
  fread(buf, 1, size, f);

  SDL_Surface *s;
  s = STBIMG_LoadFromMemory(buf, size);

  free(buf);
  fclose(f);
  return s;
}

int IMG_isPNG(SDL_RWops *src) {
  return 0;
}

SDL_Surface* IMG_LoadJPG_RW(SDL_RWops *src) {
  return IMG_Load_RW(src, 0);
}

char *IMG_GetError() {
  return "Navy does not support IMG_GetError()";
}
