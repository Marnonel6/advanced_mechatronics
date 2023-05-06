import matplotlib.pyplot as plt # for plotting
import numpy as np # for sine function

dt = 1.0/100.0 # Data was sampled at 100Hz
t = np.arange(0.0, 5.0, dt) # for 5s

# NOTE Always sample at 10 times the frequency of the signal you're sampling
# NOTE Add a physical low pass filter to act as an anti-aliasing filter for your data before you sample

# THe signal is a sine wave with a shift of 2.5 Volts
s = 2.0 * np.sin(2 * np.pi * 2.3 * t) + 2.5 # Frequency 2.3Hz

plt.plot(t,s,'b-*')
plt.xlabel('Time [s]')
plt.ylabel('Signal')
plt.title('Signal vs Time')
plt.show()

