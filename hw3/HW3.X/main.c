#include "nu32dip.h" // constants, functions for startup and UART
#include <math.h>

void WaveGenerator(float* wave, int size, float amplitude);

char message[100];
int freq = 24000000/1000; // Wait 1ms between points
float amplitude = 3.3;    // Sine wave amplitude
int data_points = 100;
float wave[100];

int main(void) {
  
  NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
  while (1) {
    // Linear lines
    //    for (int i = 0; i < 100; i++) {
    //        sprintf(message,"%f\r\n",i*3.3/100.0);
    //        NU32DIP_WriteUART1(message);
    //        _CP0_SET_COUNT(0);
    //        while(_CP0_GET_COUNT()<24000000/1000) {} // Wait 1ms between points
    //    }
    
	if (!NU32DIP_USER){ // Thus when the button is pressed send the sine wave
        // Every time you push the USER button, send a single cycle of a 
        // sine wave in 100 data points, with a 0.01 second delay between 
        // each data point.
        WaveGenerator(wave, data_points, amplitude);
	}
  }
}

// Generate a sine wave
void WaveGenerator(float* wave, int size, float amplitude) {
    float frequency = 1.0 / size;
    for (int i = 0; i < size; i++) {
        float angle = 2.0 * M_PI * frequency * i;
//        wave[i] = amplitude * sin(angle);
        sprintf(message, "%f\r\n", amplitude * sin(angle));
        NU32DIP_WriteUART1(message);
        _CP0_SET_COUNT(0);
        while(_CP0_GET_COUNT()<24000000/100) {} // Wait 1ms between points
    }
}