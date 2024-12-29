#include <msp430.h>
#include <servo.h>

servo::servo(int pin) : servoPin(pin) {
    // Constructor
    pinMode(servoPin, 1);
    configSel(servoPin);
    TA1CCR0 = 166;
    TA1CCTL1 = OUTMOD_6;
    TA1CTL = TASSEL_1 + MC_3 + ID_1;
}

void servo::angulo(int grados){
    if (grados < 0)
        grados = 0;
    if (grados > 4096)              //Como se está jugando con el rango del ADC, el valor max es 4096
        grados = 4096;

    int giro = cicloTrabajo(grados);
    TA1CCR1 = giro;
}
/*
void servo::angulo(int grados){
    if (grados < 0)
        grados = 0;
    if (grados > 180)
        grados = 180;

    int giro = cicloTrabajo(grados);
    TA1CCR1 = giro;
}

int servo::factorRelacion(int adcVal){
    int grados = adcVal/(4095/180);
    return grados;
}
*/
void servo::pinMode(int pin, int mode){
    if (mode == 1)
        P2DIR |= pin;
    else
        P2DIR &= ~pin;
}

void servo::configSel(int pin){
    if (pin == BIT0)
        P2SEL |= BIT0;
    else if (pin == BIT1)
        P2SEL |= BIT1;
}

int servo::cicloTrabajo(int grados){
    static const int maxTrabajo = 166 * 0.88;
    static const int minTrabajo = 166 * 0.98;
    static const int maxGrados = 0;
    static const int minGrados = 4096;

    int Trabajo = (grados - minGrados) * (maxTrabajo - minTrabajo) / (maxGrados - minGrados) + minTrabajo;
    return Trabajo;
}


