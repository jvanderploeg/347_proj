/* 
 * File:   uart_setup.h
 * Author: brian
 *
 * Created on May 23, 2013, 1:58 PM
 */

#ifndef UART_SETUP_H
#define	UART_SETUP_H

#ifdef	__cplusplus
extern "C" {
#endif

void setupUART2(void);
void configureUART2pins(void);
int SerialTransmit(const char *buffer);


#ifdef	__cplusplus
}
#endif

#endif	/* UART_SETUP_H */

