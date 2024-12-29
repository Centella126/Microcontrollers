/*
 * servo.h
 *
 *  Created on: Sep 25, 2023
 *      Author: rogerchiu
 */

#ifndef SERVO_H_
#define SERVO_H_

class servo
{
public:
    servo(int pin);
    void setAngle(int degrees);
private:
    int servoPin;
    void pinMode(int pin, int mode);
    void selectPinFunction(int pin);
    int degreeToDuty(int deg);
//virtual ~servo();
};

#endif /* SERVO_H_ */
