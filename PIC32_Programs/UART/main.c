// Main program for testing UART on the JVP002


#include "jvp002.h"
#include "lcd_setup.h"
#include "misc.h"
#include "adc_setup.h"
#include "uart_setup.h"

/** Configurations **/
// Configuring the Device Configuration Registers
// 80Mhz Core/Periph, Pri Osc w/PLL
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_1, FSOSCEN = ON
#pragma config ICESEL = ICS_PGx1, BWP = OFF

/** # Defines **/
#define MAX		2500

/** Globals **/
int state;
int reset;
int adc1,adc2;
char string[32];
int led;
int pwm;
int d1;
int d2;
int state_init[5] = {0,0,0,0,0};

int main(void)
{
	int delay;
	d2 = 0;
	d1 = 0;
	state = 0;
	reset = 1;
	adc1 = 0;
	adc2 = 0;
	led = 0;
	pwm = 0;
	double volt1 = 0.0;
	double volt2 = 0.0;

	// Setup functions for operation
	CP0Init();
	INTInitialize();
	ADCSetup();
	SetupButtons();
	SetupIO();
	SetupT1CountINT();
	SetupTimer2_3PWM();
        SetupUART();

	// Wait until LCD screen comes up
	wait(1000);	wait(1000);	wait(1000); wait(1000);
	LCDConfig();
	LCDSetup();



	// Start the timer
	T1CONbits.ON = 1;
        
        while(1) {
            // send the tx 1's
            U2TXREG = 0xFF;
            wait(1000);
            // send the tx 0's
            U2TXREG = 0x00;
            wait(2000);
        }
}