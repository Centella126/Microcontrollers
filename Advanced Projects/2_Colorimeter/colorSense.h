#ifndef COLORSENSE_H_
#define COLORSENSE_H_

class colorSense
{
public:
    colorSense(int pin);
    int pinAlerta(int adcVal);
    void configLeds(void);
    void configLedsRGB(void);
    void ledRed(void);
    void ledGreen(void);
    void ledBlue(void);
    void sendData(char *str);
    void configSerial(void);
    void mensaje(void);
    void parpadeo(int adcVal);
    void baseDatos(int valor);
    void configADC(void);

private:
    //virtual ~colorSense();
    void pinModo(int pin, int modo);

};

#endif /* COLORSENSE_H_ */
