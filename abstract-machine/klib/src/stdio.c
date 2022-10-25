#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)


char *itoa(int value, char *str, int radix)
{
  char reverse[36];
  int sign = value;
  char *p = reverse;
  *p++ = '\0';
  value = (value >= 0) ? value : -value;

  while(value >= 0)
  {
    *p++ = "0123456789abcdef"[value%radix];
    value /= radix;
    if (value == 0) break;
  }
  
  if(sign < 0)
  {
    *p = '-';
  }
  else
  {
    p--;
  }

  while(p >= reverse)
  {
    *str++ = *p--;
  }
  str--;
  return str;
}

int printf(const char *fmt, ...) {
  panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}


int sprintf(char *out, const char *fmt, ...) {
  va_list args;
  char *p;
  char *s;
  int d;

  va_start(args,fmt);
  for(p = out; *fmt; fmt++)
  {
    if(*fmt != '%')
    {
      *p++ = *fmt++;
      continue;
    }

    fmt++;
  
    switch (*fmt)
    {
      case 's':
        *p = '\0';
        s = va_arg(args,char *);
        strcat(p,s);
        p += strlen(p);
        break;
      case 'd':
        d = va_arg(args,int);
        itoa(d,p,10);
        //p += strlen(p);
        break;
      default:
        return -1;
    }
  }

  *p = '\0';
  va_end(args);
  return 0;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
