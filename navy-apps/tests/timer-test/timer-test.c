#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <NDL.h>

int main()
{
    struct timeval timeval;
    struct timezone timezone;
    //NDL_Init(0);
    uint32_t time;
    uint32_t msec = 500;
    //gettimeofday(&timeval,&timezone);
    while(1)
    {
        //time = NDL_GetTicks();
        gettimeofday(&timeval,&timezone);
        time = timeval.tv_sec*1000 - timeval.tv_usec/1000;
        //printf("%d\n",time);
        while(time < msec)
        {
            gettimeofday(&timeval,&timezone);
            time = timeval.tv_sec*1000 - timeval.tv_usec/1000;
            //time = NDL_GetTicks();
            //printf("%d\n",time);
        }
        //gettimeofday(&timeval,&timezone);
        printf("0.5s has passed!\n");
        msec += 500;
    }
    //NDL_Quit();
    return 0;
}