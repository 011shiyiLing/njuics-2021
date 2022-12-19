#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

int main()
{
    struct timeval *timeval;
    struct timezone *timezone;
    int count = 0;
    
    while(1)
    {
       gettimeofday(timeval,timezone);
       while(timeval->tv_usec / 100000 < timeval->tv_sec)
       {
            gettimeofday(timeval,timezone);
       }
       count ++;
       printf("Hello!%d have passed!\n",count*0.5);
       timeval->tv_usec += 500000;
    }

    return 0;
}