#include <msp430.h>
#include <stdio.h>
#include <colorSense.h>


colorSense::colorSense(int pin)
{
    pinModo(pin,1);
}

void colorSense::pinModo(int pin, int modo){
    if(modo == 1)
        P2DIR &=~ pin;
    else
        P2DIR |= pin;
}

void colorSense::configLeds(){
    P4DIR |= BIT7;
    P4OUT &=~ BIT7;
    P1DIR |= BIT0;
    P1OUT &=~ BIT0;
}

void colorSense::configLedsRGB(void){
    P1DIR |= BIT2;
    P1DIR |= BIT4;
    P1DIR |= BIT5;
}


void colorSense::sendData(char *str){
    while(*str != 0){
        while(!(UCA1IFG & UCTXIFG));
        UCA1TXBUF = *str++;
    }
}

void colorSense::configSerial(void){
    P4SEL |= BIT4 + BIT5;
    UCA1CTL1 |= UCSWRST;
    UCA1CTL1 |= UCSSEL_2;
    UCA1BR0 = 9;
    UCA1BR1 = 0;
    UCA1MCTL = UCBRS_1 + UCBRF_0;
    UCA1CTL1 &=~ UCSWRST;
    UCA1IE |= UCRXIE;
}

void colorSense::configADC(void){
    //Pines
    P6DIR &=~ BIT0;
    P6SEL |= BIT0;

    REFCTL0 &=~ REFMSTR;
    ADC12CTL0 |= ADC12ON + ADC12SHT02 + ADC12REFON;
    ADC12CTL0 &=~ ADC12REF2_5V;
    ADC12CTL1 |= ADC12SHP;
    ADC12MCTL0 = ADC12SREF_1;

    __delay_cycles(30000);

    ADC12CTL0 |= ADC12ENC;
}

void colorSense::ledRed(void){
    P1OUT |= BIT2;
    P1OUT &=~ BIT4;
    P1OUT &=~ BIT5;
   __delay_cycles(300000);
}

void colorSense::ledGreen(void){
    P1OUT &=~ BIT2;
    P1OUT |= BIT4;
    P1OUT &=~ BIT5;
    __delay_cycles(300000);
}

void colorSense::ledBlue(void){
    P1OUT &=~ BIT2;
    P1OUT &=~ BIT4;
    P1OUT |= BIT5;
    __delay_cycles(300000);
}

int colorSense::pinAlerta(int adcVal){
    if(adcVal >= 4000){
        P4OUT |= BIT7;
        P1OUT &=~ BIT0;
        __delay_cycles(100000);
        P4OUT &=~ BIT7;
        P1OUT |= BIT0;
        __delay_cycles(50000);

        return 1;
    }

    else{
        P4OUT &=~ BIT7;
        P1OUT &=~ BIT0;

        return 0;
    }
}

void colorSense::mensaje(void){
    char formato[] = "\033[2J \033[H";
    char desconectado[] = "Colorsensor off line";

    ADC12CTL0 |= ADC12SC;
    while(!(ADC12IFG & BIT0));

    int adcVal = ADC12MEM0;

    if(pinAlerta(adcVal) == 1){
        sendData(formato);
        sendData(desconectado);
    }

    else{
        parpadeo(adcVal);
    }
}

void colorSense::parpadeo(int adcVal){
    char formato[] = "\033[2J \033[H";
    int promR;
    long sumR;
    int i;
    char rojo[100];

    sumR = sumR + adcVal;

    //if(i == 10){
    promR = sumR/10;

    sendData(formato);
    //sprintf(rojo, "R = %d\n\r", promR);
    //sendData(rojo);

    sumR = 0;

        //i=0;
    baseDatos(promR);
    //}
    //i=i+1;
}

void colorSense::baseDatos(int valor){
    //char negro[] = "Negro\n\r R=0\n\r G=0\n\r B=0\n\r";
    //char blanco[] = "Blanco\n\r R=255\n\r G=255\n\r B=255\n\r";
    char cafe[] = "Cafe\n\r R=139\n\r G=93\n\r B=54\n\r";
    char azulA[] = "Azul marino\n\r R=31\n\r G=123\n\r B=226\n\r";
    char azulB[] = "Azul celeste\n\r R=0\n\r G=180\n\r B=242\n\r";
    char azulC[] = "Azul claro\n\r R=120\n\r G=177\n\r B=196\n\r";
    char verdeA[] = "Verde oscuro\n\r R=58\n\r G=171\n\r B=101\n\r";
    //char verdeB[] = "Verde brillante\n\r R=112\n\r G=177\n\r B=37\n\r";
    char verdeC[] = "Verde claro\n\r R=150\n\r G=194\n\r B=153\n\r";
    char moradoA[] = "Morado\n\r R=170\n\r G=71\n\r B=175\n\r";
    char gris[] = "Gris\n\r R=224\n\r G=222\n\r B=222\n\r";
    //char lila[] = "Lila\n\r R=83\n\r G=68\n\r B=146\n\r";
    char rojo[] = "Rojo\n\r R=255\n\r G=0\n\r B=0\n\r";
    char naranjaB[] = "Naraja\n\r R=253\n\r G=105\n\r B=46\n\r";
    char moradoB[] = "Morado bajito\n\r R=255\n\r G=197\n\r B=230\n\r";
    //char rosaB[] = "Rosita\n\r R=83\n\r G=68\n\r B=146\n\r";
    char rosaA[] = "Rosa fuerte\n\r R=253\n\r G=46\n\r B=98\n\r";
    //char naranjaA[] = "Naranja fuerte\n\r R=83\n\r G=68\n\r B=146\n\r";
    //char amarilloB[] = "Amarillo bajito\n\r R=83\n\r G=68\n\r B=146\n\r";
    //char amarilloA[] = "Amarillo fuerte\n\r R=83\n\r G=68\n\r B=146\n\r";
/*
    if(valor >=4257 && valor <=4263)    //cajita
        sendData(negro);
*//*
    if(valor >=4536 && valor <=4547)       //rojo
        sendData(blanco);
*/
    if(valor >=4373 && valor <=4393)
    //if(valor >=4238 && valor <=4241)
        sendData(cafe);

    if(valor >=4275 && valor <=4289)
    //if(valor >=4266 && valor <=4272)  //verde claro morado
        sendData(azulA);
/*
    if(valor >=43 && valor <=4412)
    //if(valor >=4290 && valor <=4294)          //café
        sendData(azulB);
*//*
    if(valor >=4479 && valor <=4488)
    //if(valor >=4294 && valor <=4295)      //verde claro
        sendData(azulC);
*/
    if(valor >=4305 && valor <=4345)
    //if(valor >=4251 && valor <=4253)
        sendData(verdeA);
/*
    if(valor >=4485 && valor <=4489)    //Azul claro
        sendData(verdeB);
*/
    if(valor >=4465 && valor <=4480)
    //if(valor >=4267 && valor <=4279)
        sendData(verdeC);
/*
    if(valor >=4383 && valor <=4388)        //café
    //if(valor >=4260 && valor <=4266)
        sendData(moradoA);
*//*
    if(valor >=4481 && valor <=4503)
    //if(valor >=4277 && valor <=4278)      //verde claro
        sendData(gris);
*//*
    if(valor >=4469 && valor <=4479)        //Gris
        sendData(lila);
*//*
    if(valor >=4547 && valor <=4558)
    //if(valor >=4292 && valor <=4293)      //rojo
        sendData(moradoB);
*//*
    if(valor >=4558 && valor <=4561)        //naranja bajito
        sendData(rosaB);
*/
    if(valor >=4520 && valor <=4544)
    //if(valor >=4255 && valor <=4256)
        sendData(rosaA);
/*
    if(valor >=4562 && valor <=4577)        //rojo
    //if(valor >=4430 && valor <=4432)
        sendData(naranjaB);
*//*
    if(valor >=4540 && valor <=4547)        //rojo
        sendData(naranjaA);
*//*
    if(valor >=4562 && valor <=4568)        //naranja bajito
        sendData(amarilloB);
*//*
    if(valor >=4544 && valor <=4548)    //Rojo
        sendData(amarilloA);
*/
    if(valor >=4550 && valor <=4561)
    //if(valor >=4243 && valor <=4244)
        sendData(rojo);

    __delay_cycles(200000);
}


