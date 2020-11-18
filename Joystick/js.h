// Joe Gamel
// ECEN2440 Final Project
// joystick peripheral
// js.c
// Version 1.4 - Last update: 11/18/20

#ifndef JS_H_
#define JS_H_

#include "msp.h"


void config_gpio(void);
void config_adc(void);
void adc_convert(void);
void config_NVIC(void);

int read_joystick_x(void);
int read_joystick_y(void);

#endif
