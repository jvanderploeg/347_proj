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

#define LED0    PORTAbits.RA0
#define LED1    PORTAbits.RA1
#define LED2    PORTCbits.RC2
#define LED3    PORTAbits.RA10
#define LED4    PORTAbits.RA7


#define SYSCLK 70000000

#endif	/* PROTOTYPES_H */

