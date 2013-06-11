/* 
 * File:   misc.h
 * Author: Jimmy
 *
 * Created on May 13, 2013, 4:19 PM
 */

#ifndef MISC_H
#define	MISC_H

#ifdef	__cplusplus
extern "C" {
#endif

void wait(int num);
void setupLEDs(void);
void setupUART2(void);
void testLEDs(void);
void configureOscillator(void);
void configureINT(void);
void configureT1(void);
void configurePWM(void);
void headLightsOn(void);
void headLightsOff(void);
void soundHorn(void);
void stopHorn(void);

#ifdef	__cplusplus
}
#endif

#endif	/* MISC_H */

