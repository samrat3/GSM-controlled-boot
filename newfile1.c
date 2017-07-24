#include<p18f458.h>
#define m1_1 PORTCbits.RC0
#define m1_2 PORTCbits.RC1
#define m2_1 PORTCbits.RC2
#define m2_2 PORTCbits.RC3
#define rs PORTAbits.AN0
#define rw PORTAbits.AN1
#define en PORTAbits.AN2
#pragma config WDT=OFF
unsigned int cmd[]={0x38,0x0E,0x01,0x06,0x80};
unsigned char disp1[]="Gsm Control boot";
unsigned char c;
unsigned char c1[]="backward";
unsigned char c2[]="forward";
unsigned char c3[]="Right";
unsigned char c4[]="left";
unsigned char c5[]="Stop";
unsigned char den[]="Denied";
void delay1()
{
    T1CONbits.TMR1ON=1;
    TMR1H=0;
    TMR1L=0;
    while(PIR1bits.TMR1IF==0);
    T1CONbits.TMR1ON=0;
    PIR1bits.TMR1IF=0;
}
void delay_nxt()
{
        TMR2=0;
        PR2=255;
        T2CONbits.TMR2ON=1;
        while(PIR1bits.TMR2IF==0);
        PIR1bits.TMR2IF=0;
        T2CONbits.TMR2ON=0;


}
void send()
{
    rs=1;
    rw=0;
    en=1;
    delay1();
    en=0;
}
void command()
{
    rs=0;
    rw=0;
    en=1;
    delay1();
    en=0;
}
void lcd()
{
    int i;
    delay1();
    for(i=0;i<5;i++)
    {
        PORTB=cmd[i];
        command();
    }
    PORTB=0x01;
    command();
}
void kickStart()
{
    int i ;
    for(i=0;i<16;i++)
    {
        PORTB=disp1[i];
        send();
    }
    PORTB=0xC0;
    command();
}
void main(void)
{
    int i,a;
    TRISA=0;
    TRISB=0;
    TRISCbits.TRISC0=0;
    TRISCbits.TRISC1=0;
    TRISCbits.TRISC2=0;
    TRISCbits.TRISC3=0;
    ADCON1=0x07;
    T1CON=0;
    TRISDbits.TRISD1=1;
    TRISDbits.TRISD2=1;
    TRISDbits.TRISD3=1;
    TRISCbits.TRISC7=1;
    RCSTA=0x90;
    T2CON=0;
    SPBRG=25;
    lcd();
    kickStart();
    while(1)
    {
        while(PIR1bits.RCIF==0);
        c=RCREG;
        PIR1bits.RCIF=0;
        switch (c)
        {
                case 'b':
                    m1_1=0;
                    m1_2=1;
                    m2_1=0;
                    m2_2=1;
                    PORTB=0x01;
                    command();
                for(i=0;i<8;i++)
                {
                    PORTB=c1[i];
                    send();
                }
                    break;
                case 'f':
                    if((PORTDbits.RD3==1)&&(PORTDbits.RD2==0)&&(PORTDbits.RD1==0))
                    {
                        m1_1=1;
                        m1_2=0;
                        m2_1=1;
                        m2_2=0;
                        
                    }
                    else if((PORTDbits.RD2==1)&&(PORTDbits.RD3==0)&&(PORTDbits.RD1==0))
                    while((PORTDbits.RD2==1)&&(PORTDbits.RD3==0)&&(PORTDbits.RD1==0))
                    {
                        m1_1=1;
                        m1_2=0;
                        m2_1=1;
                        m2_2=0;
                        for(a=0;a<15;a++)
                        delay_nxt();
                      
                        m1_1=~m1_1;
                        m2_1=~m2_1;

                    }
                    else if((PORTDbits.RD1==1)&&(PORTDbits.RD2==0)&&(PORTDbits.RD3==0))
                        while((PORTDbits.RD1==1)&&(PORTDbits.RD2==0)&&(PORTDbits.RD3==0))
                    {
                        m1_1=1;
                        m1_2=0;
                        m2_1=1;
                        m2_2=0;
                        for(a=0;a<8;a++)
                        delay_nxt();
                        m1_1=~m1_1;
                        m2_1=~m2_1;
                        

                    }

                    PORTB=0x01;
                    command();
                for(i=0;i<7;i++)
                {
                    PORTB=c2[i];
                    send();
                }
                    break;
                case 'r':
                    m1_1=1;
                    m1_2=0;
                    m2_1=0;
                    m2_2=0;
                    PORTB=0x01;
                    command();
                for(i=0;i<5;i++)
                {
                    PORTB=c3[i];
                    send();
                }
                    break;
                case 'l':
                    m1_1=0;
                    m1_2=0;
                    m2_1=0;
                    m2_2=1;
                    PORTB=0x01;
                    command();
                for(i=0;i<4;i++)
                {
                    PORTB=c4[i];
                    send();
                }
                    break;
                case 's':
                    m1_1=0;
                    m1_2=0;
                    m2_1=0;
                    m2_2=0;
                    PORTB=0x01;
                    command();
                for(i=0;i<4;i++)
                {
                    PORTB=c5[i];
                    send();
                }
                    break;
            default:
                PORTB=0x01;
                    command();
                for(i=0;i<6;i++)
                {
                    PORTB=den[i];
                    send();
                }
                break;
        }
    }
}
