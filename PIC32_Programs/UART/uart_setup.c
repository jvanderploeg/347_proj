#include <jvp002.h>
#include <misc.h>
#include <plib.h>
#include <uart_setup.h>


// Macros
// Equation to set baud rate from UART reference manual equation 21-1
#define Baud2BRG(desired_baud)      ( (SYSCLK / (16*desired_baud))-1)

int SetupUART(int desired_baud)
{
    U2STA = 0;
    U2MODE = 0x8000; // Enable UART for 8-bit data with loopback
    // no parity, 1 Stop bit
    U2STASET = 0x1400; // Enable Transmit and Receive

    U2BRG = Baud2BRG(desired_baud); // U2BRG = (FPb / (16*baud)) - 1

    // Calculate actual assigned baud rate
    int actual_baud = SYSCLK / (16 * (U2BRG+1));

    return actual_baud;
}

void SetupUARTLoopback(int desired_baud)
{
    U2STA = 0;
    U2MODE = 0x8040; // Enable UART for 8-bit data with loopback
    // no parity, 1 Stop bit
    U2STASET = 0x1400; // Enable Transmit and Receive

    U2BRG = Baud2BRG(desired_baud); // U2BRG = (FPb / (16*baud)) - 1

    // Calculate actual assigned baud rate
    int actual_baud = SYSCLK / (16 * (U2BRG+1));

    return actual_baud;
}

void SetupUART5(void) {
    U5BRG = 42; // Set Baud rate
    U5STA = 0;
    U5MODE = 0x8000; // Enable UART for 8-bit data
    // no parity, 1 Stop bit
    U5STASET = 0x1400; // Enable Transmit and Receive

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

/* SerialTransmit() transmits a string to the UART2 TX pin MSB first
 *
 * Inputs: *buffer = string to transmit */
int SerialTransmit(const char *buffer)
{
    unsigned int size = strlen(buffer);
    while(size)
    {
        while( U3STAbits.UTXBF);    // wait while TX buffer full
        U2TXREG = *buffer;          // send single character to transmit buffer

        buffer++;                   // transmit next character on following loop
        size--;                     // loop until all characters sent (when size = 0)
    }

    while(!U2STAbits.TRMT);        // wait for last transmission to finish

    return 0;
}

/* SerialReceive() is a blocking function that waits for data on
 *  the UART2 RX buffer and then stores all incoming data into *buffer
 *
 * Note that when a carriage return '\r' is received, a nul character
 *  is appended signifying the strings end
 *
 * Inputs:  *buffer = Character array/pointer to store received data into
 *          max_size = number of bytes allocated to this pointer
 * Outputs: Number of characters received */
unsigned int SerialReceive(char *buffer, unsigned int max_size, int rec_type)
{
    unsigned int num_char = 0;

    /* Wait for and store incoming data until either a carriage return is received
     *   or the number of received characters (num_chars) exceeds max_size */
    while(num_char < max_size)
    {
        while( !U2STAbits.URXDA);   // wait until data available in RX buffer
        *buffer = U2RXREG;          // empty contents of RX buffer into *buffer pointer

        if(rec_type == CHAR_MODE) {
            // insert nul character to indicate end of string
            if( (*buffer == '\r') || (*buffer == '\n') )
            {
                *buffer = '\0';
                break;
            }
        }

        buffer++;
        num_char++;
    }

    return num_char;
} // END SerialReceive()

void NU32_EnableUART2Interrupt(void) {
  // turn off the module to change the settings
  UARTEnable(UART2, UART_ENABLE_FLAGS(UART_DISABLE));

  // Configure UART1 RX Interrupt
//  UARTConfigure(UART2, UART_ENABLE_PINS_CTS_RTS );
  UARTSetFifoMode(UART2, UART_INTERRUPT_ON_TX_DONE | UART_INTERRUPT_ON_RX_NOT_EMPTY);
  UARTSetLineControl(UART2, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
  UARTSetDataRate(UART2, SYSCLK, 115200);
  UARTEnable(UART2, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));
  INTEnable(INT_U2RX, INT_ENABLED);
  INTSetVectorPriority(INT_UART_2_VECTOR, INT_PRIORITY_LEVEL_2);
  INTSetVectorSubPriority(INT_UART_2_VECTOR, INT_SUB_PRIORITY_LEVEL_0);



}