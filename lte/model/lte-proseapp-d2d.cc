/*
 * Copyright (c) 2014-2016 Gyeongsang N. University
 * InfoCom Engineering Dept, South Korea
 * $Id: lte-proseapp-d2d.cc, 2016-05-31 16:40 +0900 makhtar $
 * Advisor: Woonsup Lee 
 * Author: Makhtar Diouf 
 * 
 * *** Proximity-based Services D2D Communication over LTE ***
 *
 * This program is open source software released under the terms of the
 * GNU General Public License version 2 as published by the Free Software Foundation.
 * It is distributed WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <ns3/pointer.h>
#include <ns3/ptr.h>
#include <ns3/ipv4-header.h>
#include <ns3/ipv4-address-helper.h>
#include <ns3/on-off-helper.h>
#include <ns3/udp-client-server-helper.h>
#include <ns3/bulk-send-helper.h>
#include <ns3/packet-sink-helper.h>
#include <ns3/packet-burst.h>
#include <ns3/tcp-header.h>
#include <ns3/object-factory.h>
#include <unistd.h>
#include <iostream>

#include "lte-proseapp.h"
#include "lte-utils.h"
#include "lte-common.h"
#include "lte-ue-mac.h"
#include "lte-ue-rrc.h"
#include "lte-ue-net-device.h"
//#include "lte-control-messages.h"
#include "epc-ue-nas.h"
//#include "lte-spectrum-signal-parameters.h"
//#include "ns3/propagation-loss-model.h"
#include "ns3/spectrum-channel.h"
#include "ns3/spectrum-propagation-loss-model.h"
#include "ns3/multi-model-spectrum-channel.h"
#include "lte-spectrum-phy.h"
#include "lte-ue-phy.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("ProSeAppLteD2d");

void ProSeApp::Dettach (Ptr<LteUeNetDevice> ueLteDevice)
{
    NS_LOG_FUNCTION ("\n" << this);
    uint64_t imsi = ueLteDevice->GetImsi ();
    uint16_t cellId = ueLteDevice->GetRrc ()->GetCellId ();
    NS_LOG_INFO ("IMSI " << imsi << " - cellId " << cellId);
    // Test disconnect from EPC / eNB
    ueLteDevice->GetNas ()->Disconnect ();
    ueLteDevice->GetNas ()->SetD2dMode (true);
    //  ueLteDevice->GetRrc ()->SetD2dMode (true);
    //  ueLteDevice->GetMac ()->SetD2dMode (true);
    // ueLteDevice->GetPhy ()->GetDlSpectrumPhy ()->SetD2dMode (true);
    // ueLteDevice->m_inD2dMode = true;
}

/**
 * Set Adhoc multicast IP block for LTE-D2D attempt
 * Check the Tx position from the UE for distance calculation
 * Set direct link Tx-Rx, Schedule communication
 * See TS.24.334 $10.4.2
 * @TODO Initiating UE should send DIRECT_COMMUNICATION_REQUEST message to target UE first for 1:1 com
 * 
 */
void ProSeApp::StartLteD2d (std::list<uint64_t> lteMatches, bool wifiInUse)
{
    NS_LOG_FUNCTION (this);
    uint8_t i, nDevs = 0;
    Ptr<Node> n;
    Ptr<ProSeApp> app;
    Ptr<LteUeNetDevice> udev;
    std::stringstream ss;
    ss << "239.9.9." << this->epUID;
    // Multi-cast is UNUSED for now
    this->m_params.multicastGroupIp = Ipv4Address (ss.str ().c_str ());
    this->m_params.lteComEnd = m_comDuration;

    double dist = 0.0;
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

            /*** For now limit to 1 RX UE instead of iterating over all matches **/
            imsiIt = lteMatches.begin ();
            // for (imsiIt; imsiIt != lteMatches.end (); ++ imsiIt) {             
            if (*imsiIt != udev->GetImsi ())
                continue;

            app = n->GetObject<ProSeApp>();
            SetD2dLink (n, this->m_params);

            peerPos = n->GetObject<MobilityModel>()->GetPosition ();
            dist = CalculateDistance (srcPos, peerPos);
            this->m_params.dist = dist;

            // Invite peer to join the communication
            Simulator::ScheduleNow (&ProSeApp::JoinLteD2d, app, this->m_params, wifiInUse);
            i ++;
            break; // Go to next Node
        }
        // Stop processing nodes
        if (i >= lteMatches.size ())
            break;
    }

    // Delay start to wait for peer to be ready
    Simulator::Schedule (Seconds (m_comStartDelay), &ProSeApp::StartLteD2dTx, this, wifiInUse);
}

