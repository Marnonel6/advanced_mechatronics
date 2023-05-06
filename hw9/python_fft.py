"""
Digital signal processing.

Website for filter calculations and visualize: https://fiiir.com/

"""

import matplotlib.pyplot as plt
import numpy as np

dt = 1.0/10000.0 # Data was sampled at 10kHz
t = np.arange(0.0, 1.0, dt) # Sampled for 10s
# Signal is a constant plus 100Hz and 1000Hz sine wave signals
s = 4.0 * np.sin(2 * np.pi * 100 * t) + 0.25 * np.sin(2 * np.pi * 1000 * t) + 25

Fs = 10000 # sample rate
Ts = 1.0/Fs; # sampling interval
ts = np.arange(0,t[-1],Ts) # time vector
y = s # the data to make the fft from
n = len(y) # length of the signal
k = np.arange(n)
T = n/Fs
frq = k/T # two sides frequency range
frq = frq[range(int(n/2))] # one side frequency range
Y = np.fft.fft(y)/n # fft computing and normalization
Y = Y[range(int(n/2))]

fig, (ax1, ax2) = plt.subplots(2, 1)
ax1.plot(t,y,'b')
ax1.set_xlabel('Time')
ax1.set_ylabel('Amplitude')
ax2.loglog(frq,abs(Y),'b') # plotting the fft
ax2.set_xlabel('Freq (Hz)')
ax2.set_ylabel('|Y(freq)|')
plt.show()

# import numpy as np
# from scipy import signal
# import matplotlib.pyplot as plt

# # Generate a noisy signal
# fs = 1000  # Sampling frequency
# t = np.arange(0, 1, 1 / fs)  # Time vector
# x = np.sin(2 * np.pi * 5 * t) + np.sin(2 * np.pi * 10 * t)  # Original signal
# noise = np.random.normal(0, 0.5, len(t))  # Gaussian noise
# x_noisy = x + noise  # Noisy signal

# # Design a low-pass filter
# order = 4  # Filter order
# cutoff_freq = 50  # Cutoff frequency
# b, a = signal.butter(order, cutoff_freq / (fs / 2), btype='low')

# # Apply the filter to the noisy signal
# x_filtered = signal.lfilter(b, a, x_noisy)

# # Plot the original signal, noisy signal, and filtered signal
# plt.figure()
# plt.plot(t, x, label='Original Signal')
# plt.plot(t, x_noisy, label='Noisy Signal')
# plt.plot(t, x_filtered, label='Filtered Signal')
# plt.xlabel('Time')
# plt.ylabel('Amplitude')
# plt.legend()
# plt.show()

