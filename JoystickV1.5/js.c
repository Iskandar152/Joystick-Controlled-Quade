// Joe Gamel
// ECEN2440 Final Project
// joystick peripheral
// js.c
// Version 1.5 - Last update: 11/19/20
//
// Using a joystick peripheral as follows:
// - x-axis input to pin 4.0 (A13)
// - y-axis input to pin 4.2 (A11)
// - switch input to pin 4.4

#include "js.h"
#include "msp.h"
#include <stdio.h>

// Define size of array to store current & 7 previous joystick conversions
#define   Num_of_Results   8
int xin[Num_of_Results];
int yin[Num_of_Results];
int index;

void config_gpio(void) {
    //Tertiary mode (SELx = 1) must be selected for analog input signals

    //LED for debug
    P1->DIR |= BIT0;

    //Analog pin 4.0 (A13) set as horizontal joystick input.
    P4->DIR &= ~BIT0;
    P4->OUT |= BIT0;
    P4->SEL0 |= BIT0;
    P4->SEL1 |= BIT0;

    //Analog pin 4.2 (A11) set as vertical joystick input
    P4->DIR &= ~BIT2;
    P4->OUT |= BIT2;
    P4->SEL0 |= BIT2;
    P4->SEL1 |= BIT2;

    //Pin 4.4 set as joystick switch input
    P4->DIR &= ~BIT4;
    P4->OUT |= BIT4;
}

void config_adc(void) {
// ADC code modified from https://faculty-web.msoe.edu/johnsontimoj/EE2920/files2920/adc_14.pdf (johnsontimoj, 07/2018)
//  and msp432p401x_adc14_06.c (William Goh, 10/2016)
// using A11 = P4.2 and A13 = P4.0
// NOTE: keep enable low while making changes

//CTL0 setup
    ADC14->CTL0 |= ADC14_CTL0_ON;            //Turn on ADC
    ADC14->CTL0 |= ADC14_CTL0_MSC;          // Auto conversions after 1st is completed
    ADC14->CTL0 |= ADC14_CTL0_SHT0__8;          // 8 cycle sample period
    ADC14->CTL0 |= ADC14_CTL0_SHT1__8;          // 8 cycle sample period
    ADC14->CTL0 |= ADC14_CTL0_CONSEQ_3;          // Repeat-sequence-of-channels
    ADC14->CTL0 |= ADC14_CTL0_SHP;          // SAMPCON source from sampling timer
    ADC14->CTL0 |= ADC14_CTL0_PDIV__4;          // pre-divider of 4

//CTL1 setup
    ADC14->CTL1 |= ADC14_CTL1_RES__12BIT;           //RES = 10b - 12 bit ADC resolution
    //ADC14->CTL1 &= ~ADC14_CTL1_CSTARTADD_MASK;           //CSTARTADD = 00000b - start conversion at MEM[0]

//MCTL setup
    ADC14->MCTL[0] |= ADC14_MCTLN_INCH_11;          // MCTL[0] = A11 - y-axis
    ADC14->MCTL[1] |= ADC14_MCTLN_INCH_13|ADC14_MCTLN_EOS;      // MCTL[1] = A13 - x-axis, end of sequence

    ADC14->IER0 |= ADC14_IER0_IE0|ADC14_IER0_IE1;           // Interrupt enabled
}

void adc_convert(void) {

    ADC14->CTL0 |= ADC14_CTL0_ENC|ADC14_CTL0_SC;         // Start conversion-software trigger
}

void config_NVIC(void) {

    __NVIC_EnableIRQ(ADC14_IRQn);           // Enable ADC interrupts 0 and 1 in NVIC module
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
