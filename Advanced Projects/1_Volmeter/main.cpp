#include <msp430.h>
#include <stdio.h>
#include <servo.h>

volatile int adcVal;
int grados;
char formato[] = "\033[2J \033[H";
char nombre[100]= "Anahí Montserrat Vázquez Padilla\n\r";
char fecha[100]= "31 de Octubre del 2023\n\r";
char baja[100]= "Escala baja\n\r";
char media[100]= "Escala media\n\r";
char alta[100]= "Escala alta\n\r";
char notBaja[100]="* 10^-4\n\r";
char notMedia[100]="* 10^-4\n\r";
char notAlta[100]="* 10^-3\n\r";
char voltCero[100]="V = 0";
char data[100];

void sendData(char *str);
void mensajes(void);
int movServo(void);

servo servo(BIT0);

void main(void)
{    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    //Leds de aviso
    P4DIR |= BIT7;
    P4OUT &=~ BIT7;
    P1DIR |= BIT0;
    P1OUT &=~ BIT0;


    //Pines interrupciones
    P1DIR &=~ BIT5;
    P2DIR &=~ BIT2;


    //Pines ADC
    P6DIR &=~ BIT0;
    P6SEL |= BIT0;


    //Configurar la comunicacion serial
    P4SEL |= BIT4 + BIT5;
    UCA1CTL1 |= UCSWRST;
    UCA1CTL1 |= UCSSEL_2;
    UCA1BR0 = 9;
    UCA1BR1 = 0;
    UCA1MCTL = UCBRS_1 + UCBRF_0;
    UCA1CTL1 &=~ UCSWRST;
    UCA1IE |= UCRXIE;


    //Configuracion el ADC
    REFCTL0 &=~ REFMSTR;
    ADC12CTL0 |= ADC12ON + ADC12SHT02 + ADC12REFON;
    ADC12CTL0 &=~ ADC12REF2_5V;
    ADC12CTL1 |= ADC12SHP;
    ADC12MCTL0 = ADC12SREF_1;

    __delay_cycles(30000);

    ADC12CTL0 |= ADC12ENC;

    //Configuración interrupciones
    P1IES &=~ BIT5;
    P1IFG &=~ BIT5;
    P1IE |= BIT5;

    P2IES &=~ BIT2;
    P2IFG &=~ BIT2;
    P2IE |= BIT2;

    P2IES &=~ BIT4;
    P2IFG &=~ BIT4;
    P2IE |= BIT4;

    __bis_SR_register(GIE);
    __no_operation();
}

#pragma vector=PORT1_VECTOR
__interrupt void ISR1(void){
    P4OUT ^= BIT7;
    _delay_cycles(500000);
    mensajes();
    P1IFG &=~ BIT5;
}

#pragma vector=PORT2_VECTOR
__interrupt void ISR2(void){
    P1OUT ^= BIT0;
    _delay_cycles(500000);
    mensajes();
    P2IFG &=~ BIT2;
    P2IFG &=~ BIT4;
}


void mensajes(void){
    while((P2IN & BIT2)){           //Mientras haya voltaje en el pin, se enviará el mensaje de ese rango
        int adcVal = movServo();

        sendData(formato);
        sendData(nombre);
        sendData(fecha);
        int rangoUnoVal=(adcVal*((15000/4095)*(3/1.5))*1.180979196);        //Conversión para que el voltaje que aparece en el COM sea el que se le meta
        sendData(alta);
        sprintf(data, "V = %d", rangoUnoVal);
        sendData(data);
        sendData(notAlta);

        _delay_cycles(1500000);             //Retraso de 1 segundo y medio
    }

    while((P1IN & BIT5)){
        int adcVal = movServo();

        sendData(formato);
        sendData(nombre);
        sendData(fecha);
        int rangoDosVal=(adcVal*((15000/4095)*(3/1.5))*1.180979196);
        sendData(media);
        sprintf(data, "V = %d", rangoDosVal);
        sendData(data);
        sendData(notMedia);

        _delay_cycles(1500000);
    }

    while((P2IN & BIT4)){
        int adcVal = movServo();

        sendData(formato);
        sendData(nombre);
        sendData(fecha);
        int rangoTresVal=(adcVal*((15000/4095)*(0.3/1.5))*1.180979196);
        sendData(baja);
        sprintf(data, "V = %d", rangoTresVal);
        sendData(data);
        sendData(notBaja);

        _delay_cycles(1500000);
    }

    servo.angulo(0);        //Cuando no hay ningún pin activado, el servo regresa a 0
    sendData(formato);
    sendData(nombre);
    sendData(fecha);
    sendData(voltCero);
}

int movServo(void){
    //while(1){
        ADC12CTL0 |= ADC12SC;
        while(!(ADC12IFG & BIT0));
        adcVal = ADC12MEM0;

        //int grados = servo.factorRelacion(adcVal);
        //servo.angulo(grados);
        servo.angulo(adcVal);

        _delay_cycles(500000);


        __no_operation();
        return adcVal;
    //}
}

void sendData(char *str){
    while(*str != 0){
        while(!(UCA1IFG & UCTXIFG));
        UCA1TXBUF = *str++;
    }
}
