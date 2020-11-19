// Joe Gamel
// ECEN2440 Final Project
// joystick peripheral
// main.c
// Version 1.5 - Last update: 11/19/20

#include "msp.h"
#include "js.h"
#include <stdio.h>


void main(void) {
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	__disable_irq();            //disable interrupts

	config_gpio();
    config_adc();
    config_NVIC();

    __enable_irq();             //enable interrupts

    adc_convert();

    while(1) {

    }
}
