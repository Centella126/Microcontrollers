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
			bis.b #BIT0, &P1DIR ; Led como salida
			bis.b #BIT0, &P1OUT ; Enciende el led

;Se configura toodo el registro del temporizador
;Configurando del bit menos significativo al más, se tiene:

;Bit 15-10, no se usan
;Bit 9-8 - TASEEL-> selección de fuente de reloj
			;bis.w #TASSEL__ACLK, &TA0CTL ; Use ACLK (32 KHz).
			;Forma alternativa de escribirlo
			bis.w #TASSEL0|TASSEL1, &TA0CTL
			bic.w #TASSEL1, &TA0CTL

;Bit 7-6 - ID -> Entre cuanto se divide el reloj
			;bic.w #ID0|ID1, &TA0CTL ; Divide by 1.
			;Forma alternativa de escribirlo
			bic.w #ID__1, &TA0CTL ; Divide by 1.

;Bit 5-4 - MC -> Modo de control
			;bic.w #MC0|MC1, &TA0CTL ; Lo pone en 00, oséa en Stop
			;bis.w #MC0, &TA0CTL ; Lo pone en 01, en Up - cuenta hasta TAxCCR0
			;Forma alternativa de escribirlo
			bis.w #MC__UP, &TA0CTL

;Bit 3, no se usa
;Bit 2 - TACLR -> Limpia el temporizador
			bis.w #TACLR, &TA0CTL ; Borra el registro

;BIT 1 - TAIE -> Se habilitó la interrupción, pero la importancia de esta es después que las generales
			bis.w #TAIE, &TA0CTL ; Enable timer TA0 interrupts.

;Bit 0 - TAIFG -> Bandera de la interrupción
			bic.w #TAIFG, &TA0CTL ; No hay interrupciones pendientes

			mov.w #0x8000, &TA0CCR0 ; Make it count up to 1/4 maximum, meaning
			;1 second on off period. Define cuanto tardan los ciclos
			NOP

			bis.w #LPM3|GIE, SR ; Put the processor in Low Power Mode 3
			;(ACLK still active) & Enable Interrupts.
			NOP

			; No loop necessary. All done by
			;interrupts.

TA0_ISR:
			cmp.w #TA0IV_TAIFG, &TA0IV ; Check if the timer overflowed.
			jnz not_correct_flag ; If TA0IV not equal to 0x000e, it wasn't an
			;overflow.
			xor.b #BIT0, &P1OUT ; Toggle the LED at P1.0.
not_correct_flag:
			bic.w #TAIFG, &TA0CTL ; This is the ISR. Clear interrupt flag

			reti
                                            

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
            
            .sect   ".int52"
            .short  TA0_ISR

