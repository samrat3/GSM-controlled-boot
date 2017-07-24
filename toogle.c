#include<p18f452.h>
#pragma config WDT=OFF
void main(void)
{
    TRISD=0;
   T2CON=0;
   TMR2=0;
   PR2=0x55;
   T2CONbits.TMR2ON=1;
   while(PIR1bits.TMR2IF==0);
   PIR1bits.TMR2IF=0;
   PORTDbits.RD7=~PORTDbits.RD7;
}
