# Advanced Meachatronic projects with the [`PIC32MX170F256B`](https://www.microchip.com/en-us/product/PIC32MX170F256B) chip:

- [(hw2) Basic circuit for PIC32MX170F256B `Circuit design`](https://github.com/Marnonel6/advanced_mechatronics/tree/main/hw2)
- [(hw3) `UART` communication & live Python data plotting](https://github.com/Marnonel6/advanced_mechatronics/tree/main/hw3)
- [(hw4) `PCB design` - KiCad](https://github.com/Marnonel6/advanced_mechatronics/tree/main/hw4)
- [(hw5) `SPI` communication with a DAC](https://github.com/Marnonel6/advanced_mechatronics/tree/main/hw5)
- [(hw6) `I2C` communication with a 8-Bit I2C I/O Expander ](https://github.com/Marnonel6/advanced_mechatronics/tree/main/hw6)
- [(hw7) `I2C` communication with a IMU - MPU6050](https://github.com/Marnonel6/advanced_mechatronics/tree/main/hw7)
- [(hw8) `I2C` communication with an OLED display and an IMU MPU6050](https://github.com/Marnonel6/advanced_mechatronics/tree/main/hw8)
- [(hw9) `Digital signal processing` - Low and High-pass filters](https://github.com/Marnonel6/advanced_mechatronics/tree/main/hw9)

The following software is necessary:
- [MPLAB X IDE (v6.05)](https://www.microchip.com/en-us/tools-resources/develop/mplab-x-ide)
- [MPLAB XC32 C-Compiler (v4.21)](https://www.microchip.com/en-us/tools-resources/develop/mplab-xc-compilers)
- [Onshape](https://www.onshape.com/en/)
- [KiCad](https://www.kicad.org/)
- [Python3](https://www.python.org/downloads/)

The PIC32 was preprogrammed with a bootloader (from ME333). We will use a program called nu32utility to send the compiled code to the PIC32 using a virtual serial port. To compile nu32utility you will need gcc. gcc is already on OSX and linux, and can be installed on Windows with mingw.
