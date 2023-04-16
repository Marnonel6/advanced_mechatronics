#include "nu32dip.h" // constants, functions for startup and UART
#include "spi.h"     // SPI library
#include <math.h>

void WaveGenerator(int data_points, float bit_max_val, float amplitude, int i);
void TriangleGenerator(int data_points, float bit_max_val, int i);

float amplitude = 3.3;    // Sine wave amplitude
float Ten_bit_max = 1023; // Max value for 10 bits
int data_points = 100;

int main(void) {
  
  NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
  initSPI();
 
  while (1) {
      
      for (int i=0; i<100; i++){
          
            TriangleGenerator(data_points, Ten_bit_max, i);
            
            WaveGenerator(data_points, Ten_bit_max, amplitude, i);

            _CP0_SET_COUNT(0);
            while(_CP0_GET_COUNT()<24000000/100) {} // Wait 1ms between points
        }
  }
}

/* 2Hz sine wave and a 1Hz triangle wave. Both should range from 0V to 3.3V.
 *  The update rate of the DAC should be at least 50 times faster than the
 *  frequency of the signal you are producing
 */

// Generate a 1Hz triangle wave
void TriangleGenerator(int data_points, float bit_max_val, int i) {
    unsigned short voltage; // Desired voltage is a 16bit short, but we just use 10 bits thus 0-1023
    float slope = bit_max_val/(data_points/2);

    // Triangle wave voltage
    if (i < data_points/2){
        voltage = slope * i;
    }
    else {
        voltage = bit_max_val - (slope*(i-data_points/2));
    }
    
    // Generate DAC desired 16bit value
    unsigned short dac_val = 0;
    dac_val = 0 << 15;                 // Channel - Green wire
    dac_val = dac_val | (0b111 << 12); // Sets flags
    dac_val = dac_val | voltage << 2;  // Shift 10 bit voltage 0-1023 into 16bit short 
    
    // Send over SPI
    CS = 0; // Chip select low
    spi_io(dac_val >> 8);
    spi_io(dac_val & 0xFF);
    CS = 1; // Chip select high
}

// Generate a 2Hz sine wave
void WaveGenerator(int data_points, float bit_max_val, float amplitude, int i) {
    float frequency = 1.0 / data_points * 2;
    float angle = 2.0 * M_PI * frequency * i;
    
    // Sine wave voltage
    unsigned short voltage = bit_max_val/2 * sin(angle) + bit_max_val/2;
   
    // Generate DAC desired 16bit value
    unsigned short dac_val = 0;
    dac_val = 1 << 15;                 // Channel - Blue wire
    dac_val = dac_val | (0b111 << 12); // Sets flags
    dac_val = dac_val | voltage << 2;  // Shift 10 bit voltage 0-1023 into 16bit short 

    // Send over SPI
    CS = 0; // Chip select low
    spi_io(dac_val >> 8);
    spi_io(dac_val & 0xFF);
    CS = 1; // Chip select high
}