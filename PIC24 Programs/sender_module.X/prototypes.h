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

#define LED0 PORTBbits.RB13 // port D1 - not soldered at the moment
#define LED1 PORTAbits.RA10 // port D2
#define LED2 PORTAbits.RA7  // port D3 - not soldered at the moment
#define LED3 PORTBbits.RB14 // port D4
#define LED4 PORTBbits.RB15 // port D5


#define BATTERY_CHECK_GATE  PORTCbits.RC9

#define SYSCLK 70000000


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

