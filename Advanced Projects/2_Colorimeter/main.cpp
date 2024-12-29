#include <msp430.h> 
#include <colorSense.h>

colorSense colorSense(BIT4);

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    colorSense.configLeds();
    colorSense.configSerial();
    colorSense.configADC();
    colorSense.configLedsRGB();

    while(1){
        colorSense.ledRed();
        colorSense.mensaje();
        colorSense.ledGreen();
        colorSense.ledBlue();
    }
}









