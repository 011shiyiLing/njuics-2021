#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)


char *itoa(int value, char *str, int radix)
{
  char reverse[100];
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
  return str;
}

void itoa_printf(int value,int radix)
{
  char reverse[100];
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
    putch(*p);
    p--;
  }
}

int printf(const char *fmt, ...) {
  va_list args;
  char *s;
  int d;

  va_start(args,fmt);
  for(;*fmt;fmt++)
  {
    if(*fmt != '%')
    {
      putch(*fmt);
      continue;
    }

    fmt++;

    switch(*fmt)
    {
      case 's':
        s = va_arg(args,char *);
        while(*s != '\0')
        {
          putch(*s);
          s ++;
        }
        putch(*s);
        break;
      case 'd':
        d = va_arg(args,int);
        itoa_printf(d,10);
        break;
      default:
        return -1;
    }
  }

  va_end(args);
  return 0;
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
      *p = *fmt;
      p++;
      continue;
    }

    fmt++;
  
    switch (*fmt)
    {
      case 's':
        s = va_arg(args,char *);
        strcat(p,s);
        p += strlen(p);
        break;
      case 'd':
        d = va_arg(args,int);
        itoa(d,p,10);
        p += strlen(p);
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
