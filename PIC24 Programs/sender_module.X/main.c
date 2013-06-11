/*
 * File:   main.c
 * Author: Jake
 *
 * Created on May 2, 2013, 10:29 PM
 */

#include "prototypes.h"
#include "misc.h"
#include "uart_setup.h"
#include "adc_setup.h"

//#define _TEST

// enumerated type for state machine

enum sender_state
{
  initialize, connecting, connected_waiting, command_active, send_command
};
enum sender_state system_state;

// PIC24EP256GP204 Configuration Bit Settings


/*
#include <xc.h>

// FICD

#pragma config ICS = PGD3               // ICD Communication Channel Select bits (Communicate on PGEC3 and PGED3)
#pragma config JTAGEN = OFF             // JTAG Enable bit (JTAG is disabled)

// FPOR
#pragma config ALTI2C1 = OFF            // Alternate I2C1 pins (I2C1 mapped to SDA1/SCL1 pins)
#pragma config ALTI2C2 = OFF            // Alternate I2C2 pins (I2C2 mapped to SDA2/SCL2 pins)
#pragma config WDTWIN = WIN25           // Watchdog Window Select bits (WDT Window is 25% of WDT period)

// FWDT
#pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler bits (1:32,768)
#pragma config WDTPRE = PR128           // Watchdog Timer Prescaler bit (1:128)
#pragma config PLLKEN = ON              // PLL Lock Enable bit (Clock switch to PLL source will wait until the PLL lock signal is valid.)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable bit (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF              // Watchdog Timer Enable bit (Watchdog timer always enabled)

// FOSC
#pragma config POSCMD = XT              // Primary Oscillator Mode Select bits (XT Crystal Oscillator Mode)
#pragma config OSCIOFNC = OFF           // OSC2 Pin Function bit (OSC2 is clock output)
#pragma config IOL1WAY = ON             // Peripheral pin select configuration (Allow only one reconfiguration)
#pragma config FCKSM = CSDCMD           // Clock Switching Mode bits (Both Clock switching and Fail-safe Clock Monitor are disabled)


// FOSCSEL
#pragma config FNOSC = PRIPLL              // Oscillator Source Selection (Primary Oscillator with PLL module (XT + PLL, HS + PLL, EC + PLL))
#pragma config IESO = ON                // Two-speed Oscillator Start-up Enable bit (Start up device with FRC, then switch to user-selected oscillator source)

// FGS
#pragma config GWRP = OFF               // General Segment Write-Protect bit (General Segment may be written)
#pragma config GCP = OFF                // General Segment Code-Protect bit (General Segment Code protect is Disabled)


// The above was code generated by MPLAB using the Configuration bits window
 */

// Configuration for MPLAB v8
_FICD(ICS_PGD3 & JTAGEN_OFF)
_FPOR(ALTI2C1_ON & ALTI2C2_ON & WDTWIN_WIN25)
_FWDT(WDTPOST_PS32768 & WDTPRE_PR128 & PLLKEN_ON & WINDIS_OFF & FWDTEN_OFF)
_FOSC(POSCMD_XT & OSCIOFNC_OFF & IOL1WAY_ON & FCKSM_CSDCMD)
_FOSCSEL(FNOSC_PRIPLL & IESO_ON)
_FGS(GWRP_OFF & GCP_OFF)


char data;
char send_data;
char send_string[32] = "Hello World!";
char string[32];
int read_index;
int timer;
int wait_hold;
int timer_en;

int adc_raw;
int master_button;
int button_1_on;
int button_2_on;
int button_3_on;
int button_4_on;

int button_1_off;
int button_2_off;
int button_3_off;
int button_4_off;

int button_1_sent;
int button_2_sent;
int button_3_sent;
int button_4_sent;

int batt_check;
int batt_count;
int battery;

// Fail safe reset variables
int RESET = 0;
int reset_ack;
int reset_count;

int main(int argc, char** argv)
{
  int resp = 0;
  // Set all button counts to 0
  master_button = 0;
  button_1_on = 0;
  button_2_on = 0;
  button_3_on = 0;
  button_4_on = 0;
  
  button_1_off = 0;
  button_2_off = 0;
  button_3_off = 0;
  button_4_off = 0;

  button_1_sent = 0;
  button_2_sent = 0;
  button_3_sent = 0;
  button_4_sent = 0;

  // Initially check the battery voltage
  batt_check = 1;
  read_index = 0;

  configureOscillator();
  configureINT();
  setupUART2();
  configureUART2pins();
  configureT1();
  setupLEDs();
  setupButtons();
  setupADC1();
  // Configures miscellanious digital/analog inputs
  setupIO();
  // Setup diagnostic output to toggle when RESET is high
  TRISBbits.TRISB7 = 0;

  LED0 = 1;
  LED2 = 1;

  // make sure gate is closed
  BATTERY_CHECK_GATE = 0;

  // Turn on the timer
  T1CONbits.TON = 1;



  system_state = initialize;

  while (1)
  {
    switch (system_state)
    {
      case initialize:
        /*
         * State for initializing the bluetooth module.
         *
         * Will return to this state if we lost connection during transmission,
         * or if we do not get a response back from the paired bluetooth module
         *
         */
        CONNECTED_LED = 1;
        wait(10);
        ERROR_LED = 1;
        wait(10);

        LED0 = 0;
        wait(10);
        LED2 = 0;
        BLUETOOTH_RESET = 0;
        delay(1000);
        BLUETOOTH_RESET = 1;
        // Wait for Blueooth module to power up
        delay(4000);
        LED0 = 1;
        wait(10);
        LED2 = 1;


        // Check the battery voltage initially
        battery = checkBatteryVoltage();
        
        do
        {
          // send dollar signs to enter command mode
          char send1[] = "$$$";
          SerialTransmit(send1);

          // Reset the UART recieve buffer
          clear_recieve_buffer();

          // We should do something if we do not receive this back
          resp = expect_response("CMD\r\n", 1000);

          if (resp != 1)
          {
            BLUETOOTH_RESET = 0;
            delay(1000);
            BLUETOOTH_RESET = 1;
            delay(10000);
          }
        } while (resp != 1);

        SerialTransmit("GB\n");

        // Reset the UART recieve buffer
        clear_recieve_buffer();
        // We should do something if we do not receive this back
        resp = expect_response("0006664FAE62\r\n", 1000);

        char dev_id[] = "0006664FAE62\r\n";

        system_state = connecting;

        break;
      case connecting:
        /*
         * State for connecting to the reciever via BT.
         *
         * Handshaking works as follows:
         * 1. Send the receiver our BT ID, which we grabbed from the
         *    BT chip before entering the state machine
         * 2. Expect the receiver to send it's BT ID back. It's
         *    hardcoded in here.
         *
         * When it works, we switch to the connected state. Otherwise,
         * retry. Maybe go to an error case. Haven't written that yet.
         */

        // Assuming we're already in CMD mode from pre-state
        // machine stuff..

        do
        {
          // Reset the UART recieve buffer
          clear_recieve_buffer();

          // Fire off a connect command to the bluetooth
          SerialTransmit("C,0006664D63FA\n");

          // Wait for acknowledge from the BT chip
          resp = expect_response("TRYING\r\n", 1000);

          if (resp != 1)
          {
            ERROR_LED = 0;
            // Begin count down
            RESET = 1;
            while(reset_ack != 1)
              wait(10);
          }
          
        } while((resp != 1) && (system_state != initialize));
        // If we need to reinitialize, get out of current state
        if(system_state == initialize)
          break;

        // Turn off error and kill countdown
        ERROR_LED = 1;
        RESET = 0;
        // Make sure that it is out of RESET = 1 state
        while(reset_ack != 0)
          wait(10);


        /* We can check the status bit, #defined as CONNECTION_STATUS
         * in order to determine if we are connected. Once we have
         * connection, then we should proceed.
         */
        do
        {
          // Similar to expect_response, however this function
          // checks the CONNECTION_STATUS bit for a 1, instead of string match
          resp = expect_connection(1000);

          if(resp != 1)
          {
            ERROR_LED = 0;
            // Begin count down
            RESET = 1;
            while(reset_ack != 1)
              wait(10);
          }

        } while((resp != 1) && (system_state != initialize));
        // If we need to reinitialize, get out of current state
        if(system_state == initialize)
          break;

        // Turn off error and kill countdown
        ERROR_LED = 1;
        RESET = 0;
        // Make sure that it is out of RESET = 1 state
        while(reset_ack != 0)
          wait(10);

        do
        {
          // Send our BT chip's address for handshaking.
          SerialTransmit(dev_id);

          // Reset the UART recieve buffer again
          clear_recieve_buffer();
          
          // Wait for the other BT chip to respond with its address
          resp = expect_response("0006664D63FA\r\n", 1000);

          /*
           * At this point, if we do not receive the response back,
           * we may want to restart the whole state, and reset the
           * bluetooth module. The output #defined as BLUETOOTH_RESET
           * can accomplish it.
           */
          if(resp != 1)
          {
            ERROR_LED = 0;
            // Begin count down
            RESET = 1;
            while(reset_ack != 1)
              wait(10);
          }

        } while((resp != 1) && (system_state != initialize));
        // If we need to reinitialize, get out of current state
        if(system_state == initialize)
          break;


        // Turn off error and kill countdown
        ERROR_LED = 1;
        RESET = 0;
        // Make sure that it is out of RESET = 1 state
        while(reset_ack != 0)
          wait(10);


// If we make it here, we're connected!
        // Turn on the connection LED
        wait(10);
        CONNECTED_LED = 0;
        // Switch to connected state
        system_state = connected_waiting;
        break;
      case connected_waiting:
        /*
         * State for when the BT is connected. Poll the command button,
         * change state if warranted, rinse and repeat.
         *
         * Make sure that we are filtering the analog signal so that only
         * a "real" press changes state, as opposed to an accidental
         * press of the command button
         */

        /*
         * Hack: Send Jimmy's commands to him in a loop
         */
        delay(100);
#ifdef _TEST
        while(1)
        {
          // Send command over BT
          SerialTransmit("LeftBlink\r\n");

          // Reset the UART recieve buffer again
          clear_recieve_buffer();
          
          // wait for ACK from reciever
          // TODO: timeout logic
          resp = expect_response("ACK\n", 3000);

          delay(3000);

          // Send command over BT
          SerialTransmit("RightBlink\r\n");

          // Reset the UART recieve buffer again
          clear_recieve_buffer();

          // wait for ACK from reciever
          // TODO: timeout logic
          resp = expect_response("ACK\n", 3000);

          delay(3000);
          // Send command over BT
          SerialTransmit("HeadLights\r\n");

          // Reset the UART recieve buffer again
          clear_recieve_buffer();

          // wait for ACK from reciever
          // TODO: timeout logic
          resp = expect_response("ACK\n", 3000);

          delay(3000);
          // Send command over BT
          SerialTransmit("Wipers\r\n");

          // Reset the UART recieve buffer again
          clear_recieve_buffer();

          // wait for ACK from reciever
          // TODO: timeout logic
          resp = expect_response("ACK\n", 3000);

          delay(3000);
          // Send command over BT
          SerialTransmit("HornOn\r\n");

          // Reset the UART recieve buffer again
          clear_recieve_buffer();

          // Reset the UART recieve buffer again
          clear_recieve_buffer();

          // wait for ACK from reciever
          // TODO: timeout logic
          resp = expect_response("ACK\n", 3000);

          delay(3000);
          // Send command over BT
          SerialTransmit("HornOff\r\n");

          // Reset the UART recieve buffer again
          clear_recieve_buffer();

          // wait for ACK from reciever
          // TODO: timeout logic
          resp = expect_response("ACK\n", 3000);

          delay(3000);
        }
#endif

//  This will be checked in the timer interrupt, and the change of state will
//  be taken care of there.

        
//        // Change to the master enable button to poll the command button
//        changeADCinput(MASTER_EN_BUTTON);
//        // Poll ADC input, with digital filtering
//        for (i = 0; i < 100; i++)
//        {
//          readADC(&adc_raw);
//          if (adc_raw > BUTTON_ON_ADC)
//            master_button++;
//          wait(10);
//        }
//
//        if (master_button == 100)
//        {
//          system_state = command_active;
//          LED0 = 0;
//        }
//        else
//        {
//          system_state = connected_waiting;
//          LED0 = 1;
//        }
//
//        master_button = 0;

        LED0 = 1;
        wait(1000);
        break;
      case command_active:
        LED0 = 0;

        /*
         * State for when the command button is pressed.
         * Check the other buttons, fire the necessary command over BT,
         * wait for ack for a certain amount of time, and retry.
         *
         * Since the horn will be activated for all buttons pressed,
         * we should make sure that the user has enough time to press
         * all buttons before we send the command. Thus we should
         * setup some sort of timer once one of the buttons is pressed
         * to allow more time for the user to press all buttons.
         */

        // Change to the button
        changeADCinput(BUTTON_1);
        // Poll ADC input, with digital filtering
        readADC(&adc_raw);
        if (adc_raw > BUTTON_ON_ADC)
          button_1_on++;
        else
          button_1_off++;

        // Change to the button
        changeADCinput(BUTTON_2);
        // Poll ADC input, with digital filtering
        readADC(&adc_raw);
        if (adc_raw > BUTTON_ON_ADC)
          button_2_on++;
        else
          button_2_off++;

        // Change to the button
        changeADCinput(BUTTON_3);
        // Poll ADC input, with digital filtering
        readADC(&adc_raw);
        if (adc_raw > BUTTON_ON_ADC)
          button_3_on++;
        else
          button_3_off++;

        // Change to the button
        changeADCinput(BUTTON_4);
        // Poll ADC input, with digital filtering
        readADC(&adc_raw);
        if (adc_raw > BUTTON_ON_ADC)
          button_4_on++;
        else
          button_4_off++;

        // If the button has been pressed enough times without going off
        // Send message
        if((button_1_on > BUTTON_ON_COUNT) && (button_1_sent == 0))
        {
          LOW_VOLT_LED = 0;
          // Send command over BT
          SerialTransmit("LeftBlink\r\n");

          // Reset the UART recieve buffer again
          clear_recieve_buffer();

          // wait for ACK from reciever
          while (strcmp(string, "ACK\n") == 0);

          button_1_sent = 1;
          button_1_on = 0;
          button_1_off = 0;
        }
        // Else if the button has been off half as long as on, reset both counts
        else if(button_1_off > (BUTTON_ON_COUNT/2))
        {
          LOW_VOLT_LED = 1;
          button_1_on = 0;
          button_1_off = 0;
          button_1_sent = 0;
        }

        // If the button has been pressed enough times without going off
        // Send message
        if((button_2_on > BUTTON_ON_COUNT) && (button_2_sent == 0))
        {
          LOW_VOLT_LED = 0;
          // Send command over BT
          SerialTransmit("RightBlink\r\n");

          // Reset the UART recieve buffer again
          clear_recieve_buffer();

          // wait for ACK from reciever
          while (strcmp(string, "ACK\n") == 0);

          button_2_sent = 1;
          button_2_on = 0;
          button_2_off = 0;
        }
        // Else if the button has been off half as long as on, reset both counts
        else if(button_2_off > (BUTTON_ON_COUNT/2))
        {
          LOW_VOLT_LED = 1;
          button_2_on = 0;
          button_2_off = 0;
          button_2_sent = 0;
        }

        // If the button has been pressed enough times without going off
        // Send message
        if((button_3_on > BUTTON_ON_COUNT) && (button_3_sent == 0))
        {
          LOW_VOLT_LED = 0;
          // Send command over BT
          SerialTransmit("HeadLights\r\n");

          // Reset the UART recieve buffer again
          clear_recieve_buffer();

          // wait for ACK from reciever
          while (strcmp(string, "ACK\n") == 0);

          button_3_sent = 1;
          button_3_on = 0;
          button_3_off = 0;
        }
        // Else if the button has been off half as long as on, reset both counts
        else if(button_3_off > (BUTTON_ON_COUNT/2))
        {
          LOW_VOLT_LED = 1;
          button_3_on = 0;
          button_3_off = 0;
          button_3_sent = 0;
        }


        // If the button has been pressed enough times without going off
        // Send message
        if((button_4_on > BUTTON_ON_COUNT) && (button_4_sent == 0))
        {
          LOW_VOLT_LED = 0;
          // Send command over BT
          SerialTransmit("Wipers\r\n");

          // Reset the UART recieve buffer again
          clear_recieve_buffer();

          // wait for ACK from reciever
          while (strcmp(string, "ACK\n") == 0);

          button_4_sent = 1;
          button_4_on = 0;
          button_4_off = 0;
        }
        // Else if the button has been off half as long as on, reset both counts
        else if(button_4_off > (BUTTON_ON_COUNT/2))
        {
          LOW_VOLT_LED = 1;
          button_4_on = 0;
          button_4_off = 0;
          button_4_sent = 0;
        }


        // temporary: wait before proceeding
        wait(1000);
        break;
      case send_command:

        break;

    }
  }

  return (EXIT_SUCCESS);
}

void _ISR _T1Interrupt(void)
{
  if (timer_en == 1)
  {
    // timer variable must be set ahead of time
    // (the delay function does this)
    timer_en = 0;
    wait_hold = 0;
  }
  else if (timer > 0)
  {
    timer--;
    wait_hold = 0;
  }
  else
  {
    timer = 0;
    wait_hold = 1;
  }

  // We only want this code to run if we are in connected waiting or command active
  // and we are connected
  if(CONNECTION_STATUS == 1 && (system_state == connected_waiting || system_state == command_active))
  {
    // Change to the master enable button to poll the command button
    changeADCinput(MASTER_EN_BUTTON);
    // Poll ADC input, with digital filtering
    readADC(&adc_raw);
    if (adc_raw > BUTTON_ON_ADC)
      master_button++;
    else
      master_button = 0;

    if (master_button >= 5)
      system_state = command_active;
    else
      system_state = connected_waiting;
  }
  // If we lose the connection, change state back to connecting
  else if(CONNECTION_STATUS != 1 && (system_state == connected_waiting || system_state == command_active))
      system_state = connecting;


  // Timer to check the battery, is self triggered
  if (batt_check == 1)
  {
    battery = checkBatteryVoltage();
    batt_check = 0;
    batt_count = 0;
  }
  else if (batt_count < TEN_MIN)
    batt_count++;
  else
  {
    batt_count = 0;
    batt_check = 1;
  }

  if (battery <= LOW_VOLTAGE)
    LOW_VOLT_LED = 0;
  else
    wait(10);   //    LOW_VOLT_LED = 1;


  // Fail safe. If we get stuck somewhere, and the
  // RESET integer counts for a minute, reset the
  // whole state machine.
  if(RESET == 1)
  {
    PORTBbits.RB7 = !PORTBbits.RB7;
    // Begin reset sequence
    if(reset_ack == 0)
    {
      reset_ack = 1;
      reset_count = RESET_DELAY;
    }
    // Count down!!
    else if(reset_count > 0)
      reset_count--;
    // BOOM! We're fucked, restart everything
    else
    {
      RESET = 0;
      reset_ack = 0;
      reset_count = 0;
      system_state = initialize;
    }
  }
  // No RESET? Clear the counters and ack
  else
  {
    reset_ack = 0;
    reset_count = 0;
  }

  // Clear the interrupt flag
  IFS0bits.T1IF = 0;
}

void _ISR _U2RXInterrupt(void)
{
  // put the received character into a var
  data = U2RXREG;

  // put the char into the read buffer at the right spot
  string[read_index] = data;
  read_index++;

  // Clear the RX interrupt Flag
  IFS1bits.U2RXIF = 0;
}

void delay(int wait_ms)
{
  /*
   * Uses the timer interrupt to wait a given number of milliseconds.
   * Must be a multiple of 16ms (which is the timer rate), or it will
   * be rounded down to the nearest 16ms multiple.
   *
   * NOTE: this is not highly precise. It's pretty close, but allow a little
   * leeway if precision of your wait times are critical
   */

  // calculate number of timer "ticks" to put on the countdown
  timer = wait_ms / 16;

  // clear the "timer is done" flag
  wait_hold = 0;

  // start the countdown (see the timer interrupt to see how this works)
  timer_en = 1;

  // wait til the countdown is done
  while (wait_hold == 0);

  return;

}


