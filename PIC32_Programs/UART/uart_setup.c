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