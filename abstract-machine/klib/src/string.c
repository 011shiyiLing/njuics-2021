#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  assert(s);
  size_t count = 0;
  while(*s != '\0')
  {
    count ++;
    s ++;
  }
  return count;
}

char *strcpy(char *dst, const char *src) {
  assert(dst && src);
  if(strlen(dst) < strlen(src)) assert(0);

  char *res = dst;
  while (*src != '\0')
  {
    *dst = *src;
     dst++;
     src++;
  }
  *dst = *src;
  return res;
}

char *strncpy(char *dst, const char *src, size_t n) {
  assert(src);
  if(n < 0) assert(0);
  char *res = dst;

  while((n >0) && (*src != '\0'))
  {
    *dst = *src;
     dst++;
     src++;
     n--;
  }
  *dst = *src;

  while(n > 0)
  {
      *dst = '\0';
      dst++;
      n--;
  }
  return res;
}

char *strcat(char *dst, const char *src) {
  char *res = dst;
  const char *t = (const char *)dst;
  size_t n = strlen(t);
  int i = 0;
  while(src[i] != '\0')
  {
    dst[n] = src[i];
    n++;
    i++;
  }
  //dst[n] = '\0';
  return res;
}

int strcmp(const char *s1, const char *s2) {
  assert(s1 && s2);

  while((*s1 != '\0') && (*s1 == *s2))
  {
    s1++;
    s2++;
  }

  if(*s1 < *s2) return -1;
  else if(*s1 > *s2) return 1;
  return 0;
}

int strncmp(const char *s1, const char *s2, size_t n) {
  assert(s1 && s2);
  if(n < 0) assert(0);

  while(n != 0 && (s1 == s2))
  {
    s1++;
    s2++;
    n--;
  }
  int res = *s1 - *s2;
  if(res == 0) return 0;
  else if(res < 0) return -1;
  else if(res > 0) return 1;

  return 0;
}

void *memset(void *s, int c, size_t n) {
  assert(s);
  if(n < 0) assert(0);

  char* temp = (char*) s;
  while (n --)
  {
    *temp = c;
    temp ++;
  }
  return s;
}

void *memmove(void *dst, const void *src, size_t n) {
  assert(dst && src);
  if(n < 0) assert(0);

  void *res = dst;
  char *p1 = (char *)dst;
  char *p2 = (char *)src;
  if(dst > src && (p1 < p2 + n))
  {
    while(n --)
    {
      *(p1 + n) = *(p2 + n);
    }
  }
  else
  {
    while(n --)
    {
      *p1 = *p2;
      p1++;
      p2++;
    }
  }
 
 return res;
}

void *memcpy(void *out, const void *in, size_t n) {
  assert(out && in);
  if(n < 0) assert(0);

  void *res = out;
  char *p1 = (char *)out;
  char *p2 = (char *)in;

  while ( n --)
  {
    *p1 = *p2;
    p1++;
    p2++;
  }

  return res;
}

int memcmp(const void *s1, const void *s2, size_t n) {
  assert(s1 && s2);
  if(n < 0) assert(0);
  char *p1 = (char *)s1;
  char *p2 = (char *)s2;
  int res = 0;
  while(n--)
  {
    res = *p1 - *p2;
    if(res != 0) return res;
    p1++;
    p2++;
  }
  return res;
}

#endif
