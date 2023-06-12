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
pthread_t singlethread;
threadParams_t threadParams;


void *helloThread(void *threadp)
{
    int sum=0, i;
    threadParams_t *threadParams = (threadParams_t *)threadp;

    syslog(LOG_CRIT,"[COURSE 1][ASSIGNMENT 1]: Hello World from Thread!"); 
}


int main (int argc, char *argv[])
{

   system("uname -a | logger");   
   syslog(LOG_CRIT,"[COURSE 1][ASSIGNMENT 1]: Hello World from Main!");

   threadParams.threadIdx= 0;

   pthread_create(&singlethread,   // pointer to thread descriptor
                  (void *)0,     // use default attributes
                   helloThread, // thread function entry point
                   (void *)&(threadParams) // parameters to pass in
                     );

   pthread_join(singlethread, NULL);

}
