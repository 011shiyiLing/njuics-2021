#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)


char *itoa(int value, char *str, int radix)
{
  char reverse[1000000];
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

char *uitoa(uint32_t value,char *str, int radix)
{
  char reverse[100000000];
  char *p = reverse;
  *p++ = '\0';

  while(value != 0)
  {
    *p++ = "0123456789abcdef"[value%radix];
    value /= radix;
    if (value == 0) break;
  }
  p--;

  while(p >= reverse)
  {
    *str++ = *p--;
  }
  return str;

}

/*char *gcvt(double value,int ndigit,char *buf)
{
  char temp[72];
  int int_part;
  double float_part;

  int_part = (int)value;
  float_part = value - int_part;
  if (float_part < 0) float_part = -float_part;

  itoa(int_part,temp,10);

  char *p = temp;
  while(*p != '\0') p++;
  *p = '.';
  
  while(ndigit > 0 && float_part > 0.00000001)
  {
    *p = (int)(float_part*10) + '0';
    p++;
    float_part = (float_part * 10) - (int)(float_part*10);
    ndigit--;
  }

  *p = '\0';
  strcpy(buf,temp);
  return buf;
}*/


int vsprintf(char *out, const char *fmt, va_list ap) {
  char *p;

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
        *p = '\0';
        strcat(p,va_arg(ap,char *));
        p += strlen(p);
        break;
      case 'c':
        *p = va_arg(ap,int);
        p++;
        break;
      case 'd':
        itoa(va_arg(ap,int),p,10);
        p += strlen(p);
        break;
      case 'x':
        uitoa(va_arg(ap,uint32_t),p,16);
        p += strlen(p);
        break;
      default:
        break;
    }
  }

  *p = '\0';
  return 0;
}


int sprintf(char *out, const char *fmt, ...) {
  va_list args;
  char *p;

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
        *p = '\0';
        strcat(p,va_arg(args,char *));
        p += strlen(p);
        break;
      case 'c':
        *p = va_arg(args,int);
        p++;
        break;
      case 'd':
        itoa(va_arg(args,int),p,10);
        p += strlen(p);
        break;
      case 'x':
        uitoa(va_arg(args,uint32_t),p,16);
        p += strlen(p);
        break;
      default:
        assert(0);
    }
  }

  *p = '\0';
  va_end(args);
  return 0;
}

int printf(const char *fmt,...)
{
  char buf[1000];
  va_list args;
  memset(buf,0,sizeof(buf));
  va_start(args,fmt);
  vsprintf(buf,fmt,args);
  va_end(args);

  putstr(buf);
  return 0;
  
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
