// os.c
// Runs on LM4F120/TM4C123/MSP432
// Lab 2 starter file.
// Daniel Valvano
// February 20, 2016

#include <stdint.h>
#include "os.h"
#include "../inc/CortexM.h"
#include "../inc/BSP.h"

// function definitions in osasm.s
void StartOS(void);

typedef struct
{
	uint32_t data;
	int32_t status;
}mailbox_t;

mailbox_t OSMailBox;

tcbType tcbs[NUMTHREADS];
tcbType *RunPt;
int32_t Stacks[NUMTHREADS][STACKSIZE];

int32_t Count;
void (*PeriodUserTask1)(void);
uint32_t PeriodicUserTask1Period;
void(*PeriodicUserTask2)(void);
uint32_t PeriodicUserTask2Period;



// ******** OS_Init ************
// Initialize operating system, disable interrupts
// Initialize OS controlled I/O: systick, bus clock as fast as possible
// Initialize OS global variables
// Inputs:  none
// Outputs: none
void OS_Init(void){
  DisableInterrupts();
  BSP_Clock_InitFastest();// set processor clock to fastest speed
  // initialize any global variables as needed
  //***YOU IMPLEMENT THIS FUNCTION*****
	Count = 0;
}

void SetInitialStack(int i){
  tcbs[i].sp = &Stacks[i][STACKSIZE - 16];
	//psr
	Stacks[i][STACKSIZE - 1] = 0x01000000;
	//lr
	Stacks[i][STACKSIZE - 3] = 0x14141414;
	Stacks[i][STACKSIZE - 4] = 0x12121212;
	//R3 - R0
	Stacks[i][STACKSIZE - 5] = 0x03030303;
	Stacks[i][STACKSIZE - 6] = 0x02020202;
	Stacks[i][STACKSIZE - 7] = 0x01010101;
	Stacks[i][STACKSIZE - 8] = 0x00000000;
	//R11 - R4
	Stacks[i][STACKSIZE - 9] = 0x11111111;
	Stacks[i][STACKSIZE - 10] = 0x10101010;
	Stacks[i][STACKSIZE - 11] = 0x09090909;
	Stacks[i][STACKSIZE - 12] = 0x08080808;
	Stacks[i][STACKSIZE - 13] = 0x07070707;
	Stacks[i][STACKSIZE - 14] = 0x0606060;
	Stacks[i][STACKSIZE - 15] = 0x05050505;
	Stacks[i][STACKSIZE - 16] = 0x04040404;

}

//******** OS_AddThreads ***************
// Add four main threads to the scheduler
// Inputs: function pointers to four void/void main threads
// Outputs: 1 if successful, 0 if this thread can not be added
// This function will only be called once, after OS_Init and before OS_Launch
int OS_AddThreads(void(*thread0)(void),
                  void(*thread1)(void),
                  void(*thread2)(void), 
									void(*thread3)(void))
										{
// initialize TCB circular list
// initialize RunPt
// initialize four stacks, including initial PC
 			int32_t status;
			status = StartCritical();
// initialize TCB circular list
			tcbs[0].next = &tcbs[1];
			tcbs[1].next = &tcbs[2];
		  tcbs[2].next = &tcbs[3];
			tcbs[3].next = &tcbs[0];
											
// initialize RunPt
		RunPt = &tcbs[0];
// initialize four stacks, including initial PC
		SetInitialStack(0);
		Stacks[0][STACKSIZE - 2] = (int32_t)thread0;
		SetInitialStack(1);
		Stacks[1][STACKSIZE - 2] = (int32_t)thread1;
	  SetInitialStack(2);
		Stacks[2][STACKSIZE - 2] = (int32_t)thread2;
		SetInitialStack(3);
		Stacks[3][STACKSIZE -2] = (int32_t)thread3;
	  EndCritical(status);
										

  return 1;               // successful
}

