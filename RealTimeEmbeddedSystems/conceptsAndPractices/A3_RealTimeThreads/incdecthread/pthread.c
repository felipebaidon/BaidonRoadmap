#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>

#include <sched.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <syslog.h>

#define SCHED_POLICY  SCHED_FIFO
#define NUM_THREADS   128


typedef struct
{
    int threadIdx;
} threadParams_t;


// POSIX thread declarations and scheduling attributes
//
pthread_t threads[128];
threadParams_t threadParams[128];
pthread_attr_t fifo_sched_attr;
struct sched_param fifo_param;

// Unsafe global
int gsum=0;

void *incThread(void *threadp)
{
    int i;
    threadParams_t *threadParams = (threadParams_t *)threadp;

    for(i=0; i<threadParams -> threadIdx ; i++)
    {
        gsum=gsum+i;
    }
    

    syslog(LOG_USER |LOG_DEBUG, "[COURSE:1][ASSIGNMENT:3]: Thread idx=%d, sum[1...%d]=%d Running on core: %d", threadParams->threadIdx, threadParams ->threadIdx, gsum, sched_getcpu());
    gsum = 0;
}

void print_scheduler(void)
{
    int schedType = sched_getscheduler(getpid());
   
    switch(schedType)
    { 
        case SCHED_FIFO:
            syslog(LOG_USER|LOG_DEBUG, "Sched policy is SCHED_FIFO");
            break;
        case SCHED_RR:
            syslog(LOG_USER|LOG_DEBUG, "Sched policy is SCHED_RR");
            break;
        case SCHED_OTHER:
            syslog(LOG_USER|LOG_DEBUG, "Sched policy is SCHED_OTHER");
            break;
        default:
            syslog(LOG_USER|LOG_DEBUG, "Sched policy is UNKNOWN");
    }
}


void set_scheduler(void)
{
     cpu_set_t cpuset;
     int cpuidx, max_priority, rc;

     syslog(LOG_USER|LOG_DEBUG,"INITIAL ");print_scheduler();

    //Set policy to SCHED_FIFO
    pthread_attr_init(&fifo_sched_attr);
    pthread_attr_setinheritsched(&fifo_sched_attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy( &fifo_sched_attr, SCHED_POLICY);

    //Assign pthread to core 1 (the 0th core) on ubuntu virtual machine
    CPU_ZERO(&cpuset);
    cpuidx=(1);
    CPU_SET(cpuidx, &cpuset);
    
    pthread_attr_setaffinity_np(&fifo_sched_attr,  sizeof(cpu_set_t), &cpuset); 
    
    //set the RM priority to MAX
   max_priority = sched_get_priority_max(SCHED_POLICY); 
   fifo_param.sched_priority = max_priority;

   if((rc= sched_setscheduler(getpid(), SCHED_POLICY, &fifo_param)) < 0)
       perror("sched_setscheduler");

    pthread_attr_setschedparam(&fifo_sched_attr, &fifo_param);
    
    syslog(LOG_USER|LOG_DEBUG, "ADJUSTED ");  print_scheduler();
}


int main (int argc, char *argv[])
{
   int rc;
   int i=0;
   
   set_scheduler();

   for( i = 0; i < NUM_THREADS ; i++)
   {
       threadParams[i].threadIdx=i;
       pthread_create(&threads[i],   // pointer to thread descriptor
                  &fifo_sched_attr,     //use attributes set in set_scheduler 
                  incThread, // thread function entry point
                  (void *)&(threadParams[i]) // parameters to pass in
                 );
   }


   for(i=0; i<NUM_THREADS; i++)
     pthread_join(threads[i], NULL);

   printf("TEST COMPLETE\n");
}
