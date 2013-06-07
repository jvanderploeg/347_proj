#include "prototypes.h"
#include "misc.h"
#include "uart_setup.h"

#define Baud2BRG(desired_baud)      ( (SYSCLK / (16*desired_baud))-1)

void wait(int num)
{
    int i;
    for(i=0;i<num;i++);
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
        LED3 = 0;
    else
        LED3 = 1;
*/

}

void configureINT(void)
{
    // Enable interrupts
    INTCON2bits.GIE = 1;
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
    // Configure them as digital outputs
    ANSELAbits.ANSA0 = 0;
    ANSELAbits.ANSA1 = 0;

    // Set them to outputs
    TRISAbits.TRISA10 = 0;
    TRISAbits.TRISA7 = 0;
    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0;
    TRISCbits.TRISC2 = 0;

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


int expect_response(char* resp, int timeout_ms)
{
    int match_sentinel = 0; // signifies whether we got the response we want

    // calculate number of timer "ticks" to put on the countdown
    timer = timeout_ms/16;

    // clear the "timer is done" flag
    wait_hold = 0;

    // start the countdown (see the timer interrupt to see how this works)
    timer_en = 1;


    // wait for either the response to come back or the timer to run out
    while((match_sentinel == 0) && (wait_hold == 0))
    {
        if(strcmp(string,resp) == 0)
            match_sentinel = 1;

        else
            match_sentinel = 0;

    }

    // if we got what we wanted, return 1, otherwise return 0.
    return match_sentinel;
}

void clear_recieve_buffer(void)
{
    // Clear the UART recieve buffer
    memset(string,0,32);

    // Clear the UART recieve index
    read_index = 0;
}

void demo_code(void)
{
    // this is Jake's code that sends the receiver 1, waits for ack and such,
    // sends the reciever 2, etc.

	// Reset the UART recieve buffer
    clear_recieve_buffer();

    SerialTransmit("1\0");

    // Use the timer to wait for a couple seconds.
    // LED turns off after time's up.
    // TODO: acutally poll connection state and retry if necessary
    LED3 = 0;
    wait_hold = 0;
    timer_en = 1;
    while(wait_hold == 0);
    // Turn off LED wait again
    LED3 = 1;

    // wait for ACK from reciever
    while(strcmp(string,"ACK") != 0);

    // Use the timer to wait for a couple seconds.
    // LED turns off after time's up.
    // TODO: acutally poll connection state and retry if necessary
    LED0 = 0;
    wait_hold = 0;
    timer_en = 1;
    while(wait_hold == 0);
    LED0 = 1;

    // Reset the UART recieve buffer
    clear_recieve_buffer();

    SerialTransmit("2\0");

    // We send when the LED turns on
    LED3 = 0;
    wait_hold = 0;
    timer_en = 1;
    while(wait_hold == 0);
    // Turn off LED wait again
    LED3 = 1;

    // wait for ACK from reciever
    while(strcmp(string,"ACK") != 0);

    LED0 = 0;
    wait_hold = 0;
    timer_en = 1;
    while(wait_hold == 0);
    LED0 = 1;

    // Reset the UART recieve buffer
    clear_recieve_buffer();

    SerialTransmit("3\0");

    // We send when the LED turns on
    LED3 = 0;
    wait_hold = 0;
    timer_en = 1;
    while(wait_hold == 0);
    // Turn off LED wait again
    LED3 = 1;

    while(strcmp(string,"ACK") != 0);

    LED0 = 0;
    wait_hold = 0;
    timer_en = 1;
    while(wait_hold == 0);
    LED0 = 1;

}



