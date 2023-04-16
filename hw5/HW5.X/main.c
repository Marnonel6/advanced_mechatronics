#include "nu32dip.h" // constants, functions for startup and UART
#include "spi.h"     // SPI library
#include <math.h>

void WaveGenerator(int data_points, float amplitude, int i);
void triangle_traj(double A,double B,double C,double D, int i,int c);

float amplitude = 3.3;    // Sine wave amplitude
int data_points = 100;

int main(void) {
  
  NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
  initSPI();
 
  while (1) {
      
      for (int i=0; i<100; i++){
          
            triangle_traj(1023, 1, 0, 0, i, 0);
            
            WaveGenerator(data_points, amplitude, i);

            _CP0_SET_COUNT(0);
            while(_CP0_GET_COUNT()<24000000/100) {} // Wait 1ms between points
        }
  }
}


/* 2Hz sine wave and a 1Hz triangle wave. Both should range from 0V to 3.3V.
 *  The update rate of the DAC should be at least 50 times faster than the
 *  frequency of the signal you are producing
 */

void triangle_traj(double A,double B,double C,double D, int i,int c) {
    
    unsigned short voltage; // Desired voltage is a 16bit short, but we just use 10 bits thus 0-1023
    int steps = 100;
    int num_triangles = B;
    int i_per_triangle = steps / num_triangles;
    double slope = A / (i_per_triangle/2);

    if (i < i_per_triangle/2){
        voltage = slope * i;
    }
    else {
        voltage = A - (slope*(i-i_per_triangle/2));
    }
    
    unsigned short dac_val = 0;
    dac_val = c << 15; // c = Channel - Green wire
    dac_val = dac_val | (0b111 << 12);
    dac_val = dac_val | voltage << 2; // Shift 10 bit voltage 0-1023 into 16bit short 
    
    CS = 0; // Chip select low
    spi_io(dac_val >> 8);
    spi_io(dac_val & 0xFF);
    CS = 1; // Chip select high
}

// Generate a sine wave
void WaveGenerator(int data_points, float amplitude, int i) {
    float frequency = 1.0 / data_points * 2;
    float angle = 2.0 * M_PI * frequency * i;
        
    unsigned short voltage = 1023/2 * sin(angle) + 1023/2;
   
    unsigned short dac_val = 0;
    dac_val = 1 << 15; // c = Channel - Green wire
    dac_val = dac_val | (0b111 << 12); // Sets flags
    dac_val = dac_val | voltage << 2; // Shift 10 bit voltage 0-1023 into 16bit short 

    CS = 0; // Chip select low
    spi_io(dac_val >> 8);
    spi_io(dac_val & 0xFF);
    CS = 1; // Chip select high
}