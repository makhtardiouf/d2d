#!/usr/bin/env python3
import os
import sys
import random
import math
#from matplotlib import pylab
import matplotlib.pyplot as plt
#from pylab import *
from xml.etree.ElementTree import *

try:
    if sys.argv[1]:
        pass
except IndexError:
    print ("Please specify the input flowmonitor xml file")
    sys.exit(-1)

xmltree = ElementTree()

et = xmltree.parse(sys.argv[1])
bitrates = []
losses = []
delays = []
timing = []

for flow in et.findall("FlowStats/Flow"):

    for tpl in et.findall("Ipv4FlowClassifier/Flow"):

        if tpl.get('flowId') == flow.get('flowId'):
            break
            #       if tpl.get("destinationPort") == '49153':
            #continue

        lost = int(flow.get('lostPackets'))
        losses.append(lost)
        print("Lost packets: ", lost)

        rxPackets = int(flow.get('rxPackets'))
        print("Rxed: ", rxPackets)
        if rxPackets == 0:
            bitrates.append(0)

        #        else:
        t0 =  float(flow.get('timeFirstRxPacket')[:-2])
        t1 = float(flow.get('timeLastRxPacket')[:-2])
        duration = (t1 - t0) * 1e-9
        if duration == 0:
            duration = 1
        timing.append(duration)
        bitrates.append(8 * float(flow.get('rxBytes')) / (duration * 1e6))

        if rxPackets > 0:
            delays.append(float(flow.get('delaySum')[:-2]) * 1e-3 / rxPackets)

gap = len(bitrates) - len(timing)
while gap > 0:
    timing.append(1)
    gap -= 1

print(type(bitrates), len(bitrates))
print(type(timing), len(timing))

fig = plt.figure()
fig.suptitle('D2D simulation KPIs', fontsize=12)

ax = fig.add_subplot(111)
fig.subplots_adjust(top=0.85)
#ax.set_title('D2D simulation KPIs')

ax.set_xlabel('Packet timing')
ax.set_ylabel('Bitrate (Mbit/s)')

plt.plot(timing, bitrates)
plt.show()

# plt.plot(losses, timing)
# plt.show()
#
plt.plot(timing[:len(delays)], delays)
plt.show()

# pylab.subplot(311)
# pylab.hist(bitrates, bins=40)
# pylab.xlabel('Flowbitrate(bit/s)')
# pylab.ylabel("Numberofflows")
# pylab.subplot(312)
# pylab.hist(losses, bins=40)
# pylab.xlabel("Numberoflostpackets")
# pylab.ylabel("Numberofflows")
# pylab.subplot(313)
# pylab.hist(delays, bins=10)
# pylab.xlabel("Delay(s)")
# pylab.ylabel("Numberofflows")
# pylab.subplotsadjust(hspace=0.4)
# pylab.savefig("d2d-flowresults.pdf")
