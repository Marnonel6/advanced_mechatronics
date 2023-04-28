#include "nu32dip.h" // constants, functions for startup and UART
#include "i2c_master_noint.h"
#include "mpu6050.h"
#include <stdio.h>

void blink(int, int); // blink the LEDs function

int main(void) {
    NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
    init_mpu6050();
	
	// char array for the raw data
    unsigned char d[14]; //  Accelerations, Gyroscope, Temperature
	// floats to store the data
	float ax, ay, az, gx, gy, gz, t;

	// read whoami
    unsigned char who;
    who = whoami();
	// print whoami
    char m[100];
    sprintf(m,"I am: 0x%X \r\n", who);
    NU32DIP_WriteUART1(m);
	// if whoami is not 0x68, stuck in loop with LEDs on
    if(who != 0x68){ //  Syuck here, because the whoami value is wrong
        while(1){
        NU32DIP_GREEN = 0; // on
        NU32DIP_YELLOW = 0; // on
        }
    }

	// wait to print until you get a newline
    NU32DIP_ReadUART1(m,100);

    while (1) {
		// use core timer for exactly 100Hz loop
        _CP0_SET_COUNT(0);
//        blink(1, 30); // To see the LED's flash
        blink(1,5);

        // read IMU
        burst_read_mpu6050(d);
		// convert data
        ax = conv_xXL(d); // convert x-acceleration to float (g's)
        ay = conv_yXL(d); // convert y-acceleration to float (g's)
        az = conv_zXL(d); // convert z-acceleration to float (g's)
        gx = conv_xG(d);  // convert x-gyro rate to dps
        gy = conv_yG(d);  // convert y-gyro rate to dps
        gz = conv_zG(d);  // convert z-gyro rate to dps
        t = conv_temp(d); // convert int16_t temperature signed short to float (Celsius)
        /*
         * Print out acceleration, gyroscope and temperature for terminal visual 
         */
//         sprintf(m,"ax = %4.3f ay = %4.3f az = %4.3f gx = %4.3f gy = %4.3f gz = %4.3f Temp = %4.2f \r\n", ax, ay, ay, gx, gy, gz, t);
        /*
         * Print out only one value and use plotter.y to visualize while running this
         */
        sprintf(m,"%4.3f \r\n", ax);
        NU32DIP_WriteUART1(m);

        while (_CP0_GET_COUNT() < 48000000 / 2 / 100) { ; }
    }
}

// blink the LEDs
void blink(int iterations, int time_ms) {
    int i;
    unsigned int t;
    for (i = 0; i < iterations; i++) {
        NU32DIP_GREEN = 0; // on
        NU32DIP_YELLOW = 1; // off
        t = _CP0_GET_COUNT(); // should really check for overflow here
        // the core timer ticks at half the SYSCLK, so 24000000 times per second
        // so each millisecond is 24000 ticks
        // wait half in each delay
        while (_CP0_GET_COUNT() < t + 12000 * time_ms) {
        }

        NU32DIP_GREEN = 1; // off
        NU32DIP_YELLOW = 0; // on
        t = _CP0_GET_COUNT(); // should really check for overflow here
        while (_CP0_GET_COUNT() < t + 12000 * time_ms) {
        }
    }
}

