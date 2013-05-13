#include "prototypes.h"

void setupUART(void)
{
    
}

void setupLEDs(void)
{

    TRISAbits.TRISA10 = 0;
    TRISAbits.TRISA7 = 0;
    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0;
    TRISCbits.TRISC2 = 0;

}

void wait(int num)
{
    int i;
    for(i=0;i<num;i++);    
}

void testLEDs(void)
{
    int i;
    LED0 = 0;

    for(i=0;i<1000;i++)
        wait(1000);

    LED0 = 1;

    for(i=0;i<1000;i++)
        wait(1000);

    LED0 = 0;

    for(i=0;i<1000;i++)
        wait(1000);

    LED1 = 0;

    for(i=0;i<1000;i++)
        wait(1000);

    LED1 = 1;

    for(i=0;i<1000;i++)
        wait(1000);

    LED1 = 0;

    for(i=0;i<1000;i++)
        wait(1000);

    LED2 = 0;

    for(i=0;i<1000;i++)
        wait(1000);

    LED2 = 1;

    for(i=0;i<1000;i++)
        wait(1000);

    LED2 = 0;

    for(i=0;i<1000;i++)
        wait(1000);

    LED3 = 0;

    for(i=0;i<1000;i++)
        wait(1000);

    LED3 = 1;

    for(i=0;i<1000;i++)
        wait(1000);

    LED3 = 0;

    for(i=0;i<1000;i++)
        wait(1000);


    LED4 = 0;

    for(i=0;i<1000;i++)
        wait(1000);

    LED4 = 1;

    for(i=0;i<1000;i++)
        wait(1000);

    LED4 = 0;

    for(i=0;i<1000;i++)
        wait(1000);
}


