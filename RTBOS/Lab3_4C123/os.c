// os.c
// Runs on LM4F120/TM4C123/MSP432
// Lab 3 starter file.
// Daniel Valvano
// March 24, 2016

#include <stdint.h>
#include "os.h"
#include "CortexM.h"
#include "BSP.h"


// function definitions in osasm.s
void StartOS(void);

/* File scope functions */
void OS_DecrementSleepTime(void);
void OS_RunPeriodicEvents(void);
void OS_RunPeriodicEventThreads(void);

#define NUMTHREADS  6        // maximum number of threads
#define NUMPERIODIC 2        // maximum number of periodic threads
#define STACKSIZE   100      // number of 32-bit words in stack per thread
struct tcb{
  int32_t *sp;       // pointer to stack (valid for threads not running
  struct tcb *next;  // linked-list pointer
	int32_t *blocked;  // nonzero if blocked on this semaphore
  uint32_t sleep;		// nonzero if this thread is sleeping
//*FILL THIS IN****
};

typedef struct tcb tcbType;
tcbType tcbs[NUMTHREADS];
tcbType *RunPt;
int32_t Stacks[NUMTHREADS][STACKSIZE];

typedef void (*funcPt)(void);

funcPt PeriodicEventThreads[NUMPERIODIC];
uint32_t PeriodicEventThreadsPeriod[NUMPERIODIC];

int8_t EventThreadsIndex;
uint32_t Counter;

// ******** OS_Init ************
// Initialize operating system, disable interrupts
// Initialize OS controlled I/O: periodic interrupt, bus clock as fast as possible
// Initialize OS global variables
// Inputs:  none
// Outputs: none
void OS_Init(void){
  DisableInterrupts();
  BSP_Clock_InitFastest();// set processor clock to fastest speed
  BSP_PeriodicTask_Init(&OS_RunPeriodicEvents, 1000, 6);
	
	EventThreadsIndex = 0;
	Counter = 0;
}

void SetInitialStack(int i){
	
	tcbs[i].sp = &Stacks[i][STACKSIZE - 16];
	
	Stacks[i][STACKSIZE - 1] = 0x01000000;
	Stacks[i][STACKSIZE - 3] = 0x14141414;
	Stacks[i][STACKSIZE - 4] = 0x12121212;
	Stacks[i][STACKSIZE - 5] = 0X03030303;
	Stacks[i][STACKSIZE - 6] = 0X02020202;
	Stacks[i][STACKSIZE - 7] = 0X01010101;
	Stacks[i][STACKSIZE - 8] = 0X00000000;
	Stacks[i][STACKSIZE - 9] = 0X11111111;
	Stacks[i][STACKSIZE - 10] = 0X10101010;
	Stacks[i][STACKSIZE - 11] = 0X09090909;
	Stacks[i][STACKSIZE - 12] = 0X08080808;
	Stacks[i][STACKSIZE - 13] = 0X07070707;
	Stacks[i][STACKSIZE - 14] = 0X06060606;
	Stacks[i][STACKSIZE - 15] = 0X05050505;
	Stacks[i][STACKSIZE - 16] = 0X04040404;
}

