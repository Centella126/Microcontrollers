#include <msp430.h> 

#define BTN BIT1        //BTN=BOTON
#define LED BIT7        //Donde encuentre LED lo va a cambiarpor BIT7

/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	P4DIR |= LED;
	P1DIR &=~ BTN;
	P1REN |= BTN;
	P1OUT |= BTN;

	while(1){       //Por qué 1??? para que sea un ciclo infinito
	    if(!(P1IN & BTN)){        //En P1IN está la información del botón, se le aplica un enmascaramiento con el &
	        P4OUT |= LED;
	    }
	    else{
	        P4OUT &=~ LED;      //Si no da 1 el enmascaramiento, tons se apaga
	    }
	}


	return 0;
}
