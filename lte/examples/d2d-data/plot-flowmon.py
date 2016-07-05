#!/usr/bin/python

import sys
from optparse import OptionParser
import pylab
try:
    from xml.etree import cElementTree as ElementTree
except ImportError:
    from xml.etree import ElementTree
import bz2
from array import array
import math

class ConfidenceValue(object):

    def __init__(self, samples=None):
        if samples is None:
            self.samples = []
        else:
            self.samples = list(samples)
        self._mean = None
        self._error = None

    def add_sample(self, sample):
        self.samples.append(sample)

    # from http://experimentor.googlecode.com/svn
    def _mean_confidence_interval(self, confidence=0.95):
        from numpy import mean, array, sqrt
        import scipy.stats
        a = 1.0*array(self.samples)
        n = len(a)
        m, se = mean(a), scipy.stats.stderr(a)
        # calls the inverse CDF of the Student's t distribution
        h = se * scipy.stats.t._ppf((1 - confidence)/2.0, n-1)
        self._mean = m
        self._error = h

    @property
    def mean(self):
        if self._mean is None:
            self._mean_confidence_interval()
        return self._mean

    @property
    def error(self):
        if self._error is None:
            self._mean_confidence_interval()
        return self._error


class Flow(object):
    __slots__ = ['flowId', 'delayMean', 'packetLossRatio', 'rxBitrate', 'txBitrate']
    def __init__(self, flow_el):
        self.flowId = int(flow_el.get('flowId'))
        rxPackets = long(flow_el.get('rxPackets'))
        txPackets = long(flow_el.get('txPackets'))
        tx_duration = float(long(flow_el.get('timeLastTxPacket')[:-2]) - long(flow_el.get('timeFirstTxPacket')[:-2]))*1e-9
        rx_duration = float(long(flow_el.get('timeLastRxPacket')[:-2]) - long(flow_el.get('timeFirstRxPacket')[:-2]))*1e-9

        if rxPackets:
            self.delayMean = float(flow_el.get('delaySum')[:-2]) / rxPackets * 1e-9
        else:
            self.delayMean = None
        if rx_duration > 0:
            self.rxBitrate = long(flow_el.get('rxBytes'))*8 / rx_duration
        else:
            self.rxBitrate = None
        self.txBitrate = long(flow_el.get('txBytes'))*8 / tx_duration
        lost = float(flow_el.get('lostPackets'))
        self.packetLossRatio = lost / (rxPackets + lost)
        


class SimulationResults(object):
    def __init__(self, results_el):
        self.cpu_time = float(results_el.attrib['cpu-time'])
        self.run_number = int(results_el.attrib['run-number'])
        self.enable_monitor = {"1": True,
                               "True": True,
                               "0": False,
                               "False": False} [results_el.attrib['enable-monitor']]
        self.enable_tracing = {"1": True,
                               "True": True,
                               "0": False,
                               "False": False} [results_el.attrib['enable-tracing']]
        self.max_memory = long(results_el.attrib['max-memory'])
        self.num_nodes_side = int(results_el.attrib['num-nodes-side'])
        self.flows = [Flow(el) for el in results_el.findall('FlowMonitor/FlowStats/Flow')]
        self.xml_file_size = long(results_el.attrib['xml-file-size'])


class ResultsDb(object):
    def __init__(self):
        self.simulations = []

    def read_results_file(self, fname):
        tree = ElementTree.ElementTree()
        if fname.endswith('.bz2'):
            file_obj = bz2.BZ2File(fname)
        else:
            file_obj = open(fname)

        print "Reading XML file ",
        sys.stdout.flush()        
        for event, elem in ElementTree.iterparse(file_obj):
            if event == "end" and elem.tag == 'simulation':
                self.simulations.append(SimulationResults(elem))
                elem.clear() # won't need this any more
                sys.stdout.write(".")
                sys.stdout.flush()
        print " done."

def main():
    parser = OptionParser()
    (options, args) = parser.parse_args()

    results = ResultsDb()
    for fname in args:
        results.read_results_file(fname)
    print "Read %i simulations." % (len(results.simulations),)

    performance_results(results)
    histograms(results)
    #xml_file_size_results(results)


def mean(l):
    return sum(l)/len(l)

def stddev(l):
    m = mean(l)
    v = 0
    for x in l:
        v += (x - m)**2
    return math.sqrt(v)




def histograms(results):
    num_nodes_side_set = list(set(sim.num_nodes_side for sim in results.simulations))
    num_nodes_side_set.sort()
    num_nodes_side = num_nodes_side_set[-1]
    print "num_nodes: ", num_nodes_side**2

    delays = array('d')
    losses = array('d')
    rx_bitrates = array('d')
    tx_bitrates = array('d')
    for sim in results.simulations:
        if sim.num_nodes_side != num_nodes_side:
            continue
        if not sim.enable_monitor:
            continue
        for flow in sim.flows:
            if flow.delayMean is not None:
                delays.append(flow.delayMean)
            losses.append(flow.packetLossRatio)
            if flow.rxBitrate is not None:
                rx_bitrates.append(flow.rxBitrate)
            tx_bitrates.append(flow.txBitrate)

    tx_bitrate_cv = ConfidenceValue(tx_bitrates)
    rx_bitrate_cv = ConfidenceValue(rx_bitrates)
    delays_cv = ConfidenceValue(delays)
    losses_cv = ConfidenceValue(losses)
    
    print "Tx bitrate: mean=%r, stddev=%r, cv=[%r -- %r]" % (mean(tx_bitrates), stddev(tx_bitrates),
                                                             tx_bitrate_cv.mean - tx_bitrate_cv.error,
                                                             tx_bitrate_cv.mean + tx_bitrate_cv.error)
    print "Rx bitrate: mean=%r, stddev=%r, cv=[%r -- %r]" % (mean(rx_bitrates), stddev(rx_bitrates),
                                                             rx_bitrate_cv.mean - rx_bitrate_cv.error,
                                                             rx_bitrate_cv.mean + rx_bitrate_cv.error)
    print "Delays: mean=%r, stddev=%r, cv=[%r -- %r]" % (mean(delays), stddev(delays),
                                                         delays_cv.mean - delays_cv.error,
                                                         delays_cv.mean + delays_cv.error)
    print "Losses: mean=%r, stddev=%r, cv=[%r -- %r]" % (mean(losses), stddev(losses),
                                                         losses_cv.mean - losses_cv.error,
                                                         losses_cv.mean + losses_cv.error)

    pylab.subplot(221)
    pylab.xlabel("Delay (s)")
    pylab.ylabel("Number of Flows")
    pylab.hist(delays, 100)

    pylab.subplot(222)
    pylab.xlabel("Packet Loss Ratio")
    pylab.ylabel("Number of Flows")
    pylab.hist(losses, 100)

    pylab.subplot(223)
    pylab.xlabel("Transmitted Bitrate (bit/s)")
    pylab.ylabel("Number of Flows")
    pylab.hist(tx_bitrates, 100)

    pylab.subplot(224)
    pylab.xlabel("Received Bitrate (bit/s)")
    pylab.ylabel("Number of Flows")
    pylab.hist(rx_bitrates, 100)


    pylab.show()


def xml_file_size_results(results):
    xml_size_list = []

    num_nodes_list = []

    num_nodes_side_set = list(set(sim.num_nodes_side for sim in results.simulations))
    num_nodes_side_set.sort()

    for num_nodes_side in num_nodes_side_set:
        num_nodes_list.append(num_nodes_side*num_nodes_side)
        xml_size = ConfidenceValue()
        xml_size_list.append(xml_size)
        for sim in results.simulations:
            if sim.num_nodes_side == num_nodes_side:
                if not sim.enable_monitor:
                    continue
                xml_size.add_sample(sim.xml_file_size)
    pylab.plot(num_nodes_list, [s.mean for s in xml_size_list])
    pylab.ylabel("XML file size (B)")
    pylab.xlabel("Number of Nodes")
    pylab.show()


