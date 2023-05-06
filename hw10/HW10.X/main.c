#include "nu32dip.h" // constants, functions for startup and UART
#include "ws2812b.h"
#include <stdio.h>

int main(void) {
    NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
    ws2812b_setup();   // Init the RGB LED strip display
    
    // Array equal to the nr of LED's
    int numLEDs = 8; 
    wsColor LED_colors[numLEDs];
    
    // Assign colors to LEDs
    float green_Hue = 0.0;
    float pink_Hue = 180.0;
    float red_Hue = 120.0;
    float blue_Hue = 270.0;
    float Rainbow_Hue = 0.0;

    while (1) {
		// use core timer to calculate refresh rate
        _CP0_SET_COUNT(0);
        
        // Add colors to array
        for (int led = 0; led < numLEDs; led++){
            LED_colors[led] = HSBtoRGB(Rainbow_Hue, 1.0, 0.1);
        }
        
        // Send pulses to set colors
        ws2812b_setColor(LED_colors, numLEDs);
        
        // Increment Hue
        Rainbow_Hue += 0.05;
        // If Hue >= 360 degree set to 0. Angle wraping.
        if (Rainbow_Hue>=360.0){
            Rainbow_Hue = 0.0;
        }
                
    }
}