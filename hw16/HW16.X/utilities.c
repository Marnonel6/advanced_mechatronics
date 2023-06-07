#include "utilities.h"
#include <stdio.h>
#include "nu32dip.h"

/*
 * Setup PWM motor speed controller pins and Timer
 */
void setup_motor_timers_pins()
{
    // Select B12 and B14 pins for motor direction control
    TRISBCLR = 0b101000000000000; // 12 & 14
    MOTOR_RIGHT_DIRECTION = 0; // Clockwise
    MOTOR_LEFT_DIRECTION = 0; // Clockwise

    // 20kHz Timer for PWM for each motor
    T3CONbits.TCKPS = 0b001;      // Timer3 prescaler
    PR3 = PR3_PERIOD - 1;         // Period
    TMR3 = 0;                     // initial TMR3 count is 0
    T3CONbits.ON = 1;             // turn on Timer3
    // Assign OC1 to B15 -> Motor 1 PWM
    RPB15Rbits.RPB15R = 0b0101;
    OC1CONbits.OCM = 0b110;       // PWM mode without fault pin; other OC1CON bits are defaults
    MOTOR_RIGHT_SPEED = 1000;         // duty cycle = OC1RS/(PR3+1) = X%
    OC1R = 1000;                  // initialize before turning OC1 on; afterward it is read-only
    OC1CONbits.ON = 1;            // turn on OC1
    OC1CONbits.OCTSEL = 1;        // Use Timer 3
    // Assign OC4 to B13 -> Motor 2 PWM
    RPB13Rbits.RPB13R = 0b0101;
    OC4CONbits.OCM = 0b110;       // PWM mode without fault pin; other O4CCON bits are defaults
    MOTOR_LEFT_SPEED = 1000;         // duty cycle = OC4RS/(PR3+1) = X%
    OC4R = 1000;                  // initialize before turning OC4 on; afterward it is read-only
    OC4CONbits.ON = 1;            // turn on OC4
    OC4CONbits.OCTSEL = 1;        // Use Timer 3
}