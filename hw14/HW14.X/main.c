#include "nu32dip.h" // constants, functions for startup and UART
#include <stdio.h>
#include "utilities.h"

int main(void) {
    NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
    setup_motor_timers_pins();                // Setup timers and interrupts for motor control

    while (1) {
		// use core timer to calculate refresh rate
        MOTOR_1_DIRECTION = 0; // Clockwise
        MOTOR_1_SPEED = (int)(PR3_PERIOD*(float)50.0/100.0);
        MOTOR_2_DIRECTION = 1; // Clockwise
        MOTOR_2_SPEED = (int)(PR3_PERIOD*(float)100.0/100.0);
    }
}