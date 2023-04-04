#include "nu32dip.h" // constants, functions for startup and UART

void blink(int, int); // blink the LEDs function
void Debug(int debug1, int debug2); // debug function 

int main(void) {
  char message[100];
  int duration = 10;
  int nr_blink = 100;
  
  NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
  while (1) {
    NU32DIP_ReadUART1(message, 100); // wait here until get message from computer
    sscanf(message, "%d %d", &nr_blink, &duration);
    Debug(nr_blink, duration);
	if (NU32DIP_USER){
		blink(nr_blink, duration); // blink nr_blink times, duration [ms] time per blink
	}
  }
}

// blink the LEDs
void blink(int iterations, int time_ms){
	int i;
	unsigned int t;
	for (i=0; i< iterations; i++){
		NU32DIP_GREEN = 0; // on
		NU32DIP_YELLOW = 1; // off
		t = _CP0_GET_COUNT(); // should really check for overflow here
		// the core timer ticks at half the SYSCLK, so 24000000 times per second
		// so each millisecond is 24000 ticks
		// wait half in each delay
		while(_CP0_GET_COUNT() < t + 12000*time_ms){}
		
		NU32DIP_GREEN = 1; // off
		NU32DIP_YELLOW = 0; // on
		t = _CP0_GET_COUNT(); // should really check for overflow here
		while(_CP0_GET_COUNT() < t + 12000*time_ms){}
	}
}

// Debug function
void Debug(int debug1, int debug2){
    char val[100];
    sprintf(val,"Debug nr_blink: %d   duration: %d\r\n", debug1, debug2);
    NU32DIP_WriteUART1(val);
}