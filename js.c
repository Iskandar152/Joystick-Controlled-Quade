

#include "js.h"

#define direction (char)

void config_gpio(void) {

    //LED for debug
    P1->DIR |= BIT0;

    //Analog pin 4.0 (A13) set as horizontal joystick input.
    P4->DIR &= ~(BIT0);
    P4->OUT |= BIT0;
    P4->SEL0 |= BIT0;
    P4->SEL0 |= BIT0;

    //Analog pin 4.2 (A11) set as vertical joystick input
    P4->DIR &= ~(BIT2);
    P4->OUT |= BIT2;
    P4->SEL0 |= BIT2;
    P4->SEL0 |= BIT2;

    //Pin 4.4 set as joystick switch input
    P4->DIR &= ~(BIT4);
    P4->OUT |= BIT4;

}

void config_adc(void) {

// ADC code modified from https://faculty-web.msoe.edu/johnsontimoj/EE2920/files2920/adc_14.pdf
// using A3 = P5.5
//
// keep enable low while making changes
//
// ctrl0
//                           PDIV SHSx SHP ISSH DIVx SSELx CONSEQx rptS BUSY SHT1x SHT0x MSC  xx ON xx ENC SC
//                            01  000   1   0   000  000     10    00    0   0000  1010    1  00 1  00  0  0
// Calculated - 0x44040290 =   0100    0100       0000      0100        0000    0010    1001       0000
// PDIV = 01b - pre-divider of 4 on clock source set with SSELx
// This the MODCLK (25MHz, from the SSELx = 000b)
// SHP = 1b signal is sourced from the sampling timer
// DIVx - ADC14 clock divider. 001b is a divider of 1
// TODO CONSEQx - conversion sequence mode select, 11b - repeat-sequence-of-channels
ADC14->CTL0 = 0x44040290;
// ctrl1
// xxxx no_sel x mem5  xxxxxxxxxx 12b unsigned on reg
// 0000 000000 0 00101 0000000000 10      0    0  00
ADC14->CTL1 = 0x00050020;
//mctl [5] - since using mem5
// xxxx xxxx xxxx xxxx x enb diffb x AVCC x xx A3
// 0000 0000 0000 0000 0  0    0   0 0000 0 00 00011
ADC14->MCTL [5] = 0x00000003;
// ier0 enable interrupts on channel 5
// 0000 0000 0000 0000 0000 0000 0010 0000
ADC14->IER0 |= 0x00000020;
// all others default
// set enable
ADC14->CTL0 |= 0x00000002;
return;
}

int adc_convert(void) {
// Start sampling/conversion
ADC14->CTL0 |= 0x00000001; // start conversion
// Wait for conversion to complete and grab value
// Conversion is complete when ADC0 flag is set
while (!ADC14->IFGR0) {
;
}
// returning a full int instead of a uint16_t for simplicity
return
ADC14->MEM[0];
}

void config_NVIC(void) {
// setup NVIC
// Enable ADC
// Note: ADC is INTISR(24)
NVIC->IP[24] |= 0x20; // Set a priority
NVIC->ISER[0] |= 0x01000000; // ISER0 starts at 0
return;
}

int read_joystick_x(void){
    xin = 0;
    xin

}
