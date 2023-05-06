"""
Digital signal processing of heartbeat data taken with a photo resistor circuit. (sigD.csv)

Website for filter calculations and visualize: https://fiiir.com/
"""

import csv
import matplotlib.pyplot as plt
import numpy as np
from scipy import signal

t = []  # Time sampled
d = []  # Data measured -> e.g. Voltage

with open('sigD.csv') as f:
    # open csv file
    reader = csv.reader(f)
    for row in reader:
        # read the rows 1 one by one
        t.append(float(row[0])) # leftmost column
        d.append(float(row[1])) # second column

""" Plots the signal vs time """
# plt.plot(t, d, 'b-*')
# plt.xlabel('Time [s]')
# plt.ylabel('Signal')
# plt.title('Signal vs Time')
# plt.show()


""" Find dominant Signal frequency's and low frequency (mean) drift"""
# dt = t[1] - t[0]
# Fs = 1/dt # Calculate Sample rate
# Ts = 1.0/Fs # sampling interval
# ts = np.arange(0, t[-1], Ts) # time vector
# y = d # the data to make the fft from
# n = len(y) # length of the signal
# k = np.arange(n)
# T = n/Fs
# frq = k/T # two sides frequency range
# frq = frq[range(int(n/2))] # one side frequency range
# Y = np.fft.fft(y)/n # fft computing and normalization
# Y = Y[range(int(n/2))]

# fig, (ax1, ax2) = plt.subplots(2, 1)
# ax1.plot(t, y, 'b')
# ax1.set_xlabel('Time')
# ax1.set_ylabel('Amplitude')
# ax2.loglog(frq, abs(Y), 'b') # plotting the fft
# ax2.set_xlabel('Freq (Hz)')
# ax2.set_ylabel('|Y(freq)|')
# plt.show()

""" Filter signal """

# Design a low-pass (IR) filter
f = []                  # Output of filer
A = 0.99                # Trust old value    NOTE Higher value more filtered signal and thus output signal smoother
B = 0.01                # Trust new value    NOTE Lower value more high freq noise is filtered out, but more amplitude loss and phase shift
for dp in d:
    if len(f) == 0: # First time through just add a zero.
        f.append(0)
    else:
        f.append(A * f[-1] + B * dp)    # Previous value 'f[-1]', new value 'dp'

dt = t[1] - t[0]
Fs = 1/dt # Calculate Sample rate
Ts = 1.0/Fs # sampling interval
ts = np.arange(0, t[-1], Ts) # time vector
y1 = d # the data to make the fft from
y2 = f # filtered signal
n = len(y1) # length of the signal
k = np.arange(n)
T = n/Fs
frq = k/T # two sides frequency range
frq = frq[range(int(n/2))] # one side frequency range
Y1 = np.fft.fft(y1)/n # fft computing and normalization of ORIGINAL SIGNAL WITH NOISE NOTE
Y1 = Y1[range(int(n/2))]
Y2 = np.fft.fft(y2)/n # fft computing and normalization of FILTERED SIGNAL NOTE
Y2 = Y2[range(int(n/2))]

# Design and apply a high-pass filter
cutoff_freq = 0.01  # Cutoff frequency for drift removal
b, a = signal.butter(2, cutoff_freq, btype='high', analog=False)
y3 = signal.filtfilt(b, a, y2)
Y3 = np.fft.fft(y3)/n # fft computing and normalization of FILTERED SIGNAL NOTE
Y3 = Y3[range(int(n/2))]

fig, (ax1, ax2) = plt.subplots(2, 1)
ax1.plot(t, y1, 'b', t, y2, 'r',  t, y3, 'g')
ax1.legend(('Original Signal', 'Low pass Filtered Signal', 'High pass Filtered Signal'))
ax1.set_xlabel('Time')
ax1.set_ylabel('Amplitude')
ax2.loglog(frq, abs(Y1), 'b', frq, abs(Y2), 'r', frq, abs(Y3), 'g') # plotting the fft
ax2.legend(('Original Signal', 'Low pass Filtered Signal', 'High pass Filtered Signal'))
ax2.set_xlabel('Freq (Hz)')
ax2.set_ylabel('|Y(freq)|')
plt.show()