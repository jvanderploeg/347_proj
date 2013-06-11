#include "prototypes.h"
#include "misc.h"
#include "uart_setup.h"
#include "adc_setup.h"

#define Baud2BRG(desired_baud)      ( (SYSCLK / (16*desired_baud))-1)

void wait(int num)
{
  int i;
  for (i = 0; i < num; i++);
}

void configureOscillator(void)
{
  // Select the PLL to give a max of 140 MHz into the system,
  // which gives a 70 MHz clock to the system.
  // Starting with a 8MHz clock input, PLLPRE = 0 gives prediv2
  // PLLFBD = 68 gives M = 70, and PLLPOST = 0 gives postdiv2
  // Thus, frequency should be F = 8/2*70/2 = 140MHz -> 70MHz system
  CLKDIVbits.PLLPRE = 0x00;
  wait(100);
  PLLFBDbits.PLLDIV = 68;
  wait(100);
  CLKDIVbits.PLLPOST = 0x00;
  wait(100);

  CLKDIVbits.DOZEN = 0;
  wait(100);
  CLKDIVbits.DOZE = 0x00;
  wait(100);
  CLKDIVbits.DOZEN = 1;
  wait(100);

}

void configureINT(void)
{
  // Enable interrupts
  INTCON2bits.GIE = 1;
}

void configureT1(void)
{
  //    Setup the timer
  // Turn off the module
  T1CONbits.TON = 0;
  // Continue timer in idle mode
  T1CONbits.TSIDL = 0;
  // Not gated accumulation
  T1CONbits.TGATE = 0;
  // 1:1 scaling
  T1CONbits.TCKPS = 0x02;
  // Not synchronized to external clock
  T1CONbits.TSYNC = 0;
  // Select internal clock
  T1CONbits.TCS = 0;
  // Clear the timer register
  TMR1 = 0x0000;
  // Pretty close to 1ms given the system clock
  PR1 = 17500;

  //    Setup the interrupt
  // Set the priority of T1 interrupt to 4
  IPC0bits.T1IP = 0x04;
  // Clear the interrupt flag
  IFS0bits.T1IF = 0;
  // Enable timer1 interrupt
  IEC0bits.T1IE = 1;


}

int UART2Transmit(const char *buffer)
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

void setupLEDs(void)
{
  TRISBbits.TRISB13 = 0; // port D1 - not soldered at the moment
  TRISAbits.TRISA10 = 0; // port D2
  TRISAbits.TRISA7 = 0; // port D3 - not soldered at the moment
  TRISBbits.TRISB14 = 0; // port D4
  TRISBbits.TRISB15 = 0; // port D5

  PORTBbits.RB13 = 1; // port D1 - not soldered at the moment
  wait(10);
  PORTAbits.RA10 = 1; // port D2
  wait(10);
  PORTAbits.RA7 = 1; // port D3 - not soldered at the moment
  wait(10);
  PORTBbits.RB14 = 1; // port D4
  wait(10);
  PORTBbits.RB15 = 1; // port D5


  // Following code is for old prototype board:
  /*
   // Configure them as digital outputs
  ANSELAbits.ANSA0 = 0;
  ANSELAbits.ANSA1 = 0;

  // Set them to outputs
  TRISAbits.TRISA10 = 0;
  TRISAbits.TRISA7 = 0;
  TRISAbits.TRISA0 = 0;
  TRISAbits.TRISA1 = 0;
  TRISCbits.TRISC2 = 0;
   */
}

void setupButtons(void)
{
  // setup AN0
  TRISAbits.TRISA0 = 1;
  ANSELAbits.ANSA0 = 1;

  // setup AN1/RA1
  TRISAbits.TRISA1 = 1;
  ANSELAbits.ANSA1 = 1;

  // setup AN4/RB2
  TRISBbits.TRISB2 = 1;
  ANSELBbits.ANSB2 = 1;

  // setup AN5/RB3
  TRISBbits.TRISB3 = 1;
  ANSELBbits.ANSB3 = 1;

  // setup AN6/RC0
  TRISCbits.TRISC0 = 1;
  ANSELCbits.ANSC0 = 1;
}

void setupIO(void)
{
  // Make sure the output driving the battery checking circuit is an output
  TRISCbits.TRISC9 = 0;

  // Make sure the battery voltage input is setup as an analog input
  TRISCbits.TRISC2 = 1;
  // Make sure it is configured as analog input
  ANSELCbits.ANSC2 = 1;

  // Have the bluetooth connection status bit setup as input to processor
  // Configure as digital input
  // Turn off analog
  ANSELBbits.ANSB8 = 0;
  TRISBbits.TRISB8 = 1;

  // Setup the reset input to the bluetooth as an output, and drive it high
  TRISBbits.TRISB9 = 0;
  // Drive it high
  PORTBbits.RB9 = 1;
}

void testLEDs(void)
{
  int i;
  LED0 = 0;

  for (i = 0; i < 1000; i++)
    wait(1000);

  LED0 = 1;

  for (i = 0; i < 1000; i++)
    wait(1000);

  LED0 = 0;

  for (i = 0; i < 1000; i++)
    wait(1000);

  CONNECTED_LED = 0;

  for (i = 0; i < 1000; i++)
    wait(1000);

  CONNECTED_LED = 1;

  for (i = 0; i < 1000; i++)
    wait(1000);

  CONNECTED_LED = 0;

  for (i = 0; i < 1000; i++)
    wait(1000);

//  LED2 = 0;

  for (i = 0; i < 1000; i++)
    wait(1000);

//  LED2 = 1;

  for (i = 0; i < 1000; i++)
    wait(1000);

//  LED2 = 0;

  for (i = 0; i < 1000; i++)
    wait(1000);

  LOW_VOLT_LED = 0;

  for (i = 0; i < 1000; i++)
    wait(1000);

  LOW_VOLT_LED = 1;

  for (i = 0; i < 1000; i++)
    wait(1000);

  LOW_VOLT_LED = 0;

  for (i = 0; i < 1000; i++)
    wait(1000);


  ERROR_LED = 0;

  for (i = 0; i < 1000; i++)
    wait(1000);

  ERROR_LED = 1;

  for (i = 0; i < 1000; i++)
    wait(1000);

  ERROR_LED = 0;

  for (i = 0; i < 1000; i++)
    wait(1000);
}

void checkCommand(void)
{
  int state = 0;

  // check AN0 (port P7)
  changeADCinput(0);

  int adc_read = -1;
  readADC(&adc_read);

  if (adc_read > 200)
  {
    state++;
  }

  // check AN4 (port P4)
  changeADCinput(4);

  adc_read = -1;
  readADC(&adc_read);

  if ((adc_read > 200) && (state > 0))
  {
    state++;
  }

  if (state == 2)
  {
//    LED2 = 0;
  } else
  {
//    LED2 = 1;
  }

  return;
}

int expect_response(char* resp, int timeout_ms)
{
  int match_sentinel = 0; // signifies whether we got the response we want

  // calculate number of timer "ticks" to put on the countdown
  timer = timeout_ms / 16;

  // clear the "timer is done" flag
  wait_hold = 0;

  // start the countdown (see the timer interrupt to see how this works)
  timer_en = 1;


  // wait for either the response to come back or the timer to run out
  while ((match_sentinel == 0) && (wait_hold == 0))
  {
    if (strcmp(string, resp) == 0)
    {
      match_sentinel = 1;
    }
    else
    {
      match_sentinel = 0;
    }
  }

  // if we got what we wanted, return 1, otherwise return 0.
  return match_sentinel;
}

int expect_connection(int timeout_ms)
{
  int match_sentinel = 0; // signifies whether we got the response we want

  // calculate number of timer "ticks" to put on the countdown
  timer = timeout_ms / 16;

  // clear the "timer is done" flag
  wait_hold = 0;

  // start the countdown (see the timer interrupt to see how this works)
  timer_en = 1;


  // wait for either the response to come back or the timer to run out
  while ((match_sentinel == 0) && (wait_hold == 0))
  {
    if (CONNECTION_STATUS == 1)
    {
      match_sentinel = 1;
    }
    else
    {
      match_sentinel = 0;
    }
  }

  // if we got what we wanted, return 1, otherwise return 0.
  return match_sentinel;

}

void clear_recieve_buffer(void)
{
  // Clear the UART recieve buffer
  memset(string, 0, 32);

  // Clear the UART recieve index
  read_index = 0;
}

int checkBatteryVoltage(void)
{
  /*
   * Check the battery voltage
   */

  int bat_volt;
  // Open battery gate
  BATTERY_CHECK_GATE = 1;
  // Change ADC input to battery input
  changeADCinput(8);
  // Read adc
  readADC(&bat_volt);
  // Close battery gate
  BATTERY_CHECK_GATE = 0;

  return bat_volt;
}




