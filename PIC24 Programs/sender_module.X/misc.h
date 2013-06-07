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
void setupLEDs(void);
void setupUART2(void);
void delay(int wait_time);
void testLEDs(void);
void configureOscillator(void);
void configureINT(void);
void configureT1(void);
void setupADC1(void);
void readADC(int* adc_buff);
void demo_code(void);
void clear_recieve_buffer(void);
int expect_response(char* resp, int timeout_ms);


#ifdef	__cplusplus
}
#endif

#endif	/* MISC_H */

