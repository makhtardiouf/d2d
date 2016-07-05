/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014-2016 Gyeongsang N. University
 * InfoCom Engineering Dept, South Korea
 * 
 * $Id: epc-prose-fcn.cc, 2016-07-05 18:56 +0900 makhtar $
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

/***
 *  CONSIDER USING the GeographicalPosition objects from the TV spectrum module to be merged for UE locations
 * it could be practical for ProseVANET too
 */

#include "ns3/lte-prose-common.h"
#include "ns3/lte-prose-helper.h"
#include "ns3/lte-utils.h"
#include "epc-prose-fcn.h"
#include <thread>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("EpcProSeFcnApp");
NS_OBJECT_ENSURE_REGISTERED (EpcProSeFcnApp);

TypeId
EpcProSeFcnApp::GetTypeId (void)
{
    static TypeId tid = TypeId ("ns3::EpcProSeFcnApp")
            .SetParent<Application> ()
            .AddConstructor<EpcProSeFcnApp> ()
            .AddAttribute ("Port",
                           "Port on which we listen for incoming packets.",
                           UintegerValue (80),
                           MakeUintegerAccessor (&EpcProSeFcnApp::m_srvPort),
                           MakeUintegerChecker<uint16_t> ())
            ;
    return tid;
}

EpcProSeFcnApp::EpcProSeFcnApp ()
{
    NS_LOG_FUNCTION (this);
    m_srvPort = 80;
}

EpcProSeFcnApp::~ EpcProSeFcnApp ()
{
    NS_LOG_FUNCTION (this);
    this->Dispose ();
}

void
EpcProSeFcnApp::DoInitialize ()
{
    NS_LOG_FUNCTION (this);
}

void
EpcProSeFcnApp::DoDispose ()
{
    NS_LOG_FUNCTION (this);
}

void
EpcProSeFcnApp::StartApplication (void)
{
    NS_LOG_FUNCTION (this);

}

void
EpcProSeFcnApp::StopApplication ()
{
    NS_LOG_FUNCTION (this);
}

/**
 * @brief EpcProSeFcnApp::SetupSrv initialize and start ProSeFcn's internal http server; 
 * Schedule discovery requests processing
 * @param n node of ProSeFcn
 */
void EpcProSeFcnApp::SetupSrv (Ptr<Node> n, Ipv4Address ipv4, Ipv6Address ipv6)
{
    NS_LOG_FUNCTION (this << " Setting up ProseFcn");
    if (n != 0)
        m_proseFcnNode = n;

    m_ip = ipv4;
    m_ip6 = ipv6;
    objFactory.SetTypeId ("ns3::HttpServer");
    objFactory.Set ("Protocol", TypeIdValue (TcpSocketFactory::GetTypeId ()));

    //*** Consider using the MAC addr set by the PointToPointHelper
    if (ns3::lteUseIpv6)
    {
        m_addr = Address (Inet6SocketAddress (m_ip6, m_srvPort));
        objFactory.Set ("Local", AddressValue (Inet6SocketAddress (m_ip6, m_srvPort)));
        NS_LOG_DEBUG ("\nusing IPv6 for ProseFcn " << m_ip6 << " " << m_addr);
    }
    else
    {
        m_addr = Address (m_ip);
        objFactory.Set ("Local", AddressValue (InetSocketAddress (m_ip, m_srvPort)));
    }

    app = objFactory.Create<Application> ();
    m_proseFcnNode->AddApplication (app);
    app->SetStartTime (Seconds (0.0));

    httpSrv = DynamicCast<HttpServer> (app);
    httpSrv->SetRecvPostCallback (MakeCallback (&EpcProSeFcnApp::SaveDiscoveryParams, this));
    httpSrv->SetResponseCallback (MakeCallback (&EpcProSeFcnApp::SendResponse, this));
}

/**
 * @brief EpcProSeFcnApp::GenerateAppTempID
 * @return up to 184 bits temporary ID for ProSeApp's AppCode
 */
string EpcProSeFcnApp::GenerateAppTempID ()
{
    Ptr<UniformRandomVariable> rvar = CreateObject<UniformRandomVariable> ();
    std::ostringstream str;
    size_t bytesSize = APP_TEMP_ID_SIZE; //  / 8;

    // GetInteger generates 0 or 1 by default - thus like binary
    for (size_t i = 0; i < bytesSize; i ++)
        str << rvar->GetInteger ();

    //  NS_LOG_DEBUG("\nGenerated temp app ID: " << str.str());
    return str.str ();
}

/**
 * @brief EpcProSeFcnApp::SaveDiscoveryParams
 * @param keyval Key-Values pairs of http POST parameters
 * @todo feedback a DISCOVERY_RESPONSE(response-announce) message to the Ue, with it's TransactionId
 */
void EpcProSeFcnApp::SaveDiscoveryParams (std::map<std::string, std::string> keyval)
{
    NS_LOG_FUNCTION (this);
    m_proseUeInfo inf;
    inf.EpUID = 0;
    char c[2] = "";

    for (std::map<string, string>::iterator it = keyval.begin (); it != keyval.end (); it ++)
    {
        // Uncomment to output the value of POST parameters
        NS_LOG_DEBUG ((string) it->first << " <> " << (string) it->second);

        if (it->first.compare ("TransactionId") == 0)
        {
            inf.transactionId = it->second;
        }
        else if (it->first.compare ("AppId") == 0)
        {
            inf.proseAppId = it->second;

            // Should not be here, feedback to Ue after successful Discovery announce request
            // else if (it->first.compare("AppCode") == 0)
            //   inf.proseAppCode = it->second;
        }
        else if (it->first.compare ("EpUID") == 0)
        {
            istringstream istr (it->second);
            istr >> inf.EpUID;
        }
        else if (it->first.compare ("Command") == 0)
        {
            istringstream istr (it->second);
            istr >> c;
            inf.Command = (ProSeApp::Command)atoi (c);
        }
    }

    m_discoveryParams.push_back (inf);
    std::clog << " * "; // indicate progress
    // NS_LOG_DEBUG(inf.transactionId << inf.proseAppId << inf.EpUID << inf.Command);

    if (! savingParams)
    {
        /**         
         * Wait the registration of at least 2 UEs' requests before processing them
         *
         * Disabled some assert in default-simulator-impl.cc to avoid blockage about                                                                                   
         * Thread safety 
         */
        Simulator::Schedule (Seconds (1.0), &EpcProSeFcnApp::ProcessDiscoveryRequests, this);
        // std::thread discoveryThread (&EpcProSeFcnApp::ProcessDiscoveryRequests, this);
        // discoveryThread.detach ();
    }
    savingParams = true;
}

/**
 * @brief Create lists of UEs which have the same AppId, different Command; then report matches
 * 
 *  - only do the check if the Command is different between 2 UEs: Monitoring/Announcing
 *  - pass matches to MatchReport - **iterate for each Ue**
 *  - check if distance is within D2D range, 
 *  if not put the target Ue in a 2nd list for standard Lte communication route
 *
 */
void EpcProSeFcnApp::ProcessDiscoveryRequests ()
{
    std::cout << "\n";
    NS_LOG_FUNCTION (this);
    savingParams = false;

    if (m_discoveryParams.size () < 2)
    {
        NS_LOG_DEBUG ("\nNot enough saved requests; trying again later...\n");
        // Will be re-triggered by SaveDiscov...
        return;
    }

    uint64_t uid1 = 0; // EpUID = IMSI
    uint64_t uid2 = 0;
    string appId1, appId2;
    ProSeApp::Command cmd1, cmd2;
    std::list<uint64_t> matches;

    // Order and remove consecutive duplicates
    m_discoveryParams.sort ();
    m_discoveryParams.unique ();
    NS_LOG_DEBUG ("\nProcessing " << (int) m_discoveryParams.size () <<
                  " discovery requests... Thread ID: " << std::this_thread::get_id ());

    m_proseUeInfo headUe;
    auto it = m_discoveryParams.begin ();
    for (; it != m_discoveryParams.end (); ++ it)
    {
        // Get the 1st dUE that is in ANNOUNCE mode - Not scheduled before
        cmd1 = it->Command;
        if (cmd1 != ProSeApp::ANNOUNCE)
            continue;

        headUe = * it;
        uid1 = it->EpUID;
        appId1 = it->proseAppId;
        NS_LOG_DEBUG ("Head: dUE" << uid1);
        break;
    }

    if (cmd1 != ProSeApp::ANNOUNCE)
    {
        std::clog << "\n\tNo dUE in ANNOUNCE mode found; retrying later" << std::endl;
        return;
    }

    int firstPeer = 0;
    m_proseUeInfo peerUe;
    std::list<m_proseUeInfo> peersList;

    std::clog << "\n\tPotential D2D candidates to " << "dUE" << (int) uid1 << ": [ ";
    for (it = m_discoveryParams.begin (); it != m_discoveryParams.end (); ++ it)
    {
        uid2 = it->EpUID;
        appId2 = it->proseAppId;
        cmd2 = it->Command;

        if ((cmd2 == ProSeApp::MONITOR) && (appId1.compare (appId2) == 0))
        {
            std::clog << "dUE" << uid2 << " ";
            matches.push_back (uid2);

            if (++ firstPeer < 1)
            {
                peerUe = * it;
                peersList.push_back (peerUe);
            }
        }
    }

    if (matches.size () > 0)
    {
        std::clog << " ]\n\n";
        /* Delist the head UE and serve remaining UEs in ANNONCING mode
         * Delist 1st peer from discovery list, as it will be busy communicating
         */
        m_discoveryParams.remove (headUe);
        // Remove only the 1st peer
        // m_discoveryParams.remove(peerUe);
        ReportMatch (uid1, matches);
        for (auto p : peersList)
            m_discoveryParams.remove (p);
    }
    else
    {
        std::clog << "NO MATCH FOUND! ]\n\n";
    }
    // Serve other head DUEs
    Simulator::Schedule (Seconds (3.0),
                         &EpcProSeFcnApp::ProcessDiscoveryRequests, this);
}

/**
 * @brief Feedback matches to requesting Ue, after removing peers located beyond Dmax
 * @param imsi = EpUID
 * @param matches: list of UEs matching the discovery request
 */
void EpcProSeFcnApp::ReportMatch (uint64_t uid, std::list<uint64_t> matches)
{
    NS_LOG_FUNCTION (this << " IMSI" << uid << " Thread ID: " << std::this_thread::get_id ());
    int totalDues = matches.size ();
    bool gotSource = false;
    bool gotPeer = false;
    double dist = 0.0;
    double avgDist = 0.0;

    Vector srcPos;
    Vector peerPos;
    Ptr<Node> n;
    Ptr<LteUeNetDevice> udev;
    Ptr<ProSeApp> app;

    this->m_comType = WIFI_ONLY;
    matchParams mp;
    int nDevs = 0;

    // Iterate over all nodes
    for (NodeList::Iterator it = NodeList::Begin (); it != NodeList::End (); ++ it)
    {
        n = * it;
        nDevs = n->GetNDevices ();
        for (int j = 0; j < nDevs; j ++)
        {
            udev = n->GetDevice (j)->GetObject <LteUeNetDevice> ();
            if (udev == 0)
                continue;

            if (! gotSource)
            {
                //*** Identify source UE
                if (uid != udev->GetImsi ())
                    continue;

                gotSource = true;
                app = n->GetObject<ProSeApp>();
                srcPos = n->GetObject<MobilityModel> ()->GetPosition ();

                // Rewind once, as peers might preceed the source UE in the NodeList
                it = NodeList::Begin ();
                break; // back to NodeList iteration
            }

            gotPeer = false;
            auto uidIt = matches.begin ();
            for (int peerIdx = 0; uidIt != matches.end (); uidIt ++, peerIdx ++)
            {
                if ((*uidIt) != udev->GetImsi ())
                    continue;

                peerPos = n->GetObject<MobilityModel> ()->GetPosition ();
                dist = CalculateDistance (srcPos, peerPos);
                avgDist = (avgDist + dist) / 2;
                // Min 3m away
                if(dist <= 3)
                    continue;
                
                mp.dist = dist;
                mp.uidIt = uidIt;
                mp.peer = n;
                mp.matches = matches;
                SplitMatches (mp);
                gotPeer = true;
                break; // out of 3rd for: matches
            }

            if (gotPeer)
                break; // out of 2nd for: LteUeNetDevice
        }
    }

    if (! gotSource)
    {
        NS_LOG_DEBUG ("Could not identify announcing UE; aborting ProSe match report");
        return;
    }

    // httpSrv->SendResponse...
    // For now call the ProSeApp Tx functions - instead of using HTTP response
    // it will invite matching UEs to join
    if (app == 0)
    {
        NS_LOG_WARN ("The ProSeApp is invalid");
        return;
    }

    // int totalDues = mp.matches.size () + mp.d2dMatches.size ();
    stringstream ss;
    ss << Simulator::Now ().GetSeconds () << " \t"
            << totalDues + mp.offLimitUEs << " \t\t"
            << totalDues << "\t\t" << avgDist 
            << "\t\t" << totalDues * 100 / (totalDues + mp.offLimitUEs);

    Simulator::Schedule (Seconds (1.0), &ns3::SaveDiscoveryStats, ss.str ());

    NS_LOG_DEBUG ("dUE" << uid << " has " << totalDues << " D2D candidates in range VS. "
                  << mp.offLimitUEs << " UEs off limit");

    /// One head DUE either does WiFi or LTE-I or LTE-D
    //if(haveLteInfraCom)
    //    return;

    static bool wifiInUse = false;
    if (//! wifiInUse && 
            mp.matches.size () > 0)
    {
        wifiInUse = true;
        Simulator::Schedule (Seconds (0.25), &ProSeApp::StartWifiMulticast, app, mp.matches);
        if(this->m_comType == WIFI_ONLY)
           return;
    }

    if (mp.d2dMatches.size () > 0)
    {
        // Don't switch transmission path too early if this UE does LTE-I 
        double startTime = 0.25;
        //if (mp.peer)
        //    startTime = 10.0;
        std::thread appThread (&ProSeApp::StartLteD2d, app, mp.d2dMatches, wifiInUse);
        appThread.detach ();
 
       // Simulator::Schedule (Seconds (startTime), &ProSeApp::StartLteD2d, app, mp.d2dMatches, wifiInUse);
    }
}

void EpcProSeFcnApp::SplitMatches (matchParams &m)
{

    Ptr<ProSeApp> app = m.peer->GetObject<ProSeApp>();

    switch (this->m_comType)
    {
        case LTE_D_ONLY:
            m.d2dMatches = m.matches;
            m.matches.clear ();           
            break;

        case LTE_I_ONLY:
            m.matches.clear ();
            m.d2dMatches.clear ();
            app->StartLteInfraTx (m.peer);
            break;

        case WIFI_ONLY:            
            m.d2dMatches.clear ();
            break;

            // ALL
        default:
            /*
             ** WIFI-adhoc disabled for now
             * Allow only 2 WIFI peers otherwise 
             */
            // if( (peerIdx > 1) && (dist <= ProSe::MAX_WLAN_DISTANCE))
            if ((m.dist > ProSe::MAX_WLAN_DISTANCE) && (m.dist <= ProSe::MAX_LTED2D_DISTANCE))
            {
                NS_LOG_WARN ("\n\tLTE-D peer IMSI" << * m.uidIt << " " << m.dist << "m away");
                // Remove peer from candidates                                     
                m.matches.remove (*m.uidIt);
                m.d2dMatches.push_back (*m.uidIt);
            }
            else if ((m.dist > ProSe::MAX_LTED2D_DISTANCE) && (m.dist <= 300))
            {
                NS_LOG_INFO ("\n\tLTE-infra peer IMSI" << * m.uidIt << " " << m.dist << "m away");
                m.offLimitUEs ++;
                m.matches.remove (*m.uidIt);
                //haveLteInfraCom = true;
                app->StartLteInfraTx (m.peer);
            }

    }
}

/**
 * @brief EpcProSeFcnApp::SendResponse send through HTTP the discovery response message
 * @todo complete this
 * @param status HTTP status code
 */
void EpcProSeFcnApp::SendResponse (HttpStatus_t status)
{
    std::string response;

    switch (status)
    {
        case OK:
        {
            response = "DISCOVERY_RESPONSE(response-announce)";
            // + send AppCode
            // app->RxResponse
            break;
        }

        case BAD_REQUEST:
        {
            response = "DISCOVERY_RESPONSE(response-reject)";
            break;
        }

        default:
            break;
    }
    // ....
    NS_LOG_DEBUG (response << " " << status);
}

} // ns-3
