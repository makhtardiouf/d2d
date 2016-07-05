/*
 * Copyright (c)2016 Gyeongsang N. University
 * InfoCom Engineering Dept, South Korea
 *
 * $Id: RRC36331_PC5.asn1.h, 2016-05-17 20:45 +0900 makhtar $
 * Advisor: Woonsup Lee 
 * Author: Makhtar Diouf 
 * *** Proximity-based Services LTE-Direct (D2D) ***
 * PC5 RRC messages, SideLink
 * See also lte-rrc-sap.h, lte-rrc-header.h for the integration
 * 
 * This program is open source software released under the terms of the
 * GNU General Public License version 2 as published by the Free Software Foundation.
 * It is distributed WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "lte-rrc-sidelink.h"
#include <bitset>

namespace ns3
{

/**
  SideLink MIB
  Transmitted by a UE transmitting SLSS, i.e. acting as synchronisation reference, via SL-BCH.
 
  Signalling radio bearer: N/A, RLC-SAP: TM
  Logical channel: SBCCH, Direction: UE to UE
 */
struct MasterInformationBlockSL
{
    // Bandwidth n6, n15, n25, n50, n75, n100
    // Number of RB used
    uint8_t slBandwidth = 25;

    int tddConfigSL = 1;

    // Number in which SLSS and SL-BCH are transmitted
    std::bitset<10> directFrameNumber;

    // Subframe inside the frame
    uint8_t directSubframeNumber = 0; // ~9

    // DUE in E-UTRAN coverage.
    bool inCoverage = true;
};

/**
 * SBCCH-SL-BCH-Message class is the set of RRC messages that may be sent
 * from the UE to the UE via SL-BCH on the SBCCH logical channel.
 * @Todo Integrate with lte-control-messages.cc for message types
 */
typedef MasterInformationBlockSL SBCCHSLBCHMessageType;

struct SBCCHSLBCHMessage
{
    SBCCHSLBCHMessageType message;
};

/*
 * D2D Com TS 36.331 $5.10 & P263 SideLink
 * Indicates E-UTRAN supports the Sidelink UE information procedure and
 * may contain SL communication related resource configuration information
 * 36.331 P263
 */
struct SystemInformationBlockType18
{

    struct commConfig
    {
        // Resources by which the UE is allowed to receive SL com
        SLCommRxPoolList commRxPool;

        // Res for UE to communicated on SL via a frequency other than the primary
        SLCommTxPoolList commTxPoolNormalCommon;

        // Config by which the UE is allowed to Tx/Rx synchro
        SLSyncConfigList commSyncConfig;
    };

    bool commTxAllowRelayCommon = false;
};

/*
 * D2D SideLink Direct Discovery  P263
 * Indicates E-UTRAN supports the SideLink UE information procedure and
 * may contain SL **discovery** related resource configuration information
 * 36.331 P263
 */
struct SystemInformationBlockType19
{

    struct discConfig
    {
        SLDiscRxPoolList discRxPool;
        
        // RRC_IDLE
        SLDiscTxPoolList discTxPoolCommon;
        
        // Optional
        SLDiscTxPowerInfoList discTxPowerInfo;
        SLSyncConfigList discSyncConfig;
    };
};

// Destination identified by the ProSe Layer-2 Group ID as specified in TS 23.303
typedef std::bitset<24> SlDestinationIdentity;
typedef std::list<SlDestinationIdentity> SlDestinationInfoList;

// Frequencies below mimics LteEnbNetDevice::DlEarfcn / Ul
struct SlCommTxResourceReq
{
    int carrierFreq = 200; // ARFCN Value EUTRAr9
    SlDestinationInfoList destinationInfoList; // 1..maxSL-Dest-r12
};

struct SidelinkUEInformation
{
    // frequency on which the UE is interested to receive sidelink com
    int commRxInterestedFreq = 200;
    
    SlCommTxResourceReq commTxResourceReq;

    // UE is interested to monitor sidelink discovery
    bool discRxInterest = true;
    
    // Number of discovery message(s) the UE wants to transmit every discovery period
    uint8_t discTxResourceReq = 1; // 1..63

    enum ue_Type
    {
        relayUE,
        remoteUE
    };
};


//******* Direct Discovery 

struct SlDiscTxResourceReq
{
    int carrierFreq = 200;
    uint8_t discTxResourceReq = 1; // 1..63
};

// R13 SidelinkUEInformation-v13x0-IEs 
typedef std::list<SlDiscTxResourceReq> SlDiscTxResourceReqPerFreqList;

struct discTxResourceReq
{
    // 1..maxFreq
    int carrierFreqDiscTx = 200 + 18100;
    SlDiscTxResourceReqPerFreqList discTxResourceReqAddFreq;
};

// number of separate Public Safety related discovery message(s) the UE wants 
// to transmit every discovery period
// SlDiscTxResourceReq discTxResourceReqPS;

// R13, Optional
/*
SLGapRequest discRxGapReq;
SLGapRequest discTxGapReq;
SLSysInfoReportList discSysInfoReportList;   
 * 
// list of acquired System Information from SIBs of cells on configured carrier frequencies
 std::list<int> discSysInfoReportList;
*/

} // End namespace
