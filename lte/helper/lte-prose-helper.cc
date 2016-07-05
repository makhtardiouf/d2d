/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014-2016 Gyeongsang N. University
 * InfoCom Engineering Dept, South Korea
 *
 * 
 * $Id: lte-prose-helper.cc, 2016-07-05 18:56 +0900 makhtar $
 * Advisor: Woonsup Lee 
 * Author: Makhtar Diouf 
 * *** Proximity-based Services LTE-Direct (D2D) ***
 *
 * This program is open source software released under the terms of the
 * GNU General Public License version 2 as published by the Free Software Foundation.
 * It is distributed WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

/**** Do not add headers ending with module.h here ****/
#include <ns3/log.h>
#include <ns3/buildings-helper.h>
#include <ns3/mobility-helper.h>
#include <ns3/mobility-model.h>
#include <ns3/lte-spectrum-value-helper.h>
//#include <ns3/eps-bearer.h>
#include <ns3/emu-fd-net-device-helper.h>
#include <ns3/lte-enb-rrc.h>
#include <ns3/lte-ue-net-device.h>
#include "ns3/lte-ue-mac.h"
#include "ns3/traffic-control-helper.h"
#include "ns3/lte-utils.h"
#include "lte-prose-helper.h"

#include <map>
#include <cstdlib>

using namespace std;

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("ProSeHelper");

// Static member of common properties need to be "redefined" this way to work around the compiler's complaints
uint16_t ProSe::wlanUes = 0;
Ipv4AddressHelper ProSe::ipv4Hlte;

ProSeHelper::ProSeHelper (void)
{
    NS_LOG_FUNCTION (this);

    useHexGrid = true;
    trafficStart = 10;
    trafficDuration = 100;
    macroEnbBandwidth = 50; // nRBs, 10 MHz
    macroEnbDlEarfcn = 100;
    ProSe::ipv4Hlte.SetBase("192.168.100.0", "255.255.255.0");

}

ProSeHelper::~ ProSeHelper (void)
{
    NS_LOG_FUNCTION (this);
}

TypeId
ProSeHelper::GetTypeId (void)
{
    static TypeId tid = TypeId ("ns3::ProSeHelper").SetParent<Object> ();

    return tid;
}

void
ProSeHelper::DoDispose (void)
{
    NS_LOG_FUNCTION (this);

    // Release the mem of the ProSeApps
    for (auto&& it : apps)
    {
        it->Dispose ();
    }
    apps.clear ();
}

/**
 * @brief ProSeHelper::SetConfig Load general configuration
 * @param file Input filename
 * @param isInput if true, set as default values
 * @return ConfigStore object
 */
ConfigStore
ProSeHelper::SetConfig (string file, bool isInput)
{
    Config::SetDefault ("ns3::ConfigStore::Filename", StringValue (file + "-confinput.txt"));
    Config::SetDefault ("ns3::ConfigStore::FileFormat", StringValue ("RawText"));

    if (isInput)
    {
        NS_LOG_DEBUG ("\tLoading configuration...");
        Config::SetDefault ("ns3::ConfigStore::Mode", StringValue ("Load"));
        Config::SetDefault ("ns3::RandomWalk2dMobilityModel::Mode", StringValue ("Time"));
        Config::SetDefault ("ns3::RandomWalk2dMobilityModel::Time", StringValue ("1s"));
        Config::SetDefault ("ns3::RandomWalk2dMobilityModel::Speed", StringValue ("ns3::ConstantRandomVariable[Constant=3.0]"));
        Config::SetDefault ("ns3::RandomWalk2dMobilityModel::Bounds", StringValue ("-250|250|-250|250"));
    }
    else
    {
        // Invoke just before entering Simulator::Run ()
        NS_LOG_DEBUG ("\tSaving configuration...");
        Config::SetDefault ("ns3::ConfigStore::Filename", StringValue (file + "-confoutput.txt"));
        Config::SetDefault ("ns3::ConfigStore::Mode", StringValue ("Save"));
    }

    ConfigStore conf;
    if (isInput)
        conf.ConfigureDefaults ();
    conf.ConfigureAttributes ();
    return conf;
}

