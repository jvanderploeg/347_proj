#include "prototypes.h"

#define Baud2BRG(desired_baud)      ( (SYSCLK / (16*desired_baud))-1)

void wait(int num)
{
    int i;
    for(i=0;i<num;i++);
    return;
}

void configureOscillator(void)
{
    // Select the PLL to give a max of 140 MHz into the system,
    // which gives a 70 MHz clock to the system.
    // Starting with a 8MHz clock input, PLLPRE = 0 gives prediv2
    // PLLFBD = 68 gives M = 70, and PLLPOST = 0 gives postdiv2
    // Thus, frequency should be F = 8/2*70/2 = 140MHz -> 70MHz system
    CLKDIVbits.PLLPRE = 0x00;
    wait(100);
    PLLFBDbits.PLLDIV = 68;
    wait(100);
    CLKDIVbits.PLLPOST = 0x00;
    wait(100);

    CLKDIVbits.DOZEN = 0;
    wait(100);
    CLKDIVbits.DOZE = 0x00;
    wait(100);
    CLKDIVbits.DOZEN = 1;
    wait(100);
    return;

/*

    // Unlock the clock switching and switch to PLL type clock source
    __builtin_write_OSCCONH(0x78);
    __builtin_write_OSCCONH(0x9A);

    OSCCONbits.NOSC = 0x03;

    __builtin_write_OSCCONL(OSCCON | 0x46);
    __builtin_write_OSCCONL(OSCCON | 0x57);

    OSCCONbits.OSWEN = 1;

    wait(100);

    if(OSCCONbits.OSWEN == 0)
        Left_Blink = 0;
    else
        Left_Blink = 1;
*/

}



void soundHorn(void)
{
    Horn = 2000;
    return;

}

void stopHorn(void)
{
    Horn = 4000;
    return;

}




void configureINT(void)
{
    // Enable interrupts
    INTCON2bits.GIE = 1;
    return;
}

void configureT1(void)
{
//    Setup the timer
    // Turn off the module
    T1CONbits.TON = 0;
    // Continue timer in idle mode
    T1CONbits.TSIDL = 0;
    // Not gated accumulation
    T1CONbits.TGATE = 0;
    // 1:1 scaling
    T1CONbits.TCKPS = 0x02;
    // Not synchronized to external clock
    T1CONbits.TSYNC = 0;
    // Select internal clock
    T1CONbits.TCS = 0;
    // Clear the timer register
    TMR1 = 0x0000;
    // Pretty close to 1ms given the system clock
    PR1 = 17500;

//    Setup the interrupt
    // Set the priority of T1 interrupt to 4
    IPC0bits.T1IP = 0x04;
    // Clear the interrupt flag
    IFS0bits.T1IF = 0;
    // Enable timer1 interrupt
    IEC0bits.T1IE = 1;
    return;

    
}

int UART2Transmit(const char *buffer)
{
    unsigned int size = strlen(buffer);
    while(size)
    {
        while(U2STAbits.UTXBF);    // wait while TX buffer full
        U2TXREG = *buffer;          // send single character to transmit buffer

        buffer++;                   // transmit next character on following loop
        size--;                     // loop until all characters sent (when size = 0)
    }

    while(!U2STAbits.TRMT);        // wait for last transmission to finish

    return 0;
}


void setupLEDs(void)
{

    // Set pins 2-5 to outputs
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 0;
    TRISCbits.TRISC8 = 0;
    TRISCbits.TRISC9 = 0;

    // turn the lights off
    Left_Head = 0;
    Right_Head = 0;
    Left_Blink = 0;
    Right_Blink = 0;

    return;

}



void headLightsOn(void){
    Right_Head = 1;
    wait(100);
    Left_Head = 1;
    return;
}

void headLightsOff(void)
{
    Right_Head = 0;
    wait(100);
    Left_Head = 0;
    return;
}



void testLEDs(void)
{
    int i;
    Right_Head = 0;

    for(i=0;i<1000;i++)
        wait(1000);

    Right_Head = 1;

    for(i=0;i<1000;i++)
        wait(1000);

    Right_Head = 0;

    for(i=0;i<1000;i++)
        wait(1000);

    Left_Head = 0;

    for(i=0;i<1000;i++)
        wait(1000);

    Left_Head = 1;

    for(i=0;i<1000;i++)
        wait(1000);

    Left_Head = 0;

    for(i=0;i<1000;i++)
        wait(1000);

    Right_Blink = 0;

    for(i=0;i<1000;i++)
        wait(1000);

    Right_Blink = 1;

    for(i=0;i<1000;i++)
        wait(1000);

    Right_Blink = 0;

    for(i=0;i<1000;i++)
        wait(1000);

    Left_Blink = 0;

    for(i=0;i<1000;i++)
        wait(1000);

    Left_Blink = 1;

    for(i=0;i<1000;i++)
        wait(1000);

    Left_Blink = 0;

    for(i=0;i<1000;i++)
        wait(1000);
    return;


}


