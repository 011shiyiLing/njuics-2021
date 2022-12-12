#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define SYS_gettimeofday 19
intptr_t _syscall_(intptr_t type, intptr_t a0, intptr_t a1, intptr_t a2);

int main()
{
    struct timeval *timeval;
    struct timezone *timezone;
    int count = 0;
    
    while(1)
    {
       _syscall_(SYS_gettimeofday,&timeval,&timezone,0);
       while(timeval->tv_usec / 100000 < timeval->tv_sec)
       {
        _syscall_(SYS_gettimeofday,&timeval,&timezone,0);
       }
       count ++;
       printf("Hello!%d have passed!\n",count*0.5);
       timeval->tv_usec += 500000;
    }

    return 0;
}