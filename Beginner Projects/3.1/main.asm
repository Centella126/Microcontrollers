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

      		bis.b #BIT7, &P4DIR		;Led 4.7 como salida
			bic.b #BIT7, &P4OUT		;Led apagado
			bic.b #BIT1, &P1DIR		;Botón 1.1 como entrada
			bis.b #BIT1, &P1REN		;Habilitar resistencia de anclaje
			bis.b #BIT1, &P1OUT		;Configurar en PULL UP

      		bis.b #BIT0, &P1DIR		;Led 1.0 como salida
			bis.b #BIT0, &P1OUT		;Led encendido

			bic.b #BIT1, &P1IES		;Del perfil bajo al alto, flanco de subida
			bis.b #BIT1, &P1IE		;Habilita la interrupción
			bic.b #BIT1, &P1IFG		;No hay interrupciones pendientes, bandera en 0
			NOP

			bis.w #LPM3|GIE, SR		;OBLIGATORIA ESTA LINEA, manda al micro al mínimo consumo y en espera de alguna interrupción
			NOP

PORT1_ISR:							;Esta etiqueta está reservada para las interrupciones
			xor.b #BIT7, &P4OUT		;Cambia el estado del led 4.7
			xor.b #BIT0, &P1OUT		;Cambia el estado del led 1.0
			bic.b #BIT1, &P1IFG		;Indica ya no hay interrupciones pendientes
			RETI					;Es para retornar de una interrupción...RET es solo para los CALL

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
            
            .sect	".int47"		;Indica donde va a estar el vector interrupción, el ".int47" es propio de PORT1_ISR, lo encontramos en la biblioteca
            .short	PORT1_ISR		;Indica donde se va a realizar la acción
