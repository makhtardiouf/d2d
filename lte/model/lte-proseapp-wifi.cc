/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014-2016 Gyeongsang N. University
 * InfoCom Engineering Dept, South Korea
 *
 * $Id: lte-proseapp-wifi.cc, 2016-05-31 16:40 +0900 makhtar $
 * Advisor: Woonsup Lee 
 * Author: Makhtar Diouf 
 *
 * *** Proximity-based Services D2D Communication over WiFi-adhoc ***
 *
 * This program is open source software released under the terms of the
 * GNU General Public License version 2 as published by the Free Software Foundation.
 * It is distributed WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <ns3/pointer.h>
#include <ns3/ptr.h>
#include <ns3/ipv4.h>
#include <ns3/ipv4-address-helper.h>
#include <ns3/on-off-helper.h>
#include <ns3/udp-client-server-helper.h>
#include <ns3/packet-sink-helper.h>
#include <ns3/wifi-net-device.h>
#include "ns3/energy-module.h"
#include "ns3/wifi-radio-energy-model-helper.h"
#include "lte-proseapp.h"
#include "lte-utils.h"
#include "lte-common.h"

// TODO: set doxygen comments in headers instead of cc

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("ProSeAppWifi");

/**
 * Multicast data over WLAN to mimic WiFi-D
 * Wi-Fi adhoc essentially embeds a software access point ("Soft AP")
 * Care is taken to handle new nodes that might be added or removed from the list of matches on the fly
 * Thus we reconfigure the wifi hooks in each cycle
 * Ref wifi-adhoc.cc, csma-multicast, rate-adaptation-distance
 * @TODO break this function into shorter parts
 */
void ProSeApp::StartWifiMulticast (std::list<uint64_t> matchesInRange)
{
    NS_LOG_FUNCTION (this);
    static ProSe::DirectComParams params;
    params.l2GroupId = this->epUID;

    static YansWifiPhyHelper wlanPhy = YansWifiPhyHelper::Default ();
    double wlanTxPower = 22.0;
    wlanPhy.Set ("TxPowerLevels", UintegerValue (4));
    wlanPhy.Set ("TxPowerStart", DoubleValue (wlanTxPower));
    wlanPhy.Set ("TxPowerEnd", DoubleValue (wlanTxPower + 4));
    //    wlanPhy.Set("RxGain", DoubleValue(0));
    YansWifiChannelHelper wifiChannelH = YansWifiChannelHelper::Default ();
    wifiChannelH.AddPropagationLoss ("ns3::RangePropagationLossModel");

    wlanPhy.SetChannel (wifiChannelH.Create ());
    wlanPhy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11_RADIO);
    NqosWifiMacHelper wifiMacH = NqosWifiMacHelper::Default (); // with AdhocWifiMac

    /***
     * See rate-adaptation-distance.cc - seem to be used with STANDARD_holland only
     * Note: Wifi P2P TS requires at least 80211g
     */
    uint32_t rtsThreshold = 2200; // packet size fragmentation
    WifiHelper wifiH;
    wifiH.SetRemoteStationManager ("ns3::MinstrelWifiManager", "RtsCtsThreshold", UintegerValue (rtsThreshold));
    wifiH.SetStandard (WIFI_PHY_STANDARD_holland);

    NetDeviceContainer srcDev;
    if (m_proseAppNode->GetObject<WifiNetDevice>() == 0)
    {
        srcDev = wifiH.Install (wlanPhy, wifiMacH, m_proseAppNode);
        params.multicastSourceIp = SetupNetwork (srcDev);

        wlanPhy.EnablePcap ("d2d-wlanhead", srcDev);
        m_inputInterface = srcDev.Get (0);
        m_multicastH.SetDefaultMulticastRoute (m_proseAppNode, m_inputInterface);
    }

    // params.multicastSourceIp6 = GetIpv6Address (m_inputInterface->GetNode ()); 

    // Get Adhoc multicast IP block
    std::stringstream ss;
    int net = this->epUID;
    ss << "225." << net << "." << net << "." << net;
    params.multicastGroupIp = Ipv4Address (ss.str ().c_str ());
    ss.str ("");
    ss << "FF05::" << this->epUID;
    params.multicastGroupIp6 = Ipv6Address (ss.str ().c_str ());

    NS_LOG_DEBUG ("ProseApp multicast addresses: " << params.multicastGroupIp
                  << " " << params.multicastGroupIp6);

    uint32_t i = 0;
    int nDevs = 0;
    Ptr<Node> n;
    double dist = 0.0;

    ProSe::wlanUes = 0;
    Ptr<ProSeApp> app;
    Ptr<LteUeNetDevice> udev;
    NetDeviceContainer outputDevs;
    Ipv4InterfaceContainer ifCon;

    Vector peerPos;
    Vector srcPos = m_proseAppNode->GetObject<MobilityModel> ()->GetPosition ();
    std::list<uint64_t>::iterator imsiIt;

    for (NodeList::Iterator it = NodeList::Begin (); it != NodeList::End (); ++ it)
    {
        n = * it;
        nDevs = n->GetNDevices ();
        for (int j = 0; j < nDevs; j ++)
        {
            udev = n->GetDevice (j)->GetObject <LteUeNetDevice> ();
            if (udev == 0)
                continue;

            /*** For now limit to 1 RX UE instead of iterating over all matches: 1-to-1 **/
            imsiIt = matchesInRange.begin ();
            // for (imsiIt; imsiIt != matchesInRange.end(); ++imsiIt)
            // {
            if (*imsiIt != udev->GetImsi ())
                continue;

            // Equivalent to installing a wifi device on the UE;
            // use directly the UE's node, thus solving the mobility setting issue
            if (n->GetObject<WifiNetDevice>() == 0)
            {
                outputDevs = wifiH.Install (wlanPhy, wifiMacH, n);
                ifCon = ipv4Hwlan.Assign (outputDevs);
                // For TCP test
                params.multicastGroupIp = ifCon.GetAddress (0, 0);

                wlanPhy.EnablePcap ("d2d-wlan", outputDevs.Get (0));
            }

            app = n->GetObject<ProSeApp>();
            peerPos = n->GetObject<MobilityModel>()->GetPosition ();
            dist = CalculateDistance (srcPos, peerPos);
            params.dist = dist;

            Ipv4StaticRoutingHelper ueStaticRoutingH;
            auto ueStaticRouting = ueStaticRoutingH.GetStaticRouting (GetIpv4 (n));
            ueStaticRouting->AddHostRouteTo (params.multicastGroupIp, 1);

            Simulator::Schedule (Seconds (0.0), &ProSeApp::JoinWifiMulticast, app, params);
            NS_LOG_DEBUG ("\tPeer IMSI " << * imsiIt << " " << dist << "m away");
            i ++;
            break; // out of 2nd for, Check next node
            // }
        }

        // Stop processing nodes
        if (i >= matchesInRange.size ())
            break;
    }

    /*
    // Energy - from energy-model-example.cc
    BasicEnergySourceHelper basicSourceHelper;
    // configure energy source
    basicSourceHelper.Set ("BasicEnergySourceInitialEnergyJ", DoubleValue (0.1));
    // install source
    EnergySourceContainer sources = basicSourceHelper.Install (m_proseAppNode);
    // device energy model
    WifiRadioEnergyModelHelper radioEnergyHelper;
    // configure radio energy model
    radioEnergyHelper.Set ("TxCurrentA", DoubleValue (0.0174));
    // install device model
    radioEnergyHelper.Install (outputDevs, sources);
     */

    // Multi-cast packet generator, differentiate the port from LTE-D & LTE-I
    NS_LOG_DEBUG ("Starting Tx...");
    Address addr;
    if (ns3::lteUseIpv6)
        addr = Address (Inet6SocketAddress (params.multicastGroupIp6, params.multicastPort + 2));
    else
        addr = Address (InetSocketAddress (params.multicastGroupIp, params.multicastPort + 2));

    OnOffHelper tx ("ns3::UdpSocketFactory", addr);
    //tx.SetConstantRate (DataRate (m_comDataRate), m_packetSize);
    ApplicationContainer txC = tx.Install (m_proseAppNode);
    txC.Start (Seconds (0.01));
    txC.Stop (Seconds (m_comDuration));
}


//** Temporary solution for matching dUes to join a multicast group and Rx

void ProSeApp::JoinWifiMulticast (ProSe::DirectComParams params)
{
    NS_LOG_FUNCTION (this->m_clientIp << params.multicastGroupIp << "IMSI" << this->epUID);
    this->m_params = params;

    // PacketSink internally uses UdpSocket::MulticastJoinGroup
    // Watch out the node might change
    if (! hookedWifiSink) //|| (wlanRxC == 0) )
    {
        Address addr;
        if (ns3::lteUseIpv6)
            addr = Inet6SocketAddress (params.multicastGroupIp6, params.multicastPort + 2);
        else
            addr = InetSocketAddress (params.multicastGroupIp, params.multicastPort + 2);

        PacketSinkHelper rx = PacketSinkHelper ("ns3::UdpSocketFactory", addr);
        wlanRxC = rx.Install (m_proseAppNode);
    }
    wlanRxC.Start (Seconds (0.0));
    wlanRxC.Stop (Seconds (m_comDuration + 0.01));

    m_peerPacketSinkWifi = DynamicCast<PacketSink>(wlanRxC.Get (0));
    if (m_peerPacketSinkWifi == 0)
    {
        NS_LOG_WARN ("Custom PackedSink Rx counter could not be set");
        return;
    }

    Simulator::Schedule (MilliSeconds (5), &ProSeApp::HookWifiPacketSink, this, m_peerPacketSinkWifi);
    m_wifiComStart = Simulator::Now ().GetSeconds ();
}

void ProSeApp::HookWifiPacketSink (Ptr<PacketSink> sink)
{
    NS_LOG_FUNCTION (this);
    if (sink != 0)
    {
        Simulator::Schedule (Seconds (1.0), &ProSeApp::ProbeWifiDataStats, this);
        Simulator::Schedule (Seconds (m_comDuration + 1.0), &ProSeApp::TurnOffWifi, this);
    }
}

/**
 * @brief Measure Wifi throughput every second
 * 
 */
void ProSeApp::ProbeWifiDataStats ()
{
    NS_LOG_FUNCTION (this);
    double time = Simulator::Now ().GetSeconds () - m_wifiComStart;

    // Convert to Mega BITs per sec  
    double throughput = (m_peerPacketSinkWifi->GetTotalRx () * 8.0) / (1000000 * time);

    stringstream ss;
    ss.str ("");
    ss << time << " \t" << this->epUID << " \t" << this->m_params.dist << " \t" << throughput;
    SaveWifiDataStats (ss.str ());

    Simulator::Schedule (Seconds (1.0), &ProSeApp::ProbeWifiDataStats, this);
    NS_LOG_DEBUG ("Wifi multicast " << throughput << " MBit/s @IMSI" << this->epUID);
}

void ProSeApp::TurnOffWifi ()
{
    NS_LOG_FUNCTION (this);
    Ptr<Ipv4> ipv4 = GetIpv4 (m_proseAppNode);
    if (ipv4 != 0)
    {
        // Case of Wifi NetDevice of RX
        if (m_inputInterface == 0)
            m_inputInterface = m_proseAppNode->GetObject<NetDevice> ();

        // Will produce Dropping received packet -- interface is down
        ipv4->SetDown (2); //m_inputInterface->GetIfIndex ());  // 1
    }
    if (m_proseAppNode->GetObject<WifiNetDevice>() == 0)
        return;
    m_proseAppNode->GetObject<WifiNetDevice>()->Dispose ();
}


/***
 * Needs the delay to set the route
 
void ProSeApp::AddMulticastRoute (ProSe::DirectComParams params, NetDeviceContainer outputDevs)
{
    NS_LOG_FUNCTION (this);
    m_multicastH.AddMulticastRoute (m_proseAppNode, params.multicastSourceIp, params.multicastGroupIp,
                                    m_inputInterface, outputDevs);

    // m_multicastH.SetDefaultMulticastRoute (m_proseAppNode, m_inputInterface);
}*/

} // End namespace ns3
