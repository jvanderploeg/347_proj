/* 
 * File:   uart_setup.h
 * Author: brian
 *
 * Created on April 11, 2013, 4:22 PM
 */

//defines
#define HEX_MODE        1
#define CHAR_MODE       0

//prototypes
int SetupUART(int desired_baud);
void SetupUARTLoopback(int desired_baud);
void SetupUART5(void);
void UART_putc(char c);
void UART_puts(char *s);
int SerialTransmit(const char *buffer);
unsigned int SerialReceive(char *buffer, unsigned int max_size, int rec_type);
void NU32_EnableUART2Interrupt(void);