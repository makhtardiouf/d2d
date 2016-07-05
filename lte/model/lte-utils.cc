/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014 Piotr Gawlowicz
 * Author: Piotr Gawlowicz <gawlowicz.p@gmail.com>
 *
 * Copyright (c) 2014-2016 Gyeongsang N. University
 * InfoCom Engineering Dept, South Korea
 * Parts adapted from lena-distributed-ffr.cc, with additional features**
 *
 * $Id: lte-utils.cc, 2016-05-26 19:01 +0900 makhtar $
 * Author: Makhtar Diouf
 *
 * This program is open source software released under the terms of the
 * GNU General Public License version 2 as published by the Free Software Foundation.
 * It is distributed WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include "lte-utils.h"
#include "ns3/simple-device-energy-model.h"
#include "ns3/li-ion-energy-source.h"
#include "ns3/energy-source-container.h"
#include "ns3/animation-interface.h"
#include "ns3/ipv4.h"
#include "ns3/ipv6.h"
#include "ns3/ipv4-interface.h"
#include "ns3/ipv6-interface.h"

#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("LteUtils");


static bool nodesPosInited = false;
static bool discoveryStatsInited = false;

void SaveDiscoveryStats (string line)
{
    NS_LOG_FUNCTION_NOARGS ();
    std::fstream logFile;
    std::string filename = "d2d-discovery.txt";

    // Append mode to aggregate data across multiple runs
    //    if (!discoveryStatsInited)
    logFile.open (filename.c_str (), std::fstream::out | std::fstream::app);

    if (! logFile.is_open ())
    {
        NS_LOG_WARN ("Can't open file discovery stats file " << filename);
        return;
    }
    if (! discoveryStatsInited)
    {
        struct stat filestat;
        stat (filename.c_str (), &filestat);

        if (filestat.st_size < 10)
            logFile << "%Time(s) \tCandidates \tDiscoveredUes \tAvgDistance(m) \tRatio(%)" << std::endl; // \tWlanUes

        discoveryStatsInited = true;
    }
    logFile << line
            // <<  "\t" << (int) ProSe::wlanUes
            << std::endl;

    std::clog << "\n\nwlanUes: " << ProSe::wlanUes << std::endl;
}

void SaveWifiDataStats (string line)
{
    NS_LOG_FUNCTION_NOARGS ();
    std::fstream logFile;
    std::string filename = "d2d-wlandata.txt";
    static bool inited = false;

    // Append mode to aggregate data across multiple runs
    if (! inited)
        logFile.open (filename.c_str (), std::fstream::out | std::fstream::trunc);
    else
        logFile.open (filename.c_str (), std::fstream::out | std::fstream::app);

    if (! logFile.is_open ())
    {
        NS_LOG_WARN ("Can't open file data stats file " << filename);
        return;
    }

    if (! inited)
    {
        struct stat filestat;
        stat (filename.c_str (), &filestat);
        if (filestat.st_size < 10)
            logFile << "%Time(s) \tIMSI \tRange(m) \tDataRate(Mbps)" << std::endl;
        inited = true;
    }
    logFile << line << std::endl;
}

void SaveLteDataStats (string line)
{
    NS_LOG_FUNCTION_NOARGS ();
    std::fstream logFile;
    std::string filename = "d2d-ltedata.txt";
    static bool inited = false;

    // Append mode to aggregate data accross multiple runs
    if (! inited)
        logFile.open (filename.c_str (), std::fstream::out | std::fstream::trunc);
    else
        logFile.open (filename.c_str (), std::fstream::out | std::fstream::app);

    if (! logFile.is_open ())
    {
        NS_LOG_WARN ("Can't open file lte data stats file " << filename);
        return;
    }

    if (! inited)
    {
        struct stat filestat;
        stat (filename.c_str (), &filestat);

        if (filestat.st_size < 10)
            logFile << "%Time(s) \tIMSI \tRange(m) \tDataRate(Mbps)" << std::endl;
        inited = true;
    }
    logFile << line << std::endl;
}

//*** Ref li-ion-energy-source.cc

static void
PrintCellInfo (uint64_t imsi, Ptr<LiIonEnergySource> es)
{
    NS_LOG_DEBUG ("Ue" << imsi << " Cell voltage: " << es->GetSupplyVoltage () <<
                  "V Remaining Capacity: " << es->GetRemainingEnergy () / (3.6 * 3600) << "Ah");

    /*  if (!Simulator::IsFinished ())
    {
        Simulator::Schedule (Seconds (5.0), &PrintCellInfo, es);
    } */
}

