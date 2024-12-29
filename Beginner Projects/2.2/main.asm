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
			mov.b 	#0x05, R7
			bis.b	#BIT7, &P4DIR			; Puerto Led como salida
			bis.b	#BIT7, &P4OUT
			inc.b R7

a:
			dec.b R7
			tst.b R7
			jne outer
			jmp done
outer:
			xor.b	#BIT7, &P4OUT			; XOR al led. Generar  el parpadeo!
			mov.w	#0xffff, r5				; Al registro r5 le asignamos un valor
inner:
			dec.w	r5						; Decrementa en 1 el valor del registro r5
			tst.w	r5						; Checa si el registro r5 ha llegado a 0
			jne	inner						; Salta si la bandera Z no se ha sacado, es decir, salta si no hay 0
			jmp	outer2						; Salto al comienzo del loop, osea, si el registro ha llegado a 0
outer2:
			xor.b	#BIT7, &P4OUT
			mov.w	#0xffff, r5
inner2:
			dec.w	r5						; Decrementa en 1 el valor del registro r5
			tst.w	r5						; Checa si el registro r5 ha llegado a 0
			jne	inner2						; Salta si la bandera Z no se ha sacado, es decir, salta si no hay 0
			jmp	a
done:
			bic.b	#BIT7, &P4OUT


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
            
