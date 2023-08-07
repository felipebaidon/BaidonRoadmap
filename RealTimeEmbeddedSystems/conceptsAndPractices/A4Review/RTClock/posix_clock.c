/****************************************************************************/
/* Function: nanosleep and POSIX 1003.1b RT clock demonstration             */
/*                                                                          */
/* Sam Siewert - 02/05/2011                                                 */
/*                                                                          */
/****************************************************************************/

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <syslog.h>

#define NSEC_PER_SEC (1000000000)
#define NSEC_PER_MSEC (1000000)
#define NSEC_PER_USEC (1000)
#define ERROR (-1)
#define OK (0)
#define TEST_SECONDS (0)
#define TEST_NANOSECONDS (NSEC_PER_MSEC * 10)

void end_delay_test(void);

//timespec structure contains seconds and nanoseconds in that order
static struct timespec sleep_time = {0, 0};
static struct timespec sleep_requested = {0, 0};
static struct timespec remaining_time = {0, 0};

static unsigned int sleep_count = 0;

pthread_t main_thread;
pthread_attr_t main_sched_attr;
int rt_max_prio, rt_min_prio, min;
struct sched_param main_param;


void print_scheduler(void)
{
   int schedType;

   schedType = sched_getscheduler(getpid());

   switch(schedType)
   {
     case SCHED_FIFO:
           syslog(LOG_USER|LOG_DEBUG,"Pthread Policy is SCHED_FIFO");
           break;
     case SCHED_OTHER:
           syslog(LOG_USER|LOG_DEBUG, "Pthread Policy is SCHED_OTHER");
       break;
     case SCHED_RR:
           syslog(LOG_USER|LOG_DEBUG, "Pthread Policy is SCHED_RR");
           break;
     default:
           syslog(LOG_USER|LOG_DEBUG,"Pthread Policy is UNKNOWN");
   }
}


/*
 *
 * d_ftime
 *
 * This function computes the period duration 
 * between a start time and stop time in seconds
 */
double d_ftime(struct timespec *fstart, struct timespec *fstop)
{
  double dfstart = ((double)(fstart->tv_sec) + ((double)(fstart->tv_nsec) / 1000000000.0));
  double dfstop = ((double)(fstop->tv_sec) + ((double)(fstop->tv_nsec) / 1000000000.0));

  return(dfstop - dfstart); 
}

/*
 *   delta_t
 *
 *   This function handles the values for 
 *   seconds and nanoseconds 
 */
int delta_t(struct timespec *stop, struct timespec *start, struct timespec *delta_t)
{
  int dt_sec=stop->tv_sec - start->tv_sec;
  int dt_nsec=stop->tv_nsec - start->tv_nsec;

  //printf("\ndt calcuation\n");

  // case 1 - less than a second of change
  if(dt_sec == 0)
  {
	  //printf("dt less than 1 second\n");

	  if(dt_nsec >= 0 && dt_nsec < NSEC_PER_SEC)
	  {
	          //printf("nanosec greater at stop than start\n");
		  delta_t->tv_sec = 0;
		  delta_t->tv_nsec = dt_nsec;
	  }

	  else if(dt_nsec > NSEC_PER_SEC)
	  {
	          //printf("nanosec overflow\n");
		  delta_t->tv_sec = 1;
		  delta_t->tv_nsec = dt_nsec-NSEC_PER_SEC;
	  }

	  else // dt_nsec < 0 means stop is earlier than start
	  {
	         syslog(LOG_USER|LOG_DEBUG, "stop is earlier than start");
		 return(ERROR);  
	  }
  }

  // case 2 - more than a second of change, check for roll-over
  else if(dt_sec > 0)
  {
	  //printf("dt more than 1 second\n");

	  if(dt_nsec >= 0 && dt_nsec < NSEC_PER_SEC)
	  {
	          //printf("nanosec greater at stop than start\n");
		  delta_t->tv_sec = dt_sec;
		  delta_t->tv_nsec = dt_nsec;
	  }

	  else if(dt_nsec > NSEC_PER_SEC)
	  {
	          //printf("nanosec overflow\n");
		  delta_t->tv_sec = delta_t->tv_sec + 1;
		  delta_t->tv_nsec = dt_nsec-NSEC_PER_SEC;
	  }

	  else // dt_nsec < 0 means roll over
	  {
	          //printf("nanosec roll over\n");
		  delta_t->tv_sec = dt_sec-1;
		  delta_t->tv_nsec = NSEC_PER_SEC + dt_nsec;
	  }
  }

  return(OK);
}

static struct timespec rtclk_dt = {0, 0};
static struct timespec rtclk_start_time = {0, 0};
static struct timespec rtclk_stop_time = {0, 0};
static struct timespec delay_error = {0, 0};

//#define MY_CLOCK CLOCK_REALTIME
//#define MY_CLOCK CLOCK_MONOTONIC
//#define MY_CLOCK CLOCK_MONOTONIC_RAW
//#define MY_CLOCK CLOCK_REALTIME_COARSE
#define MY_CLOCK CLOCK_MONOTONIC_COARSE

#define TEST_ITERATIONS (100)

/* This function gets the start time before suspending the thread,
   then gets the stop time after the suspention has elapsed*/
