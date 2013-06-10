/* 
 * File:   misc.h
 * Author: Jake
 *
 * Created on May 13, 2013, 4:19 PM
 */

#ifndef MISC_H
#define	MISC_H

#ifdef	__cplusplus
extern "C" {
#endif

void wait(int num);
void delay(int wait_time);
void setupLEDs(void);
void setupUART2(void);
void setupADC1(void);
void setupIO(void);
void setupButtons(void);

void configureOscillator(void);
void configureINT(void);
void configureT1(void);

void testLEDs(void);

int checkBatteryVoltage(void);
void changeADCinput(int an_pin);
void readADC(int* adc_buff);
void demo_code(void);
void clear_recieve_buffer(void);

int expect_response(char* resp, int timeout_ms);



#ifdef	__cplusplus
}
#endif

#endif	/* MISC_H */

