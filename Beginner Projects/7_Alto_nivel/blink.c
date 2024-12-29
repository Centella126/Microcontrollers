#include <msp430.h>				


/**
 * blink.c
 */
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;		// stop watchdog timer
	P1DIR |= 0x01;					// configure P1.0 as output, Para que se cofigure como salida se pone una OR |, 0x01 es lo equivalente a BIT0

	volatile unsigned int i;		// volatile to prevent optimization, Para las variables que se moverán de valor de manera rápida se usa Volatile unsigned

	while(1)
	{
		P1OUT ^= 0x01;				// toggle P1.0, Está sacando un dato por el puerto ^ es el símbolo del XOR, le mueve el valor de la derecha al puerto de la izq..l
		for(i=10000; i>0; i--);     // delay
	}
}
