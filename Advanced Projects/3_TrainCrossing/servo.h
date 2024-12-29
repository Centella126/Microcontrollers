#ifndef SERVO_H_
#define SERVO_H_

class servo
{
public:
    servo(int pin);
    void angulo(int grados);

private:
    int servoPin;
    void pinMode(int pin, int mode);
    void configSel(int pin);
    int cicloTrabajo(int grados);


//virtual ~servo();
};

#endif /* SERVO_H_ */
