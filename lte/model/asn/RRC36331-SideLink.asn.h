
// Note: Adhoc ASN1 converter, by Makhtar Diouf
#include "RRC36331-SL20160425.asn_typedefs.h"
// Data structures are reodered
// Removed redundant r13 stuff

struct SLCommTxPoolToAddMod
{
    SLTxPoolIdentity poolIdentity;
    SLCommResourcePool pool;
};

std::list<SLCommTxPoolToAddMod> SLCommTxPoolToAddModList;

// The IE SL-CommConfig specifies the dedicated configuration information for sidelink communication. 
// In particular it concerns the transmission resource configuration for sidelink communication on the primary frequency.

struct scheduled
{
    int slRNTI;
    MACMainConfigSL macMainConfig;
    SLCommResourcePool scCommTxConfig;
    int mcs = 0; // 0~28
};

struct ueSelected
{

    struct commTxPoolNormalDedicated
    {
        SLTxPoolToReleaseList poolToReleaseList;
        SLCommTxPoolToAddModList poolToAddModList;
    };

    enum setup
    {
        scheduled,
        ueSelected
    };

    struct SLCommConfig
    {

        enum commTxResources
        {
            release = 0,
            setup = setup.ueSelected
        };
    };

    std::list<> LogicalChGroupInfoList_r13;

    struct MACMainConfigSL
    {
        PeriodicBSRTimer periodicBSRTimerSL;
        RetxBSRTimer retxBSRTimerSL;
    };


    // commTxAllowRelayDedicated
    // Indicates for whether the UE is not allowed to transmit relay related data using the configured dedicated transmission resources i.e. neither via scheduled nor via UE selected resources.
    // commTxPoolNormalDedicated
    // Indicates a pool of transmission resources the UE is allowed to use while in RRC_CONNECTED.
    // logicalChGroupInfoList
    // Indicates for each logical channel group the list of associated priorities, used as specified in TS 36.321 [6].
    // mcs
    // Indicates the Modulation and Coding Scheme as defined in TS 36.212 [23, 14.2.1]. If not configured, the selection of Modulation and Coding Scheme is up to UE implementation.
    // multipleTxAllowed
    // Indicates whether or not multiple transmissions in one SC period is allowed.
    // sc-CommTxConfig
    // Indicates a pool of resources for SC when E-UTRAN schedules Tx resources (i.e. when indices included in DCI format 5 indicate the actual data resources to be used as specified in TS 36.212 [22, 5.3.3.1.9]).
    // scheduled
    // Indicates the configuration for the case E-UTRAN schedules the transmission resources based on sidelink specific BSR from the UE.
    // ue-Selected
    // Indicates the configuration for the case the UE selects the transmission resources from a pool of resources configured by E-UTRAN.
    //
    //
    // SL-CommResourcePool
    // The IE SL-CommResourcePool specifies the configuration information for an individual pool of resources for sidelink communication. The IE covers the configuration of both the sidelink control information and the data.
    // SL-CommResourcePool information element
    // -- ASN1START

    std::bitset<35> SLTRPTSubset;

    struct SLCommResourcePool
    {
        SLCPLen scCPLen;
        SLPeriodComm scPeriod;
        SLTFResourceConfig scTFResourceConfig;
        SLCPLen dataCPLen;
        SLHoppingConfigComm dataHoppingConfig;

        struct ueSelectedResourceConfig
        {
            SLTFResourceConfig dataTFResourceConfig;
            SLTRPTSubset trptSubset;

        };

        struct rxParametersNCell
        {
            TDDConfig tddConfig;
            int syncConfigIndex;

        };

        struct txParameters
        {
            SLTxParameters scTxParameters;
            SLTxParameters dataTxParameters;

        };

        //  SLPriorityList_r13 priorityList_r13; //     

    };

    std::list<SLCommResourcePool> SLCommTxPoolList;

    //std::list<> SLCommTxPoolListExt_r13  ;

    std::list<SLCommResourcePool> SLCommRxPoolList;


    // The IE SL-CP-Len indicates the cyclic prefix length, see TS 36.211 [21].
    // SL-CP-Len information element

    // The IE SL-DiscConfig specifies the dedicated configuration information for sidelink discovery.
    // SL-DiscConfig information element

    struct scheduled
    {
        SLDiscResourcePool discTxConfig;
        SLTFIndexPairList discTFIndexList;
        SLHoppingConfigDisc discHoppingConfig;

    };

    struct ueSelected
    {

        struct discTxPoolDedicated
        {
            SLTxPoolToReleaseList poolToReleaseList;
            SLDiscTxPoolToAddModList poolToAddModList;
        };
    };

    enum setup
    {
        scheduled,
        ueSelected
    };

    struct SLDiscConfig
    {

        enum discTxResources
        {
            release = 0,
            setup = setup.ueSelected
        };

        enum discTFIndexListv1260
        {

            release = 0,

            struct setup
            {
                SLTFIndexPairListb discTFIndexListb;
            };
        };

        enum discTxInterFreqInfo_r13
        {

            release = 0,

            struct setup
            {
                ARFCNValueEUTRAr9 discTxCarrierFreq_r13;
                SLDiscTxRefCarrierDedicated_r13 discTxRefCarrierDedicated_r13;
                SLDiscTxInfoInterFreqListExt_r13 discTxInfoInterFreqListExt_r13;
            };
        };
    };

    enum gapRequestsAllowedDedicated_r13
    {
        true,
        false
    };

    enum discRxGapConfig_r13
    {
        release = 0,
        SLGapConfig_r13 setup;
    };

    enum discTxGapConfig_r13
    {
        release = 0,
        SLGapConfig_r13 setup;
    };

    enum discSysInfoReportConfig_r13
    {
        release = 0,
        SLDiscSysInfoReportFreqList_r13 setup;
    };

};

struct SLDiscTxInfoInterFreqListExt_r13
{
    std::list<> discTxFreqToAddModList_r13;
    std::list<> discTxFreqToReleaseList_r13;
};

struct SLDiscTxResourceInfoPerFreq_r13
{
    struct SLDiscTxResourceInfoPerFreq_r13
    ARFCNValueEUTRAr9 discTxCarrierFreq_r13;
    SLDiscTxResource_r13 discTxResources_r13;
    SLDiscTxResource_r13 discTxResourcesPS_r13;
    SLDiscTxRefCarrierDedicated_r13 discTxRefCarrierDedicated_r13;
    CellSelectionInfoNFreq_r13 cellSelectionInfo_r13;
};

enum SLDiscTxResource_r13
{

    release = 0,

    enum setup
    {

        struct scheduled_r13
        {
            SLDiscResourcePool discTxConfig_r13;
            SLTFIndexPairListb discTFIndexList_r13;
            SLHoppingConfigDisc discHoppingConfig_r13;
        };

        struct ueSelected_r13
        {

            struct discTxPoolDedicated_r13
            {
                SLTxPoolToReleaseList poolToReleaseList_r13;
                SLDiscTxPoolToAddModList poolToAddModList_r13;
            };
        };
    };
};

struct SLDiscTxPoolToAddMod
{
    struct SLDiscTxPoolToAddMod
    SLTxPoolIdentity poolIdentity;
    SLDiscResourcePool pool;
};
std::list<SLDiscTxPoolToAddMod> SLDiscTxPoolToAddModList;

struct SLDiscTxConfigScheduled_r13
{
    SLDiscResourcePool discTxConfig;
    SLTFIndexPairList discTFIndexList;
    SLHoppingConfigDisc discHoppingConfig;
};

struct SLDiscTxPoolDedicated_r13
{
    SLTxPoolToReleaseList poolToReleaseList;
    SLDiscTxPoolToAddModList poolToAddModList;
};

struct SLTFIndexPair
{
    int discSFIndex;
    int discPRBIndex;
};
std::list<SLTFIndexPair> SLTFIndexPairList;

std::list<> SLTFIndexPairListb;

struct SLTFIndexPairb
{
    int discSFIndexb;
    int discPRBIndexb;
};

enum SLDiscTxRefCarrierDedicated_r13
{
    typedef pCell;
    SCellIndexr10 sCell;
};

// The IE SL-DiscResourcePool specifies the configuration information for an 
// individual pool of resources for sidelink discovery.

std::list<> SLDiscTxPoolList;

std::list<> SLDiscRxPoolList;

struct SLDiscResourcePool
{
    SLCPLen cpLen;

    enum discPeriod
    {
        rf32, rf64, rf256,
        rf512, rf1024, rf16v13x0
    };

    int numRetx;
    int numRepetition;
    SLTFResourceConfig tfResourceConfig;

    struct txParameters
    {
        SLTxParameters txParametersGeneral;

        struct ueSelectedResourceConfig
        {

            enum poolSelection
            {
                poolSelection
                SLPoolSelectionConfig rsrpBased;
                typedef random;
            };
        };

        enum txProbability
        {
            p25, p50, p100

        };

    };

    struct rxParameters
    {
        TDDConfig tddConfig;
        int syncConfigIndex;
    };

    enum discPeriodv13x0
    {

        release = 0,

        enum setup
        {
            rf4, rf7, rf8, rf14
        };
    };

    struct rxParamsAddNeighFreq_r13
    {
        PhysCellId physCellId_r13;
    };

    struct txParamsAddNeighFreq_r13
    {
        PhysCellId physCellId_r13;
        double pMax;
        TDDConfig tddConfig_r13; // Cond TDD OR TDDConfigv1130

        struct freqInfo
        {
            ARFCNValueEUTRA ulCarrierFreq;

        };

        enum ulBandwidth
        {
            n6, n15, n25, n50, n100
        };
        AdditionalSpectrumEmission additionalSpectrumEmission;
        int referenceSignalPower;
        int syncConfigIndex_r13;
    };
};

struct SLPoolSelectionConfig
{
    RSRPRangeSL2 threshLow;
    RSRPRangeSL2 threshHigh;
};

// The IE SL-DiscTxPowerInfo specifies power control parameters for one or more power classes.

struct SLDiscTxPowerInfo
{
    double discMaxTxPower = 23.0;
};
std::list<SLDiscTxPowerInfo> SLDiscTxPowerInfoList;

// discMaxTxPower
// Indicates the P-Max parameter used to calculate the maximum transmit power a UE configured with the 
// concerned range class, see TS 24.333 [70, 4.2.11]. The first entry in SL-DiscTxPowerInfoList corresponds 
// to UE range class ‘short’, the second entry corresponds to ‘medium’ and the third entry corresponds to ‘long’.

// The IE SL-GapConfig indicates the gaps assigned by E-UTRAN to enable the UE to receive or 
// transmit sidelink discovery, intra or inter frequency.

struct SLGapPattern_r13
{

    enum gapPeriod_r13
    {
        rf4, rf7, rf8, rf14, rf16, rf28,
        rf64, rf128, rf256, rf512, rf1024, spare
    };

    SLOffsetIndicator gapOffset;
    std::bitset<110> gapSubframeBitmap_r13; //  ~240
};

std::list<SLGapPattern_r13> SLGapPatternList_r13;

struct SLGapConfig_r13
{
    SLGapPatternList_r13 gapPatternList_r13;
};


// The IE SL-GapRequest indicates the gaps requested by the UE to receive or transmit sidelink discovery, intra or inter frequency.

std::list<> SLGapRequest_r13;

struct SLGapFreqInfo_r13
{
    ARFCNValueEUTRAr9 carrierFreq_r13;
    SLGapPatternList_r13 gapPatternList_r13;
};

// The IE SL-HoppingConfig indicates the hopping configuration used for sidelink.

struct SLHoppingConfigComm
{
    int hoppingParameter;

    enum numSubbands
    {
        ns1, ns2

    };
    int rbOffset;

    struct SLHoppingConfigDisc
    {
        int a, b;
    };

    enum c
    {
        n1, n5
    };

    // The IE SL-OffsetIndicator indicates the offset of the pool of resources relative to SFN 0 of the cell from which it was obtained or, when out of coverage, relative to DFN 0.

    enum SLOffsetIndicator
    {
        SLOffsetIndicator
        int small;
        int large;
    };

    int SLOffsetIndicatorSync;

    // The IE SL-PeriodComm indicates the period over which resources allocated in a cell for sidelink communication.

    enum SLPeriodComm
    {
        sf40, sf60, sf70, sf80, sf120
        sf160 sf240 sf280 sf320 spare6 spare5;
        spare4 spare3 spare2 spare
    };

    // The IE SL-Priority indicates the priority of resource pool used for sidelink communication, see TS 36.211 [21].
    // SL-Priority information element

    int SLPriority_r13;
    std::list<SLPriority_r13> SLPriorityList_r13;

    // The IE SLSSID identifies a cell and is used by the receiving UE to detect asynchronous neighbouring cells, 
    //  and by transmitting UEs to extend the synchronisation signals beyond the cell’s coverage area.
    int SLSSID;

    // The IE SL-SyncConfig specifies the configuration information concerning reception of synchronisation signals 
    // from neighbouring cells as well as concerning the transmission of synchronisation signals for sidelink discovery and communication

    struct SLSyncConfig
    {
        SLCPLen syncCPLen;
        SLOffsetIndicatorSync syncOffsetIndicator;
        SLSSID slssid;

        struct txParameters
        {
            SLTxParameters syncTxParameters;
            RSRPRangeSL syncTxThreshIC;
            std::bitset<19> syncInfoReserved;
        };

        struct rxParamsNCell
        {
            PhysCellId physCellId;
        };

        enum discSyncWindow
        {
            w1, w2
        };
    };
    bool syncTxPeriodic_r13 = true;

};

std::list<SLSyncConfig> SLSyncConfigList;

struct SLSyncConfigNFreq_r13
{

    struct asyncParameters_r13
    {
        SLCPLen syncCPLen_r13;
        SLOffsetIndicatorSync syncOffsetIndicator_r13;
        SLSSID slssid_r13;

    };

    struct txParameters_r13
    {
    };
};

std::list<SLSyncConfigNFreq_r13> SLSyncConfigListNFreq;