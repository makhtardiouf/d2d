/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014-2016 Gyeongsang N. University
 * InfoCom Engineering Dept, South Korea
 *
 * 
 * $Id: d2dvanet.cc, 2016-02-23 21:40 +0900 makhtar $
 * Advisor: Woonsup Lee 
 * Author: Makhtar Diouf 
 * 
 * *** Proximity-based Services + VANET: ProseVANET ***
 *
 * This program is open source software released under the terms of the
 * GNU General Public License version 2 as published by the Free Software Foundation.
 * It is distributed WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Control LTE-A ProSe / VANET simulation
 */
#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ns3/log.h>
#include <ns3/config-store-module.h>
#include <ns3/command-line.h>
#include <ns3/callback.h>

#include <ns3/net-device-container.h>
#include <ns3/ocb-wifi-mac.h>
#include <ns3/wifi-80211p-helper.h>
#include <ns3/wave-mac-helper.h>
#include <ns3/aodv-helper.h>
#include <ns3/ns2-mobility-helper.h>
#include <ns3/rng-seed-manager.h>
#include <ns3/flow-monitor-helper.h>
#include <ns3/flow-monitor.h>
#include <ns3/gnuplot-helper.h>
#include "ns3/lte-prose-common.h"
#include "ns3/lte-utils.h"
#include "ns3/Roads.h"
#include "ns3/Controller.h"

using namespace std;
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ProSeVanet");

class Experiment
{
public:

  struct Input
  {

    Input ()
    {
    };
    // Defaults
    uint8_t maxEnbs = 10; // 7
    uint8_t maxUes = 50;
    uint8_t enbNum = 3;
    uint8_t vehicleNum = 9;
    uint8_t vehiSpeed = 50;
    uint width = 500;
    uint height = 500;

    double dUesRatio = 1; // 0.16 3GPP requirements 25/150 active users
    double cellRadius = 250.0; // meters
    double nRB = 50; // 10 MHz

    double simuTime = 20;
    uint8_t debug = 2;
    bool plot = true;
    bool useAnimation = true;

    double wlanDist = 200; //m
  };

  Experiment ()
  {
  };

  void Run (struct Experiment::Input input);

private:

  struct Input m_input;
};

static void Start (Ptr<Roads> roads)
{
  roads->Start ();
}

static void Stop (Ptr<Roads> roads)
{
  roads->Stop ();
}

void EnableLogging ();

void NetanimTrace (Ptr<ProSeHelper> d2dCtrl);

int main (int argc, char* argv[])
{
  EnableLogging ();
  Config::SetDefault ("ns3::LteEnbRrc::DefaultTransmissionMode", UintegerValue (2));
  Config::SetDefault ("ns3::LteUePhy::EnableUplinkPowerControl", BooleanValue (true));
  Config::SetDefault ("ns3::LteUePowerControl::ClosedLoop", BooleanValue (true));
  Config::SetDefault ("ns3::LteUePowerControl::AccumulationEnabled", BooleanValue (true));
  Config::SetDefault ("ns3::LteUePowerControl::Alpha", DoubleValue (1.0));
  Config::SetDefault ("ns3::LteUePowerControl::Pcmin", DoubleValue (16.0)); // min Tx power
  GlobalValue::Bind ("ChecksumEnabled", BooleanValue (true));

  Experiment experiment;
  struct Experiment::Input input;

  stringstream ss;
  int numberOfRuns = 1; // Multiple Runs
  RngSeedManager::SetSeed (1);
  RngSeedManager::SetRun (1);

  ss << "speed" << (int) input.vehiSpeed << "seed" << (int) RngSeedManager::GetSeed ()
          << "run" << (int) RngSeedManager::GetRun ();

  string conf = ss.str ();
  ss.str ("");

  for (int i = 0; i < numberOfRuns; i ++)
  {
    NS_LOG_INFO ("\n\n\tRun " << (int) i << " - configuration with " << (int) input.vehicleNum << " vehicles"
                 << " Ratio: " << (int) input.dUesRatio);

    experiment.Run (input);

    // Backup generated files
    ss.str ("");
    ss << "export SIMUOUTPUT=OUTPUT/`date \"+%Y-%m-%d\"`/" << (int) input.vehiSpeed << "kmh"
            << "_" << (int) input.vehicleNum << "Vehicles"
            << "_Ratio" << (int) input.dUesRatio
            << "; mkdir -p $SIMUOUTPUT "
            // d2d-discovery.txt needs to be aggregated
            << "; cp *txt *mob $SIMUOUTPUT; mv *xml *pcap $SIMUOUTPUT"
            ;

    system (ss.str ().c_str ());
    input.vehicleNum += 10;
  }

  ss << " mv d2d-discovery.txt d2d-discovery`date \"+%Y%m%d\"`.txt." << conf
          << "; mv d2d-nodes.txt d2d-nodes`date \"+%Y%m%d\"`.txt." << conf;

  system (ss.str ().c_str ());
  NS_LOG_INFO ("\tStopping the D2d simulator...");
  return 0;
}

