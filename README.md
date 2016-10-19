# d2d
LTE-A Proximity-based Services, Device-to-device Communication module for NS-3

This repository hosts a prototyping environment developed for EPC-level user device
discovery and communication in LTE-Advanced networks, called D2dSim. Using the popular network simulation platform NS-3, it is one of the first open-source D2D implementation attempt of a subset of the 3GPP Proximity-based Services Release 12/13.

**Implemented features:

    EPC Network-assisted device discovery
        UE Proximity Application (ProseApp) <->  EPC Proximity Function Entity (ProseFcn)
        See files: lte/model/epc-prose*, lte-proseapp*, lte/helper/lte-prose-helper*

    LTE-Direct communication  (UE <-> UE)

    WiFi adhoc, to emulate WiFi-Direct for comparison purposes   (UE <-> UE) 

**What remains to be done:

    LTE-Direct discovery; some data structures are in draft stage.

 
**1. Configuration on Ubuntu:

    apt-get install libgccxml-dev libgsl0-dev libopenmpi-dev python-dev python-pygraphviz \
    python-kiwi python-pygoocanvas python-gnome2 python-rsvg libsqlite3-dev gtk+-2.0-dev

**2. Get the ns-3 development tree:

    hg clone -r 12192 http://code.nsnam.org/ns-3-dev ns3-dev
    cd ns3-dev
    export NS3_DIR=`pwd`
    
    ** Note that recent updates of mainstream ns-3 can bring compilation or runtime failures, 
    due to TrafficControl & Queuing features that were added to the IP network module. 
    Thus, use Revision 12192 above.


**3. Pull the D2D-related changesets:

    git clone https://github.com/makhtardiouf/d2d.git
    cp -Rv d2d/* src/
    mv d2d/.git src/      # For future updates with git pull

    cd $NS3_DIR
    export CXXFLAGS="-O0 -ggdb -g3 -Wall -std=c++11 -fstrict-aliasing -Wstrict-aliasing"
    ./waf configure --build-profile=debug --enable-sudo --enable-examples


**4. Build:

    CXXFLAGS="-std=c++11" ./waf build   

**5. Run:

    ./waf --run d2dsim


** Utility scripts are in:

    $NS3_DIR/src/lte/examples/d2d-data/d2d*.bash
    d2dbuild.bash  Build and display log
    d2drun.bash  Executes D2dSim and display logged output at program termination
    d2ddbg.bash  Runs D2dSim under the GDB debugger ...
    Link them under /usr/local/bin/  for your convenience 
    

Stats files(txt, pcap) are saved under the OUTPUT directory.

Recommended environment: (K)Ubuntu 15.x, 16.x; g++ 4.9, 5.x

Detailed documentation will be written later on.

This project was initially hosted at: https://bitbucket.org/makhtardiouf/dtod

    NS-3 LTE module: (C) Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
    HTTP-related code: (C) Georgia Tech Research Corporation

    D2D features: (C) 2014-2016 Gyeonsang N. University
    InfoCom Engineering department
    Adviser: Woongsup Lee
    Implementer: Makhtar Diouf
