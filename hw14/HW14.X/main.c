#include "nu32dip.h"   // constants, functions for startup and UART
#include <stdio.h>
#include "utilities.h" // Helper functions for motor control

int main(void) {
    NU32DIP_Startup();             // cache on, interrupts on, LED/button init, UART init
    setup_motor_timers_pins();     // Setup timers and interrupts for motor control

    while (1) {
        MOTOR_1_DIRECTION = 0; // Forward -> Left wheel
        MOTOR_1_SPEED = (int)(PR3_PERIOD*(float)50.0/100.0);
        MOTOR_2_DIRECTION = 1; // Forward -> Right wheel
        MOTOR_2_SPEED = (int)(PR3_PERIOD*(float)50.0/100.0);
    }
}