#include "nu32dip.h" // constants, functions for startup and UART
#include "i2c_master_noint.h"
#include "mpu6050.h"
#include "ssd1306.h"
#include <stdio.h>
#include "font.h"

void blink(int, int); // blink the LEDs function

int main(void) {
    NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
    init_mpu6050();
    // Initialize I2C
    i2c_master_setup(); // Start and setup I2C on PIC 
   // Setup OLED screen
    ssd1306_setup();
    ssd1306_clear();  //  Switch whole screen off
    ssd1306_update(); // Takes array and update screen

	// char array for the raw data
    unsigned char d[14]; //  Accelerations, Gyroscope, Temperature
	// floats to store the data
	float ax, ay, az, gx, gy, gz, t;

	// read whoami
    unsigned char who;
    who = whoami();
	// print whoami
    char who_m[100];
    char m1[100];
    char m2[100];
    char x = 128; // Used to make who_m move on screen
    double fps = 0.0;
    sprintf(who_m,"I am: 0x%X a MPU6050 IMU.", who);

	// if whoami is not 0x68, stuck in loop with LEDs on
    if(who != 0x68){ //  Syuck here, because the whoami value is wrong
        while(1){
        NU32DIP_GREEN = 0; // on
        NU32DIP_YELLOW = 0; // on
        }
    }

    while (1) {
		// use core timer for exactly 100Hz loop
        _CP0_SET_COUNT(0);
        blink(1, 5); // To see the LED's flash
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
        // Print values to buffer
        sprintf(m1,"Z-Acceleration = %4.3f", az);
        sprintf(m2, "Refresh rate = %5.2f Hz", fps);
        // Scroll across screen
        x-=6;
        // Create screen array and draw on OLED
        drawString(who_m, x, 0);
        drawString(m1, 0, 10);
        drawString(m2, 0, 20);
        ssd1306_update(); // Takes array and update screen
        fps = 24000000.0/_CP0_GET_COUNT(); // Update screen refresh rate
        ssd1306_clear();  //  Switch whole screen off
    }
}

// blink the LEDs
void blink(int iterations, int time_ms) {
    int i;
    unsigned int t;
    for (i = 0; i < iterations; i++) {
        NU32DIP_GREEN = 1; // OFF
        NU32DIP_YELLOW = 0; // ON
        t = _CP0_GET_COUNT(); // should really check for overflow here
        // the core timer ticks at half the SYSCLK, so 24000000 times per second
        // so each millisecond is 24000 ticks
        // wait half in each delay
        while (_CP0_GET_COUNT() < t + 12000 * time_ms) {
        }

        NU32DIP_GREEN = 0; // ON
        NU32DIP_YELLOW = 1; // OFF
        t = _CP0_GET_COUNT(); // should really check for overflow here
        while (_CP0_GET_COUNT() < t + 12000 * time_ms) {
        }
        

    }
}

