#include "nu32dip.h"   // constants, functions for startup and UART
#include <stdio.h>
#include "utilities.h" // Helper functions for motor control

int main(void) {
    NU32DIP_Startup();             // cache on, interrupts on, LED/button init, UART init
    setup_motor_timers_pins();     // Setup timers and interrupts for motor control

    while (1) {

        // DC motor PWM speed
        // MOTOR_1_SPEED = (int)(PR3_PERIOD*(float)50.0/100.0);
        
        // Servo sweep between 45deg and 135deg
        for (int i = 0; i<600000; i++){
            MOTOR_1_SPEED = (int)((i/600000.0)*(186.0)) + 236;
        }
    }
}