/*
 * Copyright (c)2016 Gyeongsang N. University
 * InfoCom Engineering Dept, South Korea
 *
 * $Id: lte-rrc-sidelink.h, 2016-05-17 20:45 +0900 makhtar $
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
#include "ns3/ff-mac-common.h"
#include <list>
#include <bitset>

// Data structures are re-ordered
// SC = SideLink Communication
// PS = Public Safety
// R13 stuff removed or moved to lte-rrc-sidelink-r13.h

namespace ns3
{

//****************** DIRECT COMMUNICATION PART

// Note: for SLCPLen Cyclic prefix length, use declarations in ff-mac-common.h

/** 
 * @brief SL-PeriodComm indicates the period over which resources allocated 
 * in a cell for sidelink communication.
 * Value are in number of subframes. Value sf40 corresponds to 40 subframes
 * 
 * *** EUTRAN configures sf40, sf80, sf160 and sf320 for FDD, 
 * and the rest for TDD 0, then 1 ~ 5, then 6
 */
enum SLPeriodComm
{
    sf40 = 40, sf60 = 60, sf70 = 70, sf80 = 80, sf120 = 120,
    sf160 = 160, sf240 = 240, sf280 = 280, sf320 = 320, spare
};

/**
 *  SL-OffsetIndicator indicates the offset of the pool of resources relative 
 * to SFN 0 of the cell from which it was obtained or, when out of coverage, 
 * relative to DFN 0.
 */
enum SLOffsetIndicator
{
    small,
    large
};

// To be used as std::bitset<>

enum class SubframeBitmapSL
{
    bs4 = 4, // TDD
    bs8 = 8,
    bs12 = 12,
    bs16 = 16,
    bs30 = 30,
    bs40 = 40, // FDD
    bs42 = 42 // TDD
};

// SL-TF-ResourceConfig specifies a set of time/frequency resources

struct SLTFResourceConfig
{
    int prbNum = 25;
    int prbStart = 0;
    int prbEnd = 24;
    SLOffsetIndicator offsetIndicator = SLOffsetIndicator::small;

    std::bitset<(long unsigned int) SubframeBitmapSL::bs40> subframeBitmap;
};

//@brief SL-HoppingConfig indicates the hopping configuration

struct SLHoppingConfigComm
{
    int hoppingParameter = 0; // ~504

    enum numSubbands
    {
        ns1, ns2, ns4

    };
    int rbOffset = 0; // ~110

};

/** Bitmap which is used to indicate the set of available ‘k’ values 
 * to be used for sidelink communication (TS 36.213 14.1.1.3)
 */
typedef std::bitset<5> SLTRPTSubset; // 3~5

/** Identify a set of parameters configured for sidelink transmission, 
 * used for communication, discovery and synchronisation. 36.331 P425
 */
struct SLTxParameters
{
    // For sidelink power control; PSSCH, PSCCH...
    int alpha = 0;
    
    // pO_PSSCH,1 , pO_PSSCH,2 ...etc
    int p0 = -126; // ~31
};

/*********
 * SL-CommResourcePool information for an Individual Pool of resources 
 * for sidelink communication: control information + data
 */
struct SLCommResourcePool
{
    int scCPLen = normal;

    // Period over which resources are allocated in a cell for SC
    // see PSCCH period in TS 36.213 
    int scPeriod = sf40;
    SLTFResourceConfig scTFResourceConfig;

    int dataCPLen = normal;
    SLHoppingConfigComm dataHoppingConfig;

    struct ueSelectedResourceConfig
    {
        SLTFResourceConfig dataTFResourceConfig;
        SLTRPTSubset trptSubset;
    };

    struct rxParametersNCell
    {
        int tddConfig = 1; // 0 ~ 6
        int syncConfigIndex;
    };

    struct txParameters
    {
        SLTxParameters scTxParameters;
        SLTxParameters dataTxParameters;
    };
};

typedef std::list<SLCommResourcePool> SLCommTxPoolList;
//std::list<> SLCommTxPoolListExt_r13  ;

typedef std::list<SLCommResourcePool> SLCommRxPoolList;

struct SLCommTxPoolToAddMod
{
    int poolIdentity = 0;
    SLCommResourcePool pool;
};

typedef std::list<SLCommTxPoolToAddMod> SLCommTxPoolToAddModList;

struct MACMainConfigSL
{
    uint16_t periodicBSRTimerSL = 0;
    uint16_t retxBSRTimerSL = 0;
};

struct scheduled
{
    int slRNTI = 1;
    MACMainConfigSL macMainConfig;

    /**
       Indicates a pool of resources for SC when E-UTRAN schedules Tx resources 
       (i.e. when indices included in DCI format 5 indicate the actual data resources 
       to be used as specified in TS 36.212 [22, 5.3.3.1.9])
     */
    SLCommResourcePool scCommTxConfig;

    /** Modulation and Coding Scheme as defined in TS 36.212 [23, 14.2.1]. 
     * If not configured, the selection is up to UE implementation.
     */
    int mcs = 0; // 0~28
};


/** Used to release one or more individual pool entries used for sidelink 
 * transmission, for communication and discovery.
 */
typedef int SLTxPoolIdentity;
typedef std::list<SLTxPoolIdentity> SLTxPoolToReleaseList;

struct ueSelected
{
    //@brief Pool of transmission resources the UE is allowed to use while in RRC_CONNECTED.

    struct commTxPoolNormalDedicated
    {
        SLTxPoolToReleaseList poolToReleaseList;
        SLCommTxPoolToAddModList poolToAddModList;
    };
};

enum class _setupCommConfig
{
    // E-UTRAN schedules the transmission resources 
    // based on sidelink specific BSR from the UE
    scheduled,

    // UE selects the transmission resources 
    // from a pool of resources configured by E-UTRAN
    ueSelected
};

/** 
 * SL-CommConfig specifies the dedicated configuration information for sidelink communication.
 * In particular the transmission resource configuration on the primary frequency.
 */
struct SLCommConfig
{
     // _setupCommConfig
    enum SetupRelease_e commTxResources;
};



//****************** DIRECT DISCOVERY PART

/** 
 * SL-DiscResourcePool specifies the configuration information for an
 * individual pool of resources for sidelink discovery. 
 */
struct SLDiscResourcePool
{

    int cpLen = normal;

    enum class discPeriod
    {
        rf32 = 32, rf64 = 64, rf256 = 256,
        rf512 = 512, rf1024 = 1024, rf16v13x0 = 16
    };

    int numRetx = 1;
    int numRepetition = 1;
    SLTFResourceConfig tfResourceConfig;

    struct txParameters
    {
        SLTxParameters txParametersGeneral;

        struct ueSelectedResourceConfig
        {

            enum poolSelection
            {
                rsrpBased, // see SLPoolSelectionConfig below
                random
            };
        };

        enum txProbability
        {
            p25 = 25, p50 = 50, p100 = 100
        };
    };

    struct rxParameters
    {
        // TDDConfig 1~5 configured by EUTRAN
        int tddConfig = 1;
        int syncConfigIndex = 0;
    };

    enum _setup
    {
        rf4 = 4, rf7 = 7, rf8 = 8, rf14 = 14
    };

    // R13

    enum discPeriodR13
    {
        release = 0,
        setup = rf14
    };
};

std::list<SLDiscResourcePool> SLDiscTxPoolList;

std::list<SLDiscResourcePool> SLDiscRxPoolList;


//@brief SL-HoppingConfig indicates the hopping configuration

struct SLHoppingConfigDisc
{
    int a = 1, b = 1;

    enum c
    {
        n1, n5
    };
};

struct SLTFIndexPair
{
    int discSFIndex = 0;
    int discPRBIndex = 0;
};
typedef std::list<SLTFIndexPair> SLTFIndexPairList;

struct scheduledDC
{
    SLDiscResourcePool discTxConfig;
    SLTFIndexPairList discTFIndexList;
    SLHoppingConfigDisc discHoppingConfig;
};


struct SLDiscTxPoolToAddMod
{
    int poolIdentity;
    SLDiscResourcePool pool;
};
typedef std::list<SLDiscTxPoolToAddMod> SLDiscTxPoolToAddModList;

struct ueSelectedDC
{

    struct discTxPoolDedicated
    {
        SLTxPoolToReleaseList poolToReleaseList;
        SLDiscTxPoolToAddModList poolToAddModList;
    };
};

enum class _setupDiscConfig
{
    scheduledDC,
    ueSelectedDC
};

struct SLTFIndexPairb
{
    int discSFIndexb = 0;
    int discPRBIndexb = 0;
};
typedef std::list<SLTFIndexPairb> SLTFIndexPairListb;

struct _setupSltf
{
    SLTFIndexPairListb discTFIndexListb;
};

//@brief SL-DiscConfig specifies the dedicated configuration information for sidelink DISCOVERY

struct SLDiscConfig
{
    // _setupDiscConfig
    enum SetupRelease_e discTxResources;

    // _setupSltf
    enum SetupRelease_e discTFIndexListv1260;
};


struct SLPoolSelectionConfig
{
    /** RSRP-RangeSL2 0~7 P374 TS36.331
     * 0 corresponds to -infinity, value 1 to -110dBm, value 2 = -100dBm, 
     * and so on (i.e. in steps of 10dBm) until value 6 = -60dBm;
     * value 7 corresponds to +infinity.
     */
    int threshLow = 1;
    int threshHigh = 6;  // ThresholdEutra::THRESHOLD_RSRP; ?
};

/** 
 * SL-DiscTxPowerInfo specifies power control parameters for one or more power classes.
 * maximum transmit power a UE configured with the concerned
 * range class: short, medium, long see TS 24.333 [70, 4.2.11]. 
 */ 
struct SLDiscTxPowerInfo
{
    double discMaxTxPower = 23.0;
};

std::list<SLDiscTxPowerInfo> SLDiscTxPowerInfoList;

//int SLOffsetIndicatorSync = SLOffsetIndicator.small;

/** 
 * @brief SL-Priority indicates the priority of resource pool used 
 * for sidelink communication, see TS 36.211 [21]. 
 */
typedef int SLPriority_r13;
std::list<SLPriority_r13> SLPriorityList_r13;

// For Public Safety (PS) SLSS
bool syncTxPeriodic_r13 = true;

/** 
 * SL-SyncConfig specifies the configuration information concerning RX 
 * of synchronization signals from neighbouring cells and
 * synchronization signals TX for sidelink discovery and communication
 */
struct SLSyncConfig
{
    int syncCPLen = normal;
    int syncOffsetIndicator = SLOffsetIndicator::small;
    /**
     SLSSID identifies a cell and is used by the receiving UE to detect asynchronous neighbouring cells,
      and by transmitting UEs to extend the synchronization signals beyond the cell’s coverage area.
     */
    int slSsid = 1;

    struct txParameters
    {
        SLTxParameters syncTxParameters;
        
        // RSRP-RangeSL2-r12 
        int syncTxThreshIC = 1; // = -110dBm ~ -60 P374 36.331
        
        std::bitset<19> syncInfoReserved;
    };

    struct rxParamsNCell
    {
        int physCellId = 1;
    };

    enum discSyncWindow
    {
        w1, w2
    };
};

std::list<SLSyncConfig> SLSyncConfigList;

} // end namespace
