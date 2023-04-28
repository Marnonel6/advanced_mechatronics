#include "nu32dip.h" // constants, functions for startup and UART
#include "i2c_master_noint.h"     // SPI library
#include <math.h>

#define ADD_W 0b01000000 // 0b010 - Fixed `OP` bits. 000 - Grounded A0-1. 0 - Last bit 0 for write
#define ADD_R 0b01000001 // 0b010 - Fixed `OP` bits. 000 - Grounded A0-1. 0 - Last bit 1 for read
#define GPIO 0x09        // 0b00001001 - Make GP7 a input for the button and GP0 is output for LED
#define OLAT 0x0A        // Register for setting pin state
#define IODIR 0x00       // Initialize chip register

void HeartBeat();
void i2c_write(unsigned char Address, unsigned char Register, unsigned char Value); // Generic I2C Write

int main(void){
    NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
  
    // init I2C
    i2c_master_setup();                     // Start and setup I2C
    i2c_master_start();                     // Start 
    i2c_master_send(ADD_W);                 // 8-bit Address we want to talk to -> `OP`: Opcode Fixed value within the chip, `ADDR`: Address by wiring A0-2, `W`: Read or Write bit
    i2c_master_send(IODIR);                 //  
    i2c_master_send(0b01111111);            //  
    i2c_master_stop();                      // 'P': Stop

    while (1){
        HeartBeat();

        i2c_master_start();                 // Start 
        i2c_master_send(ADD_W);             // Write: 8-bit Address of chip -> `OP`: Opcode Fixed value within the chip, `ADDR`: Address by wiring A0-2, `W`: Write bit
        i2c_master_send(GPIO);              // Send data
        i2c_master_restart();               // SR: Restart
        i2c_master_send(ADD_R);             // Read: 8-bit Address of chip -> `OP`: Opcode Fixed value within the chip, `ADDR`: Address by wiring A0-2, `R`: Read bit
        unsigned char r = i2c_master_recv();// Read data
        i2c_master_ack(1);                  // Handshake 1 bit ACK to say the PIC is done reading
        i2c_master_stop();                  // 'P': Stop
        
        //  If button was pressed -> GP0 = 0, because the button connects GP0 to GND
        if ((r & 0b00000001) == 0){
            // Turn on LED
            i2c_write(ADD_W, OLAT, 0x80);
//            // Delay
//            _CP0_SET_COUNT(0);
//            while(_CP0_GET_COUNT()<48000000){ ; }
        }
        else {
            // Turn Off LED
            i2c_write(ADD_W, OLAT, 0x00); // 0b00000000: Turn off GP7 or LED
        }
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

/* Generic I2C Write
 * 
 * Arguments:
 *  - Address: (unsigned char) Address of I2C device
 *  - Register: (unsigned char) Register on I2C device to write to
 *  - Value: (unsigned char) Value to write to register
 * 
 * Return:
 *  - void
*/
void i2c_write(unsigned char Address, unsigned char Register, unsigned char Value){
                                     // LED turn on example: Write pin state to Turn on LED
    i2c_master_start();              // Start
    i2c_master_send(Address);        // Write: 8-bit Address of chip -> `OP`: Opcode Fixed value within the chip, `ADDR`: Address by wiring A0-2, `W`: Write bit
    i2c_master_send(Register);       // Register for setting pin state
    i2c_master_send(Value);          // 0b10000000: Turn on GP7 or LED
    i2c_master_stop();               // 'P': Stop
}