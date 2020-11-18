// Joe Gamel
// ECEN2440 Final Project
// joystick peripheral
// main.c
// Version 1.4 - Last update: 11/18/20

#include "msp.h"
#include "js.h"
#include <stdio.h>

// Define size of array to store current & previous joystick input
#define   Num_of_Results   8
volatile uint16_t xin[Num_of_Results];
volatile uint16_t yin[Num_of_Results];
static uint8_t index;

void main(void) {
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	__disable_irq();            //disable interrupts

	config_gpio();
    config_adc();
    config_NVIC();

    __enable_irq();             //enable interrupts

    while(1) {
        adc_convert();
        printf("x-axis: %d\n", xin[index]);
        printf("y-axis: %d\n", yin[index]);
        printf("index: %d\n", index);
    }
}

void ADC14_IRQHandler(void) {
    if (ADC14->IFGR0 & ADC14_IFGR0_IFG1) {
        xin[index] = ADC14->MEM[0];
        yin[index] = ADC14->MEM[1];
        index = (index + 1) & 0x7;          // Increment results index, modulo
    }

    //ADC14->CLRIFGR0 |= ADC14_CLRIFGR0_CLRIFG0; // clr flg
    //ADC14->CLRIFGR0 |= ADC14_CLRIFGR0_CLRIFG1; // clr flg
    return;
}
