#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

char *itoa(int value, char *str, int radix)
{
  char reverse[36];
  char *p = reverse;
  int sign = (value >= 0)? 1:0;

  value = (value >= 0) ? value : -value;
  *p++ = '\0';
  while(value >= 0)
  {
    *p++ = "0123456789abcdef"[value%radix];
    value /= radix;
    if (value == 0) break;
  }

  if(!sign)
  {
    *p++ = '-';
  }
  else
  {
    p--;
  } 

  while (p >= reverse)
  {
    *str++ = *p--;
  }
  return str;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list args;
  char *s;
  //int d;

  va_start(args,fmt);
  while(*fmt)
  {
    switch (*fmt++)
    {
      case 's':
        s = va_arg(args,char *);
        strcat(out,s);
        break;
      //case 'd':
        //d = va_arg(args,int);
        //out = itoa(d,out,10);
        //break;
      default:
        return -1;
    }
  }
  
 va_end(args);
 return -1;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
