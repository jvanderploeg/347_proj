#include <jvp002.h>
#include <misc.h>

void SetupUART(void)
{
    U2BRG = 42; // Set Baud rate
    U2STA = 0;
    U2MODE = 0x8000; // Enable UART for 8-bit data
    // no parity, 1 Stop bit
    U2STASET = 0x1400; // Enable Transmit and Receive
}


//lovingly lifted/modified from pic32-examples on Google Code
void UART_putc(char c)
{
    while(U2STAbits.UTXBF); //wait until transmit buffer
    U2TXREG = c;
}

void UART_puts(char *s)
{
    while(*s != '\0')
        UART_putc(*s++);

}