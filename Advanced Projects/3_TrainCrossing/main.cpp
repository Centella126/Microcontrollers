#include <msp430.h>
#include <stdio.h>
#include <servo.h>

long sensorUno;
long sensorDos;
int milliseconds;
int distUno;
int distDos;
int i;

void sendData(char *str);
void configSema(void);
void configSensores(void);
void configIndLed(void);
void configTemp(void);
void configSerial(void);

servo servo(BIT0);

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    configSema();
    configSensores();
    configIndLed();
    configTemp();
    configSerial();

    //Vamos a hacer una lectura continua del sensor, lo vamos a dejar leyendo, no sera seccionadao
    while(1){
        P1OUT &=~ BIT5; //Verde
        P1OUT &=~ BIT4; //Ambar
        P1OUT &=~ BIT3; //Rojo

        //Primero
        P2IE &=~ BIT5;
        P2OUT |= BIT4;
        //Segundo
        P2IE &=~ BIT2;
        P2OUT |= BIT3;
        __delay_cycles(10);     //El fabricante nos dice que nos eseramemos 10 milisegundos, cada ciclo de retardo dura 1 microsegudno

        //Primero
        P2OUT &=~BIT4;
        P2IFG = 0x00;   //Todas interrupciones habidas yu habiendo se desactivan
        P2IE |= BIT5;
        P2IES &=~ BIT5;      //Me habilita la interrupcion en el flanco de subida
        __delay_cycles(30000);     //Para garantizar que ya hubo un retorno del sensor, se saca calculando la velocidad del sonido ida y vuelta 5 metros (metros máximos del sensor)
        distUno = sensorUno*0.01715;  //Factor de escala de la distancia

        if(distUno <= 10 && distUno > 0){   //la distancia esta en cm
            P1OUT |= BIT0;
            servo.angulo(90);
            for(i=5; i>=0; i--){
                P1OUT ^= BIT4;
                __delay_cycles(300000);
            }
            for(i=5; i>=0; i--){
                P1OUT ^= BIT3;
                __delay_cycles(300000);
            }
        }
        else{
            P1OUT &=~ BIT0;
            P1OUT |= BIT5;
        }
        __delay_cycles(500000);

        //Segundo
        P2OUT &=~BIT3;
        P2IFG = 0x00;   //Todas interrupciones habidas yu habiendo se desactivan
        P2IE |= BIT2;
        P2IES &=~ BIT2;      //Me habilita la interrupcion en el flanco de subida
        __delay_cycles(30000);     //Para garantizar que ya hubo un retorno del sensor, se saca calculando la velocidad del sonido ida y vuelta 5 metros (metros máximos del sensor)
        distDos = sensorDos*0.01715;  //Factor de escala de la distancia

        if(distDos <= 15 && distDos > 0){   //la distancia esta en cm
            P1OUT &=~ BIT5;
            P4OUT |= BIT7;
            for(i=5; i>=0; i--){
                P1OUT ^= BIT3;
                __delay_cycles(300000);
            }
            servo.angulo(0);
        }
    }
}


#pragma vector=PORT2_VECTOR
__interrupt void PORT2_2_ISR(void){     //Hacer una evaluacion de donde esta viniendo la interrupcion
        if(P2IFG & BIT5){               //Las interrupciones vienen en puertos, ahorita se quiere saber si la interrup viene del puerto 5 o no
                                        //Para saber quien encendio la interruup lo discriminamos con una mascara y el bit que qeuremos
            if(!(P2IES & BIT5)){
                milliseconds = 0;
                TA0CTL |= BIT5;        //Borramos el contenido del timer
                P2IES |= BIT5;         //hacemos cambio de perfil
            }
            else{
                sensorUno = (long)milliseconds*1000 + (long)TA0R; //cast de int a long en esta linea para milliseconds
                                                               //lo que va despues de + no es del fabricante es de un foro
            }                                                  //TA0R es recomendado para que no se pierda precision porque al hacer millisec*1000 se pierde distancia del sensor
            P2IFG &=~ BIT5;
        }



        if(P2IFG & BIT2){               //Las interrupciones vienen en puertos, ahorita se quiere saber si la interrup viene del puerto 5 o no
                                        //Para saber quien encendio la interruup lo discriminamos con una mascara y el bit que qeuremos
            if(!(P2IES & BIT2)){
                milliseconds = 0;
                TA0CTL |= BIT2;        //Borramos el contenido del timer
                P2IES |= BIT2;         //hacemos cambio de perfil
            }
            else{
                sensorDos = (long)milliseconds*1000 + (long)TA0R; //cast de int a long en esta linea para milliseconds
                                                               //lo que va despues de + no es del fabricante es de un foro
            }                                                  //TA0R es recomendado para que no se pierda precision porque al hacer millisec*1000 se pierde distancia del sensor
            P2IFG &=~ BIT2;
        }
}


#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER_A_VECTOR(void){
    milliseconds++; //Cada que el contador llega a 1000,a pasado 1ms y se incrementa la variable milliseconds
                    //Vamos a hacer la deteccion por uno de los foquitos indicadores
}


void configSema(void){
    P1DIR |= BIT5;  //Verde
    P1OUT &=~ BIT5;
    P1DIR |= BIT4;  //Ambar
    P1OUT &=~ BIT4;
    P1DIR |= BIT3;  //Rojo
    P1OUT &=~ BIT3;
}

void configSensores(void){
    //Entradas del primer sensor
    P2DIR &=~ BIT5; //ECHO
    P2DIR |= BIT4;  //TRIGGER

    //Entradas del segundo sensor
    P2DIR &=~ BIT2; //ECHO
    P2DIR |= BIT3;  //TRIGGER
}

void configIndLed(void){
    P1DIR |= BIT0;
    P1OUT &=~ BIT0;
    P4DIR |= BIT7;
    P4OUT &=~ BIT7;
}

void configTemp(void){
    TA0CCTL0 = CCIE;    //Captura y comparacion
    TA0CCR0 = 1000; //POr??
    TA0CTL = TASSEL_2 + MC_1; //Reloj -> 1 Mhz, MC -> UP
}

void configSerial(void){
    P4SEL |= BIT4 + BIT5;   //Esto configura Tx Rx
    UCA1CTL1 |= UCSWRST;     //Reinicia toda la interfaz de comunicación serial
    UCA1CTL1 |= UCSSEL_2;   //Configura la frecuencia del reloj
    UCA1BR0 = 9;
    UCA1BR1 = 0;    //Nos dará un Baut Rate de 115200, protocolo 8N1
    UCA1MCTL = UCBRS_1 + UCBRF_0;
    UCA1CTL1 &=~ UCSWRST;
    UCA1IE |= UCRXIE;

    __bis_SR_register(GIE);
    __no_operation();
}

void sendData(char *str){
    while(*str != 0){
        while(!(UCA1IFG & UCTXIFG));
        UCA1TXBUF = *str++;
    }
}
