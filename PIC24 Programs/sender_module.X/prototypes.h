/* 
 * File:   prototypes.h
 * Author: Jake
 *
 * Created on May 13, 2013, 5:25 PM
 */

#ifndef PROTOTYPES_H
#define	PROTOTYPES_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <p24EP256GP204.h>
#include <xc.h>

// these are for the old prototype maps
//#define LED0    PORTAbits.RA0
//#define LED1    PORTAbits.RA1
//#define LED2    PORTCbits.RC2
//#define LED3    PORTAbits.RA10
//#define LED4    PORTAbits.RA7

#define LED0 PORTBbits.RB13 // port D1 - not soldered at the moment
#define LED1 PORTAbits.RA10 // port D2
#define LED2 PORTAbits.RA7  // port D3 - not soldered at the moment
#define LED3 PORTBbits.RB14 // port D4
#define LED4 PORTBbits.RB15 // port D5


#define BATTERY_CHECK_GATE  PORTCbits.RC9


#define SYSCLK 70000000

#endif	/* PROTOTYPES_H */

