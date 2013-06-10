/*
 * File:   main.c
 * Author: James Paul
 *
 * Created on June 7, 2013, 2:50 PM
 */

#include "prototypes.h"

void timer2setupPWM(void)
{
// We'll use timer 2 for the the wiper servos


// OC1 = PWM

/*
* 1.Set the PWM period by writing to the selected timer period register (PRy).
* 2.Set the PWM duty cycle by writing to the OCxRS register.
* 3.Write the OxCR register with the initial duty cycle.
* 4.Enable interrupts, if required, for the timer and Output Compare modules. The output compare interrupt is required for PWM Fault pin utilization.
* 5.Configure the Output Compare module for one of two PWM Operation modes by writing to the Output Compare mode bits, OCM<2:0> (OCxCON<2:0>).
* 6.Set the TMRy prescale value and enable the time base by setting TON (TxCON<15>) = 1.
*/

/* Setting up for OC1 */

        //Select Pin 42 RP38/RB6
        RPOR2bits.RP38R = 0x10;

	// Disable the Output Compare modules
	OC1CON1bits.OCM = 0;

        /* It is a good practice to initially clear the control bits */
        OC1CON1 = 0;
        OC1CON2 = 0;


        // TODO: Setup for 16 bit timer mode
	
        // Timer 2 for OC1
	OC1CON1bits.OCTSEL = 0;

	// All modules will be PWM mode, fault pin disabled
	OC1CON1bits.OCM = 0x06;

        // Setup the SYNC mode to be off of Timer2
        OC1CON2bits.SYNCSEL = 0b01100;

        // Invert the output since we are driving a transistor
        OC1CON2bits.OCINV = 1;

// Set the initial Duty Cycle to 50%
	// For timer 2, max duty cycle is 
	OC1R = 2000;

// Set the next Duty Cycle to 50%

        //Ignore Fault Mode
        OC1CON1bits.ENFLTA = 0;
        OC1CON1bits.ENFLTB = 0;

/* Setting up for Timer 2*/

// For timer 2, set up a 50 Hz period
	// Turn timer 1 off
	T2CONbits.TON = 0;
	// Operate in 16-Bit Synchronous Clock Counter Mode
	T2CONbits.TCS = 0;
	// Disable the Timer Gated Time Accumulation
	T2CONbits.TGATE = 0;
	// We want it to be running at 1 ms, thus 1:64 prescale from 70MHz clock and
	T2CONbits.TCKPS = 0x02;
	// Set the period 
	PR2 = 4000;
	// Clear the timer register
	TMR2 = 0x0000;

// Turn on the Timer
	T2CONbits.TON = 1;

        return;



}
