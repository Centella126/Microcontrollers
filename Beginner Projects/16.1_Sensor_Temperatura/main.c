#include <msp430.h>
#include <stdio.h>

#define TST(x,y)    (x&(y))     //Test de x y, es el amperson de x y y
#define SET(x,y)    (x|=(y))    //SET es igual a poner a 1, x es el puerto y y es el bit
#define CLR(x,y)    (x&=~(y))   //CLR es igual a poner a 0, estas son redefiniciones
#define TOG(x,y)    (x^=(y))    //XOR, de 1 a 0, de 0 a 1
#define DPIN    BIT2        //A donde van a viajar los datos


unsigned char formato[] = "\033[2J \033[H";
unsigned char msg[] = "Temperatura";
unsigned char msg2[] = "Humedad";
unsigned char msg3[] = "%";
unsigned char msg4[] = "°C";
unsigned char msg5[] = ".";
unsigned char msg6[] = "\r\n";

unsigned int HH;
unsigned int HL;
unsigned int TH;
unsigned int TL;
unsigned int checksum;
unsigned int Packet[5];

unsigned char volatile TOUT;
unsigned char volatile STIMER;

char data1[2];
char data2[2];

//Prototipos de las funciones
void serialInit(void);
void sendData(char *str);
void sensorDHT11init(void);
void signalStart(void);
unsigned char checkResponse(void);
unsigned int readByte(void);
unsigned int readPacket(unsigned int *data);
unsigned int checkSum(unsigned int *data);

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    serialInit();
    sensorDHT11init();      //permite inicializar el sensor
    while(1){
        readPacket(Packet);     //lee los 40 bits para establecer los numeros que queremos
        HH = Packet[0];         //Parte entera hum
        HL = Packet[1];         //Parte decimal hum
        TH = Packet[2];         //Parte entera temp
        TL = Packet[3];             //Parte decimal tem
        checksum = Packet[4];       //se alojan los grupos de 8 bits
        SET(TA0CTL,TACLR);       //se activa el timer
        SET(TA0CCTL0,0x10);      //se activa el bit x10 al registro
        TA0CCR0=5000;           //se el pasas el valor de la cuenta que va a realizar
        STIMER=0;

        sendData(formato);
        sendData(msg2);
        sprintf(data1,"%d",HH);
        sendData(data1);
        sendData(msg5);
        sprintf(data2,"%d",HL);
        sendData(data2);
        sendData(msg3);
        sendData(msg6);

        sendData(msg);
        sprintf(data1,"%d",TH);
        sendData(data1);
        sendData(msg5);
        sprintf(data2,"%d",TL);
        sendData(data2);
        sendData(msg4);
        sendData(msg6);
        __delay_cycles(500000);
    }
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void CCR0_ISR(void){
    STIMER++;
    TOUT= 1;
    TOG(P1OUT,BIT0);
    CLR(TA0CCTL0,CCIFG);
}

void serialInit(void){
    P4SEL = BIT5 + BIT4;
    SET(UCA1CTL1,UCSWRST);
    SET(UCA1CTL1,UCSSEL_2);
    UCA1BR0 = 9;
    UCA1BR1 = 0;
    UCA1MCTL = UCBRS_1 + UCBRF_0;
    CLR(UCA1CTL1,UCSWRST);
}

void sendData (char *str){
    while(*str != 0){
        while(!(UCA1IFG & UCTXIFG));
        UCA1TXBUF = *str++;
    }
}

void sensorDHT11init(void){
    SET(P1OUT,BIT0);
    SET(P1DIR,BIT0);
    TA0CCR0 = 5000;
    SET(TA0CCTL0,CCIE);
    TA0CTL = TASSEL_2 + ID_2 + MC_1 + TACLR;
    __bis_SR_register(GIE);
    __no_operation();
}
void signalStart(void){      //Este desmadre es un protocolo de inicializacion, sin esto no charcha
    SET(P1DIR,DPIN);
    CLR(P1OUT,DPIN);
    __delay_cycles(25000);      //25 ms
    SET(P1OUT,DPIN);
    __delay_cycles(25);     //25us
    CLR(P1DIR,DPIN);
    SET(P1SEL,DPIN);
}

unsigned char checkResponse(void){      //bandera para ver si ya respondio el micro
    TOUT = 0;
    SET(TA0CTL,TACLR);
    TA0CCR0 = 25;
    SET(TA0CCTL0,CCIE);
    while(!(TST(P1IN,DPIN))&&!TOUT);        //Espera a que haya un dato
    if(TOUT){               //Si TOUT=1 , return = 0
        return 0;
    }else{
        SET(TA0CTL,TACLR);
        SET(TA0CTL,CCIE);
        while((TST(P1IN,DPIN))&&!TOUT);
        if(TOUT){
            return 0;
        }else{
            CLR(TA0CCTL0,CCIE);
            return 1;
        }
    }
}
unsigned int readByte(void){
    TOUT = 0;
    unsigned int m = 0;
    unsigned int i;
    CLR(TA0CCTL0,CCIE);
    for(i = 8; i > 0; i--){
        while(!(TST(P1IN,DPIN)));
        SET(TA0CTL,TACLR);
        SET(TA0CTL,0x10);
        SET(TA0CCTL0,CCIE);
        while((TST(P1IN,DPIN)));
        CLR(TA0CTL,0x30);
        if(TA0R > 13){
            m |= 1 << (i-1);        //Se realiza corimiento
        }
    }
    return m;
}
unsigned int readPacket(unsigned int *data){
    signalStart();
    if(checkResponse()){
        data[0]= readByte();
        data[1]= readByte();
        data[2]= readByte();
        data[3]= readByte();
        return 1;
    }else
        return 0;
}
unsigned int checkSum(unsigned int *data){
    if (data[4] != (data[0]+data[1]+data[2]+data[3]))
        return 0;
    else
        return 1;
}
