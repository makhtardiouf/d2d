#!/bin/bash

NS3_DIR=$HOME/Documents/Code/Wireless-src/ns3-dev

clear
cd $NS3_DIR
./waf --run d2dsim --command-template="gdb --args %s <args>"
#./waf --run d2dvanet --command-template="gdb --args %s <args>"
exit
