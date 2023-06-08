// os.c
// Runs on LM4F120/TM4C123/MSP432
// A priority/blocking real-time operating system 
// Lab 4 starter file.
// Daniel Valvano
// October 9, 2016
// Hint: Copy solutions from Lab 3 into Lab 4

#include <stdint.h>
#include "os.h"
#include "CortexM.h"
#include "BSP.h"
#include "../inc/tm4c123gh6pm.h"

// function definitions in osasm.s
void StartOS(void);

#define NUMTHREADS  20       // maximum number of threads
#define NUMPERIODIC 2        // maximum number of periodic threads
#define STACKSIZE   100      // number of 32-bit words in stack per thread
struct tcb{
  int32_t *sp;       // pointer to stack (valid for threads not running
  struct tcb *next;  // linked-list pointer
  uint32_t Id;       // 0 means TCB is free
  int32_t *BlockPt;  // nonzero if blocked on this semaphore
  uint32_t Sleep;    // nonzero if this thread is sleeping
  uint32_t Priority; // 0 is highest
};
typedef struct tcb tcbType;
tcbType tcbs[NUMTHREADS];
tcbType *RunPt;
int32_t Stacks[NUMTHREADS][STACKSIZE];
uint32_t NumThread=0;  // number of threads
uint32_t static ThreadId=0;   // thread Ids are sequential from 1

void OS_DecrementSleepTime(void);

// ******** OS_Init ************
// Initialize operating system, disable interrupts
// Initialize OS controlled I/O: periodic interrupt, bus clock as fast as possible
// Initialize OS global variables
// Inputs:  none
// Outputs: none
void OS_Init(void){
  DisableInterrupts();
  BSP_Clock_InitFastest();// set processor clock to fastest speed
  NumThread=0;  // number of threads
  ThreadId=0;   // thread Ids are sequential from 1
// perform any initializations needed, 
// set up periodic timer to run runperiodicevents to implement sleeping
  BSP_PeriodicTask_InitB(&OS_DecrementSleepTime, 1000, 0);
}



//******** OS_AddThread *************** 
// add a foregound thread to the scheduler
// Inputs: pointer to a void/void foreground function
//         priority (0 is highest)
// Outputs: Thread ID if successful, 0 if this thread can not be added
// stack size must be divisable by 8 (aligned to double word boundary)
int OS_AddThread(void(*task)(void), uint32_t priority){ 
	int status;
  int n;         // index to new thread TCB 
  tcbType *NewPt;  // Pointer to nex thread TCB
  tcbType *LastPt; // Pointer to last thread TCB
  int32_t *sp;      // stack pointer
  status = StartCritical();
//  NewPt = malloc(stackSize+10);    // 9 extra bytes needed
  for(n=0; n< NUMTHREADS; n++){
    if(tcbs[n].Id == 0) break; // found it 
    if(n == NUMTHREADS-1){  
      EndCritical(status);
      return 0;          // heap is full
    }
  }
  NewPt = &tcbs[n]; 
  if(NumThread==0){
    RunPt = NewPt;  // points to first thread created
  } else{
    LastPt = RunPt;
    while(LastPt->next != RunPt){
      LastPt = LastPt->next;
    }
    LastPt->next = NewPt; // Pointer to Next  
  }
  NewPt->Priority =  priority;
  NumThread++;
  ThreadId++;
  NewPt->Id = ThreadId;
  NewPt->BlockPt =  0;    // not blocked
  NewPt->Sleep =  0;      // not sleeping

  sp = &Stacks[n][STACKSIZE-1];      // last entry of stack


// derived from uCOS-II
                                            /* Registers stacked as if auto-saved on exception    */
  *(sp)    = (long)0x01000000L;             /* xPSR , T=1                                         */
  *(--sp)  = (long)task;                    /* Entry Point                                        */
  *(--sp)  = (long)0x14141414L;             /* R14 (LR) (init value does not matter)              */
  *(--sp)  = (long)0x12121212L;             /* R12                                                */
  *(--sp)  = (long)0x03030303L;             /* R3                                                 */
  *(--sp)  = (long)0x02020202L;             /* R2                                                 */
  *(--sp)  = (long)0x01010101L;             /* R1                                                 */
  *(--sp)  = (long)0x000000000;             /* R0                                                 */

                                            /* Remaining registers saved on process stack         */
  *(--sp)  = (long)0x11111111L;             /* R11                                                */
  *(--sp)  = (long)0x10101010L;             /* R10                                                */
  *(--sp)  = (long)0x09090909L;             /* R9                                                 */
  *(--sp)  = (long)0x08080808L;             /* R8                                                 */
  *(--sp)  = (long)0x07070707L;             /* R7                                                 */
  *(--sp)  = (long)0x06060606L;             /* R6                                                 */
  *(--sp)  = (long)0x05050505L;             /* R5                                                 */
  *(--sp)  = (long)0x04040404L;             /* R4                                                 */
  NewPt->sp = sp;        // make stack "look like it was previously suspended"
  NewPt->next = RunPt;   // Pointer to first, circular linked list 
  EndCritical(status);
  return 1;
}
// ****OS_Id**********
// returns the Id for the currently running thread
// Input:  none
// Output: Thread Id (1 to NUMTHREADS)
uint32_t OS_Id(void){
  return RunPt->Id;
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
		if(tcbs[i].Sleep != 0)
		{
			tcbs[i].Sleep = tcbs[i].Sleep - 1;
		}
	}
}