//******** OS_AddThreads ***************
// Add six main threads to the scheduler
// Inputs: function pointers to six void/void main threads
// Outputs: 1 if successful, 0 if this thread can not be added
// This function will only be called once, after OS_Init and before OS_Launch
int OS_AddThreads(void(*thread0)(void),
                  void(*thread1)(void),
                  void(*thread2)(void),
                  void(*thread3)(void),
                  void(*thread4)(void),
                  void(*thread5)(void)){
  // **similar to Lab 2. initialize as not blocked, not sleeping****
 int32_t status;

 status = StartCritical();
										
 tcbs[0].next = &tcbs[1];
 tcbs[1].next = &tcbs[2];
 tcbs[2].next = &tcbs[3];
 tcbs[3].next = &tcbs[4];
 tcbs[4].next = &tcbs[5];
 tcbs[5].next = &tcbs[0];
										
 tcbs[0].blocked = 0;
 tcbs[1].blocked = 0;
 tcbs[2].blocked = 0;
 tcbs[3].blocked = 0;
 tcbs[4].blocked = 0;
 tcbs[5].blocked = 0;	
										
	SetInitialStack(0);
	Stacks[0][STACKSIZE - 2] = (int32_t)thread0;
	SetInitialStack(1);
	Stacks[1][STACKSIZE - 2] = (int32_t)thread1;
	SetInitialStack(2);
	Stacks[2][STACKSIZE - 2] = (int32_t)thread2;
	SetInitialStack(3);
	Stacks[3][STACKSIZE - 2] = (int32_t)thread3;
	SetInitialStack(4);
	Stacks[4][STACKSIZE - 2] = (int32_t)thread4;
	SetInitialStack(5);
	Stacks[5][STACKSIZE - 2] = (int32_t)thread5;
	
	RunPt = &tcbs[0];
	EndCritical(status);
										
  return 1;               // successful
}

//******** OS_AddPeriodicEventThread ***************
// Add one background periodic event thread
// Typically this function receives the highest priority
// Inputs: pointer to a void/void event thread function
//         period given in units of OS_Launch (Lab 3 this will be msec)
// Outputs: 1 if successful, 0 if this thread cannot be added
// It is assumed that the event threads will run to completion and return
// It is assumed the time to run these event threads is short compared to 1 msec
// These threads cannot spin, block, loop, sleep, or kill
// These threads can call OS_Signal
// In Lab 3 this will be called exactly twice
int OS_AddPeriodicEventThread(void(*thread)(void), uint32_t period){
	
  uint8_t success = 0;
	
	if(EventThreadsIndex < NUMPERIODIC )
	{
		PeriodicEventThreads[EventThreadsIndex] = thread;
		PeriodicEventThreadsPeriod[EventThreadsIndex] = period;
	
		EventThreadsIndex++;
		
		success = 1;
	}
	
  return success;
}

//******** OS_RunPeriodicEvents ***************
// This function runs during each widetimer5 ISR
// Inputs: none
// Outputs: none
void OS_RunPeriodicEvents(void)
{	
	OS_RunPeriodicEventThreads();
	OS_DecrementSleepTime();
}

//******** OS_PeriodicEventThreads ***************
// This function runs the available periodic events
// Inputs: none
// Outputs: none
void OS_RunPeriodicEventThreads(void)
{	
	uint8_t index = 0;
	
	if((Counter % PeriodicEventThreadsPeriod[index]) == 0)
	{
		PeriodicEventThreads[index]();
	}
	else
	{
		PeriodicEventThreads[++index]();
	}
	
	Counter = (Counter + 1)% PeriodicEventThreadsPeriod[0];
	
}


//******** OS_Launch ***************
// Start the scheduler, enable interrupts
// Inputs: number of clock cycles for each time slice
// Outputs: none (does not return)
// Errors: theTimeSlice must be less than 16,777,216
void OS_Launch(uint32_t theTimeSlice){
  STCTRL = 0;                  // disable SysTick during setup
  STCURRENT = 0;               // any write to current clears it
  SYSPRI3 =(SYSPRI3&0x00FFFFFF)|0xE0000000; // priority 7
  STRELOAD = theTimeSlice - 1; // reload value
  STCTRL = 0x00000007;         // enable, core clock and interrupt arm
  StartOS();                   // start on the first task
}

// runs every ms
void Scheduler(void){
	
	RunPt = RunPt ->next;
	while((RunPt->blocked != 0)||(RunPt -> sleep != 0))
	{
		RunPt = RunPt ->next;
	}
}

//******** OS_Suspend ***************
// Called by main thread to cooperatively suspend operation
// Inputs: none
// Outputs: none
// Will be run again depending on sleep/block status
void OS_Suspend(void){
  STCURRENT = 0;        // any write to current clears it
  INTCTRL = 0x04000000; // trigger SysTick
// next thread gets a full time slice
}

// ******** OS_Sleep ************
// place this thread into a dormant state
// input:  number of msec to sleep
// output: none
// OS_Sleep(0) implements cooperative multitasking
void OS_Sleep(uint32_t sleepTime){
	RunPt->sleep = sleepTime;
	OS_Suspend();
}

// ******** OS_DecrementSleepTime ************
// This function will decrement the sleeping time, if any, 
// of a task 
// input:  none
// output: none
void OS_DecrementSleepTime(void)
{
  uint8_t i;
		
	for(i = 0; i < NUMTHREADS ; i++)
	{
		if(tcbs[i].sleep != 0)
		{
			tcbs[i].sleep = tcbs[i].sleep - 1;
		}
	}
}

// ******** OS_InitSemaphore ************
// Initialize counting semaphore
// Inputs:  pointer to a semaphore
//          initial value of semaphore
// Outputs: none
void OS_InitSemaphore(int32_t *semaPt, int32_t value){
	*semaPt = value;
	
}

// ******** OS_Wait ************
// Decrement semaphore and block if less than zero
// Lab2 spinlock (does not suspend while spinning)
// Lab3 block if less than zero
// Inputs:  pointer to a counting semaphore
// Outputs: none
void OS_Wait(int32_t *semaPt){
	
	DisableInterrupts();
	*semaPt = *semaPt - 1;
	
	if(*semaPt < 0)
	{
		RunPt->blocked = semaPt;
		EnableInterrupts();
		OS_Suspend();
	}
	
	EnableInterrupts();
}

// ******** OS_Signal ************
// Increment semaphore
// Lab2 spinlock
// Lab3 wakeup blocked thread if appropriate
// Inputs:  pointer to a counting semaphore
// Outputs: none
void OS_Signal(int32_t *semaPt){
	
	tcbType *pt;
	DisableInterrupts();
	
	*semaPt = *semaPt + 1;
	
	if(*semaPt <= 0)
	{
		pt = RunPt ->next;
		
		while(pt->blocked != semaPt)
		{
			pt = pt ->next;
		}
		
		pt ->blocked = 0;
	}
	
	EnableInterrupts();
}

#define FSIZE 10    // can be any size
uint32_t PutI;      // index of where to put next
uint32_t GetI;      // index of where to get next
uint32_t Fifo[FSIZE];
int32_t CurrentSize;// 0 means FIFO empty, FSIZE means full
uint32_t LostData;  // number of lost pieces of data

// ******** OS_FIFO_Init ************
// Initialize FIFO.  
// One event thread producer, one main thread consumer
// Inputs:  none
// Outputs: none
void OS_FIFO_Init(void){	
	PutI = GetI = 0;
	OS_InitSemaphore(&CurrentSize, 0);
	LostData = 0;
}

// ******** OS_FIFO_Put ************
// Put an entry in the FIFO.  
// Exactly one event thread puts,
// do not block or spin if full
// Inputs:  data to be stored
// Outputs: 0 if successful, -1 if the FIFO is full
int OS_FIFO_Put(uint32_t data){
	
	if(CurrentSize < FSIZE)
	{
		Fifo[PutI] = data;
		PutI = (PutI + 1)%FSIZE;
		OS_Signal(&CurrentSize);
	}
	else
	{
		LostData++;
		return -1;
	}

  return 0;   // success

}

// ******** OS_FIFO_Get ************
// Get an entry from the FIFO.   
// Exactly one main thread get,
// do block if empty
// Inputs:  none
// Outputs: data retrieved
uint32_t OS_FIFO_Get(void){
	
	uint32_t data;
	
	OS_Wait(&CurrentSize);
	data = Fifo[GetI];
	GetI = (GetI + 1)%FSIZE;

  return data;
}



