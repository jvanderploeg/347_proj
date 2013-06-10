// UART setup file

#include "prototypes.h"
#include <string.h>


// Macros
// Equation to set baud rate from UART reference manual equation 21-1
#define Baud2BRG(desired_baud)      ( (SYSCLK / (16*desired_baud))-1)


void setupUART2(void)
{
    // Turn module off
    U2MODEbits.UARTEN = 0;

    // Turn off the CTS and RTS
    U2MODEbits.UEN = 0x00;
    // 8 bit data, no parity
    U2MODEbits.PDSEL =  00;
    // 1 bit stop bit
    U2MODEbits.STSEL = 0;
    // High speed mode
    U2MODEbits.BRGH = 0;



// Configure the Interrupt mode
    // Receive interrupt mode. Configure to interrupt on at least one character received
    U2STAbits.URXISEL = 0x00;
    // Want the receive to act on priority level 6
    IPC7bits.U2RXIP = 0x06;
    // Clear the transmit and receive interrupt flags
    IFS1bits.U2RXIF = 0;
    IFS1bits.U2TXIF = 0;
    // Enable the interrupt
    IEC1bits.U2RXIE = 1;
// Configure Baud Rate
    // Set the BRG register
    U2BRG = Baud2BRG(115200);

    // Turn module on
    U2MODEbits.UARTEN = 1;
    // Enable the Transmission
    U2STAbits.UTXEN = 1;

/*
    // turn off the module to change the settings
    UARTEnable(UART2, UART_ENABLE_FLAGS(UART_DISABLE));

    // Configure UART1 RX Interrupt
    //  UARTConfigure(UART2, UART_ENABLE_PINS_CTS_RTS );
    UARTSetFifoMode(UART2, UART_INTERRUPT_ON_TX_DONE | UART_INTERRUPT_ON_RX_NOT_EMPTY);
    UARTSetLineControl(UART2, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
    UARTSetDataRate(UART2, SYSCLK, 115200);
    UARTEnable(UART2, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));
    INTEnable(INT_U2RX, INT_ENABLED);
    INTSetVectorPriority(INT_UART_2_VECTOR, INT_PRIORITY_LEVEL_6);
    INTSetVectorSubPriority(INT_UART_2_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
*/

    return;
}

/*
Default PORT    000000  RPn tied to Default Pin
U1TX            000001  RPn tied to UART1 Transmit
U2TX            000011  RPn tied to UART2 Transmit
SDO2            001000  RPn tied to SPI2 Data Output
SCK2            001001  RPn tied to SPI2 Clock Output
SS2             001010  RPn tied to SPI2 Slave Select
C1TX(2)         001110  RPn tied to CAN1 Transmit
OC1             010000  RPn tied to Output Compare 1 Output
OC2             010001  RPn tied to Output Compare 2 Output
OC3             010010  RPn tied to Output Compare 3 Output
OC4             010011  RPn tied to Output Compare 4 Output
C1OUT           011000  RPn tied to Comparator Output 1
C2OUT           011001  RPn tied to Comparator Output 2
C3OUT           011010  RPn tied to Comparator Output 3
SYNCO1(1)       101101  RPn tied to PWM Primary Time Base Sync Output
QEI1CCMP(1)     101111  RPn tied to QEI 1 Counter Comparator Output
REFCLKO         110001  RPn tied to Reference Clock Output
C4OUT           110010  RPn tied to Comparator Output 4

 */

void configureUART2pins(void)
{
    //RPI24 will be the UART2 receive
    //RP36 will be the UART2 transmit

     RPINR19bits.U2RXR = 0x18;     
     RPOR1bits.RP36R = 0x03;        // 00   0011 - Maps to UART2TX
     return;
}



/* SerialTransmit() transmits a string to the UART2 TX pin MSB first
 *
 * Inputs: *buffer = string to transmit
 */
int SerialTransmit(const char *buffer)
{
    unsigned int size = strlen(buffer);
    while (size)
    {
        while (U2STAbits.UTXBF); // wait while TX buffer full
        U2TXREG = *buffer; // send single character to transmit buffer

        buffer++; // transmit next character on following loop
        size--; // loop until all characters sent (when size = 0)
    }

    while (!U2STAbits.TRMT); // wait for last transmission to finish

    return 0;
}

void sendAck(void) {
    SerialTransmit("ACK\n");

}
