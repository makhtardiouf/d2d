#!/bin/bash                                                                                                                                                                                                                                  
NS3_DIR=$HOME/Documents/Code/Wireless-src/ns3-dev

clear
#export 'NS_LOG=ProSeD2dHelper=level_info:EpcProSeFcnApp=level_debug|prefix_func:ProSeApp=level_debug|prefix_func:EpcSgwPgwApplication=level_info|prefix_func'
#:LteEnbMac=level_debug|prefix_func'

LOG=vhighway.log

cd $NS3_DIR; ./waf --run vhighway &> $LOG
less $LOG
exit
