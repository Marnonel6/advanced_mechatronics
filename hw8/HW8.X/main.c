#include "nu32dip.h" // constants, functions for startup and UART
#include "i2c_master_noint.h"
#include "mpu6050.h"
#include "ssd1306.h"
#include <stdio.h>
#include "font.h"

// Function prototypes
void HeartBeat();

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

	// read Who Am I
    unsigned char who;
    who = whoami();
	// print Who Am I
    char who_m[100];
    char m1[100];
    char m2[100];
    char x = 128; // Used to make who_m move on screen
    double fps = 0.0;
    sprintf(who_m,"I am: 0x%X a MPU6050 IMU.", who);

	// if Who Am I is not 0x68, stuck in loop with LEDs on
    if(who != 0x68){ //  Stuck here, because the Who Am I value is wrong
        while(1){
        NU32DIP_GREEN = 0; // on
        NU32DIP_YELLOW = 0; // on
        }
    }

    while (1) {
		// use core timer to calculate refresh rate
        _CP0_SET_COUNT(0);
        // Heartbeat LED to see if PIC is stuck in an I2C infinite loop
        HeartBeat();
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

/* 
 * Function blinks yellow LED to display that the PIC is not stuck in code
*/
void HeartBeat(){
      NU32DIP_YELLOW = 1; // OFF
      _CP0_SET_COUNT(0);
      while(_CP0_GET_COUNT()<24000000/100) {} // Wait 1ms
      NU32DIP_YELLOW = 0; // ON
      _CP0_SET_COUNT(0);
      while(_CP0_GET_COUNT()<24000000/100) {} // Wait 1ms
}