void *delay_test(void *threadID)
{
  int idx, rc;
  unsigned int max_sleep_calls=3;
  int flags = 0;
  struct timespec rtclk_resolution;

  sleep_count = 0;

  /*Get clock resolution*/
  if(clock_getres(MY_CLOCK, &rtclk_resolution) == ERROR)
  {
      perror("clock_getres");
      exit(-1);
  }
  else
  {
    syslog(LOG_USER|LOG_DEBUG, "POSIX Clock demo using system RT clock with resolution:%ld secs, %ld microsecs, %ld nanosecs", rtclk_resolution.tv_sec, (rtclk_resolution.tv_nsec/1000), rtclk_resolution.tv_nsec);
  }

  /* run up to TEST_ITERATIONS times */
  for(idx=0; idx < TEST_ITERATIONS; idx++)
  {
      syslog(LOG_USER|LOG_DEBUG,"test %d", idx);

      /* run for 10ms  */
      sleep_time.tv_sec=TEST_SECONDS;
      sleep_time.tv_nsec=TEST_NANOSECONDS;
      sleep_requested.tv_sec=sleep_time.tv_sec;
      sleep_requested.tv_nsec=sleep_time.tv_nsec;

      /* start time stamp */ 
      clock_gettime(MY_CLOCK, &rtclk_start_time);

      /* request 10ms sleep time and repeat if time remains */
      do 
      {
	      /*nanosleep function suspends the execution of the calling thread until the time
	      * specified in the firs argument has paaed or the delivery of a signal 
	      * that terminates the process
	      * if the suspend time is interrumpted by a signal handler the function returns -1
	      * and the remaining time is stored in the second argument*/
          if(rc=nanosleep(&sleep_time, &remaining_time) == 0) break;
         
          sleep_time.tv_sec = remaining_time.tv_sec;
          sleep_time.tv_nsec = remaining_time.tv_nsec;
          sleep_count++;
      } 
      /*ensure time has elapsed and the sleep count has not been exceed*/
      while (((remaining_time.tv_sec > 0) || (remaining_time.tv_nsec > 0))
		      && (sleep_count < max_sleep_calls));

      /*Get time after delay*/
      clock_gettime(MY_CLOCK, &rtclk_stop_time);


      /* Compute the difference between stop time and start time*/
      delta_t(&rtclk_stop_time, &rtclk_start_time, &rtclk_dt);
      /* Compare the obtained sleep time against the requested sleep time */
      delta_t(&rtclk_dt, &sleep_requested, &delay_error);

      end_delay_test();
  }

}

void end_delay_test(void)
{
    double real_dt;
#if 0
  printf("MY_CLOCK start seconds = %ld, nanoseconds = %ld\n", 
         rtclk_start_time.tv_sec, rtclk_start_time.tv_nsec);
  
  printf("MY_CLOCK clock stop seconds = %ld, nanoseconds = %ld\n", 
         rtclk_stop_time.tv_sec, rtclk_stop_time.tv_nsec);
#endif

  real_dt=d_ftime(&rtclk_start_time, &rtclk_stop_time);
  syslog(LOG_USER | LOG_DEBUG, "MY_CLOCK clock DT seconds = %ld, msec=%ld, usec=%ld, nsec=%ld, sec=%6.9lf", 
         rtclk_dt.tv_sec, rtclk_dt.tv_nsec/1000000, rtclk_dt.tv_nsec/1000, rtclk_dt.tv_nsec, real_dt);

  
#if 0
  printf("Requested sleep seconds = %ld, nanoseconds = %ld\n", 
         sleep_requested.tv_sec, sleep_requested.tv_nsec);

  printf("\n");
  printf("Sleep loop count = %ld\n", sleep_count);
#endif
  syslog(LOG_USER|LOG_DEBUG, "MY_CLOCK delay error = %ld, nanoseconds = %ld", 
         delay_error.tv_sec, delay_error.tv_nsec);
}

#define RUN_RT_THREAD

void main(void)
{
   int rc, scope;

   syslog(LOG_DEBUG|LOG_USER,"Before adjustments to scheduling policy");
   print_scheduler();

#ifdef RUN_RT_THREAD
   /*initialize threads attributes to default: scope, stack size, inherit sched, priority, etc */ 
   pthread_attr_init(&main_sched_attr);
   /*Set thread to explict scheduling */
   pthread_attr_setinheritsched(&main_sched_attr, PTHREAD_EXPLICIT_SCHED);
   /*Set schedulig policy to SCHED_FIFO*/
   pthread_attr_setschedpolicy(&main_sched_attr, SCHED_FIFO);

   /* Get max and min priority*/
   rt_max_prio = sched_get_priority_max(SCHED_FIFO);
   rt_min_prio = sched_get_priority_min(SCHED_FIFO);

   /*Set the priority and the scheduling mechanism by the thread indicated by getpid*/
   main_param.sched_priority = rt_max_prio;
   rc=sched_setscheduler(getpid(), SCHED_FIFO, &main_param);

   if (rc)
   {
       syslog(LOG_USER|LOG_DEBUG,"ERROR; sched_setscheduler rc is %d", rc);
       perror("sched_setschduler"); exit(-1);
   }

   syslog(LOG_USER|LOG_DEBUG,"After adjustments to scheduling policy:");
   print_scheduler();

   main_param.sched_priority = rt_max_prio;
   /*Set parameters in main_sched_attr to the ones in main_param*/
   pthread_attr_setschedparam(&main_sched_attr, &main_param);

   /*Create thread pass function delay_test, use parameters 
    * contained in main_sched_attr configured previously*/
   rc = pthread_create(&main_thread, &main_sched_attr, delay_test, (void *)0);

   if (rc)
   {
       syslog(LOG_USER|LOG_DEBUG,"ERROR; pthread_create() rc is %d", rc);
       perror("pthread_create");
       exit(-1);
   }

   /*Wait for the thread to finish*/
   pthread_join(main_thread, NULL);

   /*Destroy the main_sched_attr object, it does not have any
    * effects on the created thread*/
   if(pthread_attr_destroy(&main_sched_attr) != 0)
     perror("attr destroy");
#else
   delay_test((void *)0);
#endif

   printf("TEST COMPLETE\n");
}

