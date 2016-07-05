/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014-2016 Gyeongsang N. University
 * InfoCom Engineering Dept, South Korea
 *
 * $Id: d2dsim.cc, 2016-07-05 18:56 +0900 makhtar $
 * Advisor: Woonsup Lee 
 * Author: Makhtar Diouf 
 *
 * *** Proximity-based Services, LTE-Direct (D2D) ***
 *
 * This program is open source software released under the terms of the
 * GNU General Public License version 2 as published by the Free Software Foundation.
 * It is distributed WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Control LTE-A ProSe D2D/Cellular network simulation.
 * Progressively implemented to match the requirements of 3GPP TS 23.303, 24.334, TR 36.843...
 */
#include <string>
#include <ns3/log.h>
#include <ns3/config-store-module.h>
#include <ns3/command-line.h>
#include <ns3/net-device-container.h>
#include <ns3/lte-ue-net-device.h>
#include <ns3/lte-ue-rrc.h>
#include <ns3/rng-seed-manager.h>

#include <ns3/flow-monitor-helper.h>
#include <ns3/flow-monitor.h>
//#include <ns3/gnuplot-helper.h>
#include "ns3/default-simulator-impl.h"
#include "ns3/lte-prose-common.h"
#include "ns3/lte-utils.h"
#include <cstdlib>
#include <csignal>

using namespace std;
using namespace ns3;

NS_LOG_COMPONENT_DEFINE("D2dSimulator");

void
NetanimTrace(Ptr<ProSeHelper> d2dCtrl);

class D2dExperiment
{
public:

    struct Input
    {

        Input()
        {
            ;
        }
        // Defaults
        /*
         * As per TR36843 A.2.1.1, macroEnbNum should be 7 with 3 sectors
         * 3 sector = 3eNBs with different antenna orientation each
         * ueNum should be 150, with 25 active
         */
        uint8_t maxEnbs = 2; // 7
        uint8_t maxUes = 150;
        uint8_t enbNum = 1;
        uint8_t ueNum = 5;
        uint8_t ueSpeed = 3;
        double dUesRatio = 1; // 0.16 3GPP requirements 25/150 active users
        double nRB = 50; // 10 MHz
        double cellRadius = 250.0; // meters

        // Time for one full processing for LTE-D + LTE-infra + WiFi-adhoc
        double simuTime = 50.0;
        uint8_t debug = 1;
        bool useAnimation = false;
    };

    struct Output
    {
        uint32_t received;
        Ptr<FlowMonitor> flowmon;
    };

    D2dExperiment()
    {
        ;
    }
    struct D2dExperiment::Output Run(struct D2dExperiment::Input input);

private:
    struct Input m_input;
    struct Output m_output;
};


void EnableLogging(struct D2dExperiment::Input input);

/*
 * \ingroup ProSe D2D
 * \brief Control simulation: Basic scenario with 1 macro eNB, 3 sectors, and a few cellular and hybrid Ues
 * Parameters specified here are settable in a configuration file.
 */
int
main(int argc, char* argv[])
{
    NS_LOG_INFO("\n\tRunning the D2d simulator...");
    D2dExperiment experiment;
    struct D2dExperiment::Input input;
    // struct D2dExperiment::Output kpi;
    EnableLogging(input);
    // CLI arguments, currently unused
    CommandLine cmd;
    cmd.AddValue("enbNum", "Number of eNodeBs", input.enbNum);
    cmd.AddValue("ueNum", "Number of Cellular and ProSe D2D Ues", input.ueNum);
    cmd.AddValue("cellRadius", "Cell radius [m]", input.cellRadius);
    cmd.AddValue("nRB", "Number of resource blocks", input.nRB);
    cmd.AddValue("simuTime", "Total duration of the simulation [s]", input.simuTime);
    cmd.AddValue("debug", "Enable detailed debugging output [int >= 1]", input.debug);
    cmd.AddValue("useAnimation", "Generate Netanim xml file [bool]", input.useAnimation);
    cmd.Parse(argc, argv);

    Config::SetDefault("ns3::LteEnbRrc::DefaultTransmissionMode", UintegerValue(2));
    Config::SetDefault("ns3::LteUePhy::EnableUplinkPowerControl", BooleanValue(true));
    Config::SetDefault("ns3::LteUePowerControl::ClosedLoop", BooleanValue(true));
    Config::SetDefault("ns3::LteUePowerControl::AccumulationEnabled", BooleanValue(true));
    Config::SetDefault("ns3::LteUePowerControl::Alpha", DoubleValue(1.0));
    Config::SetDefault("ns3::LteUePowerControl::Pcmin", DoubleValue(16.0)); // min Tx power
    GlobalValue::Bind("ChecksumEnabled", BooleanValue(true));

    ns3::lteUseIpv6 = false;
    stringstream ss;
    // Randomize experiments
    RngSeedManager::SetSeed(2);
    RngSeedManager::SetRun(2);
    input.ueSpeed = 3;       // pedestrian
    input.dUesRatio = 1.0; // * 100%
    int numberOfRuns = 1; // Multiple Runs

    ss << "speed" << (int) input.ueSpeed
            << "seed" << (int) RngSeedManager::GetSeed()
            << "run" << (int) RngSeedManager::GetRun();
    string conf = ss.str();

    for (int i = 0; i < numberOfRuns; i++)
    {
        NS_LOG_INFO("\n\n\tRun " << (int) i << " - configuration with "
                    << (int) input.ueNum << " Ues"
                    << " Ratio: " << input.dUesRatio);

        experiment.Run(input);

        // @TODO Rewrite this programmatically
        // Save generated log, stats files under OUTPUT directory
        ss.str("");
        ss << "export SIMUOUTPUT=OUTPUT/`date \"+%Y%m%d\"`/"
                << (int) input.ueSpeed << "kmh"
                << "_" << (int) input.ueNum << "Ues"
                << "_Ratio" << (int) input.dUesRatio
                << "; mkdir -p $SIMUOUTPUT ;"
                << "cp *.log $SIMUOUTPUT ;"
                << "mv *txt *mob *plt *xml *tr *pcap $SIMUOUTPUT"
                ;
        system(ss.str().c_str());
        input.ueNum += 10;
    }

    ss << " mv d2d-discovery.txt d2d-discovery`date \"+%Y%m%d\"`_" << conf << ".txt;"
            << "mv d2d-nodes.txt d2d-nodes`date \"+%Y%m%d\"`_" << conf << ".txt";
    system(ss.str().c_str());
    NS_LOG_INFO("\tStopping the D2d simulator...\n See stats and pcap files under ns3-dev/OUTPUT/ directory");
    return 0;
}



