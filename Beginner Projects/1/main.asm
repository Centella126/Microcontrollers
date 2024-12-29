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

;Parte 1

;            bis.b	#BIT7, &P4DIR ; Set the P1.0 as output for the LED.
;            bis.b	#BIT0, &P1DIR
;
;loop
;			bis.b	#BIT7, &P4OUT ;
; 			bis.b	#BIT0, &P1OUT
;			jmp   	loop
;			NOP


;Parte 2

;			bis.b	#BIT7, &P4DIR	 ; Set the P4.7 as output for the LED.
;            bic.b	#BIT1, &P2DIR    ; set the P2.1 as input for switch
;            bis.b	#BIT1, &P2REN    ; Set pull up for switch on P2.1
;		    bis.b	#BIT1, &P2OUT    ; Make the pull up pull up not     down.forever
;loop:
;			bit.b	#BIT1, &P2IN  ; Check if the button is switched.
;			jnz 	off
;on:
;			bis.b	#BIT7, &P4OUT ; Toggle the LED
;			jmp 	loop
;			NOP
;off:
;			bic.b	#BIT7, &P4OUT ; Turn the LED off to start with.
;			jmp     loop
;			NOP

                                            
;Parte 3

;		bis.b #BIT7, &P4DIR; Se configura como salida, es el led
;		bic.b #BIT1, &P2DIR; Se configura como entrada, es el botón
;		bis.b #BIT1, &P2REN
;		bis.b #BIT1, &P2OUT; Se declara como Pull UP, la resintencia va a voltaje, hay un 1 lógico
;
;		bis.b #BIT0, &P1DIR
;		bic.b #BIT1, &P1DIR
;		bis.b #BIT1, &P1REN
;		bis.b #BIT1, &P1OUT
;Loop:
;		bit.b #BIT1, &P1IN; Se observa que hay en el registro PxIN, si no está presionado siempre habrá un 1
;		jnz LedOFF; Se hace un salto para apagar el led
;		jmp LedON; Hago un salto para enceder el Led
;		NOP
;LedON:
;		bis.b #BIT0, &P1OUT; Se hace salida de datos, en este caso se enciendo el led
;		jmp Loop2
;		NOP
;LedOFF:
;		bic.b #BIT0, &P1OUT; Se corta el flujo de datos,se apaga el led
;		jmp Loop2
;		NOP


;Loop2:
;		bit.b #BIT1, &P2IN
;		jnz LedOFF2
;		jmp LedON2
;		NOP
;LedOFF2:
;		bic.b #BIT7, &P4OUT
;		jmp Loop
;		NOP
;LedON2:
;		bis.b #BIT7, &P4OUT
;		jmp Loop
;		NOP


;Parte4

			bis.b	#BIT7, &P4DIR	 ; Set the P4.7 as output for the LED.
			bis.b	#BIT0, &P1DIR
            bic.b	#BIT1, &P2DIR    ; set the P2.1 as input for switch
            bis.b	#BIT1, &P2REN    ; Set pull up for switch on P2.1
		    bis.b	#BIT1, &P2OUT    ; Make the pull up pull up not     down.forever
loop:
			bit.b	#BIT1, &P2IN  ; Check if the button is switched.
			jnz 	off
on:
			bis.b	#BIT7, &P4OUT ; Toggle the LED
			bis.b	#BIT0, &P1OUT
			jmp 	loop
			NOP
off:
			bic.b	#BIT7, &P4OUT ; Turn the LED off to start with.
			bic.b	#BIT0, &P1OUT
			jmp     loop
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
            
