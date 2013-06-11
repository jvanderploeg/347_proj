/* 
 * File:   prototypes.h
 * Author: Jake
 *
 * Created on May 13, 2013, 5:25 PM
 */

#ifndef __PROTOTYPES_H

#define	__PROTOTYPES_H


#ifdef	__cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <p24EP256GP204.h>
//#include <xc.h>

// these are for the old prototype maps
//#define LED0    PORTAbits.RA0
//#define LED1    PORTAbits.RA1
//#define LED2    PORTCbits.RC2
//#define LED3    PORTAbits.RA10
//#define LED4    PORTAbits.RA7

#define LED0 				PORTBbits.RB13 // port D1 - not soldered at the moment
//#define LED1 				PORTAbits.RA10 // port D2
#define LED2 				PORTAbits.RA7  // port D3 - not soldered at the moment
//#define LED3 				PORTBbits.RB14 // port D4
//#define LED4 				PORTBbits.RB15 // port D5

#define CONNECTED_LED		PORTBbits.RB15 // port D5
#define LOW_VOLT_LED 		PORTBbits.RB14 // port D4
#define ERROR_LED			PORTAbits.RA10 // port D2

#define BATTERY_CHECK_GATE  PORTCbits.RC9
#define CONNECTION_STATUS   PORTBbits.RB8
#define BLUETOOTH_RESET     PORTBbits.RB9

// These are corresponding analog pin numbers
#define MASTER_EN_BUTTON	6
#define BUTTON_1                5
#define BUTTON_2                4
#define BUTTON_3                1
#define BUTTON_4                0

#define BUTTON_ON_ADC		1000
#define BUTTON_ON_COUNT         1000

#define SYSCLK 			70000000
// Checking on the scope, the voltage level indicates 1.9V
// and reading the adc gives us a value of 0x7FF. It should
// be linear after this calibration
#define REF_VOLT_1_9V           0x7FF
// This should correspond to around 3.2V on the battery
// Math: 	1.9V / 0x7FF = 0.928mV per count. 
//			(3.3V/2) / 0.928mV/count ~= 1777 counts
//			Err on side of caution -> 1777 ~= 1792 = 0x700
#define LOW_VOLTAGE		0x300
// Interrupt runs at 16ms. To get 10 min:
// (10 * 60)/.016 = 37500 = 0x927C (this is under max 16 bit
// int of 0xFFFF, so it should be fine)
#define TEN_MIN			0x927C
#define RESET_DELAY		0x3A9


extern int timer;
extern int wait_hold;
extern int timer_en;
extern char string[32];
extern int read_index;


#ifdef	__cplusplus
}
#endif


#else

#endif	/* PROTOTYPES_H */

