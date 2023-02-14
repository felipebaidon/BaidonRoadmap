;/*****************************************************************************/
; OSasm.s: low-level OS commands, written in assembly                       */
; Runs on LM4F120/TM4C123/MSP432
; Lab 2 starter file
; February 10, 2016
;


        AREA |.text|, CODE, READONLY, ALIGN=2
        THUMB
        REQUIRE8
        PRESERVE8

        EXTERN  RunPt            ; currently running thread
        EXPORT  StartOS
        EXPORT  SysTick_Handler
        IMPORT  Scheduler


SysTick_Handler                ; 1) Saves R0-R3,R12,LR,PC,PSR
    CPSID   I                  ; 2) Prevent interrupt during switch
	PUSH {R4 - R11}			   ; 3) Save remaining registers
	LDR R0, =RunPt			   ;R0 is a pointer to RunPt
	LDR R1, [R0]		       ;R1 = RunPT
	STR SP,[R1]				   ;Store SP to TCB
	PUSH {R0, LR}
	BL Scheduler			   ;Run Scheduler
	;LDR R1, [R1, #4]	       ;sum #4 to R0 to get next
	;STR R1, [R0]		       ;RunPt = R1
	POP {R0, LR}
	LDR R1,[R0]					;R1 = RunPt, new thread
	LDR SP,[R1]                ;SP = RunPt ->SP
	POP {R4 - R11}
    CPSIE   I                  ; 9) tasks run with interrupts enabled
    BX      LR                 ; 10) restore R0-R3,R12,LR,PC,PSR

StartOS
	LDR R0, =RunPt
	LDR R1,[R0]             ;R1 = RunPt
	LDR SP, [R1]
	POP {R4 - R11}
	POP {R0 - R3}
	POP {R12}
	ADD SP, SP, #4          ;skip LR
	POP{LR}					;start location (PC?)
	ADD SP, SP, #4           ; skip PSR
    CPSIE   I                  ; Enable interrupts at processor level
    BX      LR                 ; start first thread
	
    ALIGN
    END
