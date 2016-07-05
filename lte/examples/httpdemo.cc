/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009
 * Tom handerson
 * Updated for the latest ns-3-dev   Mahtar Diouf 
 * $Id: httpdemo.cc, 2016-02-23 21:40 +0900 makhtar $
 */

#include <ns3/log.h>
#include <ns3/config-store-module.h>
#include <ns3/command-line.h>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-module.h"
#include "ns3/ipv4-list-routing-helper.h"
#include <ns3/olsr-module.h>
#include <ns3/olsr-helper.h>
#include "ns3/point-to-point-module.h"

#include "ns3/http-client.h"
#include "ns3/http-server.h"


using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Http");

int main(int argc, char *argv[])
{
    LogLevel logLevel = (LogLevel) (LOG_PREFIX_FUNC | LOG_PREFIX_TIME | LOG_LEVEL_ALL);

    LogComponentEnable("Http", logLevel);
    LogComponentEnable("HttpClient", logLevel);
    LogComponentEnable("HttpServer", logLevel);
    LogComponentEnable("Http", logLevel);

    uint32_t totalTime = 40;
    uint32_t dataStart = 5;
    uint32_t nNodes = 2;
    std::string delay = "2ms";
    std::string dataRate = "1Mbps";

    // Allow users to override the default parameters and set it to new ones from CommandLine.
    CommandLine cmd;
    /*
     * Parameters for the p2p link
     */
    cmd.AddValue("DataRate", "The data rate for the link", dataRate);
    cmd.AddValue("Delay", "The delay for the link", delay);
    cmd.Parse(argc, argv);

    SeedManager::SetSeed(99);
    SeedManager::SetRun(1);

    //  RandomVariable::UseGlobalSeed (1, 1, 2, 3, 5, 8);
    //DefaultValue::Bind ("DropTailQueue::m_maxPackets", 30);   

    // Here, we will explicitly create two nodes.  In more sophisticated
    // topologies, we could configure a node factory.
    NS_LOG_INFO("Create nodes.");
    NodeContainer c;
    c.Create(nNodes);

    NS_LOG_INFO("Enabling OLSR Routing.");
    OlsrHelper olsr;

    Ipv4StaticRoutingHelper staticRouting;
    Ipv4ListRoutingHelper list;
    list.Add(staticRouting, 0);
    list.Add(olsr, 10);

    InternetStackHelper internet_olsr;
    internet_olsr.SetRoutingHelper(list); // has effect on the next Install ()
    internet_olsr.Install(c);

    //    InternetStackHelper internet;
    //    internet.Install(c);

    // We create the channels first without any IP addressing information
    NS_LOG_INFO("Create channels.");
    PointToPointHelper p2p;
    p2p.SetDeviceAttribute("DataRate", StringValue(dataRate));
    p2p.SetChannelAttribute("Delay", StringValue(delay));
    NetDeviceContainer devices = p2p.Install(c);

    // Later, we add IP addresses.  
    NS_LOG_INFO("Assign IP Addresses.");
    Ipv4AddressHelper ipv4;
    ipv4.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer ipInterfaces = ipv4.Assign(devices);

    NS_LOG_INFO("Create Applications.");

    //have to use low level/internal APIs here since I didn't make a helper

    //add the server to node0
    ObjectFactory webServerFactory;
    webServerFactory.SetTypeId("ns3::HttpServer");
    //these can be used to configure the parameters of the connection
    //webServerFactory.Set ("Protocol", TypeIdValue (TcpSocketFactory::GetTypeId ()));
    //webServerFactory.Set ("Local", AddressValue (InetSocketAddress (Ipv4Address::GetAny (), 80)));
    Ptr<Application> serverApplication = webServerFactory.Create<Application> ();
    c.Get(0)->AddApplication(serverApplication);
    serverApplication->SetStartTime(Seconds(0.0));

    //add the client to node1
    ObjectFactory webClientFactory;
    webClientFactory.SetTypeId("ns3::HttpClient");
    webClientFactory.Set("Remote", AddressValue(InetSocketAddress(ipInterfaces.GetAddress(0), 80)));
    Ptr<Application> clientApplication = webClientFactory.Create<Application> ();
    c.Get(1)->AddApplication(clientApplication);
    clientApplication->SetStartTime(Seconds(dataStart));

    p2p.EnablePcapAll("simple-http");
    p2p.EnableAsciiAll("simple-http.tr");

    NS_LOG_INFO("Run Simulation.");
    Simulator::Stop(Seconds(totalTime));
    Simulator::Run();
    Simulator::Destroy();
}
