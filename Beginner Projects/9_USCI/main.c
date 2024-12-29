#include <msp430.h> 


/**
 * main.c
 */
//char msg[]="\033[2J \033[H";        //Caracteres de escape
char msg1[]="[a] <------ apaga/enciende led rojo\n\r";
char msg2[]="[z] <------ apaga/enciende led verde\n\r";

void sendData(char *str);   //Función prototipo
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	P1DIR |= BIT0;
	P4DIR |= BIT7;
	P1OUT &=~ BIT0;
	P4OUT &=~ BIT7;

	P4SEL |= BIT4 + BIT5;   //Esto configura Tx Rx
	UCA1CTL1 |= UCSWRST;     //Reinicia toda la interface de comunicación serial
	UCA1CTL1 |= UCSSEL_2;   //Configura la frecuencia del reloj
	UCA1BR0 = 9;
	UCA1BR1 = 0;    //Nos dará un Baut Rate de 115200, protocolo 8N1
	UCA1MCTL = UCBRS_1 + UCBRF_0;
	UCA1CTL1 &=~ UCSWRST;
	UCA1IE |= UCRXIE;   //Rx es quien recibe y Tx es quien transmite, se configura primero Rx porque debe haber un protocolo para saber actuar cuando llega información

	sendData(msg1);     //Mandamos a llamar a las funciones
	sendData(msg2);

	__bis_SR_register(LPM0_bits + GIE); //NO USAR líneas de uso de energía aparte de las que se ven en clase
	__no_operation();


	return 0;

}
#pragma vector=USCI_A1_VECTOR     //Directiva del sistema, le decimos donde vamos a ubicar nuestro vector

__interrupt void USCI_A1_ISR(void)
{
    switch(__even_in_range(UCA1IV,4))       //Como se usa la interrupción para transmitir en serie y tiene 4 elementos que trabajar solo se utiliza la 2
                                            //que corresponde directamente a serie, ahí se coloca lo que queremos que se haga con los datos
    {
    case 0: break;
    case 1: break;
    case 2:
        while(!(UCA1IFG & UCTXIFG));        //Que se espere a que estén abajo la bandera de transmisión ...
            if(UCA1RXBUF == 'a'){           //BUF-Bufer -> guarda los datos que van llegando //Que es lo que vamos a hacer con los datos que esten llegando
                P1OUT ^= BIT0;
            }

        while(!(UCA1IFG & UCTXIFG));        //El
            if(UCA1RXBUF == 'z'){
                P4OUT ^= BIT7;
            }
    case 3: break;
    default: break;
    }
}


void sendData(char *str){        //Función para transmitir datos del micro, no puede transmitir datos numéricos
    while(*str != 0){        //Mientras sea dif. de cero aún hay direcciones que queden por recorrer
        while(!(UCA1IFG & UCTXIFG));
        UCA1TXBUF = *str++;  //Le deje el valor del puntero y lo pasa a la siguiente direc.
    }
}

