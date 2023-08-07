#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sched.h>
#include <unistd.h>
#include <syslog.h>
#define NUM_THREADS 64
#define NUM_CPUS 8

typedef struct
{
    int threadIdx;
} threadParams_t;


// POSIX thread declarations and scheduling attributes
//
pthread_t threads[NUM_THREADS];
pthread_t mainthread;
pthread_t startthread;
threadParams_t threadParams[NUM_THREADS];

pthread_attr_t fifo_sched_attr;
pthread_attr_t orig_sched_attr;
struct sched_param fifo_param;

#define SCHED_POLICY SCHED_FIFO
#define MAX_ITERATIONS (1000000)


void print_scheduler(void)
{
    int schedType = sched_getscheduler(getpid());

    switch(schedType)
    {
        case SCHED_FIFO:
            syslog(LOG_USER|LOG_DEBUG,"Pthread policy is SCHED_FIFO");
            break;
        case SCHED_OTHER:
            syslog(LOG_USER|LOG_DEBUG,"Pthread policy is SCHED_OTHER");
            break;
        case SCHED_RR:
            syslog(LOG_USER|LOG_DEBUG, "Pthread policy is SCHED_RR");
            break;
        default:
            syslog(LOG_USER|LOG_DEBUG,"Pthread policy is UNKNOWN");
    }
}

/*
 *  This function will change the scheduling mechanism from OTHER
 *  to SCHED_FIFO. it will also set the priority among other thread's
 *  parameters
 */
void set_scheduler(void)
{
    int max_prio, scope, rc, cpuidx;
    cpu_set_t cpuset;

    syslog(LOG_USER|LOG_DEBUG, "INITIAL "); print_scheduler();

    /*initialize thread's attr such as detach state, scope, stack size, policy and priority */
    pthread_attr_init(&fifo_sched_attr);
    /*thread will set its behavior from the attributes passed rather than inherit them from the main thread */
    pthread_attr_setinheritsched(&fifo_sched_attr, PTHREAD_EXPLICIT_SCHED);
    /*Sched policy is SCHED_FIFO*/
    pthread_attr_setschedpolicy(&fifo_sched_attr, SCHED_POLICY);
    CPU_ZERO(&cpuset);
    /*Assign thread to CPU 1*/
    cpuidx=(1);
    CPU_SET(cpuidx, &cpuset);
    pthread_attr_setaffinity_np(&fifo_sched_attr, sizeof(cpu_set_t), &cpuset);

    /*get max priority*/
    max_prio=sched_get_priority_max(SCHED_POLICY);
    fifo_param.sched_priority=max_prio;    
     /* Sets policy and parameters to SCHED_POLICY AND fifo_param of the calling process returned by getpid*/
    if((rc=sched_setscheduler(getpid(), SCHED_POLICY, &fifo_param)) < 0)
        perror("sched_setscheduler");

    /* Sets the scheduling attributes of fifo_sched_attr to the
     * ones in fifo_param */
    pthread_attr_setschedparam(&fifo_sched_attr, &fifo_param);

    syslog(LOG_USER|LOG_DEBUG,"ADJUSTED "); print_scheduler();
}



/* This function will be executed by each thread
 * it will compute the sum up to a thread's id 
 * MAX_ITERATIONS times and compute the elapsed time 
 * in this computation
 */
void *counterThread(void *threadp)
{
    int sum=0, i, rc, iterations;
    threadParams_t *threadParams = (threadParams_t *)threadp;
    pthread_t mythread;
    double start=0.0, stop=0.0;
    struct timeval startTime, stopTime;

    gettimeofday(&startTime, 0);
    start = ((startTime.tv_sec * 1000000.0) + startTime.tv_usec)/1000000.0;


    for(iterations=0; iterations < MAX_ITERATIONS; iterations++)
    {
        sum=0;
        for(i=1; i < (threadParams->threadIdx)+1; i++)
            sum=sum+i;
    }


    gettimeofday(&stopTime, 0);
    stop = ((stopTime.tv_sec * 1000000.0) + stopTime.tv_usec)/1000000.0;

    syslog(LOG_USER |LOG_DEBUG, "Thread idx=%d, sum[0...%d]=%d, running on CPU=%d, start=%lf, stop=%lf", 
           threadParams->threadIdx,
           threadParams->threadIdx, sum, sched_getcpu(),
           start, stop);
}


/*
 * This is the main thread, it will create and
 * wait for the rest of the threads to finish
 */
void *starterThread(void *threadp)
{
   int i, rc;

   syslog(LOG_USER|LOG_DEBUG, "starter thread running on CPU=%d", sched_getcpu());

   for(i=0; i < NUM_THREADS; i++)
   {
       threadParams[i].threadIdx=i;

       pthread_create(&threads[i],   // pointer to thread descriptor
                      &fifo_sched_attr,     // use FIFO RT max priority attributes
                      counterThread, // thread function entry point
                      (void *)&(threadParams[i]) // parameters to pass in
                     );

   }

   for(i=0;i<NUM_THREADS;i++)
       pthread_join(threads[i], NULL);

}


int main (int argc, char *argv[])
{
   int rc;
   int i, j;
   cpu_set_t cpuset;

   set_scheduler();

   CPU_ZERO(&cpuset);

   /* Returns the ID of the calling thread */
   mainthread = pthread_self();

   /* Check the affinity mask assigned to the thread*/ 
   rc = pthread_getaffinity_np(mainthread, sizeof(cpu_set_t), &cpuset);
   if (rc != 0)
       perror("pthread_getaffinity_np");
   else
   {
	   /* Print the CPU in which the main thread is running*/
       syslog( LOG_USER|LOG_DEBUG, "main thread running on CPU=%d, CPUs =", sched_getcpu());

       /*Check if the possibles CPUs (1024)  are members of the cpuset, print the ones contained in cpuset*/
       for (j = 0; j < CPU_SETSIZE; j++)
           if (CPU_ISSET(j, &cpuset))
              syslog(LOG_USER|LOG_DEBUG, " %d", j);

   }

   pthread_create(&startthread,   // pointer to thread descriptor
                  &fifo_sched_attr,     // use FIFO RT max priority attributes
                  starterThread, // thread function entry point
                  (void *)0 // parameters to pass in
                 );

   pthread_join(startthread, NULL);

   printf("\nTEST COMPLETE\n");
}
