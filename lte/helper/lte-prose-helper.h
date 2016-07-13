/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014-2016 Gyeongsang N. University
 * InfoCom Engineering Dept, South Korea
 * Proximity-based LTE-Direct (D2D)
 *
 * 
 * $Id: lte-prose-helper.h, 2016-07-13 20:30 +0900 makhtar $
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

#ifndef LTE_PROSE_HELPER_H
#define	LTE_PROSE_HELPER_H

// Do not add headers ending with module.h here
#include <ns3/config-store.h>
#include <ns3/global-value.h>
#include <ns3/object-ptr-container.h>

#include <ns3/epc-helper.h>
#include <ns3/emu-epc-helper.h>
#include <ns3/point-to-point-epc-helper.h>
#include <ns3/mobility-helper.h>
#include <ns3/mobility-model.h>
#include <ns3/lte-hex-grid-enb-topology-helper.h>

#include "ns3/lte-proseapp.h"
#include "ns3/epc-prose-fcn.h"
#include <vector>

namespace ns3
{

/* R1 141921
 * Duration 1ms
 * With target SINR 0 dB, 8PRBs split in 2 x 4PRBs non-consecutive subframes
 * Normal CP
 */
struct ueDiscoveryPayload : public ObjectPtrContainerValue
{
    uint8_t ueNum;
    Ipv4Address ip;
    uint8_t xPos;
    uint8_t yPos;
};

/**
 * @brief The ProSeHelper class
 * \ingroup lte-prose
 */
class ProSeHelper : public Object
{
public:
    ProSeHelper(void);
    ~ProSeHelper(void);

    static TypeId GetTypeId(void);
    virtual void DoDispose(void);

    void CreateNodesAndHelpers(uint8_t enbNum, uint8_t ueNum, double dUesRatio = 0.16, bool useEmulation = false);

    ConfigStore SetConfig(std::string file, bool isInput);

    void SetNodesDistribution(uint8_t dist)
    {
        m_nodesDistribution = dist;
    }

    void InstallUes(std::string mobmodel = "ns3::ConstantPositionMobilityModel");

    void InstallEpcRanInHexGrid(double cellRadius = 250);

    void InstallEnbs();

    MobilityHelper GetUeMobilityHelper()
    {
        return ueMobility;
    }

    void SetupIpNetwork();

    void InitDiscovery(NetDeviceContainer devs = NetDeviceContainer ());

    //uint8_t SearchPeer(uint8_t imsi);
   
    void DeActivateBearer(Ptr<LteUeNetDevice> netDev);
    
    void RealtimeOp();

    uint8_t trafficStart;
    uint8_t trafficDuration;

    uint16_t dUeNum;
    uint16_t cUeNum;

    NodeContainer ueD2dNodes;
    NodeContainer ueCellNodes;
    NetDeviceContainer ueD2dDevs;
    NetDeviceContainer ueCellDevs;
    NetDeviceContainer macroEnbDevs;
    
    Ptr<LteHelper> lteH;
    Ptr<PointToPointEpcHelper> epcH;
    Ptr<EmuEpcHelper> emuEpcH;

    /** Distribution of nodes */
    enum
    {
        RANDOM,
        UNIFORM,
    };

    //static 
    Ipv4InterfaceContainer ueIpInterface;
    Ptr<Node> GetProseFcnNode() {return m_proseFcnNode;}
    Ptr<Node> GetPgwNode() {return pgwNode;}
    double cellRadius;
    
private:
    void SetupProSeFcn();
    
    Ptr<LteHexGridEnbTopologyHelper> hexGridH;
    bool useHexGrid;

    NodeContainer macroEnbNodes;
    NodeContainer homeEnbNodes;
    NetDeviceContainer homeEnbDevs;

    Ptr<Node> pgwNode;
    Ipv4Address m_pgwIp;   
    Ipv6Address m_pgwIpv6;    
    
    bool m_useEmulation = false;

    // ProSeFcn
    Ptr<EpcProSeFcnApp> m_proseFcnApp;
    Ptr<Node> m_proseFcnNode;
    Ipv4AddressHelper m_proseAddrH;
    Ipv6AddressHelper m_proseAddrHv6;

    std::string dataRate;
    uint8_t macroEnbBandwidth;
    uint8_t macroEnbDlEarfcn;
    MobilityHelper ueMobility;

    InternetStackHelper inet;
    Ptr<Ipv4StaticRouting> ueStaticRouting;
    Ipv4StaticRoutingHelper ueStaticRoutingH;
    Ipv4ListRoutingHelper listRoutingH;

    std::vector< Ptr<ProSeApp> > apps;

    uint8_t m_nodesDistribution = UNIFORM;
};

} // ns-3 namespace
#endif	/* LTE_PROSE_HELPER_H */
