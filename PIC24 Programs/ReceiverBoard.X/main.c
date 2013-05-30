/* 
 * File:   main.c
 * Author: James Paul
 *
 * Created on May 30, 2013, 2:10 PM
 */

#include "uart_setup.h"
#include "prototypes.h"
#include <p24Exxxx.h>

// PIC24EP256GP204 Configuration Bit Settings
// TODO: Create these from Window > PICMemoryViews > ConfigurationBits
//

#include <xc.h>

/*
 * 
 */
int main(int argc, char** argv) {

    configureOscillator();
    configureINT();
    setupUART2();
    configureUART2pins();
    configureT1();
    setupLEDs();

    return(0);
}

