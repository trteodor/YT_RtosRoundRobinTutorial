.section .text
.cpu cortex-m4
.syntax unified
.global currentPt
.global SysTick_Handler
.global nnOsSchedulerLaunch
.thumb
.type SysTick_Handler, function
.word SysTick_Handler

.type nnOsSchedulerLaunch, function
.word nnOsSchedulerLaunch



@1. odczytaj aktualny wskaznik pod ktorym nalezy umiesicic stack pointer
@2. zapisz aktualny stack pointer do zmiennej "CurrentPt
@3. 


@Contex Switcher!
SysTick_Handler:          @save r0,r1,r2,r3,r12,lr,pc,psr      
                CPSID   I                  
                PUSH    {R4-R11}        @save r4,r5,r6,r7,r8,r9,r10,r11   

                @odczytaj aktualny wskaznik pod ktorym nalezy umiesicic stack pointer
                LDR     R0, =currentPt  @ r0 points to currentPt       
                LDR     R1, [R0]        @ r1= currentPt   

                @2. zapisz aktualny stack pointer do zmiennej "tcbs[i].stackPt"
                STR     SP, [R1]

                @odczytaj    tcbs[i].stackPt.nextPt        
                LDR     R1, [R1,#4]     @ r1 =currentPt->next

                @przypisz nextPointer jako obecny
                STR     R1, [R0]        @currentPt =r1

                @zaladuj SP z nextPointer/ AktualnystackPt
                LDR     SP, [R1]        @SP= currentPt->stackPt 

                @przywroc pozostaly kontekst  
                POP     {R4-R11}           
                CPSIE   I       

                @reszte zrobi HW
                BX      LR 
	
	


nnOsSchedulerLaunch:
            LDR     R0, =currentPt         
            LDR     R2, [R0]       @ R2 =currentPt       
            LDR     SP, [R2]       @SP = currentPt->stackPt    
            POP     {R4-R11}          @ pop 8 registers
            POP     {R0-R3}            @pop 4 registers
            POP     {R12}               @pop 1 register
            ADD     SP,SP,#4            @overjump 1 register (R14 - (LR))
            POP     {LR}                @assign to LR previously assigned PC
            ADD     SP,SP,#4            @Set Msp to Psp
            CPSIE   I                 
            BX      LR          







