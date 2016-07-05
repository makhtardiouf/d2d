

import os, sys
import numpy as np
from matplotlib import pyplot as plt

file = "DlPdcpStats.txt"
    #repr(os.getenv('NS3_DIR')) + "/" + "DlPdcpStats.txt"

if not os.path.exists(file):
    print("Could not find the LTE data stats, verify the path of: ", file)
    sys.exit()

data = np.loadtxt(file, dtype=float, comments='%')

start, end, CellId, IMSI, RNTI, LCID, nTxPDUs, TxBytes, nRxPDUs, RxBytes, delay, stdDev, min, max, PduSize, stdDev, min, max = data.T

#plt.axes([0.2, 0.1, 0.5, 0.8])
plt.xlabel("Time (s)")
plt.ylabel("Throughput (Mbit/s)")

#ueRates = [IMSI, TxBytes / (1e6 * 0.25)]
plt.plot(start, TxBytes / (1e6 * 0.25))
plt.plot(start, RxBytes / (1e6 * 0.25))

plt.legend(('TxBytes', 'RxBytes'), loc=4)
plt.show()
#plt.savefig("D2dThroughput.pdf")

fig = plt.figure()
plt.xlabel("Time (s)")
plt.ylabel("Delay (ms)")
plt.plot(start, delay * 1e3)
plt.show()
