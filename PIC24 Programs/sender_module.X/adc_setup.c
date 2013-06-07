// Setup and library functions for the adc module


#include "prototypes.h"
#include "misc.h"

void setupADC1(void)
{
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


void readADC(int* adc_buff)
{
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


