/* 
 * File:   utilities.h
 * Author: Marthinus (Marno) Nel
 *
 * Created on June 4, 2023, 6:03 PM
 */

#ifndef UTILITIES_H
#define	UTILITIES_H

#include <xc.h> // processor SFR definitions
#include <sys/attribs.h> // __ISR macro
#include "nu32dip.h"

// Motor direction pins
#define MOTOR_1_DIRECTION LATBbits.LATB14

// Motor PWM / Speed variables
#define MOTOR_1_SPEED OC1RS

// Timer 3 Period
#define PR3_PERIOD 3750

// public function prototypes in the utilities h file
void setup_motor_timers_pins();

#endif	/* UTILITIES_H */