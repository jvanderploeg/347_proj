// Main program for testing UART on the JVP002


#include "jvp002.h"
#include "lcd_setup.h"
#include "misc.h"
#include "adc_setup.h"
#include "uart_setup.h"
#include <plib.h>

/** Configurations **/
// Configuring the Device Configuration Registers
// 80Mhz Core/Periph, Pri Osc w/PLL
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_1, FSOSCEN = ON
#pragma config ICESEL = ICS_PGx1, BWP = OFF

/** # Defines **/
#define MAX		2500
#define HEX_MODE        1
#define CHAR_MODE       0

/** Globals **/
int state;
int reset;
int adc1,adc2;
char string[32] = "hello, world";
int led;
int pwm;
int d1;
int d2;
int state_init[5] = {0,0,0,0,0};
char data;
int ind;
int next = 0;

int main(void)
{

    ind = 0;
	// Setup functions for operation
	CP0Init();
	INTInitialize();
	ADCSetup();
	SetupButtons();
	SetupIO();
	SetupT1CountINT();
	SetupTimer2_3PWM();
        //SetupUART(115200);
        NU32_EnableUART2Interrupt();


	// Wait until LCD screen comes up
	wait(1000);	wait(1000);	wait(1000); wait(1000);
	LCDConfig();
	LCDSetup();


        ClearLCDScreen();

        char msg[32];
        sprintf(msg,"UART demo. Press A to start");
	WriteLCD(1,msg);

        int button = 1;
        while(button == 1) {
            button = BUTTON_A;
        }
        wait(1000);
        while(button == 0)
        {
            button = BUTTON_A;
        }
        wait(1000);
        
        // send dollar signs to enter command mode
        char send1[] = "$$$";
        SerialTransmit(send1);

        wait(1000);wait(1000);wait(1000);
        // print message and wait for press
/*        ClearLCDScreen();
        wait(1000);wait(1000);wait(1000);
        sprintf(msg,"command mode. press A");
	WriteLCD(1,msg);
        wait(1000);wait(1000);wait(1000);
        button = 1;
        while(button == 1) {
            button = BUTTON_A;
        }
        wait(1000);
        while(button == 0)
        {
            button = BUTTON_A;
        }
        wait(1000);
        wait(1000);
*/
        while(next == 0);
        next = 0;

        char send2[] = "GB\n";
        SerialTransmit(send2);

        while(1)
        {
            ClearLCDScreen();
            WriteLCD(0,string);
            wait(10000);
            wait(10000);
            wait(10000);
        }

        // receive response from chip
        char temp[4];
        SerialReceive(temp, 4, CHAR_MODE);
        
//        // print response to lcd
//        ClearLCDScreen();
//        WriteLCD(1,temp);
        

        
        char temp2[32];
        SerialReceive(temp2, 32, HEX_MODE);

        ClearLCDScreen();
        WriteLCD(1,temp2);


 /*
  * Software State machine
  *     POR:
  *         Try to connect
  *
  *
  */

        return 0;
}

 void __ISR(_UART_2_VECTOR, ipl2) IntUart1Handler(void) {
  // Is this an RX interrupt?
  if (IFS1bits.U2RXIF == 1) {

    data = U2RXREG;

    // do something with the data, like echo it back
    string[ind] = data;
    ind++;

    next = 1;

    // Clear the RX interrupt Flag
    IFS1bits.U2RXIF = 0;
  }
    // We don't care about TX interrupt
  if (IFS1bits.U2TXIF) {
      IFS1bits.U2TXIF = 0;
  }
}