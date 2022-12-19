#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
//#include <NDL.h>

int main()
{
    //struct timeval timeval;
    //struct timezone timezone;
    NDL_Init(0);
    uint32_t sec = 0;
    uint32_t usec = 0;
    //gettimeofday(&timeval,&timezone);
    while(1)
    {
       while(usec / 100000 < sec)
       {
            //gettimeofday(&timeval,&timezone);
            usec = NDL_GetTicks();
            printf("%d\n",usec);
       }
       //gettimeofday(&timeval,&timezone);
       usec = NDL_GetTicks();
       printf("Hello! 0.5s has passed!\n");
       sec += 5;
    }

    return 0;
}