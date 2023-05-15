# screen /dev/ttyACM0

from ulab import numpy as np # to get access to ulab numpy functions
import time

# Declare an array with some made up data like
a = np.array([1,3,5,2,4,8,10])
# Test some stats functions, like
print(np.min(a))
print(np.max(a))
print(np.median(a))

# Define the parameters for the sine waves
amplitude1 = 1.0
frequency1 = 10.0

amplitude2 = 0.5
frequency2 = 5.0

amplitude3 = 0.8
frequency3 = 7.0

num_points = 1024

# Generate time values
t = np.linspace(0, 2*np.pi, num_points)

# Generate the three sine waves
sine_wave1 = amplitude1 * np.sin(frequency1 * t)
sine_wave2 = amplitude2 * np.sin(frequency2 * t)
sine_wave3 = amplitude3 * np.sin(frequency3 * t)

# Sum the three sine waves
sum_wave = sine_wave1 + sine_wave2 + sine_wave3

for i in sum_wave:
    print(f"("+str(i)+",)")  # For plotting in Mu
    time.sleep(.01) # delay in seconds

#for i in sum_wave:
#    print(f"{i}")
#    time.sleep(.01) # delay in seconds

while 1:
    time.sleep(.05) # delay in seconds
