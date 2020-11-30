/*
 * pwm.c
 *
 *  Created on: Sep 14, 2020
 *  Author: James Brennan
 */
#include <HC-SR04.h>
#include "msp.h"
#include <stdio.h>

void config_pulse_timer(void){
    TIMER_A0->CTL |= TIMER_A_CTL_CLR;   //clear
    TIMER_A0->CTL |= TIMER_A_CTL_SSEL__SMCLK; //3MHz Clock
    TIMER_A0->CTL |= TIMER_A_CTL_ID__4;  //divide by 2^4
    TIMER_A0->CCR[0] |= TICKS; //load CCR0 with calculated value
    TIMER_A0->CCR[1] |= TICKS2;
    TIMER_A0->CCTL[1] |= TIMER_A_CCTLN_OUTMOD_7;  //output mode switched to transition mode
    TIMER_A0->CCTL[1] &= TIMER_A_CCTLN_OUTMOD_2;  //select toggle/reset output mode
    TIMER_A0->CCTL[1] |= TIMER_A_CCTLN_CCIE; //capture/compare interrupt

}

//start the timer by making  MC > 0

void start_pulse(void){
    TIMER_A0->CTL |= TIMER_A_CTL_MC__UP;   //up mode, counts to TAxCCR0, starts timer
}

void stop_pulse(void){
    TIMER_A0->CTL &= ~(TIMER_A_CTL_MC__STOP); //stops timer, timer must be stopped before it can be configured
}

void config_pulse_gpio(void) {
    P2->DIR |= BIT4;
    P2->OUT |= BIT4;
    P2->DS |= BIT4;
    P2->SEL0 |= BIT4;    // SEL0 = 0b01
    P2->SEL1 &= ~BIT4;   // SEL1  = 0b00
}
//clear all mode control bits in TAxCTL register to put into stop mode

void config_gpio(void){    //configuring output bits of the pulse signal, also lights an LED for visual feedback
    P1->DIR |= BIT0;
    P1->OUT |= BIT0;
    P1->DS |= BIT0;

    P2->DIR |= BIT0;
    P2->DS |= BIT0;
    P2->OUT |= BIT0;

}

void config_nvic(void) {
    __NVIC_EnableIRQ(TA0_N_IRQn);   //enabling the NVIC
}

void config_capture_timer(void){
    TIMER_A1->CTL |= TIMER_A_CTL_CLR;   //clear
    TIMER_A1->CTL |= TIMER_A_CTL_SSEL__SMCLK; //3MHz Clock
    TIMER_A1->CTL |= TIMER_A_CTL_ID__4;  //divide by 2^4
    TIMER_A1->CCR[0] |= TICKS; //load CCR0 with calculated value
    TIMER_A1->CCTL[0] |= TIMER_A_CCTLN_OUTMOD_7;  //output mode switched to transition mode
    TIMER_A1->CCTL[0] &= TIMER_A_CCTLN_OUTMOD_4;  //select Toggle output mode


    //posedge capture

    TIMER_A1->CCTL[1] |= TIMER_A_CCTLN_CCIS_0; // TA1.CCIA selected as the capture pin
    P7->SEL0 |= BIT7; //Port 7-7 secondary function
    P7->DIR &= ~BIT7;
    TIMER_A1->CCTL[1] |= TIMER_A_CCTLN_SCS; //synchronous capture


    TIMER_A1->CCTL[1] |= TIMER_A_CCTLN_CAP;//select capture mode
    //TIMER_A1->CCTL[1] |= TIMER_A_CCTLN_CCIE; //capture/compare interrupt
    TIMER_A1->CCTL[1] |= TIMER_A_CCTLN_CM_1; //capture on positive edge
    //TIMER_A1->CCTL[1] |= TIMER_A_CCTLN_CCIS_0; // TA1.CCIA selected as the capture pin

    //P7->SEL0 |= BIT7;
    //P7->DIR &= ~BIT7;

    TIMER_A1->CCTL[1] |= TIMER_A_CCTLN_CCIE; //capture/compare interrupt

    //negedge capture

    TIMER_A1->CCTL[2] |= TIMER_A_CCTLN_CCIS_0; // TA1.CCIA selected as the capture pin
    P7->SEL0 |= BIT6;
    P7->DIR &= ~BIT6;
    TIMER_A1->CCTL[2] |= TIMER_A_CCTLN_SCS; //synchronous capture

    TIMER_A1->CCTL[2] |= TIMER_A_CCTLN_CAP;//select capture mode
    //TIMER_A1->CCTL[2] |= TIMER_A_CCTLN_CCIE; //capture/compare interrupt
    TIMER_A1->CCTL[2] |= TIMER_A_CCTLN_CM_2 ; //capture on negative edge
    //TIMER_A1->CCTL[2] |= TIMER_A_CCTLN_CCIS_0; // TA1.CCIA selected as the capture pin

    //P7->SEL0 |= BIT6;
    //P7->DIR &= ~BIT6;

    TIMER_A1->CCTL[2] |= TIMER_A_CCTLN_CCIE; //capture/compare interrupt





}

//start the timer by making  MC > 0

void start_capture(void){
    TIMER_A1->CTL |= TIMER_A_CTL_MC__UP;   //up mode, counts to TAxCCR0, starts timer
}

void stop_capture(void){
    TIMER_A1->CTL &= ~(TIMER_A_CTL_MC__STOP);   //stops capture timer
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
