#include <msp430.h> 

#define BTN BIT1        //BTN=BOTON
#define LED BIT7        //Donde encuentre LED lo va a cambiarpor BIT7

#define BTN2 BIT1        //BTN=BOTON
#define LED2 BIT0

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P4DIR |= LED;
    P1DIR &=~ BTN;
    P1REN |= BTN;
    P1OUT |= BTN;

    P1DIR |= LED2;
    P1OUT |= LED2;
    P2DIR &=~ BTN2;
    P2REN |= BTN2;
    P2OUT |= BTN2;

    while(1){       //Por qué 1???
        if(!(P1IN & BTN)){        //En P1IN está la información del botón, se le aplica un enmascaramiento con el &
            P4OUT |= LED;
        }
        else{
            P4OUT &=~ LED;      //Si no da 1 el enmascaramiento, tons se apaga
        }

        if(!(P2IN & BTN2)){        //En P1IN está la información del botón, se le aplica un enmascaramiento con el &
            P1OUT &=~ LED2;
        }
        else{
            P1OUT |= LED2;      //Si no da 1 el enmascaramiento, tons se apaga
        }
    }


    return 0;
}