struct D2dExperiment::Output
D2dExperiment::Run(struct D2dExperiment::Input input)
{
    NS_LOG_INFO("\tConfiguring nodes...");
    Ptr<ProSeHelper> d2dCtrl = CreateObject<ProSeHelper>();
    /*
       d2dCtrl->SetConfig(argv[0], true);
     Reference signals periodicity for high number of Ues
     Ref g_srsPeriodicity[SRS_ENTRIES] = {0, 2, 5, 10, 20, 40, 80, 160, 320};
     */
    uint8_t srsPeriodicity = 80;
    while ((input.ueNum > srsPeriodicity) && (srsPeriodicity <= 320))
        srsPeriodicity *= 2;

    Config::SetDefault("ns3::LteEnbRrc::SrsPeriodicity", UintegerValue(srsPeriodicity));
    NS_LOG_INFO("\tSRS periodicity set to: " << (int) srsPeriodicity);

    if (input.enbNum > input.maxEnbs)
    {
        input.enbNum = input.maxEnbs;
        NS_LOG_INFO("\tLimiting the number of eNBs to " << (int) input.enbNum);
    }
    if (input.ueNum > input.maxUes)
    {
        input.ueNum = input.maxUes;
        NS_LOG_INFO("\tLimiting the number of Ues to " << (int) input.ueNum);
    }

    d2dCtrl->SetNodesDistribution(ProSeHelper::UNIFORM);
    d2dCtrl->CreateNodesAndHelpers(input.enbNum, input.ueNum, input.dUesRatio);
    d2dCtrl->InstallEpcRanInHexGrid(input.cellRadius);

    // Default scheduler is Proportional-Fair, uncomment to use Round-Robin
    d2dCtrl->lteH->SetSchedulerType ("ns3::RrFfMacScheduler");

    d2dCtrl->InstallUes("ns3::RandomWalk2dMobilityModel");
    // d2dCtrl->InstallUes("ns3::ConstantPositionMobilityModel");
    // d2dCtrl->InstallUes("ns3::ConstantVelocityMobilityModel");
    ns3::PacketMetadata::Enable();

    // Note: this cannot track multicast packets
    FlowMonitorHelper flowmonH;
    struct D2dExperiment::Output output;
    output.flowmon = flowmonH.InstallAll();

    // For visualizing node movements
    if (input.useAnimation)
    {
      ;//  Simulator::Schedule(Seconds(6.0), &NetanimTrace, d2dCtrl);
    }

    d2dCtrl->SetupIpNetwork();

    // Attach each Ue to the eNB with the strongest signal or the closest homeEnb
    d2dCtrl->lteH->Attach(d2dCtrl->ueD2dDevs);
    d2dCtrl->lteH->Attach(d2dCtrl->ueCellDevs);
    d2dCtrl->lteH->EnableTraces();
    if (input.debug > 2)
        d2dCtrl->lteH->EnableLogComponents();

    // Output nodes positions and energy consumption
    // Reduce the sampling period of DL stats
    TrackNodes("d2d-nodes.txt");
    /// Simulator::Schedule (Seconds (5.0), &GenRadioMap, d2dCtrl);
    output.flowmon->Start(Seconds(0.1));
    output.flowmon->Stop(Seconds(input.simuTime));

    NS_LOG_INFO("\tInitializing discovery procedures...");
    Simulator::Schedule(Seconds(0.5), &ProSeHelper::InitDiscovery, d2dCtrl, NetDeviceContainer());

    // Save configuration
    // d2dCtrl->SetConfig(argv[0], false);
    Simulator::Stop(Seconds(input.simuTime));

    //**** Allow threading dubbed unsafe by the default simulator
    Ptr<DefaultSimulatorImpl> sim = DynamicCast<DefaultSimulatorImpl>(Simulator::GetImplementation());
    sim->RunWithThreads();

    stringstream ss;
    ss << "d2d-flowmon_" << (int) input.ueNum << "Ues" << "_Ratio" << input.dUesRatio << ".xml";
    output.flowmon->SerializeToXmlFile(ss.str().c_str(), true, true);
    Simulator::Destroy();
    return output;
}

