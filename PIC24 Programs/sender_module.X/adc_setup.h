/* 
 * File:   adc_setup.h
 * Author: Jake
 *
 * Created on June 7, 2013, 1:37 PM
 *
 * Header file for the adc_setup.c
 *
 */

#ifndef ADC_SETUP_H
#define	ADC_SETUP_H

#ifdef	__cplusplus
extern "C" {
#endif



void setupADC1(void);
void changeADCinput(int an_pin);
void readADC(int* adc_buff);



#ifdef	__cplusplus
}
#endif

#endif	/* ADC_SETUP_H */

