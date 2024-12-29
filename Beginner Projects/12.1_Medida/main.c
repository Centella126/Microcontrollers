#include <msp430.h>
#include <stdio.h>


/**
 * main.c
 */

long sensor;
int milliseconds;
int distance;

char formato[] = "\033[2J \033[H";
char msg1[]="[c] <------ Distancia en centimetros\n\r";
char msg2[]="[m] <------ Distancia en metros\n\r";
char msg3[]="[n] <------ Distancia en milimetros\n\r";

char data[100];

void sendData(char *str);

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P2DIR &=~ BIT5; //La entrada es ECHO, es quien va a recibir la señal del sensor
    P2DIR |= BIT4;  //Tendra la salida de disparo TRIGGER


    P1DIR |= BIT0;  //CONFIGURE INDICADOR LED


    TA0CCTL0 = CCIE;    //Captura y comparacion
    TA0CCR0 = 1000; //POr??
    TA0CTL = TASSEL_2 + MC_1; //Reloj -> 1 Mhz, MC -> UP



    //Configuracion serial
    P4SEL |= BIT4 + BIT5;   //Esto configura Tx Rx
    UCA1CTL1 |= UCSWRST;     //Reinicia toda la interfaz de comunicación serial
    UCA1CTL1 |= UCSSEL_2;   //Configura la frecuencia del reloj
    UCA1BR0 = 9;
    UCA1BR1 = 0;    //Nos dará un Baut Rate de 115200, protocolo 8N1
    UCA1MCTL = UCBRS_1 + UCBRF_0;
    UCA1CTL1 &=~ UCSWRST;
    UCA1IE |= UCRXIE;

    sendData(msg1);
    sendData(msg2);
    sendData(msg3);

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
        if(distance < 50 && distance != 0){   //la distancia esta en cm
            P1OUT |= BIT0;
        }
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

#pragma vector=USCI_A1_VECTOR     //Directiva del sistema, le decimos donde vamos a ubicar nuestro vector

__interrupt void USCI_A1_ISR(void)
{
    sendData(formato);
    sendData(msg1);
    sendData(msg2);
    sendData(msg3);


    switch(__even_in_range(UCA1IV,4))       //Como se usa la interrupción para transmitir en serie y tiene 4 elementos que trabajar solo se utiliza la 2
                                           //que corresponde directamente a serie, ahí se coloca lo que queremos que se haga con los datos
    {
    case 0: break;
    case 1: break;
    case 2:
        while(!(UCA1IFG & UCTXIFG));        //Que se espere a que estén abajo la bandera de transmisión ...
            if(UCA1RXBUF == 'c'){           //BUF-Bufer -> guarda los datos que van llegando //Que es lo que vamos a hacer con los datos que esten llegando
                sprintf(data,"Distancia en centimetros= %d\n\r", distance);
                sendData(data);
            }

        while(!(UCA1IFG & UCTXIFG));
            int distancemet=0;

            if(UCA1RXBUF == 'm'){
                if(distance < 5)
                    distancemet = 0;
                if(distance >= 5 && distance <= 15)
                    distancemet = 1;
                if(distance > 15 && distance <= 25)
                    distancemet = 2;
                if(distance > 25 && distance <= 35)
                    distancemet = 3;
                if(distance > 35 && distance <= 45)
                    distancemet = 4;
                if(distance > 45)
                    distancemet = 5;

                sprintf(data,"Distancia en metros = 0.%d\n\r", distancemet);
                sendData(data);
            }

        while(!(UCA1IFG & UCTXIFG));
            if(UCA1RXBUF == 'n'){
                int distancemm = distance*10;
                sprintf(data,"Distancia en milimetros = %d\n\r", distancemm);
                sendData(data);
            }

    case 3: break;
    default: break;
    }

/*
    switch(__even_in_range(UCA1IV,4))       //Como se usa la interrupción para transmitir en serie y tiene 4 elementos que trabajar solo se utiliza la 2
                                           //que corresponde directamente a serie, ahí se coloca lo que queremos que se haga con los datos
    {
    case 0: break;
    case 1: break;
    case 2:
        while(!(UCA1IFG & UCTXIFG));        //Que se espere a que estén abajo la bandera de transmisión ...
            if(UCA1RXBUF == 'c'){           //BUF-Bufer -> guarda los datos que van llegando //Que es lo que vamos a hacer con los datos que esten llegando
                sprintf(data,"Distancia en centimetros= %d\n\r", distance);
                sendData(data);
            }

        while(!(UCA1IFG & UCTXIFG));
            if(UCA1RXBUF == 'm'){
                sprintf(data,"Distancia en metros x 10^-2 = %d\n\r", distance);
                sendData(data);
            }

        while(!(UCA1IFG & UCTXIFG));
            if(UCA1RXBUF == 'n'){
                sprintf(data,"Distancia en milimetros x 10^1 = %d\n\r", distance);
                sendData(data);
            }

    case 3: break;
    default: break;
    }
    */
}

void sendData(char *str){        //Función para transmitir datos del micro, no puede transmitir datos numéricos
    while(*str != 0){        //Mientras sea dif. de cero aún hay direcciones que queden por recorrer
        while(!(UCA1IFG & UCTXIFG));
        UCA1TXBUF = *str++;  //Le deje el valor del puntero y lo pasa a la siguiente direc.
    }
}

