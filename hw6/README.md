# `I2C` communication with a 8-Bit I2C I/O Expander (MCP23008 - Microchip)
- Use the PIC32 to read the button state from the I2C expander and then switch on the LED through the I2C expander if the button is pressed.

### Example:

https://user-images.githubusercontent.com/60977336/235218669-94e9466a-300b-4bab-80cf-00bab78425b8.mp4

### I2C connections - TOP right chip os the 8-Bit I2C I/O Expander (MCP23008 - Microchip)

![I2C](https://user-images.githubusercontent.com/60977336/235217703-0b44316a-61d4-4432-9f07-23cd853385c5.jpeg)

### Instructions:

The MCP23008 is a DIP chip that gives you 8 more general purpose IO pins. It is controlled by I2C, which uses two pins, so the overall circuit will net gain 6 controllable pins. This is useful for a chip like the PIC32MX170F256B which doesn't have many pins to begin with.

The MCP23008 has 3 address pins, so you could have 8 of the chips on the same I2C bus. That would provide 64 pins, using only 2 from the PIC! Now if you wanted to control 64 LEDs or read 64 buttons there are lots of other, probably more efficient, circuits, but the MCP23008 is relatively straight forward.

Connect the MCP23008 Vdd to 3.3V and Vss to GND. Connect the RESET pin to 3.3V. Connect the SDA to a 10k pull-up resistor, and the SCL to a 10k pull-up resistor, and connect the pins to the PIC SDA1 and SCL1. Connect the MCP23008 A0, A1, and A2 pins to GND (or whatever address you'd like to make).

Note that by connecting the RESET pin to 3.3V, the only way to reset the chip is to kill power to the whole board. Resetting is sometimes necessary because if your code happens to be in the middle of communicating with the MCP23008 when the PIC is reset, the PIC code will start fresh but the MCP23008 might have been in the middle of communication and will not know that the PIC started over, so it will never respond to new communication and the PIC will get stuck in an infinite loop. Always put some kind of heart-beat in your code so you can tell that the PIC is stuck in an infinite loop and you know to do a full power reset. We could manually control the RESET pin with an output pin from the PIC, but that would steal another PIC pin, defeating the purpose of gaining pins with the MCP23008.

To test the chip, add an LED and a 330 ohm resistor to GND from pin GP7, and add a button from pin GP0 to GND with a 10k pull-up resistor.

Add the I2C sample code to your project. The goal is to read from GP0, and if the button is pushed, turn on GP7, else turn off GP7. Blink the green LED that is connected directly to the PIC at some frequency as a heart-beat, so that you know the PIC is running. Upload your finished code into a folder called HW6 in your repo and submit a link and a demo video on Canvas.

I2C is very tricky to debug because it is hard to tell if the chip initialization, the read function, or the write function is the problem. Break the assignment into 2 parts: chip initialization and blink the LED on GP7, then read from GP0 and control GP7.

The chip must be initialized so that GP7 is an output pin and GP0 is input. You can make all the other pins inputs. Write an unsigned char to IODIR like you would a TRIS register in the PIC.

Set the bits in OLAT to turn the pins on or off, and read from GPIO to know if the button is pushed or not. Note that there are no structures like LATAbits. or PORTBbits. built into the code, so you'll probably need some bitshifting and ANDing and ORing to identify individual bits from the unsigned chars.

To write a message to the MCP23008, do a Start, send the address with the write bit (last bit is 0), send the register you want to change, send the value you want to change it to, and do a Stop.

To read a message from the MCP23008, do a Start, send the address with the write bit (last bit is 0), send the register you want to read from, do a Restart, send the address with the read bit (last bit is 1), recv from the chip, ack with a 1, and do a Stop. (If you ack with a 0, the chip expects you to immediately do another read. We'll use that feature with another chip).
