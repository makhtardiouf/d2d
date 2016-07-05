#!/bin/bash

NS3_DIR=$HOME/Documents/Code/Wireless-src/ns3-dev
EXEC=$NS3_DIR/build/src/lte/examples/ns3-dev-d2dsim-debug
#EXEC=$NS3_DIR/build/src/lte/examples/ns3-dev-d2dsim-optimized
FILE=d2d-valgrind.log

clear
cd $NS3_DIR

# ./waf shell environment variables
export NS3_EXECUTABLE_PATH=$NS3_DIR/build/src/fd-net-device:$NS3_DIR/build/src/tap-bridge
export LD_LIBRARY_PATH=/usr/lib/gcc/i686-linux-gnu/4.8:$NS3_DIR/build
export PATH=$NS3_EXECUTABLE_PATH:$PATH
export NS3_MODULE_PATH=$LD_LIBRARY_PATH

#export PYTHONPATH=$NS3_DIR/build/bindings/python:$HOME/Documents/ Code/Wireless-src/ns3-dev/src/visualizer

valgrind --log-file=$FILE --vgdb=no --track-origins=yes $EXEC
#--show-mismatched-frees=yes $EXEC

less $FILE
exit