void TrackNodes (std::string filename)
{
    NS_LOG_FUNCTION_NOARGS ();
    std::ofstream logFile;
    static bool ueIsSet = false;

    if (nodesPosInited)
        logFile.open (filename.c_str (), std::ios_base::out | std::ios_base::app);
    else
    {
        logFile.open (filename.c_str (), std::ios_base::out | std::ios_base::trunc);
        logFile << "Time(s) \tNode \t\tXpos(m) \tYpos(m) \tBattery(Ah)" << std::endl;
    }

    if (! logFile.is_open ())
    {
        NS_LOG_ERROR ("Can't open file " << filename);
        return;
    }

    Vector pos;
    Ptr<Node> node;
    Ptr<LteUePhy> phy;
    Ptr<LteUeNetDevice> udev;

    // Energy
    Ptr<SimpleDeviceEnergyModel> sem;
    Ptr<EnergySourceContainer> esCont;
    Ptr<LiIonEnergySource> es;

    for (NodeList::Iterator it = NodeList::Begin (); it != NodeList::End (); ++ it)
    {
        node = * it;
        int nDevs = node->GetNDevices ();
        for (int j = 0; j < nDevs; j ++)
        {
            udev = node->GetDevice (j)->GetObject <LteUeNetDevice> ();
            if (udev)
            {
                pos = node->GetObject<MobilityModel> ()->GetPosition ();

                if (! ueIsSet)
                {
                    // Unrelated task: reduce the sampling period of DL stats
                    phy = udev->GetPhy ();
                    phy->SetAttribute ("RsrpSinrSamplePeriod", UintegerValue (500));

                    // Hook energy source
                    sem = CreateObject<SimpleDeviceEnergyModel> ();
                    esCont = CreateObject<EnergySourceContainer> ();
                    es = CreateObject<LiIonEnergySource> ();

                    esCont->Add (es);
                    es->SetNode (node);
                    sem->SetEnergySource (es);
                    es->AppendDeviceEnergyModel (sem);

                    sem->SetNode (node);
                    node->AggregateObject (esCont);
                    // discharge at 2.33 A for 1700 seconds
                    sem->SetCurrentA (2.33);
                    PrintCellInfo (udev->GetImsi (), es);
                }
                else
                {
                    es = DynamicCast<LiIonEnergySource>(node->GetObject<EnergySourceContainer>()->Get(0));
                }

                logFile << Simulator::Now ().GetSeconds () << " \tUe" << udev->GetImsi ()
                        << " \t" << pos.x << " \t" << pos.y << " \t"
                        << es->GetRemainingEnergy() / (3.6 * 3600)
                        << std::endl;

                break; // out of inner loop, go to next node
            }
            else if (! nodesPosInited)
            {
                // Output eNBs positions only once
                Ptr<LteEnbNetDevice> enbdev = node->GetDevice (j)->GetObject<LteEnbNetDevice> ();
                if (!enbdev)
                    continue;
                        
                pos = node->GetObject<MobilityModel> ()->GetPosition ();
                logFile << Simulator::Now ().GetSeconds () << " \teNB" << enbdev->GetCellId ()
                        << " \t" << pos.x << " \t" << pos.y << std::endl;

                break; // out of inner loop, go to next node       
            }
        }
    }
    // After first run - @todo Reset when doing multiple experiments
    if (! ueIsSet)
        ueIsSet = true;

    if (! nodesPosInited)
        nodesPosInited = true;

    logFile.close ();
    Simulator::Schedule (Seconds (5.0), &ns3::TrackNodes, filename);
}

/**
 * Generate Radio Map Environment to view SINR around eNBs
 * This stop the simulation right after
 * Not flexible enough at this point
 */
