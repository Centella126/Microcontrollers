;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;
;
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.

;-------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer


;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
		;bis.b #BIT7, &P4DIR; Se configura como salida
		;bis.b #BIT7, &P4OUT; Hay flujo de datos


;Botón P1.1, LED P1.1

		bis.b #BIT7, &P4DIR; Se configura como salida, es el led
		bic.b #BIT1, &P1DIR; Se configura como entrada, es el botón
		bis.b #BIT1, &P1REN
		bic.b #BIT1, &P1OUT; Se declara como Pull UP, la resistencia va a voltaje, hay un 1 lógico

Loop:
		bit.b #BIT1, &P1IN; Se observa que hay en el registro PxIN, si no está presionado siempre habrá un 1
		jnz LedOFF; Se hace un salto para apagar el led
		jmp LedON; Hago un salto para enceder el Led
		NOP

LedON:
		bis.b #BIT7, &P4OUT; Se hace salida de datos, en este caso se enciendo el led
		jmp Loop
		NOP

LedOFF:
		bic.b #BIT7, &P4OUT; Se corta el flujo de datos,se apaga el led
		jmp Loop
		NOP


;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack
            
;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            