void ProSeApp::StartLteD2dTx (bool wifiInUse)
{
    Address addr;
    if (ns3::lteUseIpv6)
        addr = Address (Inet6SocketAddress (m_peerIp6, m_params.multicastPort));
    else
        addr = Address (InetSocketAddress (m_peerIp, m_params.multicastPort));

    NS_LOG_DEBUG ("\nHead D2D IMSI" << this->epUID << " " << m_params.multicastSourceIp <<
                  " Tx to LTE peer " << m_peerIp);

    // Use TCP for ACKing the Node's reachability 
    OnOffHelper tx ("ns3::TcpSocketFactory", addr);
    ApplicationContainer txC = tx.Install (m_proseAppNode);
    Ptr<OnOffApplication> txApp = DynamicCast<OnOffApplication>(txC.Get (0));
  //  txApp->SetAttribute ("OnTime", StringValue ("ns3::UniformRandomVariable[Min=1000.0|Max=2000.0]"));
    // txApp->SetAttribute ("OffTime", StringValue ("ns3::UniformRandomVariable[Min=2.0|Max=3.0]"));
    txApp->SetAttribute ("DataRate", DataRateValue (m_comDataRate));
    txApp->SetAttribute ("PacketSize", UintegerValue (m_packetSize));

    if (wifiInUse)
    {
        // Start after wifi has finished to avoid packets corruption
       this->m_params.lteComStart += 1; // m_comDuration;
       this->m_params.lteComEnd += 1; // (2 * m_comDuration) + 0.01;
    }

    txC.Start (Seconds (this->m_params.lteComStart));
    txC.Stop (Seconds (this->m_params.lteComEnd));
}

void ProSeApp::JoinLteD2d (ProSe::DirectComParams params, bool wifiInUse)
{
    NS_LOG_FUNCTION ("\t" << this->m_clientIp << " " << params.peerIp << " IMSI" << this->epUID);
    Address addr;
    this->m_lteRxType = "LTE-D";
    // Both Tx and Rx UEs shall have the same copy
    this->m_params = params;

    if (ns3::lteUseIpv6)
        addr = Inet6SocketAddress (GetIpv6Address (m_proseAppNode), params.multicastPort); // @todo set to 2nd Ipv6
    else
        addr = InetSocketAddress (params.peerIp, params.multicastPort);

    static PacketSinkHelper rx = PacketSinkHelper ("ns3::TcpSocketFactory", addr);
    ApplicationContainer rxC = rx.Install (m_proseAppNode);
    
        if (wifiInUse)
    {
        // Start after wifi to avoid packets corruption
        this->m_params.lteComStart += 1; // m_comDuration;
        this->m_params.lteComEnd += 1; // (2 * m_comDuration) + 0.01;
    }
    // Receiver shall wait for delayed packets
    rxC.Start (Seconds (this->m_params.lteComStart - 0.25));
    rxC.Stop (Seconds (this->m_params.lteComEnd + 0.25));

    m_peerPacketSink = DynamicCast<PacketSink>(rxC.Get (0));
    if (m_peerPacketSink == 0)
    {
        NS_LOG_WARN ("\nJoinLteD2d could not get a PacketSink");
        return;
    }

    // For calculating throughput
   // this->m_params.lteComStart = Simulator::Now ().GetSeconds ();

    m_lteD2dStatsProbEvent = Simulator::Schedule (Seconds (this->m_params.lteComStart + 1.0), &ProSeApp::ProbeLteDataStats, this, true);
    Simulator::Schedule (Seconds (this->m_params.lteComEnd + 0.5), &ProSeApp::AtComEnd, this, true);
}

/**
 * @brief LTE data statistics
 * Calculate throughput in Mega BITs per sec, Run again every sec
 * @todo Can Use lteH->GetPdcpStats ()->...
 */
