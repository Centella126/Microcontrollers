#include <msp430.h> 

volatile int tmp[50];
volatile int dif[50];
volatile int i=0;

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	__delay_cycles(2000000);
	P1DIR |= BIT2;
	P1OUT &=~ BIT2;
	__delay_cycles(25000);
	P1OUT |= BIT2;
	__delay_cycles(25);
	P1DIR &=~ BIT2;
	P1SEL |= BIT2;      //Se selecciona la otra funcion, aqui se pone en la funcion de

	TA0CTL = TASSEL_2 + MC_3 + TACLR;
	TA0CCTL1 = CCIE + CAP + CM_3 + SCS + CCIS_0;
	__bis_SR_register(GIE);
    __no_operation();

	return 0;

}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void){
    tmp[i] = TA0CCR1;
    i++;
    if(i >= 2)                              //Debe de ser mayor o igual que dos para que haya valores para comparar
        dif[i-1] = tmp[i-1] - tmp[i-2];        //Los valores mayores de 100 valen 1, los menores valen 0

    TA0CCTL1 &=~ CCIFG;
}
/*
0
0
0
1
0
1
1
0
*/

//Estos 8 primeros bits equivalen a 22, como son los priemeros 8 bits, es el porcentaje de humedad
