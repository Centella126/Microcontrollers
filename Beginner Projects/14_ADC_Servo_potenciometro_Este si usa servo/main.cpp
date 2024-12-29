#include <msp430.h>
#include <stdio.h>
#include <servo.h>


char msg[]="\n\r";
char msg2[100]="Medidas de voltajes * 10^-4\n\r";
char data[100];
void sendData(char *str);
unsigned int adcVal;
int gradosServo;
int DecadcVal;

servo  servo(BIT0);

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P6DIR &=~ BIT0;
    P6SEL |= BIT0;

    //Configurar la comunicacion serial//
    P4SEL |= BIT4 + BIT5;   //Esto configura Tx Rx
    UCA1CTL1 |= UCSWRST;     //Reinicia toda la interface de comunicación serial
    UCA1CTL1 |= UCSSEL_2;   //Configura la frecuencia del reloj
    UCA1BR0 = 9;
    UCA1BR1 = 0;    //Nos dará un Baut Rate de 115200, protocolo 8N1
    UCA1MCTL = UCBRS_1 + UCBRF_0;
    UCA1CTL1 &=~ UCSWRST;
    UCA1IE |= UCRXIE;
    //\\//\\


    //Configuracion el ADC
    REFCTL0 &=~ REFMSTR;        //hace control en el canal del convertidor analogico digital
    ADC12CTL0 |= ADC12ON + ADC12SHT02 + ADC12REFON;   //BITS de control, ADC ON, Sample and Hold, Referencia ON
    ADC12CTL0 &=~ ADC12REF2_5V;     //Vref=1.5 v
    ADC12CTL1 |= ADC12SHP;      //LA P QUE??
    ADC12MCTL0 = ADC12SREF_1;       //Nos va indicar que la referencia debe venir de la misma tarjeta

    __delay_cycles(30000);

    ADC12CTL0 |= ADC12ENC;      //Se habilita la conversion

    sendData(msg2);

    while(1){
        ADC12CTL0 |= ADC12SC;       //SC -> step conversion, da el disparo para que arranque la conversion
            while(!(ADC12IFG & BIT0));     //vamos a verificar si tenemos una conversion
            adcVal = ADC12MEM0; //El mem0 va a mandar lo que contegan el adcval

            DecadcVal=adcVal*(15000/4096);  //Da el valor del voltaje, tomando 4096 reparticiones, se usa 15000 para que sean valores enteros

            gradosServo = adcVal/22.75;      //22.75 = (rango potenciometro/ rango servo) => 2^12 (adc12) / 180 (180 grados) = 4096/180 = 22.75
            servo.setAngle(gradosServo);

            sendData(msg);
            sprintf(data, "V = %d", adcVal);
            //sprintf(data, "V = %d", DecadcVal);
            sendData(data);
            _delay_cycles(500000); //500000= medio segundo
            __no_operation();
    }
}

void sendData(char *str){
    while(*str != 0){
        while(!(UCA1IFG & UCTXIFG));
        UCA1TXBUF = *str++;
    }
}

