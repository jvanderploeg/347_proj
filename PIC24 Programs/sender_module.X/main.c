/* 
 * File:   main.c
 * Author: Jake
 *
 * Created on May 2, 2013, 10:29 PM
 */


#include "prototypes.h"
#include "misc.h"
#include "uart_setup.h"
#include <p24Exxxx.h>

// enumerated type for state machine
enum sender_state{connecting, connected_waiting, command_active};
enum sender_state system_state;

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
#pragma config FWDTEN = OFF              // Watchdog Timer Enable bit (Watchdog timer always enabled)

// FOSC
#pragma config POSCMD = XT              // Primary Oscillator Mode Select bits (XT Crystal Oscillator Mode)
#pragma config OSCIOFNC = OFF           // OSC2 Pin Function bit (OSC2 is clock output)
#pragma config IOL1WAY = ON             // Peripheral pin select configuration (Allow only one reconfiguration)
#pragma config FCKSM = CSDCMD           // Clock Switching Mode bits (Both Clock switching and Fail-safe Clock Monitor are disabled)


// FOSCSEL
#pragma config FNOSC = PRIPLL              // Oscillator Source Selection (Primary Oscillator with PLL module (XT + PLL, HS + PLL, EC + PLL))
#pragma config IESO = ON                // Two-speed Oscillator Start-up Enable bit (Start up device with FRC, then switch to user-selected oscillator source)

// FGS
#pragma config GWRP = OFF               // General Segment Write-Protect bit (General Segment may be written)
#pragma config GCP = OFF                // General Segment Code-Protect bit (General Segment Code protect is Disabled)


// The above was code generated by MPLAB using the Configuration bits window


/*
 * 
 * We need to
 * 1. Get a demo up of current pin breakout
 * 2. Setup UART
 * 3. Port over functions from PIC32 development
 *
 */

#define WAIT 50

char data;
char send_data;
char send_string[32] = "Hello World!";
char string[32];
int next;
int read_index;
int timer;
int wait_hold;
int timer_en;

void clear_recieve_buffer(void);
void delay(int wait_ms);
int expect_response(char* resp, int timeout_ms);

int main(int argc, char** argv)
{

    read_index = 0;
    next = 0;

    configureOscillator();
    configureINT();
    setupUART2();
    configureUART2pins();
    configureT1();
    setupLEDs();

    LED0 = 1;
    LED1 = 1;
    LED2 = 1;
    LED3 = 1;
    LED4 = 1;

    // Turn on the timer
    T1CONbits.TON = 1;

    // Wait for Blueooth module to power up
    delay(4000);

    LED0 = 0;
    LED1 = 0;
    LED2 = 0;
    LED3 = 0;
    while(wait_hold == 0);
    LED0 = 1;
    LED1 = 1;
    LED2 = 1;
    LED3 = 1;

    // Reset the buffer
    memset(string,0,32);
    // Reset index
    read_index = 0;

    // send dollar signs to enter command mode
    char send1[] = "$$$";

    SerialTransmit(send1);

    // TODO: make this only try for a certain amount of time using the timer
    while(strcmp(string,"CMD\r\n") != 0);


    wait(100);

    // Reset the buffer
    memset(string,0,32);
    // Reset index
    read_index = 0;

    SerialTransmit("GB\n");
    // wait for response
    while(strcmp(string,"000666487754\r\n") != 0);
    
    LED0 = 0;

    char dev_id[100];
    strcpy(dev_id,string);

    while(1) {
        switch(system_state){
            case connecting:
                /*
                 * State for connecting to the reciever via BT.
                 * 
                 * Handshaking works as follows:
                 * 1. Send the receiver our BT ID, which we grabbed from the 
                 *    BT chip before entering the state machine
                 * 2. Expect the receiver to send it's BT ID back. It's 
                 *    hardcoded in here.
                 * 
                 * When it works, we switch to the connected state. Otherwise, 
                 * retry. Maybe go to an error case. Haven't written that yet.
                 */

                // Assuming we're already in CMD mode from pre-state
                // machine stuff..

                // Reset the UART recieve buffer
                clear_recieve_buffer();

                // Fire off a connect command to the bluetooth
                SerialTransmit("C\n");
                LED1 = 1;

                // Wait for acknowledge from the BT chip
                int resp = expect_response("TRYING\r\n",2000);

                LED2 = !resp;
                
                // Use the timer to wait for 3 seconds.
                // TODO: acutally poll connection state and retry if necessary
                delay(3000);

                // Reset the UART recieve buffer again
                clear_recieve_buffer();

                // Send our BT chip's address for handshaking.
                // LED turns on when we send.
                SerialTransmit(dev_id);
                LED3 = 0;

                // Wait for the other BT chip to respond with its address
                // TODO: use the timer to only wait a certain amount of time for
                // response, and retry/alert the user
                resp = expect_response("0006664D63FA\r\n",3000);

                LED4 = !resp;

                // If we make it here, we're connected!
                // TODO: check to make sure.

                delay(500);

                LED0 = 1;
                LED1 = 1;
                LED2 = 1;
                LED3 = 1;
                LED4 = 1;
                
                // Switch to connected state
                system_state = connected_waiting;

            case connected_waiting:
                /*
                 * State for when the BT is connected. Poll the command button,
                 * change state if warranted, rinse and repeat.
                 */

                // placeholder til ADC is working...
                // start the timer and just wait for a bit
                delay(2000);

                system_state = command_active;
                


            case command_active:
                /*
                 * State for when the command button is pressed.
                 * Check the other buttons, fire the necessary command over BT,
                 * wait for ack for a certain amount of time, and retry.
                 */

                // TODO: code to check buttons and pick command
                // for now, let's assume we're sending "1".

                // Send command over BT
                SerialTransmit("1\0");

                // wait for ACK from reciever
                // TODO: timeout logic
                while(strcmp(string,"ACK") != 0);

                // temporary: wait before proceeding
                delay(1000);
        }
    }

    return (EXIT_SUCCESS);
}


void _ISR _T1Interrupt(void)
{
    // Each timer "tick" is 16ms.
    // Flip the LED to show that it's working.
    // LED4 = !LED4;

    if(timer_en == 1)
    {

        // timer variable must be set ahead of time
        // (the delay function does this)
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
    // put the received character into a var
    data = U2RXREG;

    // put the char into the read buffer at the right spot
    string[read_index] = data;
    read_index++;

    next = 1;

    // Clear the RX interrupt Flag
    IFS1bits.U2RXIF = 0;
}

void delay(int wait_ms) {
    /*
     * Uses the timer interrupt to wait a given number of milliseconds.
     * Must be a multiple of 16ms (which is the timer rate), or it will
     * be rounded down to the nearest 16ms multiple.
     *
     * NOTE: this is not highly precise. It's pretty close, but allow a little
     * leeway if precision of your wait times are critical
     */

    // calculate number of timer "ticks" to put on the countdown
    timer = wait_ms/16;

    // clear the "timer is done" flag
    wait_hold = 0;

    // start the countdown (see the timer interrupt to see how this works)
    timer_en = 1;

    // wait til the countdown is done
    while(wait_hold == 0);

    return;

}

int expect_response(char* resp, int timeout_ms) {
    int match_sentinel = 0; // signifies whether we got the response we want

    // calculate number of timer "ticks" to put on the countdown
    timer = timeout_ms/16;

    // clear the "timer is done" flag
    wait_hold = 0;

    // start the countdown (see the timer interrupt to see how this works)
    timer_en = 1;


    // wait for either the response to come back or the timer to run out
    while((match_sentinel == 0) || (wait_hold == 0)) {
        if(strcmp(string,resp) == 0) {
            match_sentinel = 1;
        }
        else {
            match_sentinel = 0;
        }   
    };

    // if we got what we wanted, return 1, otherwise return 0.
    return match_sentinel;
}

void clear_recieve_buffer(void) {
    // Clear the UART recieve buffer
    memset(string,0,32);

    // Clear the UART recieve index
    read_index = 0;
}

void demo_code(void) {
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

