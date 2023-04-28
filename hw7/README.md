# `I2C` communication with a IMU [MPU6050](https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf) (InvenSense Inc.)
- Use the PIC32 to read and [configure a MPU6050 IMU](https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf).

- Build and add program to PIC32
- Run `screen /dev/ttyUSB0 230400` in the command line to see the data from the PIC32
- Press the reset button on the PIC32 to see the ID of the IMU6050
- Press enter in the CLI to start printing the data and see it in the CLI
- Close the CLI and run the python real-time plotter: `python3 plotter.py`

### X-Acceleration plotted in realtime vs time:

[MPU6050_I2C.webm](https://user-images.githubusercontent.com/60977336/235255904-1b518ec1-2c06-474d-b66e-bd08145b5106.webm)

### All MPU6050 data:

![Screenshot from 2023-04-28 16-05-57](https://user-images.githubusercontent.com/60977336/235256172-cee47990-4ad3-4cb3-a1ef-dff48fad19ec.png)

### I2C connections - MPU6050

![MPU6050_connections](https://user-images.githubusercontent.com/60977336/235257593-c81e41c0-3966-4102-8744-c4e04c9067f4.jpg)

### Instructions:

An IMU, or inertial measurement unit, is a sensor that is used to calculate position, velocity, or acceleration. Modern IMUs are MEMS devices, with built in analog to digital converters, and their own dedicated microcontroller to control and process the data (example from Digikey).

Common IMUs combine accelerometers, gyroscopes, magnetometers, and altimeters. Often they come packaged together in a single chip (although altimeters are still new and less common). The sensors typically communicate over I2C or SPI.

We will use the MPU6050 accelerometer and gyroscope chip made by Invensense, soldered onto a breakout board by Stmedu or similar. This IMU is an old model, but somehow still available during the chip shortage (and suspiciously inexpensive, maybe a knockoff or out of spec?)

This chip measures acceleration (accelerometer) and angular velocity (gyroscopic) data about the X, Y, and Z axes with 16 bit resolution, with a variety of sensitivities. It contains a temperature sensor so that it can self correct for temperature effects, and we can also access the temperature data as a 16 bit number.

The MPU6050 can communicate over I2C (and a related chip, the MPU6000, over SPI). We will use I2C to reduce the number of pins used from the PIC.

Stemedu has made it easy to hook up the chip. Connect 3.3V to VCC, GND to GND, SDA1 to SDA, and SCL1 to SCL. The 10k pull up resistors are built onto the breakout board, so you don't have to add them. The PCB sets the 7 bit address of the chip to 0b1101000 (0x68).

A library to communicate with the MPU6050: example code.

The IMU has a register called WHO_AM_I, which contains the constant value hex 68. After initializing I2C, read from WHO_AM_I and check that you get the right value back, this will check that your I2C bus is working and the chip is plugged in and ready to communicate. If you don't get the right value back, go into an infinite loop and turn on your LEDs so that you know you need a power reset.

Doulbe check how the chip is initialized in init_mpu6050(). This chip has many cool features, but we just want the bare bones ability to read the three acceleration, three angular velocity, and temperature data at any time. Out of the dozen registers, only three need to be changed.

To turn on the chip, write to the PWR_MGMT register.

To turn on the accelerometer, write to the ACCEL_CONFIG register. Set the sensitivity to plus minus 2g.

To turn on the gyroscope, write to the GYRO_CONFIG register. Set the sensitivity to plus minus 2000 dps.

The burst_read_I2C1() function reads out sequential data registers, containing all the acceleration, gyroscope, and temperature data.

The burst_read_I2C1() function works just like reading from the IO pin expander, except the i2c_master_recv() function is in a for() loop, length times. After every recv, a i2c_master_ack(0) tells the chip that we want to keep reading. The very last read should use a i2c_master_ack(1), to let the chip know you are finally done reading.

The burst_read_I2C1() returns a unsigned char array. The next step is to reconstruct the signed short values by shifting the high bytes and ORing them with the low bytes.

Many I2C chips allow you to read data out like this, as long as the registers you need are sequential.

With a 2g sensitivity and Z facing up, the Z acceleration value should be about -16383, and the X and Y acceleration values should be about 0. When not moving, the X Y and Z gyro values should be about 0. The temperature value should increase if you touch the chip to warm it up. All of the values should fluctuate a little, and have some slight bias. The conv_() functions use the sensitivities to convert the vales to G, DPS, and degree C.