void ProSeApp::ProbeLteDataStats (bool isD2d)
{
    NS_LOG_FUNCTION ("\t" << this);
    volatile double time = Simulator::Now().GetSeconds () - this->m_params.lteComStart;
    volatile double throughput = 0;

    // Difference since the previous probe (1 sec)
    throughput = ((m_peerPacketSink->GetTotalRx () * 8.0) / (1000000 * time));
    
    //@todo update distance between moving nodes
    // this->m_params.dist = CalculateDistance (m_proseAppNode->GetObject<MobilityModel>()->GetPosition (),
    // m_peerNode->GetObject<MobilityModel>()->GetPosition ());

    stringstream ss;
    ss.width (6);
    ss << time << "\t" << this->epUID
            << " \t" << this->m_params.dist
            << " \t" << throughput;

    SaveLteDataStats (ss.str ());
    m_lteD2dStatsProbEvent = Simulator::Schedule (Seconds (1.0), &ProSeApp::ProbeLteDataStats, this, isD2d);
    NS_LOG_DEBUG (m_lteRxType << " " << throughput << " MBit/s" << " @ IMSI" << this->epUID);
}

/**
 * Tx/Rx Switch at Phy Layer as suggested by TS24.334 $10.4.2
 * Set a separate NetDevice/local IP for LTE-D2D
 * @param peerNode
 */
void ProSeApp::SetD2dLink (Ptr<Node> peerNode, ProSe::DirectComParams &params)
{
    NS_LOG_FUNCTION ("\n" << this);
    Ptr<LteUeNetDevice> peerNetDev = peerNode->GetObject<ProSeApp>()->GetUeNetDevice ();

    //   Dettach (m_ueNetDevice);
    //   Dettach (peerNetDev);
    Ptr<LteUePhy> phyA = m_ueNetDevice->GetPhy ();
    Ptr<LteUePhy> phyB = peerNetDev->GetPhy ();
    Ptr<EpcUeNas> nasA = m_ueNetDevice->GetNas ();
    Ptr<EpcUeNas> nasB = peerNetDev->GetNas ();

    nasA->m_d2dPeer.phy = phyB;
    nasA->m_d2dPeer.mac = m_ueNetDevice->GetMac ();
    nasA->m_d2dPeer.params.rnti = m_ueNetDevice->GetRrc ()->GetRnti ();
    nasA->m_d2dPeer.params.lcid = 3;
    nasA->m_d2dPeer.params.layer = 2;
   
    nasA->m_d2dPeer.nodeId = peerNode->GetId ();
    nasA->m_d2dPeer.nas = nasB; // Tx -> Rx
    nasA->m_inD2dMode = true;
    m_ueNetDevice->GetRrc ()->m_inD2dMode = true;

    nasB->m_d2dPeer.phy = phyA;
    nasB->m_d2dPeer.mac = peerNetDev->GetMac ();
    nasB->m_d2dPeer.params.rnti = peerNetDev->GetRrc ()->GetRnti ();
    nasB->m_d2dPeer.params.lcid = 3;
    nasB->m_d2dPeer.params.layer = 2;
  
    nasB->m_d2dPeer.nodeId = m_proseAppNode->GetId ();
    nasB->m_d2dPeer.nas = nasA; // Rx -> Tx       
    nasB->m_inD2dMode = true;
    peerNetDev->GetRrc ()->m_inD2dMode = true;

    // De-activate D2D at the end of communication - should move it to AtComEnd()
    Simulator::Schedule (Seconds (this->m_params.lteComEnd + 0.5), &EpcUeNas::SetD2dMode, nasA, false);
    Simulator::Schedule (Seconds (this->m_params.lteComEnd + 0.5), &EpcUeNas::SetD2dMode, nasB, false);

    NetDeviceContainer netC;
    netC.Add (m_ueNetDevice);
    netC.Add (peerNetDev);
    params.multicastSourceIp = this->SetupNetwork (netC, false);
    params.peerIp = this->m_peerIp;
    this->m_params = params;

    m_inet.EnablePcapIpv4 ("Lte-Dhead", NodeContainer (m_proseAppNode));
    m_inet.EnablePcapIpv4 ("Lte-D", NodeContainer (peerNode));
}

/**
 * Cleanup at the end of communication
 * @param isD2d
 */
void ProSeApp::AtComEnd (bool isD2d)
{
    if (isD2d && m_lteD2dStatsProbEvent.IsRunning ())
        m_lteD2dStatsProbEvent.Cancel ();
    else if (! isD2d && m_lteStatsProbEvent.IsRunning ())
        m_lteStatsProbEvent.Cancel ();
}

