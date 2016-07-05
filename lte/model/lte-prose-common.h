/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014-2016 Gyeongsang N. University
 * InfoCom Engineering Dept, South Korea
 *
 * 
 * $Id: lte-prose-common.h, 2016-05-26 19:01 +0900 makhtar $
 * Advisor: Woonsup Lee 
 * Author: Makhtar Diouf 
 *
 * *** Proximity-based Services LTE-Direct (D2D) ***
 *
 * This program is open source software released under the terms of the
 * GNU General Public License version 2 as published by the Free Software Foundation.
 * It is distributed WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef LTE_PROSE_COMMON_H
#define	LTE_PROSE_COMMON_H

#include <ns3/log.h>
#include <ns3/boolean.h>
#include <ns3/double.h>
#include <ns3/string.h>
#include <ns3/integer.h>
#include <ns3/uinteger.h>
#include <ns3/type-id.h>
#include <ns3/application.h>
#include <ns3/application-container.h>
#include <ns3/node-list.h>
#include <ns3/object-factory.h>
#include <ns3/random-variable-stream.h>

#include <ns3/socket.h>
#include <ns3/address.h>
#include <ns3/mac48-address.h>
#include <ns3/ipv4-address.h>
#include <ns3/ipv4-address-helper.h>
#include <ns3/ipv4-static-routing-helper.h>
#include <ns3/ipv4-list-routing-helper.h>

#include <ns3/ipv6-address.h>
#include <ns3/ipv6-routing-helper.h>
#include <ns3/ipv6-static-routing-helper.h>
#include <ns3/ipv6-interface-container.h>

#include <ns3/tcp-socket.h>
#include <ns3/internet-stack-helper.h>
#include <ns3/tcp-socket-factory.h>
#include <ns3/tcp-l4-protocol.h>

#include <ns3/point-to-point-helper.h>
#include <ns3/lte-ue-net-device.h>
#include <ns3/virtual-net-device.h>

#include "ns3/data-rate.h"
#include "ns3/traced-callback.h"
#include "ns3/event-id.h"
#include "ns3/packet-loss-counter.h"

#include <sstream>
#include <list>
#include <map>

namespace ns3
{

/**
 * Avoid collisions in enum vars with same names
 * See TS.23.003 $24.2.1 Numbering, addressing...
 */
namespace Scope
{

enum _Scope
{
    GLOBAL = 00,
    RESERVED = 01,
    COUNTRY_SPECIFIC = 10,
    PLMN_SPECIFIC = 11
};
}

/** ProSeFcn's MCC/MNC are included or not */
namespace E
{

enum _E
{
    UNUSED = 0,
    INCLUDED = 1
};
}

class ProSe
{
public:

    ProSe(void)
    {

        // ipv4Hlte.SetBase("192.168.100.0", "255.255.255.0");
    }

    ~ProSe(void)
    {
        ;
    }

    static const int8_t DISCOVERY_PERIOD = 30;              // sec
    static constexpr double MAX_D2D_DISTANCE = 500.0;
    static constexpr double MAX_WLAN_DISTANCE = 50.0;       // can be 150m 80211b outdoor
    static constexpr double MAX_WLAN_VANET_DISTANCE = 100.0; // 802.11n could reach 250 this
    // Range class 3 
    static constexpr double MAX_LTED2D_DISTANCE = 200; 

    static Ipv4AddressHelper ipv4Hlte;
    static uint16_t wlanUes;

    /**
     * ProSe Direct communication parameters
     * Ref: TS24.334 p19
     */
    typedef struct _DirectComParams
    {
        bool useIpv4 = true;
        uint8_t l2GroupId;

        Address multicastGroupAddr;
        Ipv4Address multicastGroupIp;
        Ipv4Address multicastSourceIp;
        Ipv6Address multicastGroupIp6;
        Ipv6Address multicastSourceIp6;

        Ipv4Address peerIp;
        Ipv6Address peerIp6;

        uint16_t multicastPort = 9000; // 9= Discard port (RFC 863)
        std::string proseKey; // not implemented

        //*** Extra utilities to be set by ProSeApp
        double lteComStart = 0.0;
        double lteComStartInfra = 0.0;
        double lteComEnd = 5.0;
        // distance between GO and peer for stats
        double dist = 0.0;

    } DirectComParams;

    /**
     * L1/L2 layers properties...
     */

    enum
    {
        // R1-141138 ProSe D2D sub-frame
        DISCOVERY_SUBFRAME_PERIODICITY = 40, // ms

        // R1-141921
        // Discovery Message Reference Signal: num of symbols
        DMRS_SYMB = 2,

        // 4 PRBs over 2 non-consecutive sub-frames
        PRB_NUM = 8,

        // resource elements
        RE_NUM = 120,

        SYMBOLS_NUM = 10,
        PAYLOAD_SIZE = 200,
        MIC_SIZE = 32, // Scrambling ?
        CRC_SIZE = 24,

        // Discovery Message size 256 bits
        DM_SIZE = PAYLOAD_SIZE + MIC_SIZE + CRC_SIZE,

    };
};

} // ns-3

#endif	/* LTE_PROSE_COMMON_H */