//******** OS_AddThreads3 ***************
// add three foregound threads to the scheduler
// This is needed during debugging and not part of final solution
// Inputs: three pointers to a void/void foreground tasks
// Outputs: 1 if successful, 0 if this thread can not be added
int OS_AddThreads3(void(*task0)(void),
                 void(*task1)(void),
                 void(*task2)(void)){							 
			int32_t status;
			status = StartCritical();
// initialize TCB circular list
			tcbs[0].next = &tcbs[1];
			tcbs[1].next = &tcbs[2];
		  tcbs[2].next = &tcbs[0];
// initialize RunPt
		RunPt = &tcbs[0];
// initialize four stacks, including initial PC
		SetInitialStack(0);
		Stacks[0][STACKSIZE - 2] = (int32_t)task0;
		SetInitialStack(1);
		Stacks[1][STACKSIZE - 2] = (int32_t)task1;
	  SetInitialStack(2);
		Stacks[2][STACKSIZE - 2] = (int32_t)task2;
	  EndCritical(status);
									 
  return 1;               // successful
}
                 
//******** OS_AddPeriodicEventThreads ***************
// Add two background periodic event threads
// Typically this function receives the highest priority
// Inputs: pointers to a void/void event thread function2
//         periods given in units of OS_Launch (Lab 2 this will be msec)
// Outputs: 1 if successful, 0 if this thread cannot be added
// It is assumed that the event threads will run to completion and return
// It is assumed the time to run these event threads is short compared to 1 msec
// These threads cannot spin, block, loop, sleep, or kill
// These threads can call OS_Signal
int OS_AddPeriodicEventThreads(void(*thread1)(void), uint32_t period1,
  void(*thread2)(void), uint32_t period2){
  
		PeriodUserTask1 = thread1;
		PeriodicUserTask1Period = period1;
		PeriodicUserTask2 = thread2;
		PeriodicUserTask2Period = period2;
		
  return 1;
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
void Scheduler(void){ // every time slice
  // run any periodic event threads if needed
  // implement round robin scheduler, update RunPt
	
	RunPt = RunPt ->next;
	
	Count = Count% PeriodicUserTask2Period;
	
	if(Count)
	{
		PeriodUserTask1();
	}
	else
	{	
		PeriodicUserTask2();
	}
	
	++Count;
  

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
// Decrement semaphore
// Lab2 spinlock (does not suspend while spinning)
// Lab3 block if less than zero
// Inputs:  pointer to a counting semaphore
// Outputs: none
void OS_Wait(int32_t *semaPt){
	DisableInterrupts();
	while((*semaPt)==0)
	{
		EnableInterrupts();
		DisableInterrupts();
	}
	*semaPt -= 1;
	EnableInterrupts();
}

// ******** OS_Signal ************
// Increment semaphore
// Lab2 spinlock
// Lab3 wakeup blocked thread if appropriate
// Inputs:  pointer to a counting semaphore
// Outputs: none
void OS_Signal(int32_t *semaPt){
	DisableInterrupts();
	(*semaPt) = (*semaPt) + 1;
	EnableInterrupts();
}




// ******** OS_MailBox_Init ************
// Initialize communication channel
// Producer is an event thread, consumer is a main thread
// Inputs:  none
// Outputs: none
void OS_MailBox_Init(void){
	OSMailBox.data = 0;
	OSMailBox.status = 0;

}

// ******** OS_MailBox_Send ************
// Enter data into the MailBox, do not spin/block if full
// Use semaphore to synchronize with OS_MailBox_Recv
// Inputs:  data to be sent
// Outputs: none
// Errors: data lost if MailBox already has data
void OS_MailBox_Send(uint32_t data){
	OSMailBox.data = data;
	OS_Signal(&OSMailBox.status);
}

// ******** OS_MailBox_Recv ************
// retreive mail from the MailBox
// Use semaphore to synchronize with OS_MailBox_Send
// Lab 2 spin on semaphore if mailbox empty
// Lab 3 block on semaphore if mailbox empty
// Inputs:  none
// Outputs: data retreived
// Errors:  none
uint32_t OS_MailBox_Recv(void){ 
	
	uint32_t data;	
	
	OS_Wait(&OSMailBox.status);	
	data = OSMailBox.data;
	OSMailBox.data = 0;

  return data;
}


