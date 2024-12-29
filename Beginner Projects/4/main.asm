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

       		bis.b #BIT0, &P1DIR
       		bis.b #BIT0, &P1OUT
   		    bic.w #MC1|MC0, &TA0CTL ; poner a 0 los bits MC0 y MC1, que son los que controlan la forma de conteo, el temporizador está detenido
       		bis.w #TACLR, &TA0CTL ; borrar el contenido del registro del temporizador 0, que el temporizador esté borrado
			bis.w #TASSEL__ACLK, &TA0CTL; elegir la fuente de frecuencia de reloj: AUXILIAR, esta trabaja a 32kHz
			bic.w #ID0|ID1, &TA0CTL; decirle como hacer la división de frecuencia, pone 00 entonces se divide entre 1
			bic.w #TAIFG, &TA0CTL; se borra la bandera
			bis.w #MC0, &TA0CTL ; le decimos que cuente en modo UP hasta alcanzar el tope, en MC1 ya tenía un 0, pongo un 1 en MCO y ya tengo 01 que es UP
			bis.w #TAIE, &TA0CTL; activa la bandera de interrupción TAIE timer A interruption Enable
			mov.w #0x4000, &TA0CCR0; mover un número al registro que será el SET POINT, define cuánto tardan las cosas
			NOP
			bis.w #LPM3|GIE, SR; configurar el sistema en el modo
			NOP ; siempre después de lo de LPM

TA0_ISR:			; crear servicio de rutina porque es una interrupción - Timer A0 Interruption Service Routine
			cmp.w #TA0IV_TAIFG, &TA0IV; ver en el vector de interrupciones si ya hubo una interrupción - TA0InterruptionVector
			jnz IFLAG;
			xor.b #BIT0, &P1OUT;

IFLAG:
			bic.w #TAIFG, &TA0CTL;
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
            
            .sect ".int52" ;es donde se encuentra el timer A0
            .short TA0_ISR
