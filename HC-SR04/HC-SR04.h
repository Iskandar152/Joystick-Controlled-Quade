/*
 * pwm.h
 *
 *  Created on: Sep 14, 2020
 *      Author: James Brennan
 */

#ifndef HC_SR04_H_
#define HC_SR04_H_

#include "msp.h"
#include <stdio.h>

//((3MHz)/(s^4)*

#define TICKS ((uint16_t)0xB3B0)
#define TICKS2 ((uint16_t)0xB3A8)   //output pulse width equals period(Ticks)-period(Ticks2), if ticks or ticks2 were loaded into CCR[0] during up mode

void config_pulse_timer(void);

void start_pulse(void);

void stop_pulse(void);

void config_pulse_gpio(void);

void config_capture_timer(void);

void start_capture(void);

void stop_capture(void);

void config_gpio(void);

void config_nvic(void);


#endif /* HC_SR04_H_ */