/**
 * @brief ProSeHelper::CreateNodesAndHelpers  Create eNbs and EPC objects; 
 * create equal number of cellular and d2d users
 * @param enbNum
 * @param ueNum
 * @TODO Split cellular ueNum from D2D ueNum
 */
void
ProSeHelper::CreateNodesAndHelpers (uint8_t enbNum, uint8_t ueNum, double dUesRatio, bool useEmulation)
{
    NS_LOG_DEBUG ("Creating nodes and helper objects");
    macroEnbNodes.Create (enbNum * 3);
   
    dUeNum = ueNum * dUesRatio;
    cUeNum = ueNum * (1 - dUesRatio);
    ueD2dNodes.Create (dUeNum);
    ueCellNodes.Create (cUeNum);

    lteH = CreateObject<LteHelper>();
    lteH->SetAttribute ("PathlossModel", StringValue ("ns3::NakagamiPropagationLossModel"));

    lteH->SetSpectrumChannelType ("ns3::MultiModelSpectrumChannel");
    lteH->SetEnbDeviceAttribute ("DlEarfcn", UintegerValue (macroEnbDlEarfcn));
    lteH->SetEnbDeviceAttribute ("UlEarfcn", UintegerValue (macroEnbDlEarfcn + 18000));
    lteH->SetEnbDeviceAttribute ("DlBandwidth", UintegerValue (macroEnbBandwidth));
    lteH->SetEnbDeviceAttribute ("UlBandwidth", UintegerValue (macroEnbBandwidth));

    epcH = CreateObject<PointToPointEpcHelper> ();
    lteH->SetEpcHelper (epcH);
    pgwNode = epcH->GetPgwNode ();
    if (ns3::lteUseIpv6)
        m_pgwIpv6 = GetIpv6Address (pgwNode);
    else
        m_pgwIp = GetIpv4Address (pgwNode);

    this->SetupProSeFcn ();
    NS_LOG_INFO ("\n\tCarrier Frequency: " << LteSpectrumValueHelper::GetCarrierFrequency (macroEnbDlEarfcn)
                 << "Hz\n\tUL/DL Bandwidth: " << LteSpectrumValueHelper::GetChannelBandwidth (macroEnbBandwidth) << "Hz");
}

/**
 * @brief ProSeHelper::InstallUes Randomly dispose Ues in the Region Of Interest (ROI)
 * Set cellular users to voice communication mode ?
 *
 * 3GPP Requirements:
 * All Ues set OUTDOOR
 * Minimum 3m of separation between Ues. Set to 10m here
 * Minimum 35m of separation between Ues and eNbs
 * @param mobmodel: mobility model
 * **** WATCH OUT: the RandomVariable used greatly impacts on the distribution of the nodes
 */
void
ProSeHelper::InstallUes (std::string mobmodel)
{
    std::stringstream ss;
    switch (m_nodesDistribution)
    {
        case RANDOM:
        {
            //@todo Min (35m) should be calculated from eNB's position
            Ptr<NormalRandomVariable> rv = CreateObject<NormalRandomVariable> ();
            ueMobility.SetPositionAllocator ("ns3::RandomDiscPositionAllocator",
                                             "X", DoubleValue (rv->GetValue (35, this->cellRadius)),
                                             "Y", DoubleValue (rv->GetValue (0, this->cellRadius))
                                             );
            break;
        }

        case UNIFORM:
        default:
        {
            // Set initial position around the center of the Cell - TEMP: reduce the range between nodes
            Ptr<UniformRandomVariable> rv = CreateObject<UniformRandomVariable> ();
            ueMobility.SetPositionAllocator ("ns3::UniformDiscPositionAllocator",
                                             "X", DoubleValue (rv->GetValue (- this->cellRadius / 3, this->cellRadius / 3)),
                                             "Y", DoubleValue (rv->GetValue (- this->cellRadius / 3, this->cellRadius / 3)),
                                             "rho", DoubleValue (this->cellRadius)
                                             );
        }
    }

    if (mobmodel.compare ("ns3::RandomWalk2dMobilityModel") == 0)
    {
        // Walk around within a 500m diameter
        double val = this->cellRadius + 100;
        ss << "-" << val << "|" << val << "|-" << val << "|" << val;
        NS_LOG_DEBUG (ss.str ());
        ueMobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                                     "Mode", StringValue ("Time"),
                                     "Time", StringValue ("1s"),
                                     "Speed", StringValue ("ns3::ConstantRandomVariable[Constant=3.0]")
                                     , "Bounds", StringValue (ss.str ())
                                     );
    }
    else
        ueMobility.SetMobilityModel (mobmodel);

    ueMobility.Install (ueD2dNodes);
    ueMobility.Install (ueCellNodes);
    //*****
    ueD2dDevs = lteH->InstallUeDevice (ueD2dNodes); //lteH->InstallD2dUeDevice(ueD2dNodes);
    ueCellDevs = lteH->InstallUeDevice (ueCellNodes);
}

/**
 * Default basic config.
 * currently unused
 */
void
ProSeHelper::InstallEnbs ()
{
    useHexGrid = false;
    MobilityHelper mobility;
    mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
    mobility.SetPositionAllocator ("ns3::UniformDiscPositionAllocator",
                                   "X", DoubleValue (0.0),
                                   "Y", DoubleValue (0.0),
                                   "rho", DoubleValue (250.0));

    mobility.Install (macroEnbNodes);
    BuildingsHelper::Install (macroEnbNodes);
    macroEnbDevs = lteH->InstallEnbDevice (macroEnbNodes);
}

/**
 * As per TR 36843 A.2.1.1
 * Macro eNBs in 3-sector sites layed out on an hexagonal grid.
 * Ref: lena-dual-stripe
 * Use a Uniform distribution to have the best randomness
 */
void
ProSeHelper::InstallEpcRanInHexGrid (double cellRadius)
{
    NS_ASSERT_MSG ((lteH != 0) && (epcH != 0), "The LteHelper and PointToPointEpcHelper should be initialized first");

    this->cellRadius = cellRadius;
    double macroEnbTxPowerDbm = 43.0;
    double interSiteDistance = 500.0;
    // Min number of sites along the X-axis of the hex grid
    uint8_t nMacroEnbSitesX = 1;

    Ptr<ListPositionAllocator> enbPosAlloc = CreateObject<ListPositionAllocator> ();
    enbPosAlloc->Add (Vector (0.0, 0.0, 0.0)); // Macro eNB
    enbPosAlloc->Add (Vector (50.0, 0, 0.0)); // PGW
    enbPosAlloc->Add (Vector (60.0, 0, 0.0)); // ProSeFcn

    MobilityHelper mobility;
    mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
    mobility.SetPositionAllocator (enbPosAlloc);

    // 1 Macro eNB for 3-sectors (cells) hex grid
    mobility.Install (macroEnbNodes);
    mobility.Install (pgwNode);
    mobility.Install (m_proseFcnNode);

    hexGridH = CreateObject<LteHexGridEnbTopologyHelper> ();
    hexGridH->SetLteHelper (lteH);
    hexGridH->SetAttribute ("InterSiteDistance", DoubleValue (interSiteDistance));
    //***** Center eNBs at 0,0
    hexGridH->SetAttribute ("MinX", DoubleValue (0));
    hexGridH->SetAttribute ("GridWidth", UintegerValue (nMacroEnbSitesX));

    Config::SetDefault ("ns3::LteEnbPhy::TxPower", DoubleValue (macroEnbTxPowerDbm));

    // For 3 sectors site see https://www.nsnam.org/docs/models/html/lte-user.html#use-of-antennamodel
    // and lena-rem-sector-antenna.cc
    // 3 eNBs with different orientation each, all nearly at the same position
    lteH->SetEnbAntennaModelType ("ns3::CosineAntennaModel");
    lteH->SetEnbAntennaModelAttribute ("Beamwidth", DoubleValue (100));
    lteH->SetEnbAntennaModelAttribute ("MaxGain", DoubleValue (0.0));
    
    // The HexGridHelper will set Positions and Orientations for each eNB sector
    macroEnbDevs = hexGridH->SetPositionAndInstallEnbDevice (macroEnbNodes);

    if (epcH)
    {
        // Enable handover
        lteH->AddX2Interface (macroEnbNodes);
        // lteH->AddX2Interface(homeEnbNodes);
    }
}

/**
 * Set default IP network configuration for D2dUes
 * Routed through EPC's PGW
 */
void
ProSeHelper::SetupIpNetwork ()
{
    NS_LOG_DEBUG ("\tConfiguring default IP networking...");
    NS_ASSERT_MSG (epcH != 0, "Error. Make sure the EPC object is initialized first.");

    inet.Install (ueD2dNodes);
    inet.Install (ueCellNodes);

    for (NetDeviceContainer::Iterator it = ueD2dDevs.Begin (); it != ueD2dDevs.End (); ++ it)
        (*it)->SetAddress (Mac48Address::Allocate ());

    for (NetDeviceContainer::Iterator it = ueCellDevs.Begin (); it != ueCellDevs.End (); ++ it)
        (*it)->SetAddress (Mac48Address::Allocate ());

    if (! ns3::lteUseIpv6)
    {
        ueIpInterface = epcH->AssignUeIpv4Address (NetDeviceContainer (ueD2dDevs));
        epcH->AssignUeIpv4Address (NetDeviceContainer (ueCellDevs));
        TrafficControlHelper tcHelper = TrafficControlHelper::Default ();
        //tcHelper.Uninstall (ueD2dDevs);
        
        inet.EnableAsciiIpv4All ("LteIpv4"); //, ueD2dNodes);
        return;
    }

    //**** IPv6 attempt
    Ipv6InterfaceContainer iface;
    if (m_useEmulation)
    {
        iface = emuEpcH->AssignUeIpv6Address (NetDeviceContainer (ueD2dDevs));
        emuEpcH->AssignUeIpv6Address (NetDeviceContainer (ueCellDevs));
    }
    else
    {
        iface = epcH->AssignUeIpv6Address (NetDeviceContainer (ueD2dDevs));
        epcH->AssignUeIpv6Address (NetDeviceContainer (ueCellDevs));
    }
    iface.SetDefaultRouteInAllNodes (m_pgwIpv6);
    inet.EnablePcapIpv6 ("ProseIpv6", ueD2dNodes);
}

/**
 * @brief ProSeHelper::InitDiscovery. Initialize discovery announce/monitor 
 * request procedures - TS24.334 6.2.2
 *
 * dUe <-> PGW <-> ProseFcn.
 * EPC-level discovery
 */
void ProSeHelper::InitDiscovery (NetDeviceContainer devs)
{
    NS_LOG_INFO ("\tInitializing discovery procedures...");
    if (devs.GetN () < 2)
        devs = ueD2dDevs;

    Ptr<Node> n;
    Ptr<LteUeNetDevice> ndev;

    uint16_t activeUeNum = 0;
    Ipv4Address ueIp;
    Ipv4Address srvIp = ueIp = Ipv4Address::GetAny ();
    Ipv6Address ueIp6;
    Ipv6Address srvIp6 = ueIp6 = Ipv6Address::GetAny ();
    Ipv6StaticRoutingHelper routingH6;
    Ptr<Ipv6StaticRouting> ueStaticRouting6;

    if (ns3::lteUseIpv6)
        srvIp6 = GetIpv6Address (m_proseFcnNode);
    else
        srvIp = GetIpv4Address (m_proseFcnNode);

    void (ProSeApp::*fp)(ProSeApp::Command cmd) = & ProSeApp::RequestDiscovery;

    for (uint16_t i = 0; i < dUeNum; i ++)
    {
        n = devs.Get (i)->GetNode ();
        ndev = devs.Get (i)->GetObject<LteUeNetDevice> ();
        this->apps.push_back (CreateObject<ProSeApp>());

        if (ns3::lteUseIpv6)
        {
            ueIp6 = GetIpv6Address (n);
            apps[i]->SetIp (ueIp6);
        }
        else
        {
            ueIp = GetIpv4Address (n);
            apps[i]->SetIp (ueIp);
        }
        NS_LOG_DEBUG ("\tProvisioning discovery for dUe" << (int) i
                      << " IP: " << ueIp << " " << ueIp6
                      << " <-> Srv IP: " << srvIp << " " << srvIp6);

        apps[i]->SetEpUID (ndev->GetImsi ());
        apps[i]->SetStopTime (Seconds (trafficDuration));
        apps[i]->SetUeNetDevice (ndev);
        apps[i]->SetupApp (n, srvIp6, srvIp, m_proseFcnApp->GetPort ());
        // This should be done after requesting discovery from ProSeFcn
        // apps[i]->SetAppCode("00000", proseFcn->GenerateAppTempID());

        if (ns3::lteUseIpv6)
        {
            ueStaticRouting6 = routingH6.GetStaticRouting (GetIpv6 (n));
            ueStaticRouting6->AddHostRouteTo (srvIp6, 1);
        }
        else
        {
            ueStaticRouting = ueStaticRoutingH.GetStaticRouting (GetIpv4 (n));
            ueStaticRouting->AddHostRouteTo (srvIp, 1);
        }
        // For ProSeFcn's calls to StartWlanMulticast
        n->AggregateObject (apps[i]);
        // For requests through PC3 interface
        n->AddApplication (apps[i]->GetHttpClient ());

        /* Randomly set dUes in Announcing or Monitoring mode
         * Schedule to run after HttpClient::StartApplication is called
         * + i adds some delay to each UE -> ProSeFcn request to allow the 
         * later to process multiple requests, as the HttpServer is not multi-threaded
         */
        if (((i % 2) == 0) && (activeUeNum < 25))
        {
            activeUeNum ++;
            Simulator::Schedule (MilliSeconds (10), fp, apps[i], ProSeApp::ANNOUNCE);
        }
        else
            Simulator::Schedule (MilliSeconds (10), fp, apps[i], ProSeApp::MONITOR);
    }

}

//***** ProSe Function Entity

void
ProSeHelper::SetupProSeFcn ()
{
    NS_LOG_FUNCTION (this);
    m_proseFcnNode = CreateObject<Node> ();
    inet.Install (m_proseFcnNode);
    NetDeviceContainer devContainer;
    Mac48Address proseFcnMacAddress = Mac48Address::Allocate ();

    //**** Interconnect PGW and ProSeFcn to RX requests from ProSeApp installed on UEs
    if (m_useEmulation)
    {
        std::string proseFcnDeviceName = "veth2";
        EmuFdNetDeviceHelper emu;
        NS_LOG_LOGIC ("ProseFcn device: " << proseFcnDeviceName);
        emu.SetDeviceName (proseFcnDeviceName);

        devContainer = emu.Install (m_proseFcnNode);
        Ptr<NetDevice> dev = devContainer.Get (0);
        NS_LOG_LOGIC ("MAC address of ProseFcn: " << proseFcnMacAddress);
        dev->SetAttribute ("Address", Mac48AddressValue (proseFcnMacAddress));
        emu.EnablePcap ("EmuProseFcn", dev);

    }
    else
    {
        PointToPointHelper p2ph;
        p2ph.SetDeviceAttribute ("DataRate", DataRateValue (DataRate ("100Mb/s")));
        // p2ph.SetDeviceAttribute ("Mtu", UintegerValue (1500));
        p2ph.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (1)));
        // Install will Allocate MACs
        devContainer = p2ph.Install (pgwNode, m_proseFcnNode);
    }
    // Interface 0 PGW <> 1 Fcn from the order of p2p/emu .Install

    m_proseFcnApp = CreateObject<EpcProSeFcnApp>();
    m_proseFcnNode->AddApplication (m_proseFcnApp);
    Ipv4Address ipAddr = Ipv4Address::GetZero ();
    Ipv6Address ipAddr6 = Ipv6Address::GetZero ();
    if (ns3::lteUseIpv6)
    {
        //m_proseAddrHv6.SetBase (Ipv6Address ("8001:800::"), Ipv6Prefix (64));
        m_proseAddrHv6.NewNetwork ();
        Ipv6InterfaceContainer iface = m_proseAddrHv6.AssignWithoutAddress (devContainer);
        // if (! m_useEmulation)
        iface.SetForwarding (0, true); // PGW is router
        iface.SetDefaultRoute (0, 1);
        inet.EnablePcapIpv6 ("ProseFcn", iface);

        // Explicit call to Ipv6StaticRouting seems unnecessary
        ipAddr6 = GetIpv6Address (m_proseFcnNode);
        if (ipAddr6 == Ipv6Address::GetZero ())
        {
            NS_LOG_WARN ("\tProseFcn has no IPv6 assigned");
        }

        // Ipv6StaticRoutingHelper routingHelper;
        // Ptr<Ipv6StaticRouting> routing = routingHelper.GetStaticRouting (GetIpv6 (m_proseFcnNode));
        // routing->AddHostRouteTo (Ipv6Address ("7001:700::"), 1);
    }
    else
    {
        m_proseAddrH.SetBase ("8.8.8.0", "255.255.255.252");
        Ipv4InterfaceContainer iface4 = m_proseAddrH.Assign (devContainer);
        inet.EnablePcapIpv4 ("ProseFcn", iface4);

        Ptr<Ipv4> ip = GetIpv4 (m_proseFcnNode);
        Ptr<Ipv4StaticRouting> proseRouting = ueStaticRoutingH.GetStaticRouting (ip);
        proseRouting->AddNetworkRouteTo (Ipv4Address ("7.0.0.0"), Ipv4Mask (8), 1);
        proseRouting->SetDefaultRoute (iface4.GetAddress (1, 0), 1, 1500);

        ipAddr = GetIpv4Address (m_proseFcnNode);
        
        TrafficControlHelper tcHelper = TrafficControlHelper::Default ();
        //tcHelper.Uninstall (devContainer);
    }

    m_proseFcnApp->SetupSrv (m_proseFcnNode, ipAddr, ipAddr6);
    NS_LOG_LOGIC ("ProseFcn IP " << ipAddr << " " << ipAddr6);
}


// UNUSED NOW
void ProSeHelper::DeActivateBearer (Ptr<LteUeNetDevice> netDev)
{
    Ptr<NetDevice> ueDevice = netDev->GetObject<NetDevice> ();
    Ptr<LteEnbNetDevice> enbDev;
    u_int16_t cellId = netDev->GetRrc ()->GetCellId ();
    for (u_int16_t i = 0; i < macroEnbDevs.GetN (); i ++)
    {
        enbDev = macroEnbDevs.Get (i)->GetObject<LteEnbNetDevice> ();
        if (enbDev->GetCellId () == cellId)
        {
            uint64_t imsi = netDev->GetImsi ();
            uint16_t rnti = netDev->GetRrc ()->GetRnti ();

            Ptr<LteEnbRrc> enbRrc = enbDev->GetRrc ();
            enbRrc->DoSendReleaseDataRadioBearer (imsi, rnti, 1);
        }
    }

}

/************
// This allows to send data from ProSeApp to ProSeFcn without generating
// traffic directly from the LteUe's main interface

        ueStaticRouting = ipv4RoutingH.GetStaticRouting(GetIpv4(appNode));
        ueStaticRouting->AddHostRouteTo(srvIp, 1);
        n->AddApplication(apps[i]);

 */

} // namespace ns3
