#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#define COUNT  10

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

    for(i=0; i<COUNT; i++)
    {
        gsum=gsum+i;
        printf("Increment thread idx=%d, gsum=%d\n", threadParams->threadIdx, gsum);
    }
}


void *decThread(void *threadp)
{
    int i;
    threadParams_t *threadParams = (threadParams_t *)threadp;

    for(i=0; i<COUNT; i++)
    {
        gsum=gsum-i;
        printf("Decrement thread idx=%d, gsum=%d\n", threadParams->threadIdx, gsum);
    }
}


typedef void* (*fnPtr[])(void *);
fnPtr fnPtrArr = {incThread, decThread};

int main (int argc, char *argv[])
{
   int rc;
   int i=0;
   int j = 0;

   for( i= 0; i< 128; i++)
   {
   threadParams[i].threadIdx=i;
   pthread_create(&threads[i],   // pointer to thread descriptor
                  (void *)0,     // use default attributes
                  fnPtrArr[j],
                  (void *)&(threadParams[i]) // parameters to pass in
                 );
   j = (j+1)%2;
   }

   for(i=0; i<128; i++)
     pthread_join(threads[i], NULL);

   printf("TEST COMPLETE\n");
}
