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


			bis.b #BIT7, &P4DIR
			bic.b #BIT7, &P4OUT
			bic.b #BIT1, &P1DIR
			bis.b #BIT1, &P1REN
			bis.b #BIT1, &P1OUT

			bis.b #BIT0, &P1DIR
			bic.b #BIT0, &P1OUT
			bic.b #BIT1, &P2DIR
			bis.b #BIT1, &P2REN
			bis.b #BIT1, &P2OUT

			bis.b #BIT1, &P1IES
			bis.b #BIT1, &P1IE
			bic.b #BIT1, &P1IFG ;Garantizar que no hay nada que hacer / Bandera
			NOP

			bis.b #BIT1, &P2IES
			bis.b #BIT1, &P2IE
			bic.b #BIT1, &P2IFG
			NOP


			bis.w #LPM3 | GIE, SR
			NOP


PORT1_ISR:
			xor.b #BIT7, &P4OUT
			bic.b #BIT1, &P1IFG
			xor.b #BIT1, &P1IES
			RETI

PORT2_ISR:
			xor.b #BIT0, &P1OUT
			bic.b #BIT1, &P2IFG
			xor.b #BIT1, &P2IES
			RETI



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
            
            .sect	".int47"		;Asignar los bits dentro del vector de interrupciones para cada interrupción
            .short	PORT1_ISR
            .sect	".int42"
            .short	PORT2_ISR