def performance_results(results):
    cpu_time_no_monitor = []
    cpu_time_monitor = []
    memory_no_monitor = []
    memory_monitor = []
    memory_tracing = []
    cpu_time_tracing = []

    num_nodes_list = []

    num_nodes_side_set = list(set(sim.num_nodes_side for sim in results.simulations))
    num_nodes_side_set.sort()

    for num_nodes_side in num_nodes_side_set:

        num_nodes_list.append(num_nodes_side*num_nodes_side)

        memory_cv_no_monitor = ConfidenceValue()
        memory_no_monitor.append(memory_cv_no_monitor)

        memory_cv_monitor = ConfidenceValue()
        memory_monitor.append(memory_cv_monitor)

        cpu_cv_no_monitor = ConfidenceValue()
        cpu_time_no_monitor.append(cpu_cv_no_monitor)

        cpu_cv_monitor = ConfidenceValue()
        cpu_time_monitor.append(cpu_cv_monitor)

        memory_cv_tracing = ConfidenceValue()
        memory_tracing.append(memory_cv_tracing)

        cpu_cv_tracing = ConfidenceValue()
        cpu_time_tracing.append(cpu_cv_tracing)

        for sim in results.simulations:
            if sim.num_nodes_side == num_nodes_side:
                if sim.enable_monitor:
                    memory_cv_monitor.add_sample(sim.max_memory)
                    cpu_cv_monitor.add_sample(sim.cpu_time)
                else:
                    if sim.enable_tracing:
                        memory_cv_tracing.add_sample(sim.max_memory)
                        cpu_cv_tracing.add_sample(sim.cpu_time)
                    else:
                        memory_cv_no_monitor.add_sample(sim.max_memory)
                        cpu_cv_no_monitor.add_sample(sim.cpu_time)

    pylab.subplot(221)
    pylab.grid(False)
    #pylab.title("Memory")
    pylab.xlabel("Number of Nodes")
    pylab.ylabel("Memory (bytes)")

    pylab.plot(
        num_nodes_list,
        [val.mean for val in memory_no_monitor],
        #[val.error for val in memory_no_monitor],
        label="Base", hold=True, linestyle='--')

    pylab.plot(
        num_nodes_list,
        [val.mean for val in memory_monitor],
        #[val.error for val in memory_monitor],
        label="Flow Monitor", hold=True)

    pylab.plot(
        num_nodes_list,
        [val.mean for val in memory_tracing],
        #[val.error for val in memory_monitor],
        label="Trace File",  linestyle=':', hold=True)

    pylab.legend(loc='best')


    pylab.subplot(223)
    pylab.grid(False)
    #pylab.title("CPU Time")
    pylab.xlabel("Number of Nodes")
    pylab.ylabel("Time (s)")

    pylab.plot(
        num_nodes_list,
        [val.mean for val in cpu_time_no_monitor],
        #[val.error for val in cpu_time_no_monitor],
        label="Base", hold=True, linestyle='--')

    pylab.plot(
        num_nodes_list,
        [val.mean for val in cpu_time_monitor],
        #[val.error for val in cpu_time_monitor],
        label="Flow Monitor", hold=True)

    pylab.plot(
        num_nodes_list,
        [val.mean for val in cpu_time_tracing],
        #[val.error for val in cpu_time_monitor],
        label="Trace File", linestyle=':', hold=True)

    pylab.legend(loc='best')

    #pylab.show()




    pylab.subplot(222)
    pylab.grid(False)
    pylab.xlabel("Number of Nodes")
    pylab.ylabel("Memory Overhead (%)")

    pylab.plot(
        num_nodes_list,
        [100*(mon.mean / nomon.mean - 1) for nomon, mon in zip(memory_no_monitor, memory_monitor)],
        label="Flow Monitor",
        hold=True)
    pylab.plot(
        num_nodes_list,
        [100*(mon.mean / nomon.mean - 1) for nomon, mon in zip(memory_no_monitor, memory_tracing)],
        linestyle='--',
        label="Trace File",
        hold=True)
    pylab.legend(loc='best')

    pylab.subplot(224)
    pylab.grid(False)
    pylab.xlabel("Number of Nodes")
    pylab.ylabel("Time Overhead (%)")

    pylab.plot(
        num_nodes_list,
        [100*(mon.mean / nomon.mean - 1) for nomon, mon in zip(cpu_time_no_monitor, cpu_time_monitor)],
        label="Monitor",
        hold=True)
    pylab.plot(
        num_nodes_list,
        [100*(mon.mean / nomon.mean - 1) for nomon, mon in zip(cpu_time_no_monitor, cpu_time_tracing)],
        linestyle='--',
        label="Trace file",
        hold=True)

    pylab.legend(loc='best')


    print "Memory overhead: %.2f%% for %i nodes" % ([100*(mon.mean / nomon.mean - 1) for nomon, mon in zip(memory_no_monitor, memory_monitor)][-1], num_nodes_list[-1])
    print "Memory for %i nodes: %s bytes with monitor, %s bytes without monitor, difference=%s" \
        % (num_nodes_list[-1], memory_monitor[-1].mean, memory_no_monitor[-1].mean,
           (memory_monitor[-1].mean - memory_no_monitor[-1].mean))
    print "CPU overhead: %.2f%% for %i nodes" % ([100*(mon.mean / nomon.mean - 1) for nomon, mon in zip(cpu_time_no_monitor, cpu_time_monitor)][-1], num_nodes_list[-1])
    print "CPU time for %i nodes: %s sec. with monitor, %s sec. without monitor, difference=%s" \
        % (num_nodes_list[-1], cpu_time_monitor[-1].mean, cpu_time_no_monitor[-1].mean,
           (cpu_time_monitor[-1].mean - cpu_time_no_monitor[-1].mean))

    pylab.show()

if __name__ == '__main__':
    main()
