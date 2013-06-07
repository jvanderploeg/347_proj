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
#include <xc.h>


#define SYSCLK 70000000


#define LED0    PORTAbits.RA0
#define LED1    PORTAbits.RA1
#define LED2    PORTCbits.RC2
#define LED3    PORTAbits.RA10
#define LED4    PORTAbits.RA7


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

