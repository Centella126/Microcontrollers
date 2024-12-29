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

;Botón P1.1, LED P1.1

		bis.b #BIT7, &P4DIR		; Se configura como salida, es el led
		bic.b #BIT1, &P1DIR		; Se configura como entrada, es el botón
		bis.b #BIT1, &P1REN		; Se activa la resistencia de anclaje
		bis.b #BIT1, &P1OUT		; Se declara como Pull UP, la resintencia va a voltaje, hay un 1 lógico

		bic.b #BIT1, &P2DIR
		bis.b #BIT1, &P2REN
		bis.b #BIT1, &P2OUT

		mov.b #0x00, R7			; Le quité el & al registro porque marcaba error

Loop:
		bit.b #BIT1, &P1IN		; Se observa que hay en el registro PxIN, si no está presionado siempre habrá un 1
		jnz Next
		jmp Inc
		NOP
Next:
		bit.b #BIT1, &P2IN
		jnz Loop
		jmp Brilli
		NOP

Inc:
		inc.b R7
		jmp Loop
		NOP




Brilli:
		inc.b R7
a:
		dec.b R7
		tst.b R7
		jne outer
		jmp done
outer:
		xor.b	#BIT7, &P4OUT			; XOR al led. Generar  el parpadeo!
		mov.w	#0xffff, R5				; Al registro r5 le asignamos un valor
inner:
		dec.w	R5						; Decrementa en 1 el valor del registro r5
		tst.w	R5						; Checa si el registro r5 ha llegado a 0
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
            
