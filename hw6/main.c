#include "nu32dip.h" // constants, functions for startup and UART
#include "i2c_master_noint.h"     // SPI library
#include <math.h>

void HeartBeat();

int main(void){
  NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init

      while (1){
          HeartBeat();
  }
}

// Function blinks green LED to display that the PIC is not stuck in code
void HeartBeat(){
      NU32DIP_GREEN = 1; // OFF
      _CP0_SET_COUNT(0);
      while(_CP0_GET_COUNT()<24000000/10) {} // Wait 10ms between points
      NU32DIP_GREEN = 0; // ON
      _CP0_SET_COUNT(0);
      while(_CP0_GET_COUNT()<24000000/10) {} // Wait 10ms between points
}