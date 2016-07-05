/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014-2016 Gyeongsang N. University
 * InfoCom Engineering Dept, South Korea
 * 
 * $Id: lte-proseapp.h, 2016-05-31 16:40 +0900 makhtar $
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
#ifndef LTE_PROSE_APP_H
#define	LTE_PROSE_APP_H

#include <ns3/lte-spectrum-phy.h>
#include <ns3/yans-wifi-helper.h>
#include <ns3/wifi-helper.h>
#include <ns3/nqos-wifi-mac-helper.h>
#include <ns3/mobility-helper.h>

#include <ns3/event-id.h>
#include <ns3/callback.h>
#include <ns3/packet-sink.h>
#include <ns3/udp-client.h>
#include <ns3/bulk-send-application.h>
#include <ns3/ipv4-address-helper.h>
#include "ns3/lte-prose-d2d-helper.h"
#include "ns3/traffic-control-helper.h"

#include "lte-prose-common.h"
#include "ns3/http-client.h"

using namespace std;

namespace ns3
{

class ProSeApp : public Application
{
public:
    static TypeId GetTypeId(void);

    ProSeApp();
    ProSeApp(Ptr<Node> n);
    ~ProSeApp();

    void DoInitialize();

    /**
     * Parameters passed to DataRadioBearerSetupRequest ()
     * FROM epc-enb-s1-sap.h  - UNUSED for now
     */
    struct DataRadioBearerSetupRequestParameters
    {
        uint16_t rnti; /**< the RNTI identifying the target UE */
        EpsBearer bearer; /**< the characteristics of the bearer */
        uint8_t bearerId; /**< the EPS Bearer Identifier */
        uint32_t gtpTeid; /**< S1-bearer GTP tunnel endpoint identifier, see 36.423 9.2.1 */
        Ipv4Address transportLayerAddress; /**< IPv4 Address of the SGW, see 36.423 9.2.1 */
        Ipv6Address transportLayerAddress6; /**< IPv6 Address of the SGW */
    };

    /**
     * \ingroup ProSe
     * Discovery command to be sent to ProSeFcn,
     * TS 24.334 $12.2.2.2
     */
    typedef enum _Command
    {
        RESERVED = 0,
        ANNOUNCE = 1,
        MONITOR = 2,
        // 3 ~ 255
        UNUSED
    } Command;

    /**
     * EPC-level discovery ranges 
     * TS 24.334 $12.3.2.8
     */
    enum RangeClass
    {
        r0 = 0, // Reserved
        r1 = 1, // ~50m
        r2 = 2, // ~ 100m
        r3 = 3, // ~ 200m
        r4 = 4 // ~ 500m
    };

    void RequestDiscovery(Command cmd);

    /**
     * \ingroup ProSe
     * @brief Construct and set the Application ID used in discovery request
     * Default Application ID matches all countries, telcos, apps...
     * Wildcard matches all~
     * @param plmnID: MCC + MNC
     * @param l1, l2, l3 - levels: business category, name, shop ID...
     * Example proseAppId "mcc300.mnc165.ProSeApp.Shops.Sports.Surfing"
     * See TS.23.003 $24.2.1 Numbering, addressing...
     */
    void SetAppId(string plmnID = "mcc*.mnc*", string l1 = "*", string l2 = "*", string l3 = "*");

    // should return the struct, not the string

    std::string GetAppId()
    {
        return proseAppId;
    }

    /**
     * \ingroup ProSe
     * Default application Code, global scope, matches all ...
     * @param plmnID: Scope(2 bits) + E(1 bit) + MCC + MNC(0~10 bits each)
     * @param tempID: allocated by ProSeFcn, format undefined by TS; length: 184 bits - plmnID = 161
     * See TS.23.003 $24.3 & lte-prose-common.h
     */
    void SetAppCode(string plmnID = "00000", string tempID = "*");

    std::string GetAppCode()
    {
        return proseAppCode;
    }

    //template<class A>
    void SetupApp(Ptr<Node> n, Ipv6Address srvIp6, Ipv4Address srvIp = Ipv4Address::GetAny(), uint16_t port = 80);

    void SetIp(Ipv4Address ip)
    {
        m_clientIp = ip;
    }

    void SetIp(Ipv6Address ip)
    {
        m_clientIp6 = ip;
    }

    void SetEpUID(uint64_t uid)
    {
        epUID = uid;
    }

    uint64_t GetEpUID()
    {
        return epUID;
    }

    void SetUeNetDevice(Ptr<LteUeNetDevice> dev)
    {
        m_ueNetDevice = dev;
    }

    Ptr<LteUeNetDevice> GetUeNetDevice()
    {
        return m_ueNetDevice;
    }

    void ProbeLteDataStats(bool isD2d = false);
    void ProbeLteDataStatsInfra ();
    
    //*** LTE Direct 

    // Dettach from EPC/eNB
    void Dettach(Ptr<LteUeNetDevice> ueLteDevice);

    void SetD2dLink(Ptr<Node> peerNode, ProSe::DirectComParams &params);

    void SetD2dSideLinkChannel(Ptr<LteUeNetDevice> peerNetDev);

    void StartLteD2d(std::list<uint64_t> lteMatches, bool wifiInUse);

    void StartLteD2dTx(bool noWifi);

    void JoinLteD2d(ProSe::DirectComParams params, bool wifiInUse);

    void AtComEnd(bool isD2d = false);


    //*** LTE infrastructure (through eNB / EPC)
    void StartLteInfraTx(Ptr<Node> peer);

    void JoinLteInfra(ProSe::DirectComParams params);

    Ipv4Address SetupNetwork(NetDeviceContainer devC, bool isWifi = true);

    Ptr<Application> GetHttpClient()
    {
        return clientApp;
    }

    //*** WiFi ad hoc

    void ProbeWifiDataStats();

    void StartWifiMulticast(std::list<uint64_t> matchesInRange);

    void JoinWifiMulticast(ProSe::DirectComParams params);

    void HookWifiPacketSink(Ptr<PacketSink> sink);

    void TurnOffWifi();

protected:
    void DoDispose(void);

private:
    virtual void StartApplication(void);
    virtual void StopApplication(void);

    struct _proseAppIdStruct
    {
        string plmnID; // MCC + MNC
        // levels
        string l0; // = "ProseApp"
        string l1;
        string l2;
        string l3;

    } proseAppIdStruct;
    std::string proseAppId;

    struct _proseAppCodeStruct
    {
        string plmnID;
        string tempID;
    } proseAppCodeStruct;

    std::string proseAppCode;
    std::string transactionId = "";

    /**
     * \ingroup ProSe
     * EPC Prose User ID = IMSI , 32 bits allocated by ProSeFcn
     * TS.23.003 $24.4.2
     * Using uint64_t because of LteUeNetDevice::GetImsi(), to avoid down conversion
     */
    uint64_t epUID;

    Ptr<Node> m_peerNode;
    Ptr<Node> m_proseAppNode;
    Ptr<LteUeNetDevice> m_ueNetDevice;
    Ipv4StaticRoutingHelper m_multicastHlte;

    ObjectFactory objFactory;
    Ptr<Application> clientApp;

    double m_wifiComStart = 0;
    double m_comDuration = 45.0;        // sec
    double m_comStartDelay = 0.25;
    string m_comDataRate = "10Mbps";    // upper bound limit of 11b is 11Mbps    
    int m_packetSize = 1420;            // If more than 536 bytes, D2D packets will be segmented 

    string m_lteRxType = "LTE-I"; // indicator for Stats: Infrastructure or Direct

    ProSe::DirectComParams m_params; // local copy for stats

    Ipv4Address m_peerIp;
    Ipv6Address m_peerIp6;
    Address m_srvAddr; //!< Remote peer address
    Ipv4Address m_srvIp;
    Ipv6Address m_srvIp6;
    uint16_t m_srvPort; //!< Remote peer port

    InternetStackHelper m_inet;
    Ptr<PacketSink> m_peerPacketSink;
    Ptr<PacketSink> m_peerPacketSinkInfra;
    Ptr<PacketSink> m_peerPacketSinkWifi;

    EventId m_lteStatsProbEvent;
    EventId m_lteD2dStatsProbEvent;

    //*** WLAN
    Ipv4AddressHelper ipv4Hwlan;
    double m_wlanTotalRxBytes = 0.0;
    ApplicationContainer wlanRxC;
    bool hookedWifiSink = false;
    Ptr<NetDevice> m_inputInterface;
    Ipv4StaticRoutingHelper m_multicastH;

public:

    uint16_t m_clientPort;  //!< Port of this sender
    Ipv4Address m_clientIp; //!< IP of this sender
    Ipv6Address m_clientIp6;
    Address m_clientAddr;

};

} // ns-3
#endif	/* LTE_PROSE_APP_H */
