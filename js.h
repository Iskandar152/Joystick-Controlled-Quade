

#ifndef JS_H_
#define JS_H_

#include "msp.h"



void config_gpio(void);
void config_adc(void);
int adc_convert(void);
void config_NVIC(void);

int read_joystick_x(void);
int read_joystick_y(void);

#endif
