#include "nu32dip.h"              // constants, functions for startup and UART
#include "i2c_master_noint.h"     // I2C library
#include <math.h>

#define ADD_W 0b01000000 // 0b010 - Fixed `OP` bits. 000 - Grounded A0-1. 0 - Last bit 0 for write
#define ADD_R 0b01000001 // 0b010 - Fixed `OP` bits. 000 - Grounded A0-1. 0 - Last bit 1 for read
#define GPIO 0x09        // 0b00001001 - Make GP7 a input for the button and GP0 is output for LED
#define OLAT 0x0A        // Register for setting pin state
#define IODIR 0x00       // Initialize chip register

// Function prototypes
void HeartBeat();
void i2c_write_pins(unsigned char Address, unsigned char Register, unsigned char Value); // Generic I2C Write
unsigned char i2c_read_pins(unsigned char Address_write, unsigned char Address_read, unsigned char Register); // Generic I2C Read

int main(void){
    // cache on, interrupts on, LED/button init, UART init
    NU32DIP_Startup();
    // Initialize I2C
    i2c_master_setup(); // Start and setup I2C on PIC

    // Initialize the GPIO pins GP0-7 on I2C device
    i2c_write_pins(ADD_W, IODIR, 0b01111111); // Make GP7 a output and all other pins input in the IODIR register

    while (1){
        // Heartbeat LED to see if PIC is stuck in an I2C infinite loop
        HeartBeat();

        // Read GPIO register states to check if button/GP0 was pushed
        unsigned char pin_states = i2c_read_pins(ADD_W, ADD_R, GPIO);
        
        //  If button was pressed -> GP0 = 0, because the button connects GP0 to GND
        if ((pin_states & 0b00000001) == 0){ // And to only receive the value in GP0
            // Turn On LED
            i2c_write_pins(ADD_W, OLAT, 0x80); // 0b10000000: Turn ON GP7/LED
        }
        else {
            // Turn Off LED
            i2c_write_pins(ADD_W, OLAT, 0x00); // 0b00000000: Turn off GP7/LED
        }
  }
}

// Function blinks yellow LED to display that the PIC is not stuck in code
void HeartBeat(){
      NU32DIP_YELLOW = 1; // OFF
      _CP0_SET_COUNT(0);
      while(_CP0_GET_COUNT()<24000000/10) {} // Wait 10ms between points
      NU32DIP_YELLOW = 0; // ON
      _CP0_SET_COUNT(0);
      while(_CP0_GET_COUNT()<24000000/10) {} // Wait 10ms between points
}

/* Generic I2C Write pin states function
 * 
 * Arguments:
 *  - Address: (unsigned char) Address of I2C device
 *  - Register: (unsigned char) Register on I2C device to write to
 *  - Value: (unsigned char) Value to write to register
 * 
 * Return:
 *  - void
*/
void i2c_write_pins(unsigned char Address, unsigned char Register, unsigned char Value){
                                     // LED turn on example: Write pin state to Turn on LED
    i2c_master_start();              // Start
    i2c_master_send(Address);        // Write: 8-bit Address of chip -> `OP`: Opcode Fixed value within the chip, `ADDR`: Address by wiring A0-2, `W`: Write bit
    i2c_master_send(Register);       // Register for setting pin state
    i2c_master_send(Value);          // 0b10000000: Turn on GP7 or LED
    i2c_master_stop();               // 'P': Stop
}

/* Generic I2C Read pin states function
 * 
 * Arguments:
 *  - Address_write: (unsigned char) Address of I2C device with WRITE bit
 *  - Address_read: (unsigned char) Address of I2C device with READ bit
 *  - Register: (unsigned char) Register on I2C device to write to
 * 
 * Return:
 *  - pin_states : (unsigned char) 8bit value read from I2C register
*/
unsigned char i2c_read_pins(unsigned char Address_write, unsigned char Address_read, unsigned char Register){
                                                     // Retrieve pin states of the GPIO register
        i2c_master_start();                          // Start 
        i2c_master_send(Address_write);              // Write: 8-bit Address of chip -> `OP`: Opcode Fixed value within the chip, `ADDR`: Address by wiring A0-2, `W`: Write bit
        i2c_master_send(Register);                   // GPIO - 0x0A - Register you want to read from on the I2C device
        i2c_master_restart();                        // SR: Restart
        i2c_master_send(Address_read);               // Read: 8-bit Address of chip -> `OP`: Opcode Fixed value within the chip, `ADDR`: Address by wiring A0-2, `R`: Read bit
        unsigned char pin_states = i2c_master_recv();// Read data from I2C device - It will receive a unsigned char of 8 bits indicating the states of the GPIO pins
        i2c_master_ack(1);                           // Handshake 1 bit ACK to say the PIC is done reading
        i2c_master_stop();                           // 'P': Stop
    
    return pin_states;
}