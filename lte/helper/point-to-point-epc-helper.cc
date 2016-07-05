/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011-2013 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Jaume Nin <jnin@cttc.es>
 *         Nicola Baldo <nbaldo@cttc.es>
 *         Manuel Requena <manuel.requena@cttc.es>
 * 
 * IPv6 support - Makhtar Diouf 
 * $Id: point-to-point-epc-helper.cc, 2016-03-29 16:19 +0900 makhtar $
 */

#include <ns3/log.h>
#include <ns3/inet-socket-address.h>
#include <ns3/mac48-address.h>
#include <ns3/eps-bearer.h>
#include <ns3/ipv4-address.h>
#include <ns3/ipv6-address.h>
#include <ns3/internet-stack-helper.h>
#include <ns3/point-to-point-helper.h>
#include <ns3/packet-socket-helper.h>
#include <ns3/packet-socket-address.h>
#include <ns3/epc-enb-application.h>
#include <ns3/epc-sgw-pgw-application.h>
#include <ns3/lte-enb-rrc.h>
#include <ns3/epc-x2.h>
#include <ns3/lte-enb-net-device.h>
#include <ns3/lte-ue-net-device.h>
#include <ns3/epc-mme.h>
#include <ns3/epc-ue-nas.h>
#include "point-to-point-epc-helper.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("PointToPointEpcHelper");

NS_OBJECT_ENSURE_REGISTERED (PointToPointEpcHelper);

PointToPointEpcHelper::PointToPointEpcHelper ()
: m_gtpuUdpPort (2152) // fixed by the standard
{
  NS_LOG_FUNCTION (this);

  // since we use point-to-point links for all S1-U links,
  // we use a /30 subnet which can hold exactly two addresses
  // (remember that net broadcast and null address are not valid)
  m_s1uIpv4AddressHelper.SetBase ("10.0.0.0", "255.255.255.252");
  m_s1uIpv6AddressHelper.SetBase (Ipv6Address ("2001::a00:0"), Ipv6Prefix (126));

  m_x2Ipv4AddressHelper.SetBase ("12.0.0.0", "255.255.255.252");
  m_x2Ipv6AddressHelper.SetBase (Ipv6Address ("2001::c00:0"), Ipv6Prefix (126));

  // we use a /8 net for all UEs
  m_ueAddressHelper.SetBase ("7.0.0.0", "255.0.0.0");
  m_ueAddressHelper6.SetBase (Ipv6Address ("7001:700::"), Ipv6Prefix (64)); // 104

  // create SgwPgwNode
  m_sgwPgw = CreateObject<Node> ();
  InternetStackHelper internet;
  internet.Install (m_sgwPgw);

  // create S1-U socket
  int retval = - 1;
  Ptr<Socket> sgwPgwS1uSocket = Socket::CreateSocket (m_sgwPgw, TypeId::LookupByName ("ns3::UdpSocketFactory"));
  Ipv4Address sgwIp = Ipv4Address::GetAny ();
  Ipv6Address sgwIpv6 = Ipv6Address::GetAny ();

  if (ns3::lteUseIpv6)
    retval = sgwPgwS1uSocket->Bind (Inet6SocketAddress (sgwIpv6, m_gtpuUdpPort));
  else
    retval = sgwPgwS1uSocket->Bind (InetSocketAddress (sgwIp, m_gtpuUdpPort));

  NS_ASSERT (retval == 0);

  // create TUN device implementing tunneling of user data over GTP-U/UDP/IP
  m_tunDevice = CreateObject<VirtualNetDevice> ();
  // allow jumbo packets
  m_tunDevice->SetAttribute ("Mtu", UintegerValue (30000));

  // yes we need this
  m_tunDevice->SetAddress (Mac48Address::Allocate ());

  m_sgwPgw->AddDevice (m_tunDevice);
  NetDeviceContainer tunDeviceContainer;
  tunDeviceContainer.Add (m_tunDevice);

  // the TUN device is on the same subnet as the UEs, so when a packet
  // addressed to an UE arrives from the internet to the WAN interface of
  // the PGW it will be forwarded to the TUN device.
  Ipv4InterfaceContainer tunDeviceIpv4IfContainer = m_ueAddressHelper.Assign (tunDeviceContainer);
  internet.EnablePcapIpv4 ("Epc", tunDeviceIpv4IfContainer);
  if (ns3::lteUseIpv6)
  {
    Ipv6InterfaceContainer iface6 = m_ueAddressHelper6.Assign (tunDeviceContainer);
    iface6.SetForwarding (0, true);
    internet.EnablePcapIpv6 ("Epc", iface6);
    NS_LOG_DEBUG ("PGW IP: " << iface6.GetAddress (0, 0));    
  }
  // create EpcSgwPgwApplication
  m_sgwPgwApp = CreateObject<EpcSgwPgwApplication> (m_tunDevice, sgwPgwS1uSocket);
  m_sgwPgw->AddApplication (m_sgwPgwApp);

  // connect SgwPgwApplication and virtual net device for tunneling
  m_tunDevice->SetSendCallback (MakeCallback (&EpcSgwPgwApplication::RecvFromTunDevice, m_sgwPgwApp));

  // Create MME and connect with SGW via S11 interface
  m_mme = CreateObject<EpcMme> ();
  m_mme->SetS11SapSgw (m_sgwPgwApp->GetS11SapSgw ());
  m_sgwPgwApp->SetS11SapMme (m_mme->GetS11SapMme ());

}

PointToPointEpcHelper::~ PointToPointEpcHelper ()
{
  NS_LOG_FUNCTION (this);
}

TypeId
PointToPointEpcHelper::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::PointToPointEpcHelper")
          .SetParent<EpcHelper> ()
          .SetGroupName ("Lte")
          .AddConstructor<PointToPointEpcHelper> ()
          .AddAttribute ("S1uLinkDataRate",
                         "The data rate to be used for the next S1-U link to be created",
                         DataRateValue (DataRate ("10Gb/s")),
                         MakeDataRateAccessor (&PointToPointEpcHelper::m_s1uLinkDataRate),
                         MakeDataRateChecker ())
          .AddAttribute ("S1uLinkDelay",
                         "The delay to be used for the next S1-U link to be created",
                         TimeValue (Seconds (0)),
                         MakeTimeAccessor (&PointToPointEpcHelper::m_s1uLinkDelay),
                         MakeTimeChecker ())
          .AddAttribute ("S1uLinkMtu",
                         "The MTU of the next S1-U link to be created. Note that, because of the additional GTP/UDP/IP tunneling overhead, you need a MTU larger than the end-to-end MTU that you want to support.",
                         UintegerValue (2000),
                         MakeUintegerAccessor (&PointToPointEpcHelper::m_s1uLinkMtu),
                         MakeUintegerChecker<uint16_t> ())
          .AddAttribute ("X2LinkDataRate",
                         "The data rate to be used for the next X2 link to be created",
                         DataRateValue (DataRate ("10Gb/s")),
                         MakeDataRateAccessor (&PointToPointEpcHelper::m_x2LinkDataRate),
                         MakeDataRateChecker ())
          .AddAttribute ("X2LinkDelay",
                         "The delay to be used for the next X2 link to be created",
                         TimeValue (Seconds (0)),
                         MakeTimeAccessor (&PointToPointEpcHelper::m_x2LinkDelay),
                         MakeTimeChecker ())
          .AddAttribute ("X2LinkMtu",
                         "The MTU of the next X2 link to be created. Note that, because of some big X2 messages, you need a big MTU.",
                         UintegerValue (3000),
                         MakeUintegerAccessor (&PointToPointEpcHelper::m_x2LinkMtu),
                         MakeUintegerChecker<uint16_t> ())
          ;
  return tid;
}

void
PointToPointEpcHelper::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  m_tunDevice->SetSendCallback (MakeNullCallback<bool, Ptr<Packet>, const Address&, const Address&, uint16_t > ());
  m_tunDevice = 0;
  m_sgwPgwApp = 0;
  m_sgwPgw->Dispose ();
}

void
PointToPointEpcHelper::AddEnb (Ptr<Node> enb, Ptr<NetDevice> lteEnbNetDevice, uint16_t cellId)
{
  NS_LOG_FUNCTION (this << enb << lteEnbNetDevice << cellId);

  NS_ASSERT (enb == lteEnbNetDevice->GetNode ());

  // add an IPv4/v6 stack to the previously created eNB
  InternetStackHelper internet;
  internet.Install (enb);

  NS_LOG_LOGIC ("number of Ipv4 ifaces of the eNB after node creation: " << enb->GetObject<Ipv4> ()->GetNInterfaces ());

  // create a point to point link between the new eNB and the SGW with
  // the corresponding new NetDevices on each side
  NodeContainer enbSgwNodes;
  enbSgwNodes.Add (m_sgwPgw);
  enbSgwNodes.Add (enb);
  PointToPointHelper p2ph;
  p2ph.SetDeviceAttribute ("DataRate", DataRateValue (m_s1uLinkDataRate));
  p2ph.SetDeviceAttribute ("Mtu", UintegerValue (m_s1uLinkMtu));
  p2ph.SetChannelAttribute ("Delay", TimeValue (m_s1uLinkDelay));
  NetDeviceContainer enbSgwDevices = p2ph.Install (enb, m_sgwPgw);

  NS_LOG_LOGIC ("number of Ipv4 ifaces of the eNB after installing p2p dev: " << enb->GetObject<Ipv4> ()->GetNInterfaces ());
  Ptr<NetDevice> enbDev = enbSgwDevices.Get (0);
  Ptr<NetDevice> sgwDev = enbSgwDevices.Get (1);

  m_s1uIpv4AddressHelper.NewNetwork ();
  Ipv4InterfaceContainer enbSgwIpIfaces = m_s1uIpv4AddressHelper.Assign (enbSgwDevices);
  internet.EnablePcapIpv4 ("Enb", enbSgwIpIfaces);
  NS_LOG_LOGIC ("number of Ipv4 ifaces of the eNB after assigning Ipv4 addr to S1 dev: " << enb->GetObject<Ipv4> ()->GetNInterfaces ());

  Ipv4Address enbAddress = enbSgwIpIfaces.GetAddress (0);
  Ipv4Address sgwAddress = enbSgwIpIfaces.GetAddress (1);

  // create S1-U socket for the ENB
  Ptr<Socket> enbS1uSocket = Socket::CreateSocket (enb, TypeId::LookupByName ("ns3::UdpSocketFactory"));
  int retval = - 1;
  uint16_t protoNumber = 0;
  Ipv6Address enbAddress6 = Ipv6Address::GetZero ();
  Ipv6Address sgwAddress6 = Ipv6Address::GetZero ();
  if (ns3::lteUseIpv6)
  {
    m_s1uIpv6AddressHelper.NewNetwork ();
    Ipv6InterfaceContainer enbSgwIpv6Ifaces = m_s1uIpv6AddressHelper.Assign (enbSgwDevices);
    enbSgwIpv6Ifaces.SetDefaultRoute (0, 1);
    enbSgwIpv6Ifaces.SetForwarding (1, true);
    
    enbAddress6 = enbSgwIpv6Ifaces.GetAddress (0, 0);
    sgwAddress6 = enbSgwIpv6Ifaces.GetAddress (1, 0);
    retval = enbS1uSocket->Bind (Inet6SocketAddress (enbAddress6, m_gtpuUdpPort));

    internet.EnablePcapIpv6 ("Enb", enbSgwIpv6Ifaces);
    protoNumber = Ipv6L3Protocol::PROT_NUMBER;
    NS_LOG_DEBUG("Enb IP: " << enbAddress6 << " Sgw IP: " << sgwAddress6);
  }
  else
  {
    protoNumber = Ipv4L3Protocol::PROT_NUMBER;
    retval = enbS1uSocket->Bind (InetSocketAddress (enbAddress, m_gtpuUdpPort));
  }
  NS_ASSERT (retval == 0);

  // create LTE socket for the ENB
  Ptr<Socket> enbLteSocket = Socket::CreateSocket (enb, TypeId::LookupByName ("ns3::PacketSocketFactory"));
  PacketSocketAddress enbLteSocketBindAddress;
  enbLteSocketBindAddress.SetSingleDevice (lteEnbNetDevice->GetIfIndex ());
  enbLteSocketBindAddress.SetProtocol (protoNumber);

  retval = enbLteSocket->Bind (enbLteSocketBindAddress);
  NS_ASSERT (retval == 0);

  PacketSocketAddress enbLteSocketConnectAddress;
  enbLteSocketConnectAddress.SetPhysicalAddress (Mac48Address::GetBroadcast ());
  enbLteSocketConnectAddress.SetSingleDevice (lteEnbNetDevice->GetIfIndex ());
  enbLteSocketConnectAddress.SetProtocol (protoNumber);

  retval = enbLteSocket->Connect (enbLteSocketConnectAddress);
  NS_ASSERT (retval == 0);

  NS_LOG_INFO ("create EpcEnbApplication");

  Ptr<EpcEnbApplication> enbApp;
  if (ns3::lteUseIpv6)
    enbApp = CreateObject<EpcEnbApplication> (enbLteSocket, enbS1uSocket, enbAddress6, sgwAddress6, cellId);
  else
    enbApp = CreateObject<EpcEnbApplication> (enbLteSocket, enbS1uSocket, enbAddress, sgwAddress, cellId);

  enb->AddApplication (enbApp);

  NS_ASSERT (enb->GetNApplications () == 1);
  NS_ASSERT_MSG (enb->GetApplication (0)->GetObject<EpcEnbApplication> () != 0, "cannot retrieve EpcEnbApplication");
  NS_LOG_LOGIC ("enb: " << enb << ", enb->GetApplication (0): " << enb->GetApplication (0));

  NS_LOG_INFO ("Create EpcX2 entity");
  Ptr<EpcX2> x2 = CreateObject<EpcX2> ();
  enb->AggregateObject (x2);

  NS_LOG_INFO ("connect S1-AP interface");

  if (ns3::lteUseIpv6)
  {
    m_mme->AddEnb (cellId, enbAddress6, enbApp->GetS1apSapEnb ());
    m_sgwPgwApp->AddEnb (cellId, enbAddress6, sgwAddress6);
  }
  else
  {
    m_mme->AddEnb (cellId, enbAddress, enbApp->GetS1apSapEnb ());
    m_sgwPgwApp->AddEnb (cellId, enbAddress, sgwAddress);
  }
  enbApp->SetS1apSapMme (m_mme->GetS1apSapMme ());
}

void
PointToPointEpcHelper::AddX2Interface (Ptr<Node> enb1, Ptr<Node> enb2)
{
  NS_LOG_FUNCTION (this << enb1 << enb2);

  // Create a point to point link between the two eNBs with
  // the corresponding new NetDevices on each side
  NodeContainer enbNodes;
  enbNodes.Add (enb1);
  enbNodes.Add (enb2);
  PointToPointHelper p2ph;
  p2ph.SetDeviceAttribute ("DataRate", DataRateValue (m_x2LinkDataRate));
  p2ph.SetDeviceAttribute ("Mtu", UintegerValue (m_x2LinkMtu));
  p2ph.SetChannelAttribute ("Delay", TimeValue (m_x2LinkDelay));
  NetDeviceContainer enbDevices = p2ph.Install (enb1, enb2);
  NS_LOG_LOGIC ("number of Ipv4 ifaces of the eNB #1 after installing p2p dev: " << enb1->GetObject<Ipv4> ()->GetNInterfaces ());
  NS_LOG_LOGIC ("number of Ipv4 ifaces of the eNB #2 after installing p2p dev: " << enb2->GetObject<Ipv4> ()->GetNInterfaces ());
  Ptr<NetDevice> enb1Dev = enbDevices.Get (0);
  Ptr<NetDevice> enb2Dev = enbDevices.Get (1);

  m_x2Ipv4AddressHelper.NewNetwork ();
  Ipv4InterfaceContainer enbIpIfaces = m_x2Ipv4AddressHelper.Assign (enbDevices);
  NS_LOG_LOGIC ("number of Ipv4 ifaces of the eNB #1 after assigning Ipv4 addr to X2 dev: " << enb1->GetObject<Ipv4> ()->GetNInterfaces ());
  NS_LOG_LOGIC ("number of Ipv4 ifaces of the eNB #2 after assigning Ipv4 addr to X2 dev: " << enb2->GetObject<Ipv4> ()->GetNInterfaces ());

  Ipv4Address enb1X2Address = enbIpIfaces.GetAddress (0);
  Ipv4Address enb2X2Address = enbIpIfaces.GetAddress (1);
  Ipv6Address enb1X2Address6 = Ipv6Address::GetZero ();
  Ipv6Address enb2X2Address6 = Ipv6Address::GetZero ();

  if (ns3::lteUseIpv6)
  {
    m_x2Ipv6AddressHelper.NewNetwork ();
    Ipv6InterfaceContainer enbIpv6Ifaces = m_x2Ipv6AddressHelper.Assign (enbDevices);
    enb1X2Address6 = enbIpv6Ifaces.GetAddress (0, 0);
    enb2X2Address6 = enbIpv6Ifaces.GetAddress (1, 0);
    NS_LOG_DEBUG("Enb1 X2IP: " << enb1X2Address6 << " Enb2 X2IP: " << enb2X2Address6);
  }

  // Add X2 interface to both eNBs' X2 entities
  Ptr<EpcX2> enb1X2 = enb1->GetObject<EpcX2> ();
  Ptr<LteEnbNetDevice> enb1LteDev = enb1->GetDevice (0)->GetObject<LteEnbNetDevice> ();
  uint16_t enb1CellId = enb1LteDev->GetCellId ();
  NS_LOG_LOGIC ("LteEnbNetDevice #1 = " << enb1LteDev << " - CellId = " << enb1CellId);

  Ptr<EpcX2> enb2X2 = enb2->GetObject<EpcX2> ();
  Ptr<LteEnbNetDevice> enb2LteDev = enb2->GetDevice (0)->GetObject<LteEnbNetDevice> ();
  uint16_t enb2CellId = enb2LteDev->GetCellId ();
  NS_LOG_LOGIC ("LteEnbNetDevice #2 = " << enb2LteDev << " - CellId = " << enb2CellId);

  if (ns3::lteUseIpv6)
  {
    enb1X2->SetIpv6 (enb1X2Address6, enb2X2Address6);
    enb2X2->SetIpv6 (enb2X2Address6, enb1X2Address6);
    // EpcX2 AddX2Interface will set cell Ids
  }
  enb1X2->AddX2Interface (enb1CellId, enb1X2Address, enb2CellId, enb2X2Address);
  enb2X2->AddX2Interface (enb2CellId, enb2X2Address, enb1CellId, enb1X2Address);

  enb1LteDev->GetRrc ()->AddX2Neighbour (enb2LteDev->GetCellId ());
  enb2LteDev->GetRrc ()->AddX2Neighbour (enb1LteDev->GetCellId ());
}

void
PointToPointEpcHelper::AddUe (Ptr<NetDevice> ueDevice, uint64_t imsi)
{
  NS_LOG_FUNCTION (this << imsi << ueDevice);
  m_mme->AddUe (imsi);
  m_sgwPgwApp->AddUe (imsi);

}

uint8_t
PointToPointEpcHelper::ActivateEpsBearer (Ptr<NetDevice> ueDevice, uint64_t imsi, Ptr<EpcTft> tft, EpsBearer bearer)
{
  NS_LOG_FUNCTION (this << ueDevice << imsi);

  /*
   we now retrieve the IP address of the UE and notify it to the SGW;
   we couldn't do it before since address assignment is triggered by
   the user simulation program, rather than done by the EPC.
   Note: Convince TcpSocketBase to not Bind() to IPv4 endpoint if we're not using it
   */
  Ptr<Node> ueNode = ueDevice->GetNode ();
  Ipv4Address ueAddr = Ipv4Address::GetZero ();
  Ipv6Address ueAddr6 = Ipv6Address::GetZero ();

  if (! ns3::lteUseIpv6)
  {
    Ptr<Ipv4> ueIpv4 = ueNode->GetObject<Ipv4> ();
    NS_ASSERT_MSG (ueIpv4 != 0, "UEs need to have assigned IPs before EPS bearers can be activated");
    int32_t interface = ueIpv4->GetInterfaceForDevice (ueDevice);
    NS_ASSERT (interface >= 0);
    NS_ASSERT (ueIpv4->GetNAddresses (interface) == 1);
    ueAddr = ueIpv4->GetAddress (interface, 0).GetLocal ();
  }
  else
    ueAddr6 = GetIpv6Address (ueNode);

  NS_LOG_LOGIC (" UE IP address: " << ueAddr << " " << ueAddr6);
  m_sgwPgwApp->SetUeAddress (imsi, ueAddr, ueAddr6);

  uint8_t bearerId = m_mme->AddBearer (imsi, tft, bearer);
  Ptr<LteUeNetDevice> ueLteDevice = ueDevice->GetObject<LteUeNetDevice> ();
  if (ueLteDevice)
  {
    ueLteDevice->GetNas ()->ActivateEpsBearer (bearer, tft);
  }
  return bearerId;
}

Ptr<Node>
PointToPointEpcHelper::GetPgwNode ()
{
  return m_sgwPgw;
}

Ipv4InterfaceContainer
PointToPointEpcHelper::AssignUeIpv4Address (NetDeviceContainer ueDevices)
{
  return m_ueAddressHelper.Assign (ueDevices);
}

Ipv6InterfaceContainer
PointToPointEpcHelper::AssignUeIpv6Address (NetDeviceContainer ueDevices)
{
  return m_ueAddressHelper6.Assign (ueDevices);//AssignWithoutAddress (ueDevices);
}

Ipv4Address
PointToPointEpcHelper::GetUeDefaultGatewayAddress ()
{
  // return the address of the tun device
  return m_sgwPgw->GetObject<Ipv4> ()->GetAddress (1, 0).GetLocal ();
}

Ipv6Address
PointToPointEpcHelper::GetUeDefaultGatewayAddress6 ()
{
  return GetIpv6Address (m_sgwPgw);
}


} // namespace ns3
