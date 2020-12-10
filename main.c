// Joe Gamel, James Brennan
// ECEN2440 Final Project
// joystick, HC-SR04 (US sensor) peripheral
// main.c
// Version 1.7 - Last update: 12/9/20

#include "msp.h"
#include "js.h"
#include "HC-SR04.h"
#include <stdio.h>

#define   Num_of_Results   8
int xin[Num_of_Results];
int yin[Num_of_Results];
int index;

void main(void) {
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	__disable_irq();            //disable interrupts

	stop_pulse();
	stop_capture();
	config_TimerA0_nvic();
	config_ADC_NVIC();
	config_pulse_timer();
	config_pulse_gpio();
	config_capture_timer();
	config_US_gpio();
	config_js_gpio();
    config_adc();

    __enable_irq();             //enable interrupts

    start_pulse();
    start_capture();
    adc_convert();

    while(1) {

    }
}

void ADC14_IRQHandler(void) {

        yin[index] = ADC14->MEM[0];
        xin[index] = ADC14->MEM[1];
        index = (index + 1) & 0x7;          // Increment results index, modulo

        printf("x-axis: %d\n", xin[index]);
        printf("y-axis: %d\n", yin[index]);
        printf("index: %d\n", index);

    ADC14->CLRIFGR0 |= ADC14_CLRIFGR0_CLRIFG0; // clr flg
    ADC14->CLRIFGR0 |= ADC14_CLRIFGR0_CLRIFG1; // clr flg
}

void TA0_N_IRQHandler(void){
    double a = 0;
    //printf("%.5lf", a);
    TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG; //clear flag
    //TIMER_A1->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG; //clear flag
    P1->OUT ^= BIT0;                            //toggle LED
    P2->OUT ^= BIT0;

    //the following code would preferably be moved to main and done on the processor

    a = TIMER_A1->CCR[2] - TIMER_A1->CCR[1];    //substracting positive edge count from negative edge
    a=(a/(187500));   //converts count to time based on input clock frequency
    a=a*(170);   //converts time to distance
    a=(.2422)*a-.0037;    //error correction function, experimentally determined, may need to be adjusted for individual sensors
    printf("%.5lf \n", a);  //prints distance to the console, useful for debugging
}
