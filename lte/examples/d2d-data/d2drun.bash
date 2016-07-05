#!/bin/bash                                                                                                                                                                                                                                  
NS3_DIR=$HOME/Documents/Code/Wireless-src/ns3-dev
LOGFILE=d2d-run.log

clear
#export 'NS_LOG=ProSeD2dHelper=level_info:EpcProSeFcnApp=level_debug|prefix_func:ProSeApp=level_debug|prefix_func:EpcSgwPgwApplication=level_info|prefix_func'
#:LteEnbMac=level_debug|prefix_func'
cp  $NS3_DIR/build/src/lte/examples/ns3-dev-d2dsim-debug  $NS3_DIR/qns3sim/

cd $NS3_DIR; ./waf --run d2dsim &> $LOGFILE
#cd $NS3_DIR; ./waf --run d2dvanet &> $LOGFILE

#for i in Prose*pcap; do tcpdump -r $i; done  >> $LOGFILE

# Collect wifi SNR
grep -in "snr(dB)" d2d-run.log  > d2d-proseapp-wifi-SNR`date "+%Y%m%d"`.txt

less $LOGFILE
rm  $LOGFILE
exit