void Experiment::Run (struct Experiment::Input input)
{
  NS_LOG_INFO ("\tConfiguring nodes...");
  Ptr<ProSeHelper> d2dCtrl = CreateObject<ProSeHelper>();

  if (input.vehicleNum > 80)
    Config::SetDefault ("ns3::LteEnbRrc::SrsPeriodicity", UintegerValue (320));
  else if (input.vehicleNum > 40)
    Config::SetDefault ("ns3::LteEnbRrc::SrsPeriodicity", UintegerValue (160));
  else
    Config::SetDefault ("ns3::LteEnbRrc::SrsPeriodicity", UintegerValue (80));

  if (input.enbNum > input.maxEnbs)
  {
    input.enbNum = input.maxEnbs;
    NS_LOG_INFO ("\tLimiting the number of eNBs to " << (int) input.enbNum);
  }
  if (input.vehicleNum > input.maxUes)
  {
    input.vehicleNum = input.maxUes;
    NS_LOG_INFO ("\tLimiting the number of vehicles to " << (int) input.vehicleNum);
  }

  d2dCtrl->CreateNodesAndHelpers (input.enbNum, input.vehicleNum, input.dUesRatio);
  d2dCtrl->InstallEpcRanInHexGrid (input.cellRadius);

  // Default scheduler is Proportional-Fair, uncomment to use Round-Robin
  // d2dCtrl->lteH->SetSchedulerType ("ns3::RrFfMacScheduler");

  // For visualizing node movements
  if (input.useAnimation)
  {
    //  Simulator::Schedule (Seconds (0.3), &NetanimTrace, d2dCtrl);
  }

  ns3::PacketMetadata::Enable ();
  d2dCtrl->SetupIpNetwork ();
  d2dCtrl->lteH->EnableTraces ();

  if (input.debug > 2)
    d2dCtrl->lteH->EnableLogComponents ();

  Ptr<Roads> roads = CreateObject<Roads> ();
  roads->SetLteHelpers (d2dCtrl->lteH, d2dCtrl->epcH);
  roads->SetWidth (input.width);
  roads->SetHeight (input.height);
  
  Ptr<Controller> controller = CreateObject<Controller> ();
  controller->SetVehiclesNumber (input.vehicleNum);
  controller->SetRoads (roads);
  controller->Plot = input.plot;

  roads->SetControlVehicleCallback (MakeCallback (&Controller::ControlVehicle, controller));
  roads->SetInitVehiclesCallback (MakeCallback (&Controller::InitVehicles, controller));
  roads->SetTxDataCallback (MakeCallback (&ProSeHelper::InitDiscovery, d2dCtrl));

  // Output nodes positions and energy consumption
  // + Unrelated task: also reduce the sampling period of DL stats
  CheckNodes ("d2d-nodes.txt");
  Simulator::Schedule (Seconds (0.0), &Start, roads);
  Simulator::Schedule (Seconds (1.0), &Roads::PrintVehicles, roads);
  Simulator::Schedule (Seconds (input.simuTime), &Stop, roads);
  Simulator::Stop (Seconds (input.simuTime));
  Simulator::Run ();
  Simulator::Destroy ();
}

void EnableLogging ()
{
  LogLevel logAll = (LogLevel) (LOG_PREFIX_FUNC | LOG_PREFIX_TIME | LOG_LEVEL_ALL);
  LogLevel logInfo = (LogLevel) (LOG_PREFIX_FUNC | LOG_PREFIX_TIME | LOG_LEVEL_INFO);

  LogComponentEnable ("ProSeVanet", logAll);
  LogComponentEnable ("Ns2MobilityHelper", logInfo);
  LogComponentEnable ("Roads", logAll);
  LogComponentEnable ("Vehicle", logAll);

  LogComponentEnable ("LteUtils", logAll);
  LogComponentEnable ("ProSeApp", logAll);
  LogComponentEnable ("EpcProSeFcnApp", logAll);
  LogComponentEnable ("ProSeHelper", logAll);
  LogComponentEnable ("LteHexGridEnbTopologyHelper", logAll);
  LogComponentEnable ("HttpClient", logAll);
  LogComponentEnable ("HttpServer", logAll);
  LogComponentEnable ("EpcSgwPgwApplication", logAll);
  LogComponentEnable ("EpcEnbApplication", logAll);

  LogComponentEnable ("TcpSocketBase", logAll);
  LogComponentEnable ("OnOffApplication", logInfo);
  LogComponentEnable ("UdpClient", logAll);
  LogComponentEnable ("PacketSink", logInfo);

  LogComponentEnable ("YansWifiPhy", logAll);
  LogComponentEnable ("MinstrelWifiManager", logAll);
  LogComponentEnable ("YansWifiHelper", logAll);
  LogComponentEnable ("WifiHelper", logAll);

  LogComponentEnable ("Ipv4StaticRouting", logAll);
  // LogComponentEnable ("Ipv4L3Protocol", logAll);
  // LogComponentEnable ("Ipv6L3Protocol", logAll);
  LogComponentEnable ("Ipv6StaticRouting", logAll);
  // LogComponentEnable ("Icmpv6L4Protocol", logAll);
  // LogComponentEnable ("Ipv6Interface", logAll);
  // LogComponentEnable ("Ipv6RawSocketImpl", logAll);

  LogComponentEnable ("WaypointMobilityModel", logAll);

}