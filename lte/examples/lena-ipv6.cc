/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Jaume Nin <jaume.nin@cttc.cat>
 *
 * IPv6 version of lena-simple-epc.cc - Makhtar Diouf 
 * ./waf --run lena-ipv6 &> lena-ipv6.log
 * View log file or lena-ipv6*.pcap with wireshark
 *
 * $Id: lena-ipv6.cc, 2016-02-23 21:40 +0900 makhtar $
 */

#include "ns3/lte-helper.h"
#include "ns3/epc-helper.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include <ns3/inet6-socket-address.h>
#include <ns3/ipv6-address.h>
#include <ns3/ipv6-address-helper.h>
#include <ns3/ipv6-routing-helper.h>
#include <ns3/radvd-helper.h>
#include <ns3/ping6-helper.h>

#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/lte-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/config-store.h"

using namespace ns3;

/**
 * Sample simulation script for LTE+EPC. It instantiates several eNodeB,
 * attaches one UE per eNodeB starts a flow for each UE to  and from a remote host.
 * It also  starts yet another flow between each UE pair.
 */

NS_LOG_COMPONENT_DEFINE ("EpcIpv6Example");

int
main (int argc, char *argv[])
{
  // Trigger IPv6
  ns3::lteUseIpv6 = true;
  bool verbose = true;

  uint16_t numberOfNodes = 2;
  double simTime = 4.0;
  double distance = 60.0;
  double interPacketInterval = 100;

  // Command line arguments
  CommandLine cmd;
  cmd.AddValue ("numberOfNodes", "Number of eNodeBs + UE pairs", numberOfNodes);
  cmd.AddValue ("simTime", "Total duration of the simulation [s])", simTime);
  cmd.AddValue ("distance", "Distance between eNBs [m]", distance);
  cmd.AddValue ("interPacketInterval", "Inter packet interval [ms])", interPacketInterval);
  cmd.Parse (argc, argv);

  if (verbose)
  {
    LogLevel logAll = (LogLevel) (LOG_PREFIX_FUNC | LOG_PREFIX_TIME | LOG_LEVEL_ALL);
    LogComponentEnable ("PacketSink", logAll);
    LogComponentEnable ("UdpClient", logAll);

    LogComponentEnable ("PointToPointEpcHelper", logAll);
    LogComponentEnable ("EpcSgwPgwApplication", logAll);
    LogComponentEnable ("EpcEnbApplication", logAll);
    LogComponentEnable ("EpcUeNas", logAll);
    LogComponentEnable ("LtePdcp", logAll);
    LogComponentEnable ("LteRlcAm", logAll);
    //  LogComponentEnable ("LteRlc", logAll);
    LogComponentEnable ("LteUeMac", logAll);
    LogComponentEnable ("Ipv6L3Protocol", logAll);
    LogComponentEnable ("Ipv6StaticRouting", logAll);
    LogComponentEnable ("Icmpv6L4Protocol", logAll);
    LogComponentEnable ("Ipv6Interface", logAll);
    //LogComponentEnable ("Ipv6RawSocketImpl", logAll);
  }

  Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();
  Ptr<PointToPointEpcHelper> epcHelper = CreateObject<PointToPointEpcHelper> ();
  lteHelper->SetEpcHelper (epcHelper);
  lteHelper->EnableTraces ();
  // lteHelper->EnableLogComponents ();
  
  Ptr<Node> pgw = epcHelper->GetPgwNode ();

  // Create a single RemoteHost
  NodeContainer remoteHostContainer;
  remoteHostContainer.Create (1);
  Ptr<Node> remoteHost = remoteHostContainer.Get (0);
  InternetStackHelper internet;
  internet.Install (remoteHostContainer);

  // Create the remote network
  PointToPointHelper p2ph;
  p2ph.SetDeviceAttribute ("DataRate", DataRateValue (DataRate ("10Mbps")));
  p2ph.SetDeviceAttribute ("Mtu", UintegerValue (1500));
  p2ph.SetChannelAttribute ("Delay", TimeValue (Seconds (0.010)));
  NetDeviceContainer internetDevices = p2ph.Install (pgw, remoteHost);
  //internetDevices.Get (1)->SetAddress (Mac48Address::Allocate ());
  p2ph.EnablePcap ("lenaipv6-rhost", internetDevices);

  Ipv6AddressHelper ipv6h;
 // ipv6h.SetBase (Ipv6Address ("2001:5::"), Ipv6Prefix (64));
 // ipv6h.NewNetwork ();
  Ipv6InterfaceContainer internetIpIfaces = ipv6h.AssignWithoutAddress (internetDevices);
  //internetIpIfaces.SetForwarding (0, true);
  //internetIpIfaces.SetDefaultRoute (0, 0);

  // interface 0 is localhost, 1 is the p2p device
  Ipv6Address remoteHostAddr = internetIpIfaces.GetAddress (1, 0);

  NodeContainer ueNodes;
  NodeContainer enbNodes;
  enbNodes.Create (numberOfNodes / 2);
  ueNodes.Create (numberOfNodes);

  // Install Mobility Model
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  for (uint16_t i = 0; i < numberOfNodes; i ++)
  {
    positionAlloc->Add (Vector (distance * i, 0, 0));
  }
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.SetPositionAllocator (positionAlloc);
  mobility.Install (enbNodes);
  mobility.Install (ueNodes);

  // Install LTE Devices to the nodes
  NetDeviceContainer enbLteDevs = lteHelper->InstallEnbDevice (enbNodes);
  NetDeviceContainer ueLteDevs = lteHelper->InstallUeDevice (ueNodes);

  // Install the IP stack on the UEs
  internet.Install (ueNodes);
  for (NetDeviceContainer::Iterator it = ueLteDevs.Begin (); it != ueLteDevs.End (); ++ it)
    (*it)->SetAddress (Mac48Address::Allocate ());

  Ipv6InterfaceContainer ueIpIface = epcHelper->AssignUeIpv6Address (NetDeviceContainer (ueLteDevs));
  internet.EnablePcapIpv6 ("lenaipv6", ueIpIface);

  // This sets Ipv6StaticRouting internally
  ueIpIface.SetDefaultRouteInAllNodes (epcHelper->GetUeDefaultGatewayAddress6 ());
 
//  RadvdHelper radvdHelper;
//  //radvdHelper.AddAnnouncedPrefix (internetIpIfaces.GetInterfaceIndex (1), Ipv6Address ("2001:5::0"), 64);
//  radvdHelper.AddAnnouncedPrefix (ueIpIface.GetInterfaceIndex (1), Ipv6Address ("2001:700::0"), 64);
//  radvdHelper.AddAnnouncedPrefix (internetIpIfaces.GetInterfaceIndex (1), Ipv6Address ("FF02::1"), 64);  
//  radvdHelper.AddAnnouncedPrefix (internetIpIfaces.GetInterfaceIndex (1), Ipv6Address ("FF02::2"), 64);
//  radvdHelper.EnableDefaultRouterForInterface(internetIpIfaces.GetInterfaceIndex (1));
//  ApplicationContainer radvdApps = radvdHelper.Install (pgw);
//  radvdApps.Start (Seconds (0.01));
//  radvdApps.Stop (Seconds (10.0));

  // Attach one UE per eNodeB
  //for (uint16_t i = 0; i < numberOfNodes; i ++)
  //{
    lteHelper->Attach (ueLteDevs); //.Get (i), enbLteDevs.Get (i));
    // side effect: the default EPS bearer will be activated
  //}

  // Install and start applications on UEs and remote host
  uint16_t dlPort = 1234;
  uint16_t ulPort = 2000;
  uint16_t otherPort = 3000;
  ApplicationContainer clientApps;
  ApplicationContainer serverApps;
  for (uint32_t u = 0; u < ueNodes.GetN (); ++ u)
  {
    ++ ulPort;
    ++ otherPort;
    PacketSinkHelper dlPacketSinkHelper ("ns3::UdpSocketFactory", Inet6SocketAddress (Ipv6Address::GetAny (), dlPort));
    PacketSinkHelper ulPacketSinkHelper ("ns3::UdpSocketFactory", Inet6SocketAddress (Ipv6Address::GetAny (), ulPort));
    PacketSinkHelper packetSinkHelper ("ns3::UdpSocketFactory", Inet6SocketAddress (Ipv6Address::GetAny (), otherPort));
    serverApps.Add (dlPacketSinkHelper.Install (ueNodes.Get (u)));
    serverApps.Add (ulPacketSinkHelper.Install (remoteHost));
    serverApps.Add (packetSinkHelper.Install (ueNodes.Get (u)));

    UdpClientHelper dlClient (ueIpIface.GetAddress (u, 0), dlPort);
    dlClient.SetAttribute ("Interval", TimeValue (MilliSeconds (interPacketInterval)));
    dlClient.SetAttribute ("MaxPackets", UintegerValue (1000000));

    UdpClientHelper ulClient (remoteHostAddr, ulPort);
    ulClient.SetAttribute ("Interval", TimeValue (MilliSeconds (interPacketInterval)));
    ulClient.SetAttribute ("MaxPackets", UintegerValue (1000000));

    UdpClientHelper client (ueIpIface.GetAddress (u, 0), otherPort);
    client.SetAttribute ("Interval", TimeValue (MilliSeconds (interPacketInterval)));
    client.SetAttribute ("MaxPackets", UintegerValue (1000000));

    clientApps.Add (dlClient.Install (remoteHost));
    clientApps.Add (ulClient.Install (ueNodes.Get (u)));
    if (u + 1 < ueNodes.GetN ())
    {
      clientApps.Add (client.Install (ueNodes.Get (u + 1)));
    }
    else
    {
      clientApps.Add (client.Install (ueNodes.Get (0)));
    }
  }
  serverApps.Start (Seconds (0.1));
  clientApps.Start (Seconds (0.1));

  Ping6Helper ping6;
  ping6.SetRemote (GetIpv6Address (ueNodes.Get (0)));
  //ping6.SetIfIndex (ueIpIface.GetInterfaceIndex (1));
  ApplicationContainer apps = ping6.Install (remoteHost);
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (3.0));

  Simulator::Stop (Seconds (simTime));
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;

}