/**
 * See LteHelper::DoInitialize for adding Fading, PathLoss
 */
void ProSeApp::SetD2dSideLinkChannel (Ptr<LteUeNetDevice> peerNetDev)
{
    NS_LOG_FUNCTION ("\n" << this);

    ObjectFactory m_channelFactory;
    m_channelFactory.SetTypeId (MultiModelSpectrumChannel::GetTypeId ());

    Ptr<SpectrumChannel> d2dUl = CreateObject<MultiModelSpectrumChannel>(); // m_channelFactory.Create<SpectrumChannel> ();
    Ptr<SpectrumChannel> d2dDl = CreateObject<MultiModelSpectrumChannel>();

    ObjectFactory pathlossModelFactory = ObjectFactory ();
    pathlossModelFactory.SetTypeId ("ns3::FriisSpectrumPropagationLossModel");
    Ptr<Object> m_uplinkPathlossModel = pathlossModelFactory.Create ();
    Ptr<SpectrumPropagationLossModel> ulPlm = m_uplinkPathlossModel->GetObject<SpectrumPropagationLossModel> ();
    d2dUl->AddSpectrumPropagationLossModel (ulPlm);
    //  d2dUl->SetPropagationDelayModel (CreateObject<ConstantSpeedPropagationDelayModel> ());

    Ptr<Object> m_downlinkPathlossModel = pathlossModelFactory.Create ();
    Ptr<SpectrumPropagationLossModel> dlPlm = m_downlinkPathlossModel->GetObject<SpectrumPropagationLossModel> ();
    d2dDl->AddSpectrumPropagationLossModel (dlPlm);
    //  d2dDl->SetPropagationDelayModel (CreateObject<ConstantSpeedPropagationDelayModel> ());


    /** Connect the Rx peer DL  to Tx UE's UL channel
     * See SpectrumChannel::AddRx
     * attach a SpectrumPhy instance to a SpectrumChannel instance, 
     * so that the SpectrumPhy can receive packets sent on that channel.
     * 
     * Just like eNB Peer DL SpectrumChannel will call StartRx
     * LteHelper::InstallSingleEnbDevice
     * It will remove a previous entry of this phy (the eNB one) if it exists
     * 
     * TODO: workaround FDD channel access, the physical layer for transmission cannot be used for reception
     * lte-spectrum-phy.cc, line=679
     * todo: INTERCONNECT each UE UL/DL to its peer DL/UL
     */

    Ptr<LteSpectrumPhy> ulPhyA = m_ueNetDevice->GetPhy ()->GetUlSpectrumPhy ();
    Ptr<LteSpectrumPhy> dlPhyA = m_ueNetDevice->GetPhy ()->GetDlSpectrumPhy ();
    Ptr<const SpectrumModel> sm = dlPhyA->GetRxSpectrumModel ();
    Ptr<const SpectrumValue> noisePsd = new SpectrumValue (sm);
    ulPhyA->SetNoisePowerSpectralDensity (noisePsd);
    ulPhyA->SetChannel (d2dUl);
    dlPhyA->SetChannel (d2dDl);
    //    d2dDl->AddRx (dlPhyA);

    Ptr<LteSpectrumPhy> ulPhyB = peerNetDev->GetPhy ()->GetDlSpectrumPhy ();
    Ptr<LteSpectrumPhy> dlPhyB = peerNetDev->GetPhy ()->GetDlSpectrumPhy ();

    sm = dlPhyB->GetRxSpectrumModel ();
    noisePsd = new SpectrumValue (sm);
    //dlPhyB->SetState (LteSpectrumPhy::IDLE);
    ulPhyB->SetNoisePowerSpectralDensity (noisePsd);
    ulPhyB->SetChannel (d2dUl);
    dlPhyB->SetChannel (d2dDl);

    // ulPhyB takes the place of the eNB phy
    d2dUl->AddRx (ulPhyB);
    d2dUl->AddRx (ulPhyA);
    d2dDl->AddRx (dlPhyB);
    d2dDl->AddRx (dlPhyA);

    // @todo SetChannel back of ul/dlPhy to initial state @ AtComEnd()
    // ...
}
} // End namespace ns3
