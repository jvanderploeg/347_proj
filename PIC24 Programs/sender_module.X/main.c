/* 
 * File:   main.c
 * Author: Jake
 *
 * Created on May 2, 2013, 10:29 PM
 */


#include "prototypes.h"
#include "uart_setup.h"
#include "misc.h"
#include <p24Exxxx.h>


// PIC24EP256GP204 Configuration Bit Settings

#include <xc.h>

// FICD
#pragma config ICS = PGD3               // ICD Communication Channel Select bits (Communicate on PGEC3 and PGED3)
#pragma config JTAGEN = OFF             // JTAG Enable bit (JTAG is disabled)

// FPOR
#pragma config ALTI2C1 = OFF            // Alternate I2C1 pins (I2C1 mapped to SDA1/SCL1 pins)
#pragma config ALTI2C2 = OFF            // Alternate I2C2 pins (I2C2 mapped to SDA2/SCL2 pins)
#pragma config WDTWIN = WIN25           // Watchdog Window Select bits (WDT Window is 25% of WDT period)

// FWDT
#pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler bits (1:32,768)
#pragma config WDTPRE = PR128           // Watchdog Timer Prescaler bit (1:128)
#pragma config PLLKEN = ON              // PLL Lock Enable bit (Clock switch to PLL source will wait until the PLL lock signal is valid.)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable bit (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = ON              // Watchdog Timer Enable bit (Watchdog timer always enabled)

// FOSC
#pragma config POSCMD = XT              // Primary Oscillator Mode Select bits (XT Crystal Oscillator Mode)
#pragma config OSCIOFNC = OFF           // OSC2 Pin Function bit (OSC2 is clock output)
#pragma config IOL1WAY = ON             // Peripheral pin select configuration (Allow only one reconfiguration)
#pragma config FCKSM = CSDCMD           // Clock Switching Mode bits (Both Clock switching and Fail-safe Clock Monitor are disabled)

// FOSCSEL
#pragma config FNOSC = PRIPLL           // Oscillator Source Selection (Primary Oscillator with PLL module (XT + PLL, HS + PLL, EC + PLL))
#pragma config IESO = ON               // Two-speed Oscillator Start-up Enable bit (Start up with user-selected oscillator source)

// FGS
#pragma config GWRP = OFF               // General Segment Write-Protect bit (General Segment may be written)
#pragma config GCP = OFF                // General Segment Code-Protect bit (General Segment Code protect is Disabled)

// The above was code generated by MPLAB using the Configuration bits window


// defines and variables
#define WAIT 1000

char data;
char send_data;
char send_string[32] = "Hello World!";
char string[32];
int next;
int read_index;
int timer;
int wait_hold;
int timer_en;


/*
 * 
 * We need to
 * 1. Get a demo up of current pin breakout
 * 2. Setup UART
 * 3. Port over functions from PIC32 development
 *
 */
int main(int argc, char** argv)
{
    int i;

    read_index = 0;
    next = 0;


    setupLEDs();

    LED0 = 1;
    LED1 = 1;
    LED2 = 1;
    LED3 = 1;
    LED4 = 1;

    configureOscillator();
    configureINT();
    setupUART2();
    configureUART2pins();
    configureT1();

    // Turn on the timer
    T1CONbits.TON = 1;

    wait_hold = 0;
    timer_en = 1;
    while(wait_hold == 0);


//    while(1)
//    {
//        for(i=0;i<12;i++)
//        {
//           SerialTransmit("H");
//        }
//    }

        // Reset the string
    memset(string,0,32);
    // Reset index
    read_index = 0;

    // send dollar signs to enter command mode
    char send1[] = "$$$";
    SerialTransmit(send1);

    // Wait until the timer has loaded
//    verify_time = 0;
//    verify_ack = 1;
//    while(verify_ack != 0);

    // TODO: make this only try for a certain amount of time using the timer
    while(strcmp(string,"CMD\r\n") != 0);



    wait(100000);

    // Put the bluetooth into command mode
    SerialTransmit("C\n");
    next = 0;

    while(next != 1);

    if(string[read_index-1] == 'T') // We have the CMD response
    {
        LED0 = 0;
        LED1 = 1;
    }
    else                            // We did not get the appropriate response
    {
        LED0 = 1;
        LED1 = 0;
    }

    while(1)
    {
        for(i=0;i<12;i++)
        {
           SerialTransmit("H");
        }


    }


    testLEDs();

    return (EXIT_SUCCESS);
}

void _ISR _T1Interrupt(void)
{
    LED4 = !LED4;

    if(timer_en == 1)
    {
        timer = WAIT;
        timer_en = 0;
        wait_hold = 0;
    }
    else if(timer > 0)
    {
        timer--;
        wait_hold = 0;
    }
    else
    {
        timer = 0;
        wait_hold = 1;
    }

    // Clear the interrupt flag
    IFS0bits.T1IF = 0;
}



void _ISR _U2RXInterrupt(void)
{
    data = U2RXREG;

    // do something with the data, like echo it back
    string[read_index] = data;
    read_index++;

    next = 1;

    // Clear the RX interrupt Flag
    IFS1bits.U2RXIF = 0;
}


