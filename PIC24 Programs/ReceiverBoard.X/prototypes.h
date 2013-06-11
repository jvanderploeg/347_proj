/* 
 * File:   prototypes.h
 * Author: Jimmy
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


#define Right_Head      PORTCbits.RC6
#define Left_Head       PORTCbits.RC7
#define Right_Blink     PORTCbits.RC8
#define Left_Blink      PORTCbits.RC9

#define Wipers          OC1R

#define Horn            OC2R

#define Blink_Time      1000
#define Slow_Time       10000
#define Medium_Time     7500
#define Fast_Time       5000




#define SYSCLK 70000000


#endif	/* PROTOTYPES_H */

