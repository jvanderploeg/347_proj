#include "prototypes.h"

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

void setupADC1(void){
    // config steps from ADC datasheet

    // turn off ADC
    AD1CON1bits.ADON = 0;

    //1. Select 10-bit or 12-bit mode (ADxCON1<10>).
    AD1CON1bits.AD12B = 1;

    //2. Select the voltage reference source to match the expected range on the analog inputs
    //(ADxCON2<15:13>).
    //AVDD is ref high, AVss is ref low
    AD1CON2bits.VCFG = 0x000;

    //3. Select the analog conversion clock to match the desired data rate with the processor clock
    //(ADxCON3<7:0>).
    // use system clock
    AD1CON3bits.ADRC = 0;

    // run at instruction clock rate
    AD1CON3bits.ADCS = 0x00000000;


    //4. Determine how inputs must be allocated to S&H channels (ADxCHS0<15:0> and
    //ADxCHS123<15:0>).
    // AN7 is the battery monitor on the old board.
    // AN8 is the battery monitor on the new board.
    AD1CHS0bits.CH0SA = 7;


    //5. Determine how many S&H channels must be used (ADxCON2<9:8>).
    // set to channel 0 only
    AD1CON2bits.CHPS = 0x00;


    //6. Determine how sampling must occur (ADxCON1<3>, ADxCSSH<15:0> and
    //ADxCSSL<15:0>).
    // no need to set ADxCON1 in 12-bit mode
    /* "The ADxCHS123 and ADxCHS0 registers select the input pins to be 
     * connected to the S&H amplifiers. The ADCSSH/L registers select inputs to 
     * be sequentially scanned." */

    // turn off scanning for now
    // note to self, channel scanning could be badass for our purposes
    AD1CON2bits.CSCNA = 0;
    
    //7. Select manual or auto-sampling.
    // set to manual
    AD1CON1bits.ASAM = 0;

    //8. Select the conversion trigger and sampling time.
    // don't care, we're sampling manually

    //9. Select how the data format for the conversion results must be stored in the buffer (ADxCON1<9:8>).
    // unsigned integer
    AD1CON1bits.FORM = 0x00;

    //10. Set the ADDMAEN bit to configure the ADC module to use DMA.
    // turn off DMA
    AD1CON4bits.ADDMAEN = 0x0;

    //11. Select the interrupt rate or DMA Buffer Pointer increment rate (ADxCON2<9:5>).
    // how frequently do we want this thing to fire an interrupt?
    // set to every sample for now
    AD1CON2bits.SMPI = 0x0000;

    //12. Select the number of samples in the DMA buffer for each ADC module input (ADxCON4<2:0>).
    // no need to set, dma is turned off

    //13. Configure the ADC interrupt (if required):
    //a) Clear the ADxIF bit
    //b) Select the interrupt priority (ADxIP<2:0)
    //c) Set the ADxIE bit
    // no need, manual

    //14. Configure the DMA channel (if needed).
    // no need

    //15. Enable the DMA channel.
    // no need

    //16. Turn on the ADC module (ADxCON1<15>).
    AD1CON1bits.ADON = 1;
}

void readADC(int* adc_buff){
    // Start sampling the ADC
    AD1CON1bits.SAMP = 1;

    // Wait for acq time
    wait(100);

    // End sampling the ADC
    AD1CON1bits.SAMP = 0;

    // Wait for conversion
    while(AD1CON1bits.DONE != 1);

    // Manually clear the done bit
    AD1CON1bits.DONE = 0;

    *adc_buff = ADC1BUF0;

    return;
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


