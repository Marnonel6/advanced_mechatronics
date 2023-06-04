#include "nu32dip.h" // constants, functions for startup and UART
#include <stdio.h>
#include "utilities.h"

int main(void) {
    NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
    setup_motor_timers_pins();                // Setup timers and interrupts for motor control

    while (1) {
		// use core timer to calculate refresh rate
        MOTOR_DIRECTION = 0; // Clockwise
        OC1RS = (int)(PR3_PERIOD*(float)100.0/100.0);
    }
}