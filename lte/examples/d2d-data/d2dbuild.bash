#!/bin/bash

NS3_DIR=$HOME/Documents/Code/Wireless-src/ns3-dev
EXEC=$NS3_DIR/build/src/lte/examples/ns3-dev-d2dsim-debug

clear
cd $NS3_DIR

# It's sometimes necessary to force this
#mv -v $EXEC $EXEC.bak

./waf build &> d2d-build.log #&  tail -f d2d-build.log

less d2d-build.log
exit