void
EnableLogging(struct D2dExperiment::Input input)
{
    LogLevel logAll = (LogLevel) (LOG_PREFIX_FUNC | LOG_PREFIX_TIME | LOG_LEVEL_ALL);
    LogLevel logInfo = (LogLevel) (LOG_PREFIX_FUNC | LOG_PREFIX_TIME | LOG_LEVEL_INFO);

    LogComponentEnable("D2dSimulator", logAll);
    LogComponentEnable("LteUtils", logAll);
    LogComponentEnable("ProSeApp", logAll);
    LogComponentEnable("ProSeAppLteD2d", logAll);
    LogComponentEnable("ProSeAppWifi", logAll);
    LogComponentEnable("ProSeHelper", logAll);
    LogComponentEnable("EpcProSeFcnApp", logAll);

    LogComponentEnable("PointToPointHelper", logAll);
    LogComponentEnable("PointToPointEpcHelper", logAll);
   // LogComponentEnable("LteUeNetDevice", logAll);
   
    LogComponentEnable("EpcSgwPgwApplication", logAll);
    
    /// TEMP
    //LogComponentEnable ("Ipv4AddressHelper", logAll);
    //LogComponentEnable ("LteSpectrumPhy", logAll);
    //LogComponentEnable ("MultiModelSpectrumChannel", logAll);
    // LogComponentEnable("Ipv4StaticRouting", logAll);
    // LogComponentEnable("TcpSocketBase", logAll);
    LogComponentEnable("OnOffApplication", logInfo);
    LogComponentEnable("UdpClient", logAll);
    LogComponentEnable("PacketSink", logInfo);

    if (input.debug)
    {
        // ...
        if (input.debug > 1)
        {
            LogComponentEnable("MobilityHelper", logAll);
            LogComponentEnable("PositionAllocator", logAll);
            LogComponentEnable("LteHexGridEnbTopologyHelper", logAll);

            LogComponentEnable("HttpClient", logAll);
            LogComponentEnable("HttpServer", logAll);
           
            LogComponentEnable("EpcSgwPgwApplication", logAll);
            LogComponentEnable("EpcEnbApplication", logAll);
            LogComponentEnable("YansWifiHelper", logAll);
            
            // LogComponentEnable ("Ipv6Interface", logAll);
            // LogComponentEnable ("Ipv6RawSocketImpl", logAll);
            // LogComponentEnable("RandomWalk2d", logAll);
            // LogComponentEnable ("LteUeMac", logAll);
            LogComponentEnable("EpcUeNas", logAll);
            LogComponentEnable("LteNetDevice", logAll);
            LogComponentEnable("LteEnbNetDevice", logAll);
            LogComponentEnable("LteRlcTm", logAll);

            LogComponentEnable("Ipv4L3Protocol", logAll);
            LogComponentEnable("Ipv6L3Protocol", logAll);
            LogComponentEnable("Icmpv6L4Protocol", logAll);
            LogComponentEnable("Ipv4StaticRouting", logAll);
            LogComponentEnable("Ipv6StaticRouting", logAll);

            LogComponentEnable("YansWifiPhy", logAll);
            LogComponentEnable("MinstrelWifiManager", logAll);
            LogComponentEnable("YansWifiHelper", logAll);
            LogComponentEnable("WifiHelper", logAll);
        }
    }
}

/** Note: there's a compilation issue on OsX */
void
NetanimTrace(Ptr<ProSeHelper> d2dCtrl)
{
    NS_LOG_FUNCTION_NOARGS();
    char label[10] = "*";
    double animPollInterval = 1.0;
    string animationFile = string("d2d-anim.xml");
    AnimationInterface anim(animationFile);
    anim.SetMobilityPollInterval(Seconds(animPollInterval));
    anim.EnablePacketMetadata(true);

    uint32_t i = 0;
    // uint32_t nNodes = NodeList::GetNNodes ();
    Ptr<Node> node;
    Ptr<MobilityModel> mob;
    Ptr<LteUeNetDevice> udev;
    Ptr<LteEnbNetDevice> enbdev;

    for (NodeList::Iterator it = NodeList::Begin();
            it != NodeList::End(); ++it, ++i)
    {
        node = *it;
        int nDevs = node->GetNDevices();
        for (int j = 0; j < nDevs; j++)
        {
            mob = node->GetObject<MobilityModel> ();
            if (mob == 0)
            {
                anim.SetConstantPosition(node, 0, 0);
                anim.UpdateNodeSize(i, 10, 10);
                anim.UpdateNodeDescription(node, string(label));
                continue;
            }

            udev = node->GetDevice(j)->GetObject <LteUeNetDevice> ();
            if (udev)
            {
                if (i > 1) // TEMP
                {
                    sprintf(label, "dUe%0d", i);
                    anim.UpdateNodeColor(node, 0, 0, 194); // blue for dUes
                }
                else
                {
                    sprintf(label, "cUe%0d", i);
                    anim.UpdateNodeColor(node, 60, 181, 44); // green for cUes
                }
                anim.UpdateNodeSize(i, 10, 10);
                anim.UpdateNodeDescription(node, string(label));
            }
            else
            {
                enbdev = node->GetDevice(j)->GetObject <LteEnbNetDevice> ();
                if (enbdev)
                {
                    sprintf(label, "eNB%d", i);
                }
                anim.UpdateNodeSize(i, 20, 20);
                anim.UpdateNodeDescription(node, string(label));
            }
        }
    }
    anim.UpdateNodeDescription(d2dCtrl->GetPgwNode(), "PGW");
    anim.UpdateNodeDescription(d2dCtrl->GetProseFcnNode(), "ProSeFcn");
    NS_LOG_INFO("\tNetwork animation trace packets count: " << anim.GetTracePktCount());
}
