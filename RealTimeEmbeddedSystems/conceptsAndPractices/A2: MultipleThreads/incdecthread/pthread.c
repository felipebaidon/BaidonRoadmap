#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <syslog.h>

typedef struct
{
    int threadIdx;
} threadParams_t;


// POSIX thread declarations and scheduling attributes
//
pthread_t threads[128];
threadParams_t threadParams[128];


// Unsafe global
int gsum=0;

void *incThread(void *threadp)
{
    int i;
    threadParams_t *threadParams = (threadParams_t *)threadp;

    for(i=0; i< threadParams ->threadIdx + 1; i++)
    {
        gsum= gsum+ i;
    }

    syslog(LOG_CRIT, "[COURSE:1][ASSIGNMENT:2] Increment thread idx=%d, gsum=%d\n", threadParams->threadIdx, gsum);
    gsum = 0;
}


int main (int argc, char *argv[])
{
   int i=0;
   system( "uname -a | logger");

   for( i=0; i < 128 ; i++)
   {
   threadParams[i].threadIdx=i;
   pthread_create(&threads[i],   // pointer to thread descriptor
                  (void *)0,     // use default attributes
                  incThread, // thread function entry point
                  (void *)&(threadParams[i]) // parameters to pass in
                 );
   }

   for(i=0; i<128; i++)
     pthread_join(threads[i], NULL);

  
}
