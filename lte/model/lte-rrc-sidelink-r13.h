
#include "lte-pc5-asn1.h"

//  SLPriorityList_r13 priorityList_r13; //
// std::list<SLPriorityList_r13> LogicalChGroupInfoList_r13;

struct _setupDC
{
    ARFCNValueEUTRAr9 discTxCarrierFreq_r13;
    SLDiscTxRefCarrierDedicated_r13 discTxRefCarrierDedicated_r13;
    SLDiscTxInfoInterFreqListExt_r13 discTxInfoInterFreqListExt_r13;
};

struct SLDiscConfig
{
    // Initial part ... in rrc-sidelink.h

    enum discTxInterFreqInfo_r13
    {
        release = 0,
        setup = _setupDc
    };

    enum gapRequestsAllowedDedicated_r13
    {
        true,
        false
    };

    enum discRxGapConfig_r13
    {
        release = 0,
        setup = SLGapConfig_r13
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
    ARFCNValueEUTRAr9 discTxCarrierFreq_r13;
    SLDiscTxResource_r13 discTxResources_r13;
    SLDiscTxResource_r13 discTxResourcesPS_r13;
    SLDiscTxRefCarrierDedicated_r13 discTxRefCarrierDedicated_r13;
    CellSelectionInfoNFreq_r13 cellSelectionInfo_r13;
};

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

enum setup
{
    scheduled_r13,
    ueSelected_r13
};

enum SLDiscTxResource_r13
{
    release = 0,
    setup = setup.ueSelected_r13
};


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

struct SLDiscResourcePool
{
    // ... in rrc-sidelink.h

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

struct SLSyncConfigNFreq_r13
{

    struct asyncParameters_r13
    {
        SLCPLen syncCPLen_r13;
        SLOffsetIndicatorSync syncOffsetIndicator_r13 = SLOffsetIndicator.small;
        SLSSID slssid_r13;
    };

    struct txParameters_r13
    {
        SLTxParameters syncTxParameters;
        RSRPRangeSL syncTxThreshIC;
        std::bitset<19> syncInfoReserved;
        bool syncTxPeriodic = true;
    };

    struct rxParameters_r1s
    {

        enum discSyncWindow
        {
            w1, w2
        };
    };
};

std::list<SLSyncConfigNFreq_r13> SLSyncConfigListNFreq;

// SL-SyncConfigListNFreq-r12 ::= SEQUENCE (SIZE (1..maxSL-SyncConfig-r12)) OF SL-SyncConfigNFreq-r13

//*** EUTRA-RRC-Definitions.asn

struct SLSysInfoReport
{
    PLMNIdentityList plmnIdentityList;
    CellIdentity cellIdentity13;
    ARFCNValueEUTRAr9 carrierFreqInfo13;
    SLDiscRxPoolList discRxResources;
    SLDiscTxPoolList discTxPoolCommon;
    SLDiscTxPowerInfoList discTxPowerInfo;

    struct discSyncConfig
    {

        struct asyncParameters
        {
            SLCPLen syncCPLen;
            SLOffsetIndicatorSync syncOffsetIndicator;
            SLSSID slssid;
        };

        struct txParameters
        {
            SLTxParameters syncTxParameters;
            RSRPRangeSL syncTxThreshIC;
            std::bitset<19> syncInfoReserved;
            bool syncTxPeriodic = true;
        };
    };

    struct cellSelectionInfo
    {
        QRxLevMin qRxLevMin;
        int qRxLevMinOffset;
    };

    enum qHyst
    {
        dB0, dB1, dB2, dB3, dB4, dB5, dB6, dB8, dB10, dB12, dB14, dB16, dB18, dB20, dB22, dB24
    };

    QRxLevMin qRxLevMin;
    TReselection tReselectionEUTRA;
    TDDConfig tddConfig;

    struct freqInfo
    {
        ARFCNValueEUTRA ulCarrierFreq;

        enum ulBandwidth
        {
            n6, n15, n25, n50, n100
        };
        AdditionalSpectrumEmission additionalSpectrumEmission;

    };

    double pMax = 23.0;
    int referenceSignalPower;
};

struct SlSysInfoReport; //  TBDefined
std::list<SlSysInfoReport> SlSysInfoReportList;


// Indicates for each logical channel group the list of associated priorities, used as specified in TS 36.321 [6].

enum SLDiscTxRefCarrierDedicated_r13
{
    pCell,
    sCell // SCellIndexr10 
};



//***********
// SL-GapConfig indicates the gaps assigned by E-UTRAN to enable the UE to receive or
// transmit sidelink discovery, intra or inter frequency.

struct SLGapPattern_r13
{

    enum gapPeriod_r13
    {
        rf4 = 4, rf7 = 7, rf8 = 8, rf14 = 14, rf16 = 16, rf28 = 28,
        rf64 = 64, rf128 = 128, rf256 = 256, rf512 = 512, rf1024 = 1024, spare
    };

    SLOffsetIndicator gapOffset = SLOffsetIndicator.small;
    std::bitset<110> gapSubframeBitmap_r13; //  ~240
};

std::list<SLGapPattern_r13> SLGapPatternList_r13;

struct SLGapConfig_r13
{
    SLGapPatternList_r13 gapPatternList_r13;
};


// SL-GapRequest indicates the gaps requested by the UE to receive or transmit sidelink discovery, intra or inter frequency.

std::list<> SLGapRequest_r13;

struct SLGapFreqInfo_r13
{
    ARFCNValueEUTRAr9 carrierFreq_r13;
    SLGapPatternList_r13 gapPatternList_r13;
};
