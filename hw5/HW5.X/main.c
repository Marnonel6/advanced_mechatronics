#include "nu32dip.h" // constants, functions for startup and UART
#include "spi.h"     // SPI library
#include <math.h>

//void WaveGenerator(float* wave, int size, float amplitude);
unsigned short triangle_traj(double A,double B,double C,double D, int i,int c);

//
//char message[100];
//int freq = 24000000/1000; // Wait 1ms between points
//float amplitude = 3.3;    // Sine wave amplitude
//int data_points = 100;
//float wave[100];

int main(void) {
  
  NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
  initSPI();
 
  while (1) {
      
      
      for (int i=0; i<100; i++){
//            LATBbits.LATB13 = 0; // Chip select low
//            unsigned short p = trig_traj(128, (350*M_PI)/10000, 0, 128, i, 1);
//            spi_io(p >> 8);
//            spi_io(p);
//            LATBbits.LATB13 = 1; // Chip select high   

            CS = 0; // Chip select low
            unsigned short p = triangle_traj(1023, 1, 0, 0, i, 0);
            spi_io(p >> 8);
            spi_io(p & 0xFF);
            CS = 1; // Chip select high
            
            _CP0_SET_COUNT(0);
            while(_CP0_GET_COUNT()<24000000/100) {} // Wait 1ms between points

        }

    
//	if (!NU32DIP_USER){ // Thus when the button is pressed send the sine wave
//        // Every time you push the USER button, send a single cycle of a 
//        // sine wave in 100 data points, with a 0.01 second delay between 
//        // each data point.
//        WaveGenerator(wave, data_points, amplitude);
//	}
  }
}


/* 2Hz sine wave and a 1Hz triangle wave. Both should range from 0V to 3.3V.
 *  The update rate of the DAC should be at least 50 times faster than the
 *  frequency of the signal you are producing
 *  
 *  5ms delay
 * 0 - 1023 where sin() outputs -1 to 1 &0xFF
 */

unsigned short triangle_traj(double A,double B,double C,double D, int i,int c) {
    
    unsigned short dec;
    int steps = 100;
    int num_triangles = B;
    int i_per_triangle = steps / num_triangles;
    double slope = A / (i_per_triangle/2);
//    while(i > i_per_triangle){
//        i -= i_per_triangle;
//    }
    if (i < i_per_triangle/2){
        dec = slope * i;
    }
    else {
        dec = A - (slope*(i-i_per_triangle/2));
    }
    
//    dec = 511;
//    dec = dec & 
    
    unsigned short p = 0;
    p = c << 15; // c = Channel
    p = p | (0b111 << 12);
//    p = p | dec << 4;
    p = p | dec << 2;
    
    return p;
}

//// Generate a sine wave
//void WaveGenerator(float* wave, int size, float amplitude) {
//    float frequency = 1.0 / size;
//    for (int i = 0; i < size; i++) {
//        float angle = 2.0 * M_PI * frequency * i;
////        wave[i] = amplitude * sin(angle);
//        sprintf(message, "%f\r\n", amplitude * sin(angle));
//        NU32DIP_WriteUART1(message);
//        _CP0_SET_COUNT(0);
//        while(_CP0_GET_COUNT()<24000000/100) {} // Wait 1ms between points
//    }
//}