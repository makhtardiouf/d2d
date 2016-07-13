/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014-2016 Gyeongsang N. University
 * InfoCom Engineering Dept, South Korea
 *
 * $Id: epc-prose-fcn.h, 2016-05-26 19:01 +0900 makhtar $
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
#ifndef EPC_PROSEFCN_H
#define	EPC_PROSEFCN_H

#include "lte-prose-common.h"
#include "lte-proseapp.h"
#include "ns3/http-server.h"

namespace ns3
{

/**
 * @brief The EpcProSeFcnApp class for ProSe Function Entity
 * \ingroup lte-prose
 */
class EpcProSeFcnApp : public Application
{
public:
    static TypeId GetTypeId(void);

    EpcProSeFcnApp();
    ~EpcProSeFcnApp();

    void DoInitialize();
    void DoDispose();

    void SetPort(uint8_t);

    uint16_t
    GetPort()
    {
        return m_srvPort;
    }

    Ipv4Address
    GetIp()
    {
        return m_ip;
    }

    void SetupSrv(Ptr<Node> n, Ipv4Address ipv4, Ipv6Address ipv6);

    /**
     * \ingroup lte-prose
     * length: 184 bits - plmnID = 161 bits
     * Format undefined by TS
     * @return Temporary ID to be allocated to ProSeApp
     */
    std::string GenerateAppTempID();

private:

#define APP_TEMP_ID_SIZE  161 // bits

    /**
     Limit simulations to one particular communication type or all
     */
    typedef enum {
        LTE_D_ONLY,
        LTE_I_ONLY,
        WIFI_ONLY,
        ALL
        
    } ComType;
    
    /**
     * \ingroup lte-prose
     * TS 24.334 section $12.2.2.8  [0-255]
     * See TS.23.003 $24 for numbering, addressing...
     */
    enum
    {
        RESERVED = 0,
        INVALID_APPLICATION = 1,
        UNKNOWN_PROSE_APPLICATION_ID,
        UE_AUTHORISATION_FAILURE,

        UNKNOWN_PROSE_APPLICATION_CODE,
        INVALID_MIC,
        INVALID_UTC_BASED_COUNTER,
        INVALID_MESSAGE_FORMAT,
        SCOPE_VIOLATION_IN_PROSE_APPLICATION_ID // 8

    } PC3_ProtoControlCause;

           
    typedef struct _mp {
        double dist;
        Ptr<Node> peer;
        int offLimitUEs = 0;
        std::list<uint64_t>::iterator uidIt;        
        std::list<uint64_t> matches;
        std::list<uint64_t> d2dMatches;
    } matchParams ;
    
    
    virtual void StartApplication(void);

    virtual void StopApplication(void);

    void SendResponse(HttpStatus_t status);

    void SaveDiscoveryParams(std::map<std::string, std::string> keyval);

    void ProcessDiscoveryRequests();

    void ReportMatch(uint64_t imsi1, std::list<uint64_t> matches);

    void SplitMatches(matchParams &m);

    
    public:
        
 typedef struct _proseUeInfo
    {
        std::string transactionId;
        uint64_t EpUID;
        std::string proseAppId;
        // std::string proseAppCode; // should not be here
        ProSeApp::Command Command;
   
        _proseUeInfo()
        {
            ;
        }

        friend bool operator==(const _proseUeInfo &a, const _proseUeInfo &b)
        {
            return ( (a.EpUID == b.EpUID));
        } // && (a.Command == b.Command) );}

        friend bool operator<(const _proseUeInfo&a, const _proseUeInfo &b)
        {
            return ( (a.EpUID < b.EpUID));
        } // && (a.Command < b.Command) );}

        bool Matches(std::string appId)
        {
            return (this->proseAppId.compare(appId) == 0);
        }

    } m_proseUeInfo;

    Ipv4Address m_ip;
    Ipv6Address m_ip6;
    Address m_addr;
    uint16_t m_srvPort;

    Ptr<Application> app;
    Ptr<HttpServer> httpSrv;
    ObjectFactory objFactory;

    Ptr<Node> m_proseFcnNode;
    volatile bool savingParams = false;
    std::list<m_proseUeInfo> m_discoveryParams;

    //  uint32_t m_sent; //!< Counter for sent packets
    Address m_peerAddress; //!< Remote peer/client address
    Ipv4Address m_peerIp;
    uint16_t m_peerPort;

    ComType m_comType = ALL;
};

} // ns-3

#endif	/* EPC_PROSEFCN_H */
