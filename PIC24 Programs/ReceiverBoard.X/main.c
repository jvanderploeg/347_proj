/* 
 * File:   main.c
 * Author: James Paul
 *
 * Created on May 30, 2013, 2:10 PM
 */

#include "uart_setup.h"
#include "prototypes.h"
#include <p24Exxxx.h>
#include "misc.h"
#include "timersetup.h"

// Uncomment for horn/wipers test
//#define _TEST

// PIC24EP256GP204 Configuration Bit Settings

#include <xc.h>

enum receiver_state{connecting, connected_waiting};
enum receiver_state system_state;
void display_States(void);
void clear_receive_buffer(void);
void _ISR _U2RXInterrupt(void);



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

char data;
char string[32];
int read_index;
int next;
int HeadLights;
int BlinkL;
int BlinkR;
int Wipe;
int received_command;
int timer_en;
int timer;
int wait_hold;
int BLcount;
int BRcount;
int Wipecount;
int Wipedown;

int main(int argc, char** argv) {
    HeadLights = 0;
    BlinkL = 0;
    BlinkR = 0;
    Wipe = 0;
    Wipedown = 0;
    BLcount = 0;
    BRcount = 0;
    Wipecount = 0;


    configureOscillator();
    configureINT();
    setupUART2();
    configureUART2pins();
    configureT1();
    setupLEDs();
    TRISBbits.TRISB6 = 0;
    TRISBbits.TRISB9 = 0;
    timer2setupPWM();

    T1CONbits.TON = 1;
    T2CONbits.TON = 1;


    system_state = connecting;

#ifdef _TEST
    while(1)
    {
        soundHorn();
        delay(1000);
        stopHorn();
        delay(1000);
        Wipers = 2200;
        delay(1000);
        Wipers = 1200;
        delay(1000);
    }
#endif

    while(1){
        switch(system_state){

            //This state connects and sends/recieves acknowledgments
            //with the receiver board.
            case connecting:
                while(strcmp(string,"0006664FAE62\r\n") != 0)
                    headLightsOn();
                headLightsOff();
                clear_receive_buffer();
                char send1[] = "0006664D63FA\r\n";
                SerialTransmit(send1);
                system_state = connected_waiting;

            //This loops forever updating the states of the car according
            //to commands sent from the sender module.

            case connected_waiting:
                if(received_command == 1)
                {
                    if (strcmp(string,"LeftBlink\r\n") == 0)
                    {
                        if (BlinkL == 0)
                            BlinkL = 1;
                        else
                            BlinkL = 0;
                    }

                    if (strcmp(string,"RightBlink\r\n") == 0)
                    {
                        if (BlinkR == 0)
                            BlinkR = 1;
                        else
                            BlinkR = 0;
                    }

                    if (strcmp(string,"HeadLights\r\n") == 0)
                    {
                        HeadLights = !HeadLights;
                        if (HeadLights == 0)
                            headLightsOff();
                        else
                            headLightsOn();
                    }
                            
                    if (strcmp(string,"Wipers\r\n") == 0)
                    {
                        if (Wipe == 3)
                        {
                            Wipers = 2200;
                            Wipe = 0;
                        }
                        else
                        {
                            Wipers = 1200;
                            Wipe++;
                        }
                    }
                    //If a horn command is issued immediately sound the horn.
                    if (strcmp(string,"HornOn\r\n") == 0)
                        soundHorn();

                    if (strcmp(string,"HornOff\r\n") == 0)
                        stopHorn();

                    if (strcmp(string,"0006664FAE62\r\n") == 0)
                    {
                        char send1[] = "0006664D63FA\r\n";
                        SerialTransmit(send1);
                    }

                    // clear buffer and send acknowledge
                    clear_receive_buffer();
                    sendAck();

                    // reset command sentinel variable
                    received_command = 0;

                   
                    
                }
                //display_States actually implements the commands called by
                //the sender module (LED's/PWM) according to the current
                //gloabal variable states.

        }
    }


    return(0);
}

void _ISR _U2RXInterrupt(void)
{
    // put the received character into a var
    data = U2RXREG;

    // put the char into the read buffer at the right spot
    string[read_index] = data;
    read_index++;

    if(data == '\n'){
        received_command = 1;
    }

    next = 1;

    // Clear the RX interrupt Flag
    IFS1bits.U2RXIF = 0;
    return;
}

void clear_receive_buffer(void) {
    memset(string,0,32);
    read_index = 0;
    return;
}





void _ISR _T1Interrupt(void)
{
    // Each timer "tick" is 16ms.
    // Flip the LED to show that it's working.
    // LED4 = !LED4;

    if(BLcount > Blink_Time)
    {
        if(BlinkL == 1 && BlinkR == 1)
        {
            if(Left_Blink == 1)
            {
                wait(10);
                Left_Blink = 0;
                wait(10);
                Right_Blink = 0;
            }
            else
            {
                wait(10);
                Left_Blink = 1;
                wait(10);
                Right_Blink = 1;
            }
        }
        else if(BlinkL == 1 && BlinkR == 0)
        {
            wait(10);
            Left_Blink = !Left_Blink;
            wait(10);
            Right_Blink = 0;
        }
        else if(BlinkL == 0 && BlinkR == 1)
        {
            wait(10);
            Right_Blink = !Right_Blink;
            wait(10);
            Left_Blink = 0;
        }
        else if(BlinkL == 0 && BlinkR == 0)
        {
            wait(10);
            Left_Blink = 0;
            wait(10);
            Right_Blink = 0;
        }
        BLcount = 0;
    }
    else
        BLcount++;

    if(Wipe == 0)
    {
        Wipers = 1200;
        Wipedown = 1;
        Wipecount = 0;
    }
    else if(Wipe == 1)
    {
        if(Wipecount > Slow_Time)
        {
            if (Wipedown == 1) {
                Wipers = 2200;
                Wipedown = 0;
                Wipecount = 0;
            }
            else {
                Wipers = 1200;
                Wipedown = 1;
                Wipecount = 0;
            }
        }
        else
            Wipecount++;
    }
    else if(Wipe == 2)
    {
        if(Wipecount > Medium_Time)
        {
            if (Wipedown == 1) {
                Wipers = 2200;
                Wipedown = 0;
                Wipecount = 0;
            }
            else {
                Wipers = 1200;
                Wipedown = 1;
                Wipecount = 0;
            }
        }
        else
            Wipecount++;
    }
    else if(Wipe == 3) {
        if(Wipecount > Fast_Time)
        {
            if (Wipedown == 1) {
                Wipers = 2200;
                Wipedown = 0;
                Wipecount = 0;
            }
            else {
                Wipers = 1200;
                Wipedown = 1;
                Wipecount = 0;
            }
        }
        else
            Wipecount++;
    }


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