//******** OS_Launch ***************
// Start the scheduler, enable interrupts
// Inputs: number of clock cycles for each time slice
// Outputs: none (does not return)
// Errors: theTimeSlice must be less than 16,777,216
void OS_Launch(uint32_t theTimeSlice){
  STCTRL = 0;                  // disable SysTick during setup
  STCURRENT = 0;               // any write to current clears it
  SYSPRI3 =(SYSPRI3&0x0000FFFF)|0xE0E00000; // priority 7, SysTick and PendSV
  STRELOAD = theTimeSlice - 1; // reload value
  STCTRL = 0x00000007;         // enable, core clock and interrupt arm
  StartOS();                   // start on the first task
}

//******** Scheduler *************** 
// This function executes every time slice, it looks at all 
//  threads in the TCB list and chooses the highest priority
//  thread that is not blocked nor sleeping.
void Scheduler(void){
	 tcbType *pt, *bestpt;
	 uint32_t max = 255;
	
	 pt = RunPt;
 	
	do{
			pt = pt -> next;
		
			if(((pt->BlockPt == 0) && (pt ->Sleep == 0)) && (pt->Priority < max))
			{
				max = pt -> Priority;
				bestpt = pt;
			}
		
	  }
	while(RunPt != pt);
		
	RunPt = bestpt;	
  
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
// ******** OS_Kill ************
// kill the currently running thread, release its TCB memory
// input:  none
// output: none
tcbType *previousPt;   // Pointer to previous thread in list before current thread
tcbType *nextPt;       // Pointer to next thread in list after current thread
tcbType *killPt;       // Pointer to thread being killed
// RunPt will point to thread will be killed 
void OS_Kill(void){  // no local variables allowed
  DisableInterrupts();        // atomic
  NumThread--;
  if(NumThread==0){
    for(;;){};     // crash
  }
  RunPt->Sleep = 0xFFFFFFFF;  // can't rerun this thread, it will be dead
  killPt = RunPt;             // kill current thread
	Scheduler();                // RunPt points to thread to run next
//********initially RunPt points to thread to kill********
//               /----\           /----\          /----\
//               |    |  killPt-> |    |          |    |
//               |next----------> |next---------> |next--->
//               |    |           |    |          |    |
//               \----/           \----/          \----/
  killPt->Id = 0;         // mark as free
 
  previousPt = killPt;    // eventually, it will point to previous thread 
  nextPt = killPt->next;  // nextPt points to the thread after 
  while((previousPt->next)!=killPt){
    previousPt = previousPt->next;  
  }
//****previousPt -> one before, RunPt to thread to kill *********
//               /----\           /----\          /----\
// previousPt -> |    |  killPt-> |    | nextPt-> |    |
//               |next----------> |next---------> |next--->
//               |    |           |    |          |    |
//               \----/           \----/          \----/
  previousPt->next = nextPt; // remove from list
//****remove thread which we are killing *********
//               /----\                           /----\
// previousPt -> |    |                  nextPt-> |    |
//               |next--------------------------> |next--->
//               |    |                           |    |
//               \----/                           \----/
  STCURRENT = 0;        // next thread get full slice
  EnableInterrupts();
  INTCTRL = 0x10000000; // trigger pendSV to start next thread
  for(;;){};            // can not return
}
// ******** OS_Sleep ************
// place this thread into a dormant state
// input:  number of msec to sleep
// output: none
// OS_Sleep(0) implements cooperative multitasking
void OS_Sleep(uint32_t sleepTime){
	RunPt->Sleep = sleepTime;
	OS_Suspend();

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
		RunPt->BlockPt = semaPt;
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
		
		while(pt->BlockPt != semaPt)
		{
			pt = pt ->next;
		}
		
		pt ->BlockPt = 0;
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

// *****periodic events****************
int32_t *PeriodicSemaphore0;
uint32_t Period0; // time between signals
int32_t *PeriodicSemaphore1;
uint32_t Period1; // time between signals
void RealTimeEvents(void){int flag=0;
  static int32_t realCount = -10; // let all the threads execute once
  // Note to students: we had to let the system run for a time so all user threads ran at least one
  // before signalling the periodic tasks
  realCount++;
  if(realCount >= 0){
    if((realCount%Period0)==0){
      OS_Signal(PeriodicSemaphore0);
      flag = 1;
    }
    if((realCount%Period1)==0){
      OS_Signal(PeriodicSemaphore1);
      flag=1;
    }
    if(flag){
      OS_Suspend();
    }
  }
}
//****NOTE: this uses PeriodicTimer, not PeriodicTimerC***********
// ******** OS_PeriodTrigger0_Init ************
// Initialize periodic timer interrupt to signal 
// Inputs:  semaphore to signal
//          period in ms
// priority level at 0 (highest)
// Outputs: none
void OS_PeriodTrigger0_Init(int32_t *semaPt, uint32_t period){
  PeriodicSemaphore0 = semaPt;
  Period0 = period;
  BSP_PeriodicTask_Init(&RealTimeEvents,1000,0);
}
// ******** OS_PeriodTrigger1_Init ************
// Initialize periodic timer interrupt to signal 
// Inputs:  semaphore to signal
//          period in ms
// priority level at 0 (highest)
// Outputs: none
void OS_PeriodTrigger1_Init(int32_t *semaPt, uint32_t period){
  PeriodicSemaphore1 = semaPt;
  Period1 = period;
  BSP_PeriodicTask_Init(&RealTimeEvents,1000,0);
}

//****edge-triggered event************
int32_t *edgeSemaphore;
// ******** OS_EdgeTrigger_Init ************
// Initialize button1, PD6, to signal on a falling edge interrupt
// Inputs:  semaphore to signal
//          priority
// Outputs: none
void OS_EdgeTrigger_Init(int32_t *semaPt, uint8_t priority){
  edgeSemaphore = semaPt;
//***IMPLEMENT THIS***
// 1) activate clock for Port D
// allow time for clock to stabilize
// 2) no need to unlock PD6
// 3) disable analog on PD6
// 4) configure PD6 as GPIO
// 5) make PD6 input
// 6) disable alt funct on PD6
// disable pull-up on PD6
// 7) enable digital I/O on PD6  
// (d) PD6 is edge-sensitive 
//     PD6 is not both edges 
//     PD6 is falling edge event 
// (e) clear PD6 flag
// (f) arm interrupt on PD6
// priority on Port D edge trigger is NVIC_PRI0_R	31 – 29
// enable is bit 3 in NVIC_EN0_R
		GPIO_PORTD_IM_R &= ~0x40;
	  GPIO_PORTD_IS_R &= ~0X40;
	  GPIO_PORTD_IBE_R &= ~0x40;
	  GPIO_PORTD_IEV_R &= ~0x40;
		GPIO_PORTD_ICR_R = 0x40;
		GPIO_PORTD_IM_R |= 0x40;
// priority on Port D edge trigger is NVIC_PRI0_R	31 – 29	
		NVIC_PRI0_R  |= (priority << 29);
// enable is bit 3 in NVIC_EN0_R
	  NVIC_EN0_R |= 0x08;
}

// ******** OS_EdgeTrigger_Restart ************
// restart button1 to signal on a falling edge interrupt
// rearm interrupt
// Inputs:  none
// Outputs: none
void OS_EdgeTrigger_Restart(void){
// rearm interrupt 3 in NVIC
	GPIO_PORTD_IM_R |= 0x40;
// clear flag6
	GPIO_PORTD_ICR_R = 0x40;
}
void GPIOPortD_Handler(void){
	if(GPIO_PORTD_RIS_R & 0x40)
	{
		// step 1 acknowledge by clearing flag
		GPIO_PORTD_ICR_R = 0x40;
    // step 2 signal semaphore (no need to run scheduler)
	  OS_Signal(edgeSemaphore);
    // step 3 disarm interrupt to prevent bouncing to create multiple signals
	  GPIO_PORTD_IM_R &= ~0x40;
	}
}


