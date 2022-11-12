#include "stdint.h"

void  nnOsSchedulerLaunch(void);

#define SYSPRI3         (*((volatile uint32_t *)0xE000ED20))
#define INTCTRL         (*((volatile uint32_t *)0xE000ED04))

/*SysTick struct from cmsis*/
typedef struct
{
  volatile uint32_t CTRL;                   /*!< Offset: 0x000 (R/W)  SysTick Control and Status Register */
  volatile uint32_t LOAD;                   /*!< Offset: 0x004 (R/W)  SysTick Reload Value Register */
  volatile uint32_t VAL;                    /*!< Offset: 0x008 (R/W)  SysTick Current Value Register */
  volatile const  uint32_t CALIB;                  /*!< Offset: 0x00C (R/ )  SysTick Calibration Register */

} SysTick_Type;

#define SCS_BASE            (0xE000E000UL)                            /*!< System Control Space Base Address */
#define SysTick_BASE        (SCS_BASE +  0x0010UL)                    /*!< SysTick Base Address */
#define SysTick             ((SysTick_Type   *)     SysTick_BASE  )   /*!< SysTick configuration struct */

#ifndef   __ASM
  #define __ASM                                  __asm
#endif

#define BUS_FREQ 				16000000
uint32_t MILLIS_PRESCALER;


#define NUM_OF_THREADS  3        
#define STACKSIZE   100      

struct tcb{
  int32_t *stackPt;       
  struct tcb *nextPt;  
};

typedef struct tcb tcbType;
tcbType tcbs[NUM_OF_THREADS];

int32_t TCB_STACK[NUM_OF_THREADS][STACKSIZE];

void osKernelStackInit(int i){
  tcbs[i].stackPt = &TCB_STACK[i][STACKSIZE-16]; 
  TCB_STACK[i][STACKSIZE-1] = 0x01000000;   /*The T (24)bit in EPSR must be "set" always instruction pop may overwrite it 
  												This is reset value - 0x01000000*/ 
}

tcbType *currentPt;

uint8_t osKernelAddThreads(void(*task0)(void),void(*task1)(void),void(*task2)(void))
	{ 

  __ASM volatile ("cpsid i" : : : "memory"); //Diasble irq

  tcbs[0].nextPt = &tcbs[1]; 
  tcbs[1].nextPt = &tcbs[2]; 
  tcbs[2].nextPt = &tcbs[0]; 


  osKernelStackInit(0); 
	TCB_STACK[0][STACKSIZE-2] = (int32_t)(task0); 
  
	osKernelStackInit(1);
	TCB_STACK[1][STACKSIZE-2] = (int32_t)(task1); 
  
	osKernelStackInit(2);
	TCB_STACK[2][STACKSIZE-2] = (int32_t)(task2); 

	currentPt = &tcbs[0]; //Tutaj bedziemy przechowywac adres stosu


__ASM volatile ("cpsie i" : : : "memory"); //Enable irq

		return 1;              
}

void nnOsKernelInit(void)
{
    __ASM volatile ("cpsid i" : : : "memory"); //Diasble irq
	MILLIS_PRESCALER=(BUS_FREQ/1000);

}


void osKernelLaunch(uint32_t quanta)
{
	SysTick->CTRL =0;
	SysTick->VAL=0;
	SysTick->LOAD = (quanta* MILLIS_PRESCALER)-1;


    SYSPRI3 =(SYSPRI3&0x00FFFFFF)|0xE0000000;

	SysTick->CTRL =0x00000007;
	nnOsSchedulerLaunch();
}

 void osThreadYield(void)
 {
	 	SysTick->VAL = 0;
	    INTCTRL = 0x04000000;
 }







