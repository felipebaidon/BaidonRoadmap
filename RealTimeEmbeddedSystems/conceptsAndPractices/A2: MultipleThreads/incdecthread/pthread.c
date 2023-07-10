#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <syslog.h>

//This is the structure definition used to store and refer to the thread id
//it is passed to pthread_create upon request 
typedef struct
{
    int threadIdx;
} threadParams_t;


// POSIX thread declarations and scheduling attributes
//
//arrays to store the created arrays and their parameters, respectively
pthread_t threads[128];
threadParams_t threadParams[128];

// Unsafe global
//This variable will store the sum up to the thread's id for each thread
int gsum=0;

//This function increments a counter up to the threads id number and displays the
//result to syslog

void *incThread(void *threadp)
{
   int i = 0;

   threadParams_t *threadParams = (threadParams_t *)threadp;

    for(i= 0; i < threadParams->threadIdx + 1; i++)
        gsum= gsum + i;
    
    syslog(LOG_CRIT,"[COURSE:1][ASSIGNMENT:2]: Increment thread idx=%d, gsum=%d\n", threadParams->threadIdx, gsum); //this function will print to syslog the parameters used
    gsum = 0;
}


int main (int argc, char *argv[])
{
   int rc;
   int i=0;
   
   system("uname -a | logger");  //The system function is used to pass an instruction from a source file to the command line. The command line instruction in this case will request to print the system information but instead of showing it as a normal command line output it will send the result to the syslog 
   
   for( i= 0; i< 128; i++)
   {
   threadParams[i].threadIdx=i;
   pthread_create(&threads[i],   // pointer to thread descriptor
                  (void *)0,     // use default attributes
                  incThread,
                  (void *)&(threadParams[i]) // parameters to pass in
                 );
   }

   for(i=0; i<128; i++)
     pthread_join(threads[i], NULL); //waits for all of the threads to terminate

   printf("TEST COMPLETE\n");
}