void GenRadioMap (Ptr<ProSeHelper> d2dCtrl)
{
    PrintEnbAttrListToFile ("OUTPUT/gnuplottable-enbs.txt");
    PrintUeAttrListToFile ("OUTPUT/gnuplottable-ues.txt");

    Ptr<RadioEnvironmentMapHelper> remHelper = CreateObject<RadioEnvironmentMapHelper> ();

    Ptr<LteSpectrumPhy> enbDlSpectrumPhy = d2dCtrl->macroEnbDevs.Get (0)->GetObject<LteEnbNetDevice> ()->GetPhy ()->GetDownlinkSpectrumPhy ();
    Ptr<SpectrumChannel> dlChannel = enbDlSpectrumPhy->GetChannel ();
    uint32_t dlChannelId = dlChannel->GetId ();
    NS_LOG_INFO ("DL ChannelId: " << dlChannelId);
    stringstream ss;
    ss << "/ChannelList/" << dlChannelId;
    double bound = 500; //d2dCtrl->cellRadius;

    remHelper->SetAttribute ("ChannelPath", StringValue (ss.str ()));
    remHelper->SetAttribute ("OutputFile", StringValue ("d2d-radiomap.txt"));
    remHelper->SetAttribute ("XMin", DoubleValue (- bound));
    remHelper->SetAttribute ("XMax", DoubleValue (bound));
    remHelper->SetAttribute ("XRes", UintegerValue (bound / 10));
    remHelper->SetAttribute ("YMin", DoubleValue (- bound));
    remHelper->SetAttribute ("YMax", DoubleValue (bound));
    remHelper->SetAttribute ("YRes", UintegerValue (bound / 10));
    remHelper->SetAttribute ("Z", DoubleValue (30.0));
    remHelper->SetAttribute ("StopWhenDone", BooleanValue (true));
    //    remHelper->SetAttribute("Earfcn", UintegerValue(100));
    remHelper->SetAttribute ("Bandwidth", UintegerValue (100));
    //    remHelper->SetAttribute("UseDataChannel", BooleanValue(false));
    remHelper->Install ();
}

//*** Unused ?

void ProcessUeSinr (uint16_t cellId, uint64_t imsi, uint16_t rnti, double sinrLinear)
{

    std::vector<int> d2dUes;
    std::vector<int> cellUes;
    std::vector<double> d2dSinr;
    std::vector<double> cellularSinr;
    if (imsi < 5)
    {
        d2dUes.push_back (imsi);
        d2dSinr.push_back (10 * log10 (sinrLinear));
    }
    else
    {
        cellUes.push_back (imsi);
        cellularSinr.push_back (10 * log10 (sinrLinear));
    }
}

void
PrintUeAttrListToFile (std::string filename)
{
    std::ofstream logFile;
    logFile.open (filename.c_str (), std::ios_base::out | std::ios_base::trunc);
    if (! logFile.is_open ())
    {
        NS_LOG_ERROR ("Can't open file " << filename);
        return;
    }
    Ptr<Node> node;
    Ptr<LteUeNetDevice> udev;

    for (NodeList::Iterator it = NodeList::Begin (); it != NodeList::End (); ++ it)
    {
        node = * it;
        int nDevs = node->GetNDevices ();
        for (int j = 0; j < nDevs; j ++)
        {
            udev = node->GetDevice (j)->GetObject <LteUeNetDevice> ();
            if (! udev)
                continue;
            
                Vector pos = node->GetObject<MobilityModel> ()->GetPosition ();
            logFile << "set label \"" << udev->GetImsi ()
                    << "\" at " << pos.x << "," << pos.y
                    << " left font \"Helvetica,4\" textcolor rgb \"grey\" "
                    << "front point pt 1 ps 0.3 lc rgb \"grey\" offset 0,0"
                    << std::endl;
        }
    }
}

void
PrintEnbAttrListToFile (std::string filename)
{
    std::ofstream logFile;
    logFile.open (filename.c_str (), std::ios_base::out | std::ios_base::trunc);
    if (! logFile.is_open ())
    {
        NS_LOG_ERROR ("Can't open file " << filename);
        return;
    }
    Ptr<Node> node;
    Ptr<LteEnbNetDevice> enbdev;

    for (NodeList::Iterator it = NodeList::Begin (); it != NodeList::End (); ++ it)
    {
        node = * it;
        int nDevs = node->GetNDevices ();
        for (int j = 0; j < nDevs; j ++)
        {
            enbdev = node->GetDevice (j)->GetObject <LteEnbNetDevice> ();
            if (! enbdev)
                continue;
            
                Vector pos = node->GetObject<MobilityModel> ()->GetPosition ();
            logFile << "set label \"" << enbdev->GetCellId ()
                    << "\" at " << pos.x << "," << pos.y
                    << " left font \"Helvetica,4\" textcolor rgb \"white\" "
                    << "front  point pt 2 ps 0.3 lc rgb \"white\" offset 0,0"
                    << std::endl;
        }
    }
}

} // ns-3 namespace

