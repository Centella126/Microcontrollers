/*
 * servo.cpp
 *
 *  Created on: 27 sep 2023
 *      Author: Anahi Monserrat
 */


#include <msp430.h>
#include <servo.h>

servo::servo(int pin) : servoPin(pin) {
    // Constructor
    pinMode(servoPin, 1);
    selectPinFunction(servoPin);
    TA1CCR0 = 166;
    TA1CCTL1 = OUTMOD_6;
    TA1CTL = TASSEL_1 + MC_3 + ID_1;
}

void servo::setAngle(int degrees) {
    if (degrees < 0) degrees = 0;
    if (degrees > 180) degrees = 180;

    int dutyCycle = degreeToDuty(degrees);
    TA1CCR1 = dutyCycle;
}

void servo::pinMode(int pin, int mode) {
    if (mode == 1)
        P2DIR |= pin;
    else
        P2DIR &= ~pin;
}

void servo::selectPinFunction(int pin) {
    if (pin == BIT0)
        P2SEL |= BIT0;
    else if (pin == BIT1)
        P2SEL |= BIT1;
}
int servo::degreeToDuty(int deg) {
    static const int maxDuty = 166 * 0.88;
    static const int minDuty = 166 * 0.98;
    static const int maxDegrees = 180;
    static const int minDegrees = 0;

    int duty = (deg - minDegrees) * (maxDuty - minDuty) / (maxDegrees - minDegrees) + minDuty;
    return duty;
}



