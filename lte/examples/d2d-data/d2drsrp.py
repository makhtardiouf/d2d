#!/usr/bin/env python3
"""
Plot SINR, RSRP of UEs
Loads the data generated from the LTE simulation, in a brute force fashion
Note: Run it from directory where the data file is located
(C) 2016 Makhtar Diouf
$Id$
"""
import os
import sys
import numpy as np
from matplotlib import pyplot as plt
#import math
#import statistics

file = "DlRsrpSinrStats.txt"
if len(sys.argv) > 1:
    file = sys.argv[1]
stats = file.split('.')[0]

if not os.path.exists(file):
    print("Could not find the LTE data stats, verify the path of: ", file)
    sys.exit()

print("Loading data from", file, " please wait...")
data = np.loadtxt(file, dtype=float, comments='%')

isUplink = False
if "Ul" in file:
    isUplink = True
    Time, CellId, IMSI, RNTI, Sinr, SINR_dB = data.T
else:
    Time, CellId, IMSI, RNTI, RSRP, RSRP_dBm, Sinr, SINR_dB = data.T

UES = [20, 2] #, "Cell"] # range(1, 11)
UESTR = ['LTE-I', 'LTE-D']

#if not isUplink:
#    sinrPlt = plt.subplot(211)
#    sinrPlt.set_title(" SINR vs Time (" + stats + ")")

#else:
#    plt.title("DL SINR vs Time (" + stats + ")")

plt.xlabel("Time (s)")
plt.ylabel("SINR (dB)")
plt.title(''.join(file[:2]).upper() + " SINR vs Time (" + stats + ")")

def plotUe(imsi=1):
    sinr = [0] * len(SINR_dB)
    times = [0] * len(sinr)
    i, j = 0, 0

    for ue in IMSI:
        if IMSI[i] == imsi:
            sinr[j] = SINR_dB[i]
            times[j] = Time[i]
            j += 1
        i += 1
    #print(sinr[:j-1], times[:j-1])
    sinr = sinr[:j - 1]
    times = times[:j - 1]
    plt.plot(times, sinr)


for i in UES:
    plotUe(i)

# Plot for the whole Cell
#plt.plot(Time, SINR_dB)
plt.legend(UESTR)
fname = stats + ".png"

#if not isUplink:
#    plt.subplot(212)
#    plt.psd(RSRP, Fc=9600)  # Fc = Center frequency

plt.savefig(fname)
plt.show()
print("See image", fname)
