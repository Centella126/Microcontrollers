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

;||Parte 1||

;     		bis.b   #BIT0,&P1DIR
;outer:
;			xor.b   #BIT0,&P1OUT
;;			mov.w   #0x2710,r5		;Parpadea más lento
;			mov.w   #0x1710,r5		;Parpadea más rápido
;inner:
;			dec.w   r5
;			tst.w   r5
;			jne inner
;			jmp outer
;			NOP



;||Parte 2||

     		bis.b   #BIT7, &P4DIR
;     		bis.b   #BIT7, &P4OUT		;Jala sin esta línea
     		bic.b	#BIT0, &P1DIR
;     		bic.b	#BIT0, &P1OUT
outer:
			xor.b   #BIT0, &P1OUT
			xor.b   #BIT7, &P4OUT
			mov.w   #0xFFFF, r5
inner:
			dec.w   r5
			tst.w   r5
			jne inner
			jmp outer
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
            
