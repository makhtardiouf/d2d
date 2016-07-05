# d2d
LTE-A Proximity-based Services, Device-to-device Communication module for NS-3

This repository hosts a prototyping environment developed for EPC-level user device
discovery and communication in LTE-Advanced networks, called D2dSim. 
Using the popular network simulation platform NS-3, it is one of the first open-source D2D-capable environment,
realistic and standard-compliant implementation of a subset of Proximity-based Services.

Implemented features:

    - EPC Network-assisted device discovery
        - UE Proximity Application (ProseApp) <->  EPC Proximity Function Entity (ProseFcn)
        - Start with these files: lte/model/epc-prose*, lte-proseapp*
        - lte/helper/lte-prose-helper*

    - LTE-Direct communication  (UE <-> UE)

    - WiFi adhoc, to emulate WiFi-Direct for comparison purposes   (UE <-> UE) 

What remains to be done:
    - LTE-Direct discovery; some data structures are in draft stage.

 
Configuration on Ubuntu:

apt-get install gtk+-2.0-dev  libgsl0-dev  libopenmpi-dev python-dev python-pygraphviz \
python-kiwi python-pygoocanvas python-gnome2 python-rsvg libsqlite3-dev   libgccxml-dev

    - cd ns3d2d
    - export NS3_DIR=`pwd`
    - export CXXFLAGS=" -O0   -ggdb   -g3   -Wall  -std=c++11 -fstrict-aliasing -Wstrict-aliasing "
    - ./waf configure --build-profile=debug --enable-sudo --enable-examples --enable-mpi 

Build:

    - cd $NS3_DIR
    - CXXFLAGS="-std=c++11" ./waf build   

Test:
    - ./waf --run d2dsim

Stats files(txt, pcap) will be generated under the OUTPUT directory
Detailed documentation will be written later on.

This project was initially hosted at: https://bitbucket.org/makhtardiouf/dtod

(C) 2014-2016 Gyeonsang N. University, Makhtar Diouf

InfoCom Engineering dept

