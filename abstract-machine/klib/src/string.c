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
  
  char *res = dst;
  while (*src != '\0')
  {
    *dst = *src;
     dst++;
     src++;
  }
  *dst = '\0';
  return res;
}

char *strncpy(char *dst, const char *src, size_t n) {
  assert(dst && src);
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
  while(*dst != '\0')
  {
    dst++;
  }
  while(*src != '\0')
  {
    *dst = *src;
    src++;
    dst++;
  }
  //*dst = '\0';
  return res;
}

int strcmp(const char *s1, const char *s2) {
  assert(s1 && s2);

  while((*s1 != '\0') && (*s2 != '\0') && (*s1 == *s2))
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
  int res = 0;

  while(n != 0 )
  {
    res = *s1 - *s2;
    if(res != 0) break;
    s1++;
    s2++;
    n--;
  }
  
  return res;
}

void *memset(void *s, int c, size_t n) {
  assert(s);
  if(n < 0) assert(0);

  char* temp = (char*) s;
  while (n != 0)
  {
    *temp = c;
    temp ++;
    n --;
  }
  *temp = '\0';
  return s;
}

void *memmove(void *dst, const void *src, size_t n) {
  assert(dst && src);
  if(n < 0) assert(0);

  void *res = dst;
  char *p1 = (char *)dst;
  char *p2 = (char *)src;
  //memory overlap
  if(dst > src && (p1 < p2 + n))
  {
    while(n != 0)
    {
      *(p1 + n) = *(p2 + n);
      n--;
    }
  }
  else
  {
    while(n != 0)
    {
      *p1 = *p2;
      p1++;
      p2++;
      n--;
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

  while ( n != 0)
  {
    *p1 = *p2;
    p1++;
    p2++;
    n--;
  }

  return res;
}

int memcmp(const void *s1, const void *s2, size_t n) {
  assert(s1 && s2);
  if(n < 0) assert(0);

  char *p1 = (char *)s1;
  char *p2 = (char *)s2;
  int res = 0;

  while(n != 0)
  {
    res = *p1 - *p2;
    if (res != 0) break;
    p1++;
    p2++;
    n--;
  }

  return res;
}

#endif
