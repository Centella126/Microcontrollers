#include <msp430.h> 
#include <stdio.h>


/**
 * main.c
 */

long sensor;
int milliseconds;
int distance;

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	P2DIR &=~ BIT5; //La entrada es ECHO, es quien va a recibir la señal del sensor
	P2DIR |= BIT4;  //Tendra la salida de disparo TRIGGER


	P1DIR |= BIT0;  //CONFIGURE INDICADOR LED


	TA0CCTL0 = CCIE;    //Captura y comparacion
	TA0CCR0 = 1000; ///POr??
	TA0CTL = TASSEL_2 + MC_1; //Reloj -> 1 Mhz, MC -> UP

	__bis_SR_register(GIE);
	__no_operation();

	//Vamos a hacer una lectura continua del sensor, lo vamos a dejar leyendo, no sera seccionadao

	while(1){   //while infinito
	    P2IE &=~ BIT5;
        P2OUT |= BIT4;
        __delay_cycles(10);     //El fabricante nos dice que nos eseramemos 10 milisegundos, cada ciclo de retardo dura 1 microsegudno

        P2OUT &=~BIT4;
        P2IFG = 0x00;   //Todas interrupciones habidas yu habiendo se desactivan
        P2IE |= BIT5;
        P2IES &=~ BIT5;      //Me habilita la interrupcion en el flanco de subida
        __delay_cycles(30000);     //Para garantizar que ya hubo un retorno del sensor, se saca calculando la velocidad del sonido ida y vuelta 5 metros (metros máximos del sensor)
        distance = sensor*0.01715;  //Factor de escala de la distancia
        if(distance < 50 && distance != 0)   //la distancia esta en cm
            P1OUT |= BIT0;
        else
            P1OUT &=~ BIT0;
	}

}

#pragma vector=PORT2_VECTOR
__interrupt void PORT2_2_ISR(void){     //Hacer una evaluacion de donde esta viniendo la interrupcion
        if(P2IFG & BIT5){       //Las interrupciones vienen en puertos, ahorita se quiere saber si la interrup viene del puerto 5 o no
                   //Para saber quien encendio la interruup lo discriminamos con una mascara y el bit que qeuremos

            if(!(P2IES & BIT5)){
                milliseconds = 0;
                TA0CTL |= BIT5;        //Borramos el contenido del timer
                P2IES |= BIT5;      //hacemos cambio de perfil
            }
            else{
                sensor = (long)milliseconds*1000 + (long)TA0R; //cast de int a long en esta linea para milliseconds
                                                    //lo que va despues de + no es del fabricante es de un foro
                        //TA0R es recomendado para que no se pierda precision porque al hacer millisec*1000 se pierde distancia del sensor
            }
            P2IFG &=~ BIT5;
        }
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER_A_VECTOR(void){
    milliseconds++; //Cada que el contador llega a 1000,a pasado 1ms y se incrementa la variable milliseconds
    //Vamos a hacer la deteccion por uno de los foquitos indicadores

}



