#include <sdl-file.h>
#include <stdio.h>

SDL_RWops* SDL_RWFromFile(const char *filename, const char *mode) {
  printf("SDL_RWFromFile\n");
  return NULL;
}

SDL_RWops* SDL_RWFromMem(void *mem, int size) {
  printf("SDL_RWFromMem\n");
  return NULL;
}
