#include <msp430.h> 
#include <servo.h>

int n = 90;

/**
 * main.c
 */


/*
Desarrollar un código en lenguaje C++ para msp430f5529 para el control de un servomotor.
el motor deberá estar controlado  por una clase llamada servo
usar 2 botones para controlar el movimiento del motor, un botón mueve el motor en sentido horario con una resolución de 5 grados, el otro en sentido antihorario, misma resolución.
para lo botones usar interrupciones
 */


servo servo(BIT0);
int main(void)
{
    servo.setAngle(n);

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P1DIR &=~ BIT1;      //P1.1 como entrada
    P1REN |= BIT1;
    P1OUT |= BIT1;

    P2DIR &=~ BIT1;      //P1.1 como entrada
    P2REN |= BIT1;
    P2OUT |= BIT1;

    P1IES |= BIT1;
    P1IFG &=~ BIT1;
    P1IE |= BIT1;

    P2IES |= BIT1;
    P2IFG &=~ BIT1;
    P2IE |= BIT1;

    __bis_SR_register(GIE);
    __no_operation();

    return 0;
}

#pragma vector=PORT1_VECTOR//Configuracion de las interrupciones
__interrupt void ISR1(void){
    n+=5;
    servo.setAngle(n);      // SE VA A 0
    P1IFG &=~ BIT1;
}

#pragma vector=PORT2_VECTOR//Configuracion de las interrupciones
__interrupt void ISR2(void){
    n-=5;
    servo.setAngle(n);      // SE VA A 0
    P2IFG &=~ BIT1;
}



    
    
