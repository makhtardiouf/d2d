#!/bin/bash
clear
DATA_DIR=$HOME/Documents/Code/Wireless-src/ns3-dev/src/lte/examples/d2d-data

# Folder where LTE traces files are outputted
STATS_DIR=$HOME/Documents/Code/Wireless-src/ns3-dev/OUTPUT/$(date +%Y%m%d)/3kmh_15Ues_Ratio1

echo; echo "Using $DATA_DIR/d2d-gnuplotinput.plt"
#cp $DATA_DIR/d2d-gnuplotinput.plt $STATS_DIR/

# For saving traces under SCM at each commit
cp $STATS_DIR/{Dl,Ul}*.txt $DATA_DIR
cd $DATA_DIR
gnuplot d2d-gnuplotinput.plt

cp -v $STATS_DIR/build/src/lte/examples/ns3-dev-d2dsim-debug-anim.xml $DATA_DIR/
cp  *.png $STATS_DIR

#display d2d-ulsinr.png
#display d2d-dlpower.png
#display d2d-dlsinr.png
echo "Check generated image files in $STATS_DIR"
exit
