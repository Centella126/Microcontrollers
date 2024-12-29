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

      		bis.b #BIT7, &P4DIR		;Configurar el led 4.7 y el boton 1.1
			bic.b #BIT7, &P4OUT
			bic.b #BIT1, &P1DIR
			bis.b #BIT1, &P1REN
			bis.b #BIT1, &P1OUT

			bic.b #BIT1, &P1IES		;Activar las interrupciones
			bis.b #BIT1, &P1IE
			bic.b #BIT1, &P1IFG


      		bis.b #BIT0, &P1DIR		;Configurar el led 1.0 y el boton 2.1
			bic.b #BIT0, &P1OUT
			bic.b #BIT1, &P2DIR
			bis.b #BIT1, &P2REN
			bis.b #BIT1, &P2OUT

			bic.b #BIT1, &P2IES		;Activar las interrupciones
			bis.b #BIT1, &P2IE
			bic.b #BIT1, &P2IFG
			NOP

			bis.w #LPM3|GIE, SR		;Mandar al mínimo consumo el micro
			NOP

PORT1_ISR:
			xor.b #BIT7, &P4OUT		;Cambiar el estado del led 4.7 y mandar la bandera a 0
			bic.b #BIT1, &P1IFG
			RETI

PORT2_ISR:
			xor.b #BIT0, &P1OUT		;Cambiar el estado del led 1.0 y mandar la bandera a 0
			bic.b #BIT1, &P2IFG
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
            
            .sect	".int47"	;Asignar los bits dentro del vector de interrupciones para cada interrupción
            .short	PORT1_ISR
            .sect	".int42"
            .short	PORT2_ISR
