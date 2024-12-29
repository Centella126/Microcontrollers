#include <msp430.h>
#include <servo.h>


/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    servo servo(BIT0);
    servo.setAngle(180);

    return 0;

}
