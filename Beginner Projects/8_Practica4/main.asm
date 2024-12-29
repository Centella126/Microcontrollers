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

			bis.b	#BIT0, &P1DIR	; Set the LED at P1.0 as output. bic.b	#BIT0, &P1OUT	; Ensure the LED is off to start with. bic.w	#MC0|MC1, &TA0CTL	; Turn off timer TA0.
			bic.b 	#BIT0, &P1OUT ; Ensure the LED is off to start with.
			bic.w 	#MC0|MC1, &TA0CTL ; Turn off timer TA0.
			bis.w	#TACLR, &TA0CTL	; Clear the timer TAR.
			bis.w	#TASSEL__ACLK, &TA0CTL ; Use ACLK (32 KHz). This means 2 seconds a period.

			bic.w	#ID0|ID1, &TA0CTL	; Divide by 1.
			bic.w	#TAIFG, &TA0CTL	; Clear the interrupt flag.
			bis.w	#MC0, &TA0CTL	; Set timer mode to count to TA0CCR0.
			bis.w	#TAIE, &TA0CTL	; Enable timer TA0 interrupts.
			mov.w	#0x3fff, &TA0CCR0	; Make it count up to 1/4 maximum, meaning 1 second on off period. bis.b	#BIT1, &P1REN	; Enable resistor on P1. bis.b	#BIT1, &P1OUT	; Make it a pull up resistor.

			bis.b	#BIT1, &P1REN ; Enable resistor on P1.
			bis.b 	#BIT1, &P1OUT ; Make it a pull up resistor.
			bis.b	#BIT1, &P1IE	; Enable the interrupt on P1.1.
			bic.b	#BIT1, &P1IFG	; Clear the P1.1 interrupt flag.
			bis.b	#BIT1, &P1IES	; Look for high to low transitions only.
			mov.w	#0,	r4	; This will toggle to make a delay for the switch on P1.1.
			nop

			bis.w	#LPM3|GIE, SR	; Put the processor in Low Power Mode 3 (ACLK still active) & Enable Interrupts.
			nop ; No loop necessary. All done by interrupts.

TA0_ISR:
			cmp.w	#TA0IV_TAIFG, &TA0IV	; Check if the timer overflowed.
			jnz	not_correct_flag	; If TA0IV not equal to 0x000e, it wasn't an overflow.
			xor.b	#BIT0, &P1OUT	; Toggle the LED at P1.0.
			cmp	#1, r4	; Check flag to see if we need to debounce the switch.

			jnz	debonuce
			bis.b	#BIT1, &P1IE	; Enable the interrupt on P1.1 if it isn't the
			; first time after the button was pressed.

debonuce:
			mov.w	#0, r4	; Reset the flag on the button switch.

not_correct_flag:
			bic.w	#TAIFG, &TA0CTL	; This is the ISR. Clear interrupt flag
			reti


PORT1_ISR:	; This is the ISR that gets called when
			; you push the S2 (P1.1 button).
			bic.w	#MC0|MC1, &TA0CTL	; Turn off timer TA0.
			bis.w	#ID0|ID1, &TA0CTL	; Divide by 8.
			bis.w	#MC0, &TA0CTL	; Set timer mode to count to TA0CCR0.
			bis.w	#TAIE, &TA0CTL	; Enable timer TA0 interrupts. bic.b	#BIT1, &P1IFG	; Clear the Port 1 interrupt flag. bic.b	#BIT1, &P1IE	; Disable the interrupt on P1.1.
			bic.b 	#BIT1, &P1IFG ; Clear the Port 1 interrupt flag.
			bic.b 	#BIT1, &P1IE ; Disable the interrupt on P1.1.
			mov.w	#1, r4	; This is the flag that the button was just
			; pushed.
			Reti
                                            

;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack
            
;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
			.sect	".reset"	; MSP430 RESET Vector
			.short RESET
			.sect	".int52"	; Timer0_A5 Interrupt Service Routine /* 0xFFE8 Timer0_A5 CC1-4, TA */
			.short TA0_ISR	; page 49, http://www.ti.com/lit/ug/slau157af/slau157af.pdf
			.sect ".int47"	; Port1 Interrupt vector (FFDE).
			.short PORT1_ISR

			.end
