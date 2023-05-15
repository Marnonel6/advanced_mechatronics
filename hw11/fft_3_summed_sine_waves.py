"""
Three summed sine waves. (Sumed_sive_waves.csv)

Identify the 3 frequencies.

"""

import csv
import matplotlib.pyplot as plt
import numpy as np
from scipy import signal

t = []  # Time sampled
d = []  # Data measured -> e.g. Voltage

with open('Sumed_sive_waves.csv') as f:
    # open csv file
    reader = csv.reader(f)
    for row in reader:
        # read the rows 1 one by one
        t.append(float(row[0])) # leftmost column
        d.append(float(row[1])) # second column

""" Find dominant Signal frequency's and low frequency (mean) drift"""
dt = t[1] - t[0]
Fs = 1/dt # Calculate Sample rate
Ts = 1.0/Fs # sampling interval
ts = np.arange(0, t[-1], Ts) # time vector
y = d # the data to make the fft from
n = len(y) # length of the signal
k = np.arange(n)
T = n/Fs
frq = k/T # two sides frequency range
frq = frq[range(int(n/2))] # one side frequency range
Y = np.fft.fft(y)/n # fft computing and normalization
Y = Y[range(int(n/2))]

fig, (ax1, ax2) = plt.subplots(2, 1)
ax1.plot(t, y, 'b')
ax1.set_xlabel('Time')
ax1.set_ylabel('Amplitude')
ax2.loglog(frq, abs(Y), 'b') # plotting the fft
ax2.set_xlabel('Freq (Hz)')
ax2.set_ylabel('|Y(freq)|')
plt.show()