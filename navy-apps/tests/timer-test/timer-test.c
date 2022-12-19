#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

int main()
{
    struct timeval timeval;
    struct timezone timezone;
    
    while(1)
    {
       gettimeofday(&timeval,&timezone);
       while(timeval.tv_usec / 100000 < timeval.tv_sec)
       {
            gettimeofday(&timeval,&timezone);
       }
       gettimeofday(&timeval,&timezone);
       printf("Hello! 0.5s has passed!\n");
       timeval.tv_usec += 500;
    }

    return 0;
}