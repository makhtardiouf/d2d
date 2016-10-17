/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- 
 * Copyright (c) 2014-2016 Gyeongsang N. University
 * InfoCom Engineering Dept, South Korea
 *
 * $Id: lte-proseapp-infra.cc, 2016-07-05 18:56 +0900 makhtar $
 * Advisor: Woonsup Lee 
 * Author: Makhtar Diouf 
 *
 * *** Proximity-based Services: mode selection, standard Communication over LTE infrastructure ***
 *
 * This program is open source software released under the terms of the
 * GNU General Public License version 2 as published by the Free Software Foundation.
 * It is distributed WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <ns3/ipv4-address-helper.h>
#include <ns3/on-off-helper.h>
#include <ns3/udp-client-server-helper.h>
#include <ns3/packet-sink-helper.h>
#include <ns3/aodv-helper.h>
#include <ns3/olsr-helper.h>

#include <random>
#include "lte-proseapp.h"
#include "lte-utils.h"
#include "lte-common.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("ProSeApp");

TypeId
ProSeApp::GetTypeId (void)
{
    static TypeId tid = TypeId ("ns3::ProSeApp")
            .SetParent<Application> ()
            ;
    return tid;
}

ProSeApp::ProSeApp ()
{
    NS_LOG_FUNCTION ("\n" << this);
    m_proseAppNode = CreateObject<Node> ();
    m_inet.Install (m_proseAppNode);
    SetAppId ();
}

ProSeApp::ProSeApp (Ptr<Node> n)
{
    m_proseAppNode = n;
    SetAppId ();
}

ProSeApp::~ ProSeApp ()
{
    NS_LOG_FUNCTION ("\n" << this);
}

void
ProSeApp::DoInitialize ()
{
    NS_LOG_FUNCTION ("\n" << this);

}

void
ProSeApp::DoDispose ()
{
    NS_LOG_FUNCTION ("\n" << this);
    // chain up
    Application::DoDispose ();
}

void
ProSeApp::StartApplication (void)
{
    NS_LOG_FUNCTION ("\n" << this);
}

void
ProSeApp::StopApplication (void)
{
    NS_LOG_FUNCTION ("\n" << this);
}

/**
 * @brief ProSeApp::SetupApp
 * @param n node of the Ue
 * @param srvIp ProSeFcn's IP
 * @param port ProSeFcn's port number
 */
void
ProSeApp::SetupApp (Ptr<Node> n, Ipv6Address srvIp6, Ipv4Address srvIp, uint16_t port)
{
    NS_LOG_FUNCTION ("\n" << this << n << srvIp << srvIp6 << port);
    if (n != 0)
        m_proseAppNode = n;

    m_srvPort = port;
    objFactory.SetTypeId ("ns3::HttpClient");

    if (ns3::lteUseIpv6)
    {
        m_srvIp6 = srvIp6;
        m_srvAddr = Address (srvIp6);
        objFactory.Set ("Remote", AddressValue (Inet6SocketAddress (m_srvIp6, m_srvPort)));
    }
    else
    {
        m_srvIp = srvIp;
        m_srvAddr = Address (srvIp);
        objFactory.Set ("Remote", AddressValue (InetSocketAddress (m_srvIp, m_srvPort)));
    }

    clientApp = objFactory.Create<Application> ();
    m_proseAppNode->AddApplication (clientApp);

    /** Start after ProSeFcn. 
     ** Avoid "EpcUeNas:Send(): NAS OFF, discarding packet" 
     ** that delays the TcpSocketBase::Connect towards ProSeFcn
     */
    clientApp->SetStartTime (MilliSeconds (5));
}

/**
 * @brief ProSeApp::RequestDiscovery request procedures to ProSeFcn
 * @param cmd ANNOUNCE or MONITOR
 * @TODO include timeout T4000...
 * Ref: TS 24.334 $12.3
 */
void
ProSeApp::RequestDiscovery (Command cmd)
{
    NS_LOG_FUNCTION ("\n" << this);
    std::stringstream ip, ss;

    if (ns3::lteUseIpv6)
        ip << m_clientIp6;
    else
        ip << m_clientIp;
    ss << ip.str() << Simulator::Now ().GetNanoSeconds ();
    transactionId = ss.str ();
    ss.str ("");
    ss << "TransactionId=" << transactionId
            //  << "&AppCode=" << GetAppCode()
            << "&AppId=" << GetAppId ()
            << "&EpUID=" << GetEpUID ()
            << "&RangeClass=" << RangeClass::r3 // ~200m
            << "&WlanIndication=true"           // willing to engage in WLAN Direct com
            << "&Command=" << cmd;

    Ptr<HttpClient> app = DynamicCast<HttpClient> (clientApp);
    int txed = app->HttpPost ("/DISCOVERY_REQUEST", ss.str ());
    if (txed != - 1)
    {
        NS_LOG_INFO ("\tProSeApp@" << ip.str () << ":" << m_clientPort
                     << " TXed " << txed << " bytes to "
                     << m_srvIp << " " << m_srvIp6 << ":" << m_srvPort
                     << " Cmd=" << cmd
                     //<< " [" << ss.str () << "] "
                     );
    }
    else
        NS_LOG_INFO ("\tProSeApp@" << ip.str () << " TX failed. Retrying in "
                     << (int) ProSe::DISCOVERY_PERIOD << "s...");

    /** 
     * Re-run the same Command in cycles (uncomment if needed)
     * Gives a deterministic hint on which UE Txes or Rxes
     */
    ///  Simulator::Schedule (Seconds (ProSe::DISCOVERY_PERIOD), &ProSeApp::RequestDiscovery, this, cmd);
}

void
ProSeApp::SetAppId (string plmnID, string l1, string l2, string l3)
{
    NS_LOG_FUNCTION ("\n" << this);
    proseAppIdStruct.plmnID = plmnID;
    proseAppIdStruct.l0 = "ProSeApp";
    proseAppIdStruct.l1 = l1;
    proseAppIdStruct.l2 = l2;
    proseAppIdStruct.l3 = l3;
    std::ostringstream sstream;

    sstream << plmnID << "." << proseAppIdStruct.l0 << "."
            << l1 << "." << l2 << "." << l3;

    proseAppId = sstream.str ();
    NS_LOG_DEBUG ("AppID: " << proseAppId);
}

/**
 * @brief ProSeApp::SetAppCode
 * @param plmnID
 * @param tempID
 * TODO: should be generated by ProSeFcn and fedback by HTTP to Ue along with a timer T4000
 */
void
ProSeApp::SetAppCode (string plmnID, string tempID)
{
    NS_LOG_FUNCTION ("\n" << this);
    // watch out, appcode is currently 167 bits because of MCC, MNC set to 0
    proseAppCodeStruct.plmnID = plmnID;
    proseAppCodeStruct.tempID = tempID;
    proseAppCode = plmnID.append (tempID);
    //    NS_LOG_DEBUG("AppCode: " << proseAppCode);
}

/**
 * Randomize the network base for different wlan heads
 * to avoid address collisions during concurrent calls
 */
Ipv4Address ProSeApp::SetupNetwork (NetDeviceContainer devC, bool isWifi)
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng (rd ()); // seed the generator
    std::uniform_int_distribution<> distr (this->epUID, 254); // define the range

    std::stringstream ss;
    Ipv4Address headIp;
    Ipv4InterfaceAddress iface;
    Ipv4InterfaceContainer ifCon;
    if (isWifi)
    {
        ss << "10." << distr (eng) << "." << this->epUID << ".0";
        ipv4Hwlan.SetBase (ss.str ().c_str (), "255.255.255.0");
        ipv4Hwlan.NewNetwork ();
        ifCon = ipv4Hwlan.Assign (devC);

        TrafficControlHelper tcHelper = TrafficControlHelper::Default ();
        //tcHelper.Uninstall (devC);
        // Temp workaround
        headIp = ipv4Hwlan.NewAddress (); // ifCon.GetAddress(0, 1);//GetIpv4Address (this->GetNode ());
    }
    else
    {
        //*** 2nd IP LTE D2D        
        // Fix the address space for now to avoid "route not found" issue when re-processing
        // Watch out for Address Collision if another announcing UE triggers this
        /*   ss << "192.168.100.0" ; //<< distr (eng) << ".0";
           ProSe::ipv4Hlte.SetBase (ss.str ().c_str (), "255.255.255.0");
         *  ProSe::ipv4Hlte.NewNetwork ();
         */

        ifCon = ProSe::ipv4Hlte.Assign (devC);
        // 2nd IP of the UE
        headIp = ifCon.GetAddress (0, 1);
        m_peerIp = ifCon.GetAddress (1, 1);

        static bool routeIsSet = false;
        if (! routeIsSet)
        {
            Ipv4ListRoutingHelper listRoutingH;
            AodvHelper aodvH;
            listRoutingH.Add (aodvH, 1);
            m_inet.SetRoutingHelper (listRoutingH);

            // We avoid bumping on the 7.0.0.0 address space
            Ptr<Ipv4StaticRouting> ueStaticRouting = m_multicastHlte.GetStaticRouting (devC.Get (1)->GetNode ()->GetObject<Ipv4>());
            ueStaticRouting->AddHostRouteTo (headIp, headIp, 1);
            ueStaticRouting->SetDefaultRoute (headIp, 1, 1500);

            ueStaticRouting = m_multicastHlte.GetStaticRouting (devC.Get (0)->GetNode ()->GetObject<Ipv4>());
            ueStaticRouting->AddHostRouteTo (m_peerIp, m_peerIp, 1);
            ueStaticRouting->SetDefaultRoute (m_peerIp, 1, 1500);
            routeIsSet = true;
        }
        //   m_inet.EnablePcapIpv4 ("d2d-lte", ifCon);
    }
    return headIp;
}

/**
 * TODO: IPv6
 * @param peer
 */
void ProSeApp::StartLteInfraTx (Ptr<Node> peer)
{
    NS_LOG_FUNCTION ("\n" << this);

    ProSe::DirectComParams params;
    params.lteComEnd = m_comDuration;
    params.dist = CalculateDistance (m_proseAppNode->GetObject<MobilityModel> ()->GetPosition (),
                                     peer->GetObject<MobilityModel>()->GetPosition ());

    this->m_params = params;
    Simulator::ScheduleNow (&ProSeApp::JoinLteInfra, peer->GetObject<ProSeApp>(), params);

    m_peerIp = GetIpv4Address (peer);
    NS_LOG_DEBUG ("IMSI" << this->epUID << " " << m_clientIp << " Tx to LTE peer " << m_peerIp);

    // Differentiate with the D2D Rx port
    Address addr = Address (InetSocketAddress (m_peerIp, params.multicastPort + 1));
    OnOffHelper tx ("ns3::TcpSocketFactory", addr);
    ApplicationContainer txC = tx.Install (m_proseAppNode);
    Ptr<OnOffApplication> txApp = DynamicCast<OnOffApplication>(txC.Get (0));
    //   txApp->SetAttribute ("OnTime", StringValue ("ns3::UniformRandomVariable[Min=1000.0|Max=2000.0]"));
    //   txApp->SetAttribute ("OffTime", StringValue ("ns3::UniformRandomVariable[Min=1.0|Max=2.0]"));
    string comLteIDataRate = "10Mbps";
    txApp->SetAttribute ("DataRate", DataRateValue (comLteIDataRate));
    txApp->SetAttribute ("PacketSize", UintegerValue (m_packetSize));

    m_inet.EnablePcapIpv4 ("Lte-Ihead", NodeContainer (m_proseAppNode));
    //TrafficControlHelper tcHelper = TrafficControlHelper::Default ();
    //tcHelper.Uninstall (m_proseAppNode->GetObject<NetDevice> ());

    txC.Start (Seconds (0.1));
    txC.Stop (Seconds (params.lteComEnd));
}

/**
 * Peer listen on its own IP to receive data
 * @param params
 */
void ProSeApp::JoinLteInfra (ProSe::DirectComParams params)
{
    NS_LOG_FUNCTION ("\n" << this->m_clientIp << "IMSI" << this->epUID);
    Address addr;
    if (ns3::lteUseIpv6)
        addr = Inet6SocketAddress (this->m_clientIp6, params.multicastPort + 1);
    else
        addr = InetSocketAddress (this->m_clientIp, params.multicastPort + 1);

    m_inet.EnablePcapIpv4 ("Lte-I", NodeContainer (m_proseAppNode));
    PacketSinkHelper rx = PacketSinkHelper ("ns3::TcpSocketFactory", addr);
    ApplicationContainer rxC = rx.Install (this->m_proseAppNode);
    //TrafficControlHelper tcHelper = TrafficControlHelper::Default ();
    //tcHelper.Uninstall (m_proseAppNode->GetObject<NetDevice> ());
    
    rxC.Start (Seconds (0.0));
    rxC.Stop (Seconds (params.lteComEnd + 0.1));

    m_peerPacketSinkInfra = DynamicCast<PacketSink>(rxC.Get (0));
    if (m_peerPacketSinkInfra == 0)
    {
        NS_LOG_WARN ("JoinLteD2d could not get a PacketSink");
        return;
    }

    this->m_params = params;
    // For calculating throughput
    this->m_params.lteComStartInfra = Simulator::Now ().GetSeconds ();

    m_lteStatsProbEvent = Simulator::Schedule (Seconds (1.0), &ProSeApp::ProbeLteDataStatsInfra, this);
    Simulator::Schedule (Seconds (this->m_params.lteComEnd + 0.5), &ProSeApp::AtComEnd, this, false);
}

/**
 * @brief LTE data statistics
 * Calculate throughput in Mega BITs per sec, Run again every sec
 * @todo Can Use lteH->GetPdcpStats ()->...
 */
void ProSeApp::ProbeLteDataStatsInfra ()
{
    NS_LOG_FUNCTION ("\n" << this);
    volatile double time = Simulator::Now ().GetSeconds () - this->m_params.lteComStartInfra;

    // Difference since the previous probe (1 sec)
    volatile double throughput = ((m_peerPacketSinkInfra->GetTotalRx () * 8.0) / (1000000 * time));

    stringstream ss;
    ss.width (6);
    ss << time << "\t" << this->epUID
            << " \t" << this->m_params.dist
            << " \t" << throughput;

    SaveLteDataStats (ss.str ());
    Simulator::Schedule (Seconds (1.0), &ProSeApp::ProbeLteDataStatsInfra, this);
    NS_LOG_DEBUG (m_lteRxType << " " << throughput << " MBit/s" << " @ IMSI" << this->epUID);
}

}
