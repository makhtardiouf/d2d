
// Note: Adhoc ASN1 converter, by Makhtar Diouf
#include "RRC36331_defs_asn1_typedefs.h"
// See also lte-rrc-sap.h and lte-rrc-header.h for existing stuff
// @TODO skip over part that are not for D2D/SideLink

// -- ASN.1 module found by ./crfc2asn1.pl in EUTRA-RRC36331.txt at line 4853
// EUTRA-RRC-Definitions DEFINITIONS AUTOMATIC TAGS ::=

MasterInformationBlock BCCHBCHMessageType;

// The BCCH-BCH-Message class is the set of RRC messages that may be sent from the E-UTRAN to the UE via BCH on the BCCH logical channel.
struct BCCHBCHMessage {
    BCCHBCHMessageType message;
};

// -- ASN1STOP
// -	BCCH-DL-SCH-Message
// The BCCH-DL-SCH-Message class is the set of RRC messages that may be sent
// from the E-UTRAN to the UE via DL-SCH on the BCCH logical channel.

enum BCCHDLSCHMessageType {
    systemInformation,
    systemInformationBlockType1
};

struct BCCHDLSCHMessage {
    BCCHDLSCHMessageType message;
};

// Removed MCCH-Message, coz is for MBMS

// The PCCH-Message class is the set of RRC messages that may
// be sent from the E-UTRAN to the UE on the PCCH logical channel.

enum PCCHMessageType {
    paging
};

struct PCCHMessage {
    PCCHMessageType message;
};

// The DL-CCCH-Message class is the set of RRC messages that may be sent
// from the E-UTRAN to the UE on the downlink CCCH logical channel.

enum  DLCCCHMessageType {
    rrcConnectionReestablishment,
    rrcConnectionReestablishmentReject,
    rrcConnectionReject,
    rrcConnectionSetup
};

struct DLCCCHMessage {
    DLCCCHMessageType message;
};

// The DL-DCCH-Message class is the set of RRC messages that may be sent from
// the E-UTRAN to the UE or from the E-UTRAN to the RN on the downlink DCCH logical channel.

// The UL-CCCH-Message class is the set of RRC messages that may be sent
// from the UE to the E-UTRAN on the uplink CCCH logical channel.

// The UL-DCCH-Message class is the set of RRC messages that may be sent
// from the UE to the E-UTRAN or from the RN to the E-UTRAN on the uplink DCCH logical channel.

enum messageClassExtension {

    UEAssistanceInformationr11 ,
    InDeviceCoexIndicationr11,
    MBMSInterestIndicationr11,
    SCGFailureInformation,
    SidelinkUEInformation
};


// 6.2.2	Message definitions

// The CounterCheck message is used by the E-UTRAN to indicate the current COUNT MSB values associated to each DRB
// and to request the UE to compare these to its COUNT MSB values and to report the comparison results to E-UTRAN.
// Signalling radio bearer: SRB1
// RLC-SAP: AM
// Logical channel: DCCH
// Direction: E-UTRAN to UE
// CounterCheck message
// -- ASN1START

struct CounterCheck {
    RRCTransactionIdentifier rrcTransactionIdentifier;

    enum criticalExtensions {
        CounterCheckr8IEs counterCheckr8;
    };

};

struct CounterCheckr8IEs {
    DRBCountMSBInfoList drbCountMSBInfoList;
   
};

std::list DRBCountMSBInfoList;

struct DRBCountMSBInfo {
    DRBIdentity drbIdentity;
    int countMSBUplink;
    int countMSBDownlink;
};

// CounterCheck field descriptions
// count-MSB-Downlink
// Indicates the value of 25 MSBs from downlink COUNT associated to this DRB.
// count-MSB-Uplink
// Indicates the value of 25 MSBs from uplink COUNT associated to this DRB.
// drb-CountMSB-InfoList
// Indicates the MSBs of the COUNT values of the DRBs.
//
// -	CounterCheckResponse
// The CounterCheckResponse message is used by the UE to respond to a CounterCheck message.
// Signalling radio bearer: SRB1
// RLC-SAP: AM
// Logical channel: DCCH
// Direction: UE to E-UTRAN
// CounterCheckResponse message
// -- ASN1START

struct CounterCheckResponse {
    RRCTransactionIdentifier rrcTransactionIdentifier;

    enum criticalExtensions {

        CounterCheckResponser8IEs counterCheckResponser8;
    };

    struct CounterCheckResponser8IEs {
        DRBCountInfoList drbCountInfoList;
       
    };

    struct DRBCountInfo {
        DRBIdentity drbIdentity;
        int countUplink;
        int countDownlink;
    };

    std::list<DRBCountInfo>DRBCountInfoList;

// CounterCheckResponse field descriptions
// count-Downlink
// Indicates the value of downlink COUNT associated to this DRB.
// count-Uplink
// Indicates the value of uplink COUNT associated to this DRB.
// drb-CountInfoList
// Indicates the COUNT values of the DRBs.

// -	DLInformationTransfer
// The DLInformationTransfer message is used for the downlink transfer of NAS or non-3GPP dedicated information.
// Signalling radio bearer: SRB2 or SRB1 (only if SRB2 not established yet. If SRB2 is suspended, E-UTRAN does not send this message until SRB2 is resumed.)
// RLC-SAP: AM
// Logical channel: DCCH
// Direction: E-UTRAN to UE
// DLInformationTransfer message

    struct DLInformationTransfer {
        RRCTransactionIdentifier rrcTransactionIdentifier;

            DLInformationTransferr8IEs dlInformationTransferr8;          
    };

    struct DLInformationTransferr8IEs {

        enum dedicatedInfoType {
          
            DedicatedInfoNAS dedicatedInfoNAS;

        };

    };


// -	HandoverFromEUTRAPreparationRequest (CDMA2000)
// The HandoverFromEUTRAPreparationRequest message is used to trigger the handover preparation procedure with a CDMA2000 RAT. This message is also used to trigger a tunneled preparation procedure with a CDMA2000 1xRTT RAT to obtain traffic channel resources for the enhanced CS fallback to CDMA2000 1xRTT, which may also involve a concurrent preparation for handover to CDMA2000 HRPD. Also, this message is used to trigger the dual Rx/Tx redirection procedure with a CDMA2000 1xRTT RAT.
// Signalling radio bearer: SRB1
// RLC-SAP: AM
// Logical channel: DCCH
// Direction: E-UTRAN to UE
// HandoverFromEUTRAPreparationRequest message

    HandoverFromEUTRAPreparationRequest struct {
        RRCTransactionIdentifier rrcTransactionIdentifier;

        enum criticalExtensions {

            handoverFromEUTRAPreparationRequestr8;
            HandoverFromEUTRAPreparationRequestr8IEs;
            spare3 typedef spare2 typedef spare1 typedef;
        };
    };


    enum TDMAssistanceInfor11 {

        TDMAssistanceInfor11
        struct drxAssistanceInfor11 {

            enum drxCycleLengthr11 {
                sf40, sf64, sf80, sf128
                sf256 spare2 spare1
            };
            int drxOffsetr11;

            enum drxActiveTimer11 {
                sf20, sf30, sf40, sf60
                sf100 spare2 spare1
            };
        };
        IDCSubframePatternListr11 idcSubframePatternListr11;
    };

    IDCSubframePatternr11 enum {
        std::bitset<4> subframePatternFDDr11;

        enum subframePatternTDDr11 {
            subframePatternTDDr11
            std::bitset<70> subframeConfig0r11;
            std::bitset<10> subframeConfig15r11;
            std::bitset<60> subframeConfig6r11;
        };
    };

    std::list<IDCSubframePatternr11> IDCSubframePatternListr11''


    SEQU RSTDInterFreqInfoListr10;
    ;

    struct RSTDInterFreqInfor10 {
        ARFCNValueEUTRA carrierFreqr10;
        int measPRSOffsetr10;

        ;
        ARFCNValueEUTRAv9e0 carrierFreqv1090;

        ;
    };
    ;
// -- ASN1STOP
//
// InterFreqRSTDMeasurementIndication field descriptions
// carrierFreq
// The EARFCN value of the carrier received from upper layers for which the UE needs to perform the inter-frequency RSTD measurements. If the UE includes carrierFreq-v1090, it shall set carrierFreq-r10 to maxEARFCN.
// measPRS-Offset
// Indicates the requested gap offset for performing inter-frequency RSTD measurements. It is the smallest subframe offset from the beginning of subframe 0 of SFN=0 of the serving cell of the requested gap for measuring PRS positioning occasions in the carrier frequency carrierFreq for which the UE needs to perform the inter-frequency RSTD measurements. The PRS positioning occasion information is received from upper layers. The value of measPRS-Offset is obtained by mapping the starting subframe of the PRS positioning occasion in the measured cell onto the corresponding subframe in the serving cell and is calculated as the serving cell's number of subframes from SFN=0 mod 40.
// The UE shall take into account any additional time required by the UE to start PRS measurements on the other carrier when it does this mapping for determining the measPRS-Offset.
// NOTE: Figure 6.2.2-1 illustrates the measPRS-Offset field.
// rstd-InterFreqIndication
// Indicates the inter-frequency RSTD measurement action, i.e. the UE is going to start or stop inter-frequency RSTD measurement.
//
//
// Figure 6.2.2-1 (informative): Exemplary calculation of measPRS-Offset field.
//
// -	LoggedMeasurementConfiguration
// The LoggedMeasurementConfiguration message is used by E-UTRAN to configure the UE to
// perform logging of measurement results while in RRC_IDLE or to perform logging
// of measurement results for MBSFN while in both RRC_IDLE and RRC_CONNECTED.
// It is used to transfer the logged measurement configuration for network
// performance optimisation, see TS 37.320 [60].
// Signalling radio bearer: SRB1
// RLC-SAP: AM
// Logical channel: DCCH
// Direction: E-UTRAN to UE
// LoggedMeasurementConfiguration message

    struct LoggedMeasurementConfigurationr10 {

        enum criticalExtensions {

            criticalExtensions
            enum c1 {
                c1
                LoggedMeasurementConfigurationr10IEs loggedMeasurementConfigurationr10;
                spare3 typedef spare2 typedef spare1 typedef;
            };
            struct
                } criticalExtensionsFuture {
        };
    };
    ;
    ;

    LoggedMeasurementConfigurationr10IEs struct {
        TraceReferencer10 traceReferencer10;
        char traceRecordingSessionRefr10;
        char tceIdr10;
        AbsoluteTimeInfor10 absoluteTimeInfor10;
        Need OR AreaConfigurationr10 areaConfigurationr10;
        LoggingDurationr10 loggingDurationr10;
        LoggingIntervalr10 loggingIntervalr10;
        LoggedMeasurementConfigurationv1080IEs nonCriticalExtension;
    };
    ;

    LoggedMeasurementConfigurationv1080IEs struct {
        char T string lateNonCriticalExtensionr10;
        LoggedMeasurementConfigurationv1130IEs nonCriticalExtension;
    };
    ;

    LoggedMeasurementConfigurationv1130IEs struct {
        Need OR PLMNIdentityList3r11 plmnIdentityListr11;
        Need OR AreaConfigurationv1130 areaConfigurationv1130;
        LoggedMeasurementConfigurationv1250IEs nonCriticalExtension;
    };
    ;

    LoggedMeasurementConfigurationv1250IEs struct {
        TargetMBSFNAreaList targetMBSFNAreaList;
        struct
            } nonCriticalExtension {
    };
    ;
    SEQU TargetMBSFNAreaList;
    ;

    struct TargetMBSFNArea {
        Need OR MBSFNAreaId mbsfnAreaId;
        ARFCNValueEUTRAr9 carrierFreq;

        ;
    };
    ;
// -- ASN1STOP
//
// LoggedMeasurementConfiguration field descriptions
// absoluteTimeInfo
// Indicates the absolute time in the current cell.
// areaConfiguration
// Used to restrict the area in which the UE performs measurement logging to cells broadcasting either one of the included cell identities or one of the included tracking area codes/ identities.
// plmn-IdentityList
// Indicates a set of PLMNs defining when the UE performs measurement logging as well as the associated status indication and information retrieval i.e. the UE performs these actions when the RPLMN is part of this set of PLMNs.
// targetMBSFN-AreaList
// Used to indicate logging of MBSFN measurements and further restrict the area and frequencies for which the UE performs measurement logging for MBSFN. If both MBSFN area id and carrier frequency are present, a specific MBSFN area is indicated. If only carrier frequency is present, all MBSFN areas on that carrier frequency are indicated. If there is no entry in the list, any MBSFN area is indicated.
// tce-Id
// Parameter Trace Collection Entity Id: See TS 32.422 [58].
// traceRecordingSessionRef
// Parameter Trace Recording Session Reference: See TS 32.422 [58]
//
// -	MasterInformationBlock
// The MasterInformationBlock includes the system information transmitted on BCH.
// Signalling radio bearer: N/A
// RLC-SAP: TM
// Logical channel: BCCH
// Direction: E-UTRAN to UE
// MasterInformationBlock
// -- ASN1START
    ;

    struct MasterInformationBlock {

        enum dlBandwidth {
            dlBandwidth,
            n6 n15 n25 n50 n75 n100
        };
        PHICHConfig phichConfig;
        std::bitset<8> systemFrameNumber;
        std::bitset<10> spare;
    };
    ;
    ;
// -- ASN1STOP
//
// MasterInformationBlock field descriptions
// dl-Bandwidth
// Parameter: transmission bandwidth configuration, NRB in downlink, see TS 36.101 [42, table 5.6-1]. n6 corresponds to 6 resource blocks, n15 to 15 resource blocks and so on.
// systemFrameNumber
// Defines the 8 most significant bits of the SFN. As indicated in TS 36.211 [21, 6.6.1], the 2 least significant bits of the SFN are acquired implicitly in the P-BCH decoding, i.e. timing of 40ms P-BCH TTI indicates 2 least significant bits (within 40ms P-BCH TTI, the first radio frame: 00, the second radio frame: 01, the third radio frame: 10, the last radio frame: 11). One value applies for all serving cells of a Cell Group (i.e. MCG or SCG). The associated functionality is common (i.e. not performed independently for each cell).
//
// -	MBMSCountingRequest
// The MBMSCountingRequest message is used by E-UTRAN to count the UEs that are receiving or interested to receive specific MBMS services.
// Signalling radio bearer: N/A
// RLC-SAP: UM
// Logical channel: MCCH
// Direction: E-UTRAN to UE
// MBMSCountingRequest message
// -- ASN1START
    ;

    struct MBMSCountingRequestr10 {
        CountingRequestListr10 countingRequestListr10;
        char T string lateNonCriticalExtension;
        struct
            } nonCriticalExtension {
    };
    ;
    SEQU CountingRequestListr10;
    ;

    struct CountingRequestInfor10 {
        TMGIr9 tmgir10;

        ;
    };
    ;
// -- ASN1STOP
//
// -	MBMSCountingResponse
// The MBMSCountingResponse message is used by the UE to respond to an MBMSCountingRequest message.
// Signalling radio bearer: SRB1
// RLC-SAP: AM
// Logical channel: DCCH
// Direction: UE to E-UTRAN
// MBMSCountingResponse message
// -- ASN1START
    ;

    struct MBMSCountingResponser10 {

        enum criticalExtensions {

            criticalExtensions
            enum c1 {
                c1
                MBMSCountingResponser10IEs countingResponser10;
                spare3 typedef spare2 typedef spare1 typedef;
            };
            struct
                } criticalExtensionsFuture {
        };
    };
    ;

    struct MBMSCountingResponser10IEs {
        int mbsfnAreaIndexr10;
        CountingResponseListr10 countingResponseListr10;
        char T string lateNonCriticalExtension;
        struct
            } nonCriticalExtension {
    };
    ;
    SEQU CountingResponseListr10;
    ;

    struct CountingResponseInfor10 {
        int countingResponseServicer10;

        ;
    };
    ;
// -- ASN1STOP
//
// MBMSCountingResponse field descriptions
// countingResponseList
// List of MBMS services which the UE is receiving or interested to receive. Value 0 for field countingResponseService corresponds to the first entry in countingRequestList within MBMSCountingRequest, value 1 corresponds to the second entry in this list and so on.
// mbsfn-AreaIndex
// Index of the entry in field mbsfn-AreaInfoList within SystemInformationBlockType13. Value 0 corresponds to the first entry in mbsfn-AreaInfoList within SystemInformationBlockType13, value 1 corresponds to the second entry in this list and so on.
//
// -	MBMSInterestIndication
// The MBMSInterestIndication message is used to inform E-UTRAN that the UE is receiving/ interested to receive or no longer receiving/ interested to receive MBMS via an (SC-)MRB.
// Signalling radio bearer: SRB1
// RLC-SAP: AM
// Logical channel: DCCH
// Direction: UE to E-UTRAN
// MBMSInterestIndication message
// -- ASN1START
    ;

    struct MBMSInterestIndicationr11 {

        enum criticalExtensions {

            criticalExtensions
            enum c1 {
                c1
                MBMSInterestIndicationr11IEs interestIndicationr11;
                spare3 typedef spare2 typedef spare1 typedef;
            };
            struct
                } criticalExtensionsFuture {
        };
    };
    ;

    struct MBMSInterestIndicationr11IEs {
        CarrierFreqListMBMSr11 mbmsFreqListr11;
        , enum true
    } mbmsPriorityr11 {
        true
    }
    mbmsPriorityr11,
    char T string lateNonCriticalExtension;
    MBMSInterestIndication_r13IEs nonCriticalExtension;
};
;

struct MBMSInterestIndication_r13IEs {
    MBMSServiceList_r13 mbmsServices_r13;
    struct
        } nonCriticalExtension {
};
;
// -- ASN1STOP
//
// MBMSInterestIndication field descriptions
// mbms-FreqList
// List of MBMS frequencies on which the UE is receiving or interested to receive MBMS via an (SC-)MRB.
// mbms-Priority
// Indicates whether the UE prioritises MBMS reception above unicast reception. The field is present (i.e. value true), if the UE prioritises reception of all listed MBMS frequencies above reception of any of the unicast bearers. Otherwise the field is absent.
//
// -	MBSFNAreaConfiguration
// The MBSFNAreaConfiguration message contains the MBMS control information applicable for an MBSFN area. For each MBSFN area included in SystemInformationBlockType13 E-UTRAN configures an MCCH (i.e. the MCCH identifies the MBSFN area) and signals the MBSFNAreaConfiguration message.
// Signalling radio bearer: N/A
// RLC-SAP: UM
// Logical channel: MCCH
// Direction: E-UTRAN to UE
// MBSFNAreaConfiguration message
// -- ASN1START
;

struct MBSFNAreaConfigurationr9 {
    CommonSFAllocPatternListr9 commonSFAllocr9;

    enum commonSFAllocPeriodr9 {
        commonSFAllocPeriodr9,
        rf4 rf8 rf16 rf32 rf64 rf128 rf256
    };
    PMCHInfoListr9 pmchInfoListr9;
    MBSFNAreaConfigurationv930IEs nonCriticalExtension;
};
;

MBSFNAreaConfigurationv930IEs struct {
    char T string lateNonCriticalExtension;
    MBSFNAreaConfigurationv1250IEs nonCriticalExtension;
};
;

MBSFNAreaConfigurationv1250IEs struct {
    Need OR PMCHInfoListExt pmchInfoListExt;
    struct
        } nonCriticalExtension {
};
;
SEQU CommonSFAllocPatternListr9;
;
// -- ASN1STOP
//
// MBSFNAreaConfiguration field descriptions
// commonSF-Alloc
// Indicates the subframes allocated to the MBSFN area. E-UTRAN always sets this field to cover at least the subframes configured by SystemInformationBlockType13 for this MCCH, regardless of whether any MBMS sessions are ongoing.
// commonSF-AllocPeriod
// Indicates the period during which resources corresponding with field commonSF-Alloc are divided between the (P)MCH that are configured for this MBSFN area. The subframe allocation patterns, as defined by commonSF-Alloc, repeat continously during this period. Value rf4 corresponds to 4 radio frames, rf8 corresponds to 8 radio frames and so on. The commonSF-AllocPeriod starts in the radio frames for which: SFN mod commonSF-AllocPeriod = 0.
// pmch-InfoList
// EUTRAN may include pmch-InfoListExt even if pmch-InfoList does not include maxPMCH-PerMBSFN entries. EUTRAN configures at most maxPMCH-PerMBSFN entries i.e. across pmch-InfoList and pmch-InfoListExt.
//
// -	MeasurementReport
// The MeasurementReport message is used for the indication of measurement results.
// Signalling radio bearer: SRB1
// RLC-SAP: AM
// Logical channel: DCCH
// Direction: UE to E-UTRAN
// MeasurementReport message
// -- ASN1START
;

struct MeasurementReport {

    enum criticalExtensions {

        criticalExtensions
        enum c1 {
            c1
            MeasurementReportr8IEs measurementReportr8;
            spare7 typedef;
            spare6 typedef spare5 typedef spare4 typedef;
            spare3 typedef spare2 typedef spare1 typedef;
        };
        struct
            } criticalExtensionsFuture {
    };
};
;

struct MeasurementReportr8IEs {
    MeasResults measResults;
    MeasurementReportv8a0IEs nonCriticalExtension;
};
;

MeasurementReportv8a0IEs struct {
    char T string lateNonCriticalExtension;
    struct
        } nonCriticalExtension {
};
;
// -- ASN1STOP
//
// -	MobilityFromEUTRACommand
// The MobilityFromEUTRACommand message is used to command handover or a cell change from E-UTRA to another RAT (3GPP or non-3GPP), or enhanced CS fallback to CDMA2000 1xRTT.
// Signalling radio bearer: SRB1
// RLC-SAP: AM
// Logical channel: DCCH
// Direction: E-UTRAN to UE
// MobilityFromEUTRACommand message
// -- ASN1START
;

struct MobilityFromEUTRACommand {
    RRCTransactionIdentifier rrcTransactionIdentifier;

    enum criticalExtensions {

        criticalExtensions
        enum c1 {
            c1
            MobilityFromEUTRACommandr8IEs mobilityFromEUTRACommandr8;
            MobilityFromEUTRACommandr9IEs mobilityFromEUTRACommandr9;
            typedef spare2 typedef spare1;
        };
        struct
            } criticalExtensionsFuture {
    };
};
;

struct MobilityFromEUTRACommandr8IEs {
    bool csFallbackIndicator;

    enum purpose {
        purpose
        Handover handover;
        CellChangeOrder cellChangeOrder;
    };
    MobilityFromEUTRACommandv8a0IEs nonCriticalExtension;
};
;

MobilityFromEUTRACommandv8a0IEs struct {
    char T string lateNonCriticalExtension;
    MobilityFromEUTRACommandv8d0IEs nonCriticalExtension;
};
;

MobilityFromEUTRACommandv8d0IEs struct {
    Cond GERAN BandIndicatorGERAN bandIndicator;
    struct
        } nonCriticalExtension {
};
;

struct MobilityFromEUTRACommandr9IEs {
    bool csFallbackIndicator;

    enum purpose {
        purpose
        Handover handover;
        CellChangeOrder cellChangeOrder;
        ECSFBr9 eCSFBr9;

        ;
    };
    MobilityFromEUTRACommandv930IEs nonCriticalExtension;
};
;

MobilityFromEUTRACommandv930IEs struct {
    char T string lateNonCriticalExtension;
    MobilityFromEUTRACommandv960IEs nonCriticalExtension;
};
;

MobilityFromEUTRACommandv960IEs struct {
    Cond GERAN BandIndicatorGERAN bandIndicator;
    struct
        } nonCriticalExtension {
};
;

struct Handover {

    enum targetRATType {
        targetRATType,
        utra geran cdma20001XRTT cdma2000HRPD;
        spare3 spare2 spare1
    } spare4;
    char T string targetRATMessageContainer;
    Cond UTRAGERAN char nasSecurityParamFromEUTRA;
    Cond PSHO SIOrPSIGERAN systemInformation;
};
;

struct CellChangeOrder {

    enum t304 {
        t304,
        ms100 ms200 ms500 ms1000;
        ms2000 ms4000 ms8000 spare1
    };

    enum targetRATType {

        targetRATType
        struct geran {
            PhysCellIdGERAN physCellId;
            CarrierFreqGERAN carrierFreq;
            std::bitset<2> networkControlOrder;
            SIOrPSIGERAN systemInformation;
        };

        ;
    };
};
;

enum SIOrPSIGERAN {
    SIOrPSIGERAN
    SystemInfoListGERAN si;
    SystemInfoListGERAN psi;
};
;

struct ECSFBr9 {
    char T string messageContCDMA20001XRTTr9;

    enum mobilityCDMA2000HRPDr9 {
        mobilityCDMA2000HRPDr9,
        handover redirection;
    };
    Cond concHO char T string messageContCDMA2000HRPDr9;
    Cond concRedir CarrierFreqCDMA2000 redirectCarrierCDMA2000HRPDr9;
};
;
// -- ASN1STOP
//
// MobilityFromEUTRACommand field descriptions
// bandIndicator
// Indicates how to interpret the ARFCN of the BCCH carrier.
// carrierFreq
// contains the carrier frequency of the target GERAN cell.
// cs-FallbackIndicator
// Value true indicates that the CS Fallback procedure to UTRAN or GERAN is triggered.
// messageContCDMA2000-1XRTT
// This field contains a message specified in CDMA2000 1xRTT standard that either tells the UE to move to specific 1xRTT target cell(s) or indicates a failure to allocate resources for the enhanced CS fallback to CDMA2000 1xRTT.
// messageContCDMA2000-HRPD
// This field contains a message specified in CDMA2000 HRPD standard that either tells the UE to move to specific HRPD target cell(s) or indicates a failure to allocate resources for the handover to CDMA2000 HRPD.
// mobilityCDMA2000-HRPD
// This field indicates whether or not mobility to CDMA2000 HRPD is to be performed by the UE and it also indicates the type of mobility to CDMA2000 HRPD that is to be performed; If this field is not present the UE shall perform only the enhanced CS fallback to CDMA2000 1xRTT.
// nas-SecurityParamFromEUTRA
// Used to deliver the key synchronisation and Key freshness for the E-UTRAN to UTRAN handovers as specified in TS 33.401. The content of the parameter is defined in TS24.301.
// networkControlOrder
// Parameter NETWORK_CONTROL_ORDER in TS 44.060 [36].
// purpose
// Indicates which type of mobility procedure the UE is requested to perform. EUTRAN always applies value e-CSFB in case of enhanced CS fallback to CDMA2000 (e.g. also when that procedure results in handover to CDMA2000 1XRTT only, in handover to CDMA2000 HRPD only or in redirection to CDMA2000 HRPD only),
// redirectCarrierCDMA2000-HRPD
// The redirectCarrierCDMA2000-HRPD indicates a CDMA2000 carrier frequency and is used to redirect the UE to a HRPD carrier frequency.
// SystemInfoListGERAN
// If purpose = CellChangeOrder and if the field is not present, the UE has to acquire SI/PSI from the GERAN cell.
// t304
// Timer T304 as described in section 7.3. Value ms100 corresponds with 100 ms, ms200 corresponds with 200 ms and so on.
// targetRAT-Type
// Indicates the target RAT type.
// targetRAT-MessageContainer
// The field contains a message specified in another standard, as indicated by the targetRAT-Type, and carries information about the target cell identifier(s) and radio parameters relevant for the target radio access technology. NOTE 1.
//
// A complete message is included, as specified in the other standard.
//
// Conditional presence
// Explanation
// concHO
// The field is mandatory present if the mobilityCDMA2000-HRPD is set to "handover"; otherwise the field is optional present, need ON.
// concRedir
// The field is mandatory present if the mobilityCDMA2000-HRPD is set to "redirection"; otherwise the field is not present.
// GERAN
// The field should be present if the purpose is set to "handover" and the targetRAT-Type is set to "geran"; otherwise the field is not present
// PSHO
// The field is mandatory present in case of PS handover toward GERAN; otherwise the field is optionally present, but not used by the UE
// UTRAGERAN
// The field is mandatory present if the targetRAT-Type is set to "utra" or "geran"; otherwise the field is not present
//
// NOTE 1:	The correspondence between the value of the targetRAT-Type, the standard to apply and the message contained within the targetRAT-MessageContainer is shown in the table below:
// targetRAT-Type
// Standard to apply
// targetRAT-MessageContainer
// cdma2000-1XRTT
// C.S0001 or later, C.S0007 or later, C.S0008 or later
//
// cdma2000-HRPD
// C.S0024 or later
//
// geran
// GSM TS 04.18, version 8.5.0 or later, or 3GPP TS 44.018 (clause 9.1.15)
//
// 3GPP TS 44.060, version 6.13.0 or later (clause 11.2.43)
//
// 3GPP TS 44.060, version 7.6.0 or later (clause 11.2.46)
// HANDOVER COMMAND
//
//
// PS HANDOVER COMMAND
//
//
// DTM HANDOVER COMMAND
// utra
// 3GPP TS 25.331 (clause 10.2.16a)
// Handover TO UTRAN command
//
// -	Paging
// The Paging message is used for the notification of one or more UEs.
// Signalling radio bearer: N/A
// RLC-SAP: TM
// Logical channel: PCCH
// Direction: E-UTRAN to UE
// Paging message
// -- ASN1START
;

struct Paging {
    PagingRecordList pagingRecordList;
    , enum true
} systemInfoModification {
    true
}
systemInfoModification,
, enum true
}
etwsIndication {
    true
}
etwsIndication,
Pagingv890IEs nonCriticalExtension;
};
;

struct Pagingv890IEs {
    char T string lateNonCriticalExtension;
    Pagingv920IEs nonCriticalExtension;
};
;

struct Pagingv920IEs {
    , enum true
} cmasIndicationr9 {
    true
}
cmasIndicationr9,
Pagingv1130IEs nonCriticalExtension;
};
;

struct Pagingv1130IEs {
    , enum true
} eabParamModificationr11 {
    true
}
eabParamModificationr11,
Pagingv13xxIEs nonCriticalExtension;
};
;

struct Pagingv13xxIEs {
    , enum true
} redistributionIndication_r13 {
    true
}
redistributionIndication_r13,
, enum true
}
systemInfoModificationeDRX_r13 {
    true
}
systemInfoModificationeDRX_r13,
struct
    }
nonCriticalExtension {
};
;
SEQU PagingRecordList;
;

struct PagingRecord {
    PagingUEIdentity ueIdentity;

    enum cnDomain {
        ps,

        ;
    };
    ;

    enum PagingUEIdentity {
        PagingUEIdentity
        STMSI sTMSI;
        IMSI imsi;

        ;
    };
    ;
    SEQU IMSI;
    ;
    int IMSIDigit;
    ;
    // -- ASN1STOP
    //
    // Paging field descriptions
    // cmas-Indication
    // If present: indication of a CMAS notification.
    // cn-Domain
    // Indicates the origin of paging.
    // eab-ParamModification
    // If present: indication of an EAB parameters (SIB14) modification.
    // etws-Indication
    // If present: indication of an ETWS primary notification and/ or ETWS secondary notification.
    // imsi
    // The International Mobile Subscriber Identity, a globally unique permanent subscriber identity, see TS 23.003 [27]. The first element contains the first IMSI digit, the second element contains the second IMSI digit and so on.
    // redistributionIndication
    // If present: indication to trigger E-UTRAN Inter-frequency Redistribution procedure as specified in 5.2.4.10 [4]
    // systemInfoModification
    // If present: indication of a BCCH modification other than SIB10, SIB11, SIB12 and SIB14.
    // systemInfoModification-eDRX
    // If present: indication of a BCCH modification other than SIB10, SIB11, SIB12 and SIB14 for UEs in extended DRX. This indication applies only to UEs having eDRX cycle longer than the BCCH modification period.
    // ue-Identity
    // Provides the NAS identity of the UE that is being paged.
    //
    // -	ProximityIndication
    // The ProximityIndication message is used to indicate that the UE is entering or leaving the proximity of one or more CSG member cell(s).
    // Signalling radio bearer: SRB1
    // RLC-SAP: AM
    // Logical channel: DCCH
    // Direction: UE to E-UTRAN
    // ProximityIndication message
    // -- ASN1START
    ;

    ProximityIndicationr9 struct {

        enum criticalExtensions {

            criticalExtensions
            enum c1 {
                c1
                ProximityIndicationr9IEs proximityIndicationr9;
                spare3 typedef spare2 typedef spare1 typedef;
            };
            struct
                } criticalExtensionsFuture {
        };
    };
    ;

    ProximityIndicationr9IEs struct {

        enum typer9 {

            entering,
            enum carrierFreqr9 {
                carrierFreqr9
                ARFCNValueEUTRA eutrar9;
                ARFCNValueUTRA utrar9;

                ;
                ARFCNValueEUTRAv9e0 eutra2v9e0;
            };
            ProximityIndicationv930IEs nonCriticalExtension;
        };
        ;

        ProximityIndicationv930IEs struct {
            char T string lateNonCriticalExtension;
            struct
                } nonCriticalExtension {
        };
        ;
        // -- ASN1STOP
        //
        // ProximityIndication field descriptions
        // carrierFreq
        // Indicates the RAT and frequency of the CSG member cell(s), for which the proximity indication is sent. For E-UTRA and UTRA frequencies, the UE shall set the ARFCN according to a band it previously considered suitable for accessing (one of) the CSG member cell(s), for which the proximity indication is sent.
        // type
        // Used to indicate whether the UE is entering or leaving the proximity of CSG member cell(s).
        //
        // -	RNReconfiguration
        // The RNReconfiguration is a command to modify the RN subframe configuration and/or to convey changed system information.
        // Signalling radio bearer: SRB1
        // RLC-SAP: AM
        // Logical channel: DCCH
        // Direction: E-UTRAN to RN
        // RNReconfiguration message
        // -- ASN1START
        ;

        struct RNReconfigurationr10 {
            RRCTransactionIdentifier rrcTransactionIdentifier;

            enum criticalExtensions {

                criticalExtensions
                enum c1 {
                    c1
                    RNReconfigurationr10IEs rnReconfigurationr10;
                    spare3 typedef spare2 typedef spare1 typedef;
                };
                struct
                    } criticalExtensionsFuture {
            };
        };
        ;

        struct RNReconfigurationr10IEs {
            RNSystemInfor10 rnSystemInfor10;
            RNSubframeConfigr10 rnSubframeConfigr10;
            char T string lateNonCriticalExtension;
            struct
                } nonCriticalExtension {
        };
        ;

        struct RNSystemInfor10 {
            char systemInformationBlockType1r10;
            SystemInformationBlockType2 systemInformationBlockType2r10;

            ;
        };
        ;
        // -- ASN1STOP
        //
        // -	RNReconfigurationComplete
        // The RNReconfigurationComplete message is used to confirm the successful completion of an RN reconfiguration.
        // Signalling radio bearer: SRB1
        // RLC-SAP: AM
        // Logical channel: DCCH
        // Direction: RN to E-UTRAN
        // RNReconfigurationComplete message
        // -- ASN1START
        ;

        struct RNReconfigurationCompleter10 {
            RRCTransactionIdentifier rrcTransactionIdentifier;

            enum criticalExtensions {

                criticalExtensions
                enum c1 {
                    c1
                    RNReconfigurationCompleter10IEs rnReconfigurationCompleter10;
                    spare3 typedef spare2 typedef spare1 typedef;
                };
                struct
                    } criticalExtensionsFuture {
            };
        };
        ;

        struct RNReconfigurationCompleter10IEs {
            char T string lateNonCriticalExtension;
            struct
                } nonCriticalExtension {
        };
        ;
        // -- ASN1STOP
        //
        // -	RRCConnectionReconfiguration
        // The RRCConnectionReconfiguration message is the command to modify an RRC connection. It may convey information for measurement configuration, mobility control, radio resource configuration (including RBs, MAC main configuration and physical channel configuration) including any associated dedicated NAS information and security configuration.
        // Signalling radio bearer: SRB1
        // RLC-SAP: AM
        // Logical channel: DCCH
        // Direction: E-UTRAN to UE
        // RRCConnectionReconfiguration message
        // -- ASN1START
        ;

        struct RRCConnectionReconfiguration {
            RRCTransactionIdentifier rrcTransactionIdentifier;

            enum criticalExtensions {

                criticalExtensions
                enum c1 {
                    c1
                    RRCConnectionReconfigurationr8IEs rrcConnectionReconfigurationr8;
                    spare7 typedef;
                    spare6 typedef spare5 typedef spare4 typedef;
                    spare3 typedef spare2 typedef spare1 typedef;
                };
                struct
                    } criticalExtensionsFuture {
            };
        };
        ;

        RRCConnectionReconfigurationr8IEs struct {
            MeasConfig measConfig;
            Cond HO MobilityControlInfo mobilityControlInfo;
            SEQU dedicatedInfoNASList;
            Cond nonHO DedicatedInfoNAS;
            Cond HOtoEUTRA RadioResourceConfigDedicated radioResourceConfigDedicated;
            Cond HO SecurityConfigHO securityConfigHO;
            RRCConnectionReconfigurationv890IEs nonCriticalExtension;
        };
        ;

        RRCConnectionReconfigurationv890IEs struct {
            char lateNonCriticalExtension;
            RRCConnectionReconfigurationv920IEs nonCriticalExtension;
        };
        ;
Late noncritical extensions :
        ;

        RRCConnectionReconfigurationv8m0IEs struct {
            Following field is only for pre REL10 late noncritical extensions;
        char T string lateNonCriticalExtension;
        RRCConnectionReconfigurationv10i0IEs nonCriticalExtension;
    };
    ;

    RRCConnectionReconfigurationv10i0IEs struct {
        AntennaInfoDedicatedv10i0 antennaInfoDedicatedPCellv10i0;
        Following field is only for late noncritical extensions from REL10;
        struct
            } nonCriticalExtension {
    };
    ;
Regular noncritical extensions :
    ;

    RRCConnectionReconfigurationv920IEs struct {
        OtherConfigr9 otherConfigr9;
        Cond HOReestab, enum true
    } fullConfigr9 {
        true
    }
    fullConfigr9,
    RRCConnectionReconfigurationv1020IEs nonCriticalExtension;
};
;

RRCConnectionReconfigurationv1020IEs struct {
    SCellToReleaseListr10 sCellToReleaseListr10;
    SCellToAddModListr10 sCellToAddModListr10;
    RRCConnectionReconfigurationv1130IEs nonCriticalExtension;
};
;

RRCConnectionReconfigurationv1130IEs struct {
    char systemInfomationBlockType1Dedicatedr11;
    RRCConnectionReconfigurationv1250IEs nonCriticalExtension;
};
;

RRCConnectionReconfigurationv1250IEs struct {

    enum wlanOffloadInfo {
        wlanOffloadInfo
        typedef release;

        struct setup {
            WLANOffloadConfig wlanOffloadConfigDedicated;

            enum t350 {
                min5, min10, min20, min30
                Need OR min120 min180 spare1
            };
        };
    };
    Cond nonFullConfig SCGConfiguration scgConfiguration;
    SLSyncTxControl slSyncTxControl;
    SLDiscConfig slDiscConfig;
    SLCommConfig slCommConfig;
    RRCConnectionReconfigurationv13xyIEs nonCriticalExtension;
};
;

RRCConnectionReconfigurationv13xyIEs struct {
    SCellToReleaseListExt_r13 sCellToReleaseListExt_r13;
    SCellToAddModListExt_r13 sCellToAddModListExt_r13;
    struct
        } nonCriticalExtension {
};
;

struct SLSyncTxControl {
    enum off
} networkControlledSyncTx {
    on,
};
;

struct PSCellToAddMod {
    SCellIndexr10 sCellIndex;

    struct cellIdentification {
        PhysCellId physCellId;
        ARFCNValueEUTRAr9 dlCarrierFreq;
        Cond SCellAdd
    };
    Cond SCellAdd RadioResourceConfigCommonPSCell radioResourceConfigCommonPSCell;
    Cond SCellAdd2 RadioResourceConfigDedicatedPSCell radioResourceConfigDedicatedPSCell;

    ;
    AntennaInfoDedicatedv10i0 antennaInfoDedicatedPSCellv1280;
    ;
    SCellIndex_r13 sCellIndex_r13;

    ;
};
;

PowerCoordinationInfo struct {
    int pMeNB;
    int pSeNB;
    int powerControlMode;
};
;
SEQU SCellToAddModListr10;
;
SEQU SCellToAddModListExt_r13;
;

struct SCellToAddModr10 {
    SCellIndexr10 sCellIndexr10;

    struct cellIdentificationr10 {
        PhysCellId physCellIdr10;
        ARFCNValueEUTRA dlCarrierFreqr10;
        Cond SCellAdd
    };
    Cond SCellAdd RadioResourceConfigCommonSCellr10 radioResourceConfigCommonSCellr10;
    Cond SCellAdd2 RadioResourceConfigDedicatedSCellr10 radioResourceConfigDedicatedSCellr10;

    ;
    Cond EARFCNmax ARFCNValueEUTRAv9e0 dlCarrierFreqv1090;
    ;
    AntennaInfoDedicatedv10i0 antennaInfoDedicatedSCellv10i0;

    ;
};
;

struct SCellToAddMod_r13 {
    SCellIndexv13xy sCellIndexv13xy;

    struct cellIdentification_r13 {
        PhysCellId physCellId_r13;
        ARFCNValueEUTRAr9 dlCarrierFreq_r13;
        Cond SCellAdd
    };
    Cond SCellAdd RadioResourceConfigCommonSCellr10 radioResourceConfigCommonSCell_r13;
    Cond SCellAdd2 RadioResourceConfigDedicatedSCellr10 radioResourceConfigDedicatedSCell_r13;
};
;
SEQU SCellToReleaseListr10;
;
SEQU SCellToReleaseListExt_r13;
;

enum SCGConfiguration {
    SCGConfiguration
    typedef release;

    struct setup {

        struct scgConfigPartMCG {
            int scgCounter;
            PowerCoordinationInfo powerCoordinationInfo;

            ;
        };
        SCGConfigPartSCG scgConfigPartSCG;
    };
};
;

struct SCGConfigPartSCG {
    RadioResourceConfigDedicatedSCG radioResourceConfigDedicatedSCG;
    SCellToReleaseListr10 sCellToReleaseListSCG;
    PSCellToAddMod pSCellToAddMod;
    SCellToAddModListr10 sCellToAddModListSCG;
    MobilityControlInfoSCG mobilityControlInfoSCG;

    ;

    ;
    SCellToReleaseListExt_r13 sCellToReleaseListSCGExt_r13;
    SCellToAddModListExt_r13 sCellToAddModListSCGExt_r13;

    ;
};
;

struct SecurityConfigHO {

    enum handoverType {

        handoverType
        struct intraLTE {
            Cond fullConfig SecurityAlgorithmConfig securityAlgorithmConfig;
            bool keyChangeIndicator;
            NextHopChainingCount nextHopChainingCount;
        };

        struct interRAT {
            SecurityAlgorithmConfig securityAlgorithmConfig;
            char nasSecurityParamToEUTRA;
        };
    };

    ;
};
;
;
// -- ASN1STOP
//
// RRCConnectionReconfiguration field descriptions
// dedicatedInfoNASList
// This field is used to transfer UE specific NAS layer information between the network and the UE. The RRC layer is transparent for each PDU in the list.
// fullConfig
// Indicates the full configuration option is applicable for the RRC Connection Reconfiguration message.
// keyChangeIndicator
// true is used only in an intra-cell handover when a KeNB key is derived from a KASME key taken into use through the latest successful NAS SMC procedure, as described in TS 33.401 [32] for KeNB re-keying. false is used in an intra-LTE handover when the new KeNB key is obtained from the current KeNB key or from the NH as described in TS 33.401 [32].
// nas-securityParamToEUTRA
// This field is used to transfer UE specific NAS layer information between the network and the UE. The RRC layer is transparent for this field, although it affects activation of AS- security after inter-RAT handover to E-UTRA. The content is defined in TS 24.301.
// networkControlledSyncTx
// This field indicates whether the UE shall transmit synchronisation information (i.e. become synchronisation source). Value On indicates the UE to transmit synchronisation information while value Off indicates the UE to not transmit such information.
// nextHopChainingCount
// Parameter NCC: See TS 33.401 [32]
// p-MeNB
// Indicates the guaranteed power for the MeNB, as specified in 36.213 [23].
// powerControlMode
// Indicates the power control mode used in DC. Value 1 corresponds to DC power control mode 1 and value 2 indicates DC power control mode 2, as specified in 36.213 [23].
// p-SeNB
// Indicates the guaranteed power for the SeNB as specified in 36.213 [23, Table 5.1.4.2-1].
// sCellIndex
// In case of DC, the SCellIndex is unique within the scope of the UE i.e. an SCG cell can not use the same value as used for an MCG cell. For PSCellToAddMod, if sCellIndex-r13 is present, sCellIndex-r12 shall be ignored.
// sCellToAddModListSCG
// Indicates the SCG cell to be added or modified. The field is used for SCG cells other than the PSCell (which is added/ modified by field pSCellToAddMod).
// sCellToReleaseListSCG
// Indicates the SCG cell to be released. The field is also used to release the PSCell e.g. upon change of PSCell, upon system information change for the PSCell.
// scg-Counter
// A counter used upon initial configuration of SCG security as well as upon refresh of S-KeNB. E-UTRAN includes the field upon SCG change when one or more SCG DRBs are configured. Otherwise E-UTRAN does not include the field.
// t350
// Timer T350 as described in section 7.3. Value minN corresponds to N minutes.
//
// Conditional presence
// Explanation
// EARFCN-max
// The field is mandatory present if dl-CarrierFreq-r10 is included and set to maxEARFCN. Otherwise the field is not present.
// fullConfig
// This field is mandatory present for handover within E-UTRA when the fullConfig is included; otherwise it is optionally present, Need OP.
// HO
// The field is mandatory present in case of handover within E-UTRA or to E-UTRA; otherwise the field is not present.
// HO-Reestab
// This field is optionally present, need ON, in case of handover within E-UTRA or upon the first reconfiguration after RRC connection re-establishment; otherwise the field is not present.
// HO-toEUTRA
// The field is mandatory present in case of handover to E-UTRA or for reconfigurations when fullConfig is included; otherwise the field is optionally present, need ON.
// nonFullConfig
// The field is not present in case of handover within E-UTRA when the fullConfig is included or in case of handover to E-UTRA; otherwise it is optional present, need ON.
// nonHO
// The field is not present in case of handover within E-UTRA or to E-UTRA; otherwise it is optional present, need ON.
// SCellAdd
// The field is mandatory present upon SCell addition; otherwise it is not present.
// SCellAdd2
// The field is mandatory present upon SCell addition; otherwise it is optionally present, need ON.
//
// -	RRCConnectionReconfigurationComplete
// The RRCConnectionReconfigurationComplete message is used to confirm the successful completion of an RRC connection reconfiguration.
// Signalling radio bearer: SRB1
// RLC-SAP: AM
// Logical channel: DCCH
// Direction: UE to E-UTRAN
// RRCConnectionReconfigurationComplete message
// -- ASN1START
;

RRCConnectionReconfigurationComplete struct {
    RRCTransactionIdentifier rrcTransactionIdentifier;

    enum criticalExtensions {
        criticalExtensions
        rrcConnectionReconfigurationCompleter8;
        RRCConnectionReconfigurationCompleter8IEs;
        struct
            } criticalExtensionsFuture {
    };
};
;

RRCConnectionReconfigurationCompleter8IEs struct {
    RRCConnectionReconfigurationCompletev8a0IEs nonCriticalExtension;
};
;

RRCConnectionReconfigurationCompletev8a0IEs struct {
    char T string lateNonCriticalExtension;
    RRCConnectionReconfigurationCompletev1020IEs nonCriticalExtension;
};
;

RRCConnectionReconfigurationCompletev1020IEs struct {
    , enum true
} rlfInfoAvailabler10 {
    true
}
rlfInfoAvailabler10,
, enum true
} logMeasAvailabler10 {
true
}
logMeasAvailabler10,
RRCConnectionReconfigurationCompletev1130IEs nonCriticalExtension;
};
;

RRCConnectionReconfigurationCompletev1130IEs struct {
, enum true
} connEstFailInfoAvailabler11 {
true
}
connEstFailInfoAvailabler11,
RRCConnectionReconfigurationCompletev1250IEs nonCriticalExtension;
};
;

RRCConnectionReconfigurationCompletev1250IEs struct {
, enum true
} logMeasAvailableMBSFN {
true
}
logMeasAvailableMBSFN,
struct
}
nonCriticalExtension {
};
;
// -- ASN1STOP
//
// -	RRCConnectionReestablishment
// The RRCConnectionReestablishment message is used to re-establish SRB1.
// Signalling radio bearer: SRB0
// RLC-SAP: TM
// Logical channel: CCCH
// Direction: E-UTRAN to UE
// RRCConnectionReestablishment message
// -- ASN1START
;

struct RRCConnectionReestablishment {
RRCTransactionIdentifier rrcTransactionIdentifier;

enum criticalExtensions {

    criticalExtensions
    enum c1 {
        c1
        RRCConnectionReestablishmentr8IEs rrcConnectionReestablishmentr8;
        spare7 typedef;
        typedef spare6 typedef spare5 typedef spare4;
        typedef spare3 typedef spare2 typedef spare1;
    };
    struct
        } criticalExtensionsFuture {
};
};
;

RRCConnectionReestablishmentr8IEs struct {
RadioResourceConfigDedicated radioResourceConfigDedicated;
NextHopChainingCount nextHopChainingCount;
RRCConnectionReestablishmentv8a0IEs nonCriticalExtension;
};
;

RRCConnectionReestablishmentv8a0IEs struct {
char T string lateNonCriticalExtension;
struct
    } nonCriticalExtension {
};
;
// -- ASN1STOP
//
// -	RRCConnectionReestablishmentComplete
// The RRCConnectionReestablishmentComplete message is used to confirm the successful completion of an RRC connection reestablishment.
// Signalling radio bearer: SRB1
// RLC-SAP: AM
// Logical channel: DCCH
// Direction: UE to E-UTRAN
// RRCConnectionReestablishmentComplete message
// -- ASN1START
;

RRCConnectionReestablishmentComplete struct {
RRCTransactionIdentifier rrcTransactionIdentifier;

enum criticalExtensions {
    criticalExtensions
    rrcConnectionReestablishmentCompleter8;
    RRCConnectionReestablishmentCompleter8IEs;
    struct
        } criticalExtensionsFuture {
};
};
;

RRCConnectionReestablishmentCompleter8IEs struct {
RRCConnectionReestablishmentCompletev920IEs nonCriticalExtension;
};
;

RRCConnectionReestablishmentCompletev920IEs struct {
, enum true
} rlfInfoAvailabler9 {
true
}
rlfInfoAvailabler9,
RRCConnectionReestablishmentCompletev8a0IEs nonCriticalExtension;
};
;

RRCConnectionReestablishmentCompletev8a0IEs struct {
char T string lateNonCriticalExtension;
RRCConnectionReestablishmentCompletev1020IEs nonCriticalExtension;
};
;

RRCConnectionReestablishmentCompletev1020IEs struct {
, enum true
} logMeasAvailabler10 {
true
}
logMeasAvailabler10,
RRCConnectionReestablishmentCompletev1130IEs nonCriticalExtension;
};
;

RRCConnectionReestablishmentCompletev1130IEs struct {
, enum true
} connEstFailInfoAvailabler11 {
true
}
connEstFailInfoAvailabler11,
RRCConnectionReestablishmentCompletev1250IEs nonCriticalExtension;
};
;

RRCConnectionReestablishmentCompletev1250IEs struct {
, enum true
} logMeasAvailableMBSFN {
true
}
logMeasAvailableMBSFN,
struct
}
nonCriticalExtension {
};
;
// -- ASN1STOP
//
// RRCConnectionReestablishmentComplete field descriptions
// rlf-InfoAvailable
// This field is used to indicate the availability of radio link failure or handover failure related measurements
//
// -	RRCConnectionReestablishmentReject
// The RRCConnectionReestablishmentReject message is used to indicate the rejection of an RRC connection reestablishment request.
// Signalling radio bearer: SRB0
// RLC-SAP: TM
// Logical channel: CCCH
// Direction: E-UTRAN to UE
// RRCConnectionReestablishmentReject message
// -- ASN1START
;

RRCConnectionReestablishmentReject struct {

enum criticalExtensions {
    criticalExtensions
    rrcConnectionReestablishmentRejectr8;
    RRCConnectionReestablishmentRejectr8IEs;
    struct
        } criticalExtensionsFuture {
};
};
;

RRCConnectionReestablishmentRejectr8IEs struct {
RRCConnectionReestablishmentRejectv8a0IEs nonCriticalExtension;
};
;

RRCConnectionReestablishmentRejectv8a0IEs struct {
char T string lateNonCriticalExtension;
struct
    } nonCriticalExtension {
};
;
// -- ASN1STOP
//
// -	RRCConnectionReestablishmentRequest
// The RRCConnectionReestablishmentRequest message is used to request the reestablishment of an RRC connection.
// Signalling radio bearer: SRB0
// RLC-SAP: TM
// Logical channel: CCCH
// Direction: UE to E-UTRAN
// RRCConnectionReestablishmentRequest message
// -- ASN1START
;

RRCConnectionReestablishmentRequest struct {

enum criticalExtensions {
    criticalExtensions
    rrcConnectionReestablishmentRequestr8;
    RRCConnectionReestablishmentRequestr8IEs;
    struct
        } criticalExtensionsFuture {
};
};
;

RRCConnectionReestablishmentRequestr8IEs struct {
ReestabUEIdentity ueIdentity;
ReestablishmentCause reestablishmentCause;
std::bitset<2> spare;
};
;

struct ReestabUEIdentity {
CRNTI cRNTI;
PhysCellId physCellId;
ShortMACI shortMACI;
};
;

enum ReestablishmentCause {
ReestablishmentCause,
reconfigurationFailure handoverFailure;
otherFailure spare1
};
;
// -- ASN1STOP
//
// RRCConnectionReestablishmentRequest field descriptions
// physCellId
// The Physical Cell Identity of the PCell the UE was connected to prior to the failure.
// reestablishmentCause
// Indicates the failure cause that triggered the re-establishment procedure.
// ue-Identity
// UE identity included to retrieve UE context and to facilitate contention resolution by lower layers.
//
// -	RRCConnectionReject
// The RRCConnectionReject message is used to reject the RRC connection establishment.
// Signalling radio bearer: SRB0
// RLC-SAP: TM
// Logical channel: CCCH
// Direction: E-UTRAN to UE
// RRCConnectionReject message
// -- ASN1START
;

struct RRCConnectionReject {

enum criticalExtensions {

    criticalExtensions
    enum c1 {
        c1
        RRCConnectionRejectr8IEs rrcConnectionRejectr8;
        spare3 typedef spare2 typedef spare1 typedef;
    };
    struct
        } criticalExtensionsFuture {
};
};
;

struct RRCConnectionRejectr8IEs {
int waitTime;
RRCConnectionRejectv8a0IEs nonCriticalExtension;
};
;

RRCConnectionRejectv8a0IEs struct {
char T string lateNonCriticalExtension;
RRCConnectionRejectv1020IEs nonCriticalExtension;
};
;

struct RRCConnectionRejectv1020IEs {
int extendedWaitTimer10;
RRCConnectionRejectv1130IEs nonCriticalExtension;
};
;

struct RRCConnectionRejectv1130IEs {

struct deprioritisationReqr11 {

    enum deprioritisationTyper11 {
        frequency,
        enum min30
    } deprioritisationTimerr11 {
        min5, min10
    };
    struct
        } nonCriticalExtension {
};
;
// -- ASN1STOP
//
// RRCConnectionReject field descriptions
// deprioritisationReq
// Indicates whether the current frequency or RAT is to be de-prioritised. The UE shall be able to store a depriotisation request for up to 8 frequencies (applicable when receiving another frequency specific deprioritisation request before T325 expiry).
// deprioritisationTimer
// Indicates the period for which either the current carrier frequency or E-UTRA is deprioritised. Value minN corresponds to N minutes.
// extendedWaitTime
// Value in seconds for the wait time for Delay Tolerant access requests.
// waitTime
// Wait time value in seconds.
//
// -	RRCConnectionRelease
// The RRCConnectionRelease message is used to command the release of an RRC connection.
// Signalling radio bearer: SRB1
// RLC-SAP: AM
// Logical channel: DCCH
// Direction: E-UTRAN to UE
// RRCConnectionRelease message
// -- ASN1START
;

struct RRCConnectionRelease {
    RRCTransactionIdentifier rrcTransactionIdentifier;

    enum criticalExtensions {

        criticalExtensions
        enum c1 {
            c1
            RRCConnectionReleaser8IEs rrcConnectionReleaser8;
            spare3 typedef spare2 typedef spare1 typedef;
        };
        struct
            } criticalExtensionsFuture {
    };
};
;

struct RRCConnectionReleaser8IEs {
    ReleaseCause releaseCause;
    RedirectedCarrierInfo redirectedCarrierInfo;
    IdleModeMobilityControlInfo idleModeMobilityControlInfo;
    RRCConnectionReleasev890IEs nonCriticalExtension;
};
;

struct RRCConnectionReleasev890IEs {
    char lateNonCriticalExtension;
    RRCConnectionReleasev920IEs nonCriticalExtension;
};
;
Late non critical extensions;

RRCConnectionReleasev9e0IEs struct {
    Cond NoRedirectr8 RedirectedCarrierInfov9e0 redirectedCarrierInfov9e0;
    Cond IdleInfoEUTRA IdleModeMobilityControlInfov9e0 idleModeMobilityControlInfov9e0;
    struct
        } nonCriticalExtension {
};
;
Regular non critical extensions;

struct RRCConnectionReleasev920IEs {

    enum cellInfoListr9 {
        cellInfoListr9
        CellInfoListGERANr9 geranr9;
        CellInfoListUTRAFDDr9 utraFDDr9;
        CellInfoListUTRATDDr9 utraTDDr9;

        ;
        CellInfoListUTRATDDr10 utraTDDr10;
        Cond Redirection
    };
    RRCConnectionReleasev1020IEs nonCriticalExtension;
};
;

struct RRCConnectionReleasev1020IEs {
    int extendedWaitTimer10;
    struct
        } nonCriticalExtension {
};
;

enum ReleaseCause {
    loadBalancingTAUrequired,
    other csFallbackHighPriorityv1020 spare1
};
;

enum RedirectedCarrierInfo {
    RedirectedCarrierInfo
    ARFCNValueEUTRA eutra;
    CarrierFreqsGERAN geran;
    ARFCNValueUTRA utraFDD;
    ARFCNValueUTRA utraTDD;
    CarrierFreqCDMA2000 cdma2000HRPD;
    CarrierFreqCDMA2000 cdma20001xRTT;

    ;
    CarrierFreqListUTRATDDr10 utraTDDr10;
};
;

struct RedirectedCarrierInfov9e0 {
    ARFCNValueEUTRAv9e0 eutrav9e0;
};
;
SEQU CarrierFreqListUTRATDDr10;
;

struct IdleModeMobilityControlInfo {
    FreqPriorityListEUTRA freqPriorityListEUTRA;
    FreqsPriorityListGERAN freqPriorityListGERAN;
    FreqPriorityListUTRAFDD freqPriorityListUTRAFDD;
    FreqPriorityListUTRATDD freqPriorityListUTRATDD;
    BandClassPriorityListHRPD bandClassPriorityListHRPD;
    BandClassPriorityList1XRTT bandClassPriorityList1XRTT;

    enum t320 {
        t320,
        min5 min10 min20 min30 min60 min120 min180;
        Need OR spare1
    };

    ;
    FreqPriorityListExtEUTRA freqPriorityListExtEUTRA;
    ;
    FreqPriorityListEUTRAv13xy freqPriorityListEUTRAv13xy;
    FreqPriorityListExtEUTRAv13xy freqPriorityListExtEUTRAv13xy;

    ;
};
;

struct IdleModeMobilityControlInfov9e0 {
    SEQU freqPriorityListEUTRAv9e0;
};
;
SEQU FreqPriorityListEUTRA;
;
SEQU FreqPriorityListExtEUTRA;
;
SEQU FreqPriorityListEUTRAv13xy;
;
SEQU FreqPriorityListExtEUTRAv13xy;
;

struct FreqPriorityEUTRA {
    ARFCNValueEUTRA carrierFreq;
    CellReselectionPriority cellReselectionPriority;
};
;

struct FreqPriorityEUTRAv9e0 {
    Cond EARFCNmax ARFCNValueEUTRAv9e0 carrierFreqv9e0;
};
;

struct FreqPriorityEUTRA {
    ARFCNValueEUTRAr9 carrierFreq;
    CellReselectionPriority cellReselectionPriority;
};
;

struct FreqPriorityEUTRAv13xy {
    CellReselectionSubPriority_r13 cellReselectionSubPriority_r13;
};
;
SEQU FreqsPriorityListGERAN;
;

struct FreqsPriorityGERAN {
    CarrierFreqsGERAN carrierFreqs;
    CellReselectionPriority cellReselectionPriority;
};
;
SEQU FreqPriorityListUTRAFDD;
;

struct FreqPriorityUTRAFDD {
    ARFCNValueUTRA carrierFreq;
    CellReselectionPriority cellReselectionPriority;
};
;
SEQU FreqPriorityListUTRATDD;
;

struct FreqPriorityUTRATDD {
    ARFCNValueUTRA carrierFreq;
    CellReselectionPriority cellReselectionPriority;
};
;
SEQU BandClassPriorityListHRPD;
;

struct BandClassPriorityHRPD {
    BandclassCDMA2000 bandClass;
    CellReselectionPriority cellReselectionPriority;
};
;
SEQU BandClassPriorityList1XRTT;
;

struct BandClassPriority1XRTT {
    BandclassCDMA2000 bandClass;
    CellReselectionPriority cellReselectionPriority;
};
;
SEQU CellInfoListGERANr9;
;

struct CellInfoGERANr9 {
    PhysCellIdGERAN physCellIdr9;
    CarrierFreqGERAN carrierFreqr9;
    SystemInfoListGERAN systemInformationr9;
};
;
SEQU CellInfoListUTRAFDDr9;
;

struct CellInfoUTRAFDDr9 {
    PhysCellIdUTRAFDD physCellIdr9;
    char T string utraBCCHContainerr9;
};
;
SEQU CellInfoListUTRATDDr9;
;

struct CellInfoUTRATDDr9 {
    PhysCellIdUTRATDD physCellIdr9;
    char T string utraBCCHContainerr9;
};
;
SEQU CellInfoListUTRATDDr10;
;

struct CellInfoUTRATDDr10 {
    PhysCellIdUTRATDD physCellIdr10;
    ARFCNValueUTRA carrierFreqr10;
    char T string utraBCCHContainerr10;
};
;
// -- ASN1STOP
//
// RRCConnectionRelease field descriptions
// carrierFreq or bandClass
// The carrier frequency (UTRA and E-UTRA) and band class (HRPD and 1xRTT) for which the associated cellReselectionPriority is applied.
// carrierFreqs
// The list of GERAN carrier frequencies organised into one group of GERAN carrier frequencies.
// cellInfoList
// Used to provide system information of one or more cells on the redirected inter-RAT carrier frequency. The system information can be used if, upon redirection, the UE selects an inter-RAT cell indicated by the physCellId and carrierFreq (GERAN and UTRA TDD) or by the physCellId (other RATs). The choice shall match the redirectedCarrierInfo. In particular, E-UTRAN only applies value utra-TDD-r10 in case redirectedCarrierInfo is set to utra-TDD-r10.
// cellReselectionSubPriority
// Fractional priority value added to cellReselectionPriority for the carrier.
// extendedWaitTime
// Value in seconds for the wait time for Delay Tolerant access requests.
// freqPriorityListX
// Provides a cell reselection priority for each frequency, by means of separate lists for each RAT (including E-UTRA). The UE shall be able to store at least 3 occurrences of FreqsPriorityGERAN. If E-UTRAN includes freqPriorityListEUTRA-v9e0 it includes the same number of entries, and listed in the same order, as in freqPriorityListEUTRA (i.e. without suffix). Field freqPriorityListExt includes additional neighbouring inter-frequencies, i.e. extending the size of the inter-frequency carrier list using the general principles specified in 5.1.2. EUTRAN only includes freqPriorityListExtEUTRA if freqPriorityListEUTRA (i.e without suffix) includes maxFreq entries.
// idleModeMobilityControlInfo
// Provides dedicated cell reselection priorities. Used for cell reselection as specified in TS 36.304 [4]. For E-UTRA and UTRA frequencies, a UE that supports multi-band cells for the concerned RAT considers the dedicated priorities to be common for all overlapping bands (i.e. regardless of the ARFCN that is used).
// redirectedCarrierInfo
// The redirectedCarrierInfo indicates a carrier frequency (downlink for FDD) and is used to redirect the UE to an E-UTRA or an inter-RAT carrier frequency, by means of the cell selection upon leaving RRC_CONNECTED as specified in TS 36.304 [4].
// releaseCause
// The releaseCause is used to indicate the reason for releasing the RRC Connection. The cause value cs-FallbackHighPriority is only applicable when redirectedCarrierInfo is present with the value set to utra-FDD, utra-TDD or utra-TDD-r10.
// E-UTRAN should not set the releaseCause to loadBalancingTAURequired or to cs-FallbackHighPriority if the extendedWaitTime is present.
// systemInformation
// Container for system information of the GERAN cell i.e. one or more System Information (SI) messages as defined in TS 44.018 [45, table 9.1.1].
// t320
// Timer T320 as described in section 7.3. Value minN corresponds to N minutes.
// utra-BCCH-Container
// Contains System Information Container message as defined in TS 25.331 [19].
//
// Conditional presence
// Explanation
// EARFCN-max
// The field is mandatory present if the corresponding carrierFreq (i.e. without suffix) is set to maxEARFCN. Otherwise the field is not present.
// IdleInfoEUTRA
// The field is optionally present, need OP, if the IdleModeMobilityControlInfo (i.e. without suffix) is included and includes freqPriorityListEUTRA; otherwise the field is not present.
// NoRedirect-r8
// The field is optionally present, need OP, if the redirectedCarrierInfo (i.e. without suffix) is not included; otherwise the field is not present.
// Redirection
// The field is optionally present, need ON, if the redirectedCarrierInfo is included and set to geran, utra-FDD, utra-TDD or utra-TDD-r10; otherwise the field is not present.
//
// -	RRCConnectionRequest
// The RRCConnectionRequest message is used to request the establishment of an RRC connection.
// Signalling radio bearer: SRB0
// RLC-SAP: TM
// Logical channel: CCCH
// Direction: UE to E-UTRAN
// RRCConnectionRequest message
// -- ASN1START
;

struct RRCConnectionRequest {

    enum criticalExtensions {
        criticalExtensions
        RRCConnectionRequestr8IEs rrcConnectionRequestr8;
        struct
            } criticalExtensionsFuture {
    };
};
;

struct RRCConnectionRequestr8IEs {
    InitialUEIdentity ueIdentity;
    EstablishmentCause establishmentCause;
    std::bitset<1> spare;
};
;

enum InitialUEIdentity {
    InitialUEIdentity
    STMSI sTMSI;
    std::bitset<40> randomValue;
};
;

enum EstablishmentCause {
    EstablishmentCause,
    emergency highPriorityAccess mtAccess moSignalling;
    moData delayTolerantAccessv1020 moVoiceCallv1280 spare1
};
;
// -- ASN1STOP
//
// RRCConnectionRequest field descriptions
// establishmentCause
// Provides the establishment cause for the RRC connection request as provided by the upper layers. W.r.t. the cause value names: highPriorityAccess concerns AC11..AC15, 'mt' stands for 'Mobile Terminating' and 'mo' for 'Mobile Originating.
// randomValue
// Integer value in the range 0 to 240 ? 1.
// ue-Identity
// UE identity included to facilitate contention resolution by lower layers.
//
// -	RRCConnectionSetup
// The RRCConnectionSetup message is used to establish SRB1.
// Signalling radio bearer: SRB0
// RLC-SAP: TM
// Logical channel: CCCH
// Direction: E-UTRAN to UE
// RRCConnectionSetup message
// -- ASN1START
;

struct RRCConnectionSetup {
    RRCTransactionIdentifier rrcTransactionIdentifier;

    enum criticalExtensions {

        criticalExtensions
        enum c1 {
            c1
            RRCConnectionSetupr8IEs rrcConnectionSetupr8;
            spare7 typedef;
            spare6 typedef spare5 typedef spare4 typedef;
            spare3 typedef spare2 typedef spare1 typedef;
        };
        struct
            } criticalExtensionsFuture {
    };
};
;

struct RRCConnectionSetupr8IEs {
    RadioResourceConfigDedicated radioResourceConfigDedicated;
    RRCConnectionSetupv8a0IEs nonCriticalExtension;
};
;

RRCConnectionSetupv8a0IEs struct {
    char T string lateNonCriticalExtension;
    struct
        } nonCriticalExtension {
};
;
// -- ASN1STOP
//
// -	RRCConnectionSetupComplete
// The RRCConnectionSetupComplete message is used to confirm the successful completion of an RRC connection establishment.
// Signalling radio bearer: SRB1
// RLC-SAP: AM
// Logical channel: DCCH
// Direction: UE to E-UTRAN
// RRCConnectionSetupComplete message
// -- ASN1START
;

struct RRCConnectionSetupComplete {
    RRCTransactionIdentifier rrcTransactionIdentifier;

    enum criticalExtensions {

        criticalExtensions
        enum c1 {
            c1
            RRCConnectionSetupCompleter8IEs rrcConnectionSetupCompleter8;
            spare3 typedef spare2 typedef spare1 typedef;
        };
        struct
            } criticalExtensionsFuture {
    };
};
;

RRCConnectionSetupCompleter8IEs struct {
    int selectedPLMNIdentity;
    RegisteredMME registeredMME;
    DedicatedInfoNAS dedicatedInfoNAS;
    RRCConnectionSetupCompletev8a0IEs nonCriticalExtension;
};
;

RRCConnectionSetupCompletev8a0IEs struct {
    char T string lateNonCriticalExtension;
    RRCConnectionSetupCompletev1020IEs nonCriticalExtension;
};
;

RRCConnectionSetupCompletev1020IEs struct {
    , enum mapped
} gummeiTyper10 {
    native,
    , enum true
}
rlfInfoAvailabler10 {
    true
}
rlfInfoAvailabler10,
, enum true
} logMeasAvailabler10 {
true
}
logMeasAvailabler10,
, enum notRequired
}
rnSubframeConfigReqr10 {
required,
RRCConnectionSetupCompletev1130IEs nonCriticalExtension;
};
;

RRCConnectionSetupCompletev1130IEs struct {
, enum true
} connEstFailInfoAvailabler11 {
true
}
connEstFailInfoAvailabler11,
RRCConnectionSetupCompletev1250IEs nonCriticalExtension;
};
;

RRCConnectionSetupCompletev1250IEs struct {
, enum spare
} mobilityState {
normal, medium
, enum true
}
mobilityHistoryAvail {
true
}
mobilityHistoryAvail,
, enum true
}
logMeasAvailableMBSFN {
true
}
logMeasAvailableMBSFN,
struct
}
nonCriticalExtension {
};
;

struct RegisteredMME {
PLMNIdentity plmnIdentity;
std::bitset<16> mmegi;
MMEC mmec;
};
;
// -- ASN1STOP
//
// RRCConnectionSetupComplete field descriptions
// gummei-Type
// This field is used to indicate whether the GUMMEI included is native (assigned by EPC) or mapped (from 2G/3G identifiers).
// mmegi
// Provides the Group Identity of the registered MME within the PLMN, as provided by upper layers, see TS 23.003 [27].
// mobilityState
// This field indicates the UE mobility state (as defined in TS 36.304 [4, 5.2.4.3]) just prior to UE going into RRC_CONNECTED state. The UE indicates the value of medium and high when being in Medium-mobility and High-mobility states respectively. Otherwise the UE indicates the value normal.
// registeredMME
// This field is used to transfer the GUMMEI of the MME where the UE is registered, as provided by upper layers.
// rn-SubframeConfigReq
// If present, this field indicates that the connection establishment is for an RN and whether a subframe configuration is requested or not.
// selectedPLMN-Identity
// Index of the PLMN selected by the UE from the plmn-IdentityList included in SIB1. 1 if the 1st PLMN is selected from the plmn-IdentityList included in SIB1, 2 if the 2nd PLMN is selected from the plmn-IdentityList included in SIB1 and so on.
//
// -	SCGFailureInformation
// The SCGFailureInformation message is used to provide information regarding failures detected by the UE.
// Signalling radio bearer: SRB1
// RLC-SAP: AM
// Logical channel: DCCH
// Direction: UE to E-UTRAN
// SCGFailureInformation message
// -- ASN1START
;

struct SCGFailureInformation {

enum criticalExtensions {

    criticalExtensions
    enum c1 {
        c1
        SCGFailureInformationIEs scgFailureInformation;
        spare3 typedef spare2 typedef spare1 typedef;
    };
    struct
        } criticalExtensionsFuture {
};
};
;

struct SCGFailureInformationIEs {
FailureReportSCG failureReportSCG;
struct
    } nonCriticalExtension {
};
;

struct FailureReportSCG {

enum failureType {
    t313Expiry,
    rlcMaxNumRetx scgChangeFailure
};
MeasResultServFreqListr10 measResultServFreqList;
MeasResultList2EUTRAr9 measResultNeighCells;

;

;
MeasResultServFreqListExt_r13 measResultServFreqListExt_r13;

;
};
;
// -- ASN1STOP
//
// -	SCPTMConfiguration
// The SCPTMConfiguration message contains the control information applicable for MBMS services transmitted via SC-MRB.
// Signalling radio bearer: N/A
// RLC-SAP: UM
// Logical channel: SC-MCCH
// Direction: E-UTRAN to UE
// SCPTMConfiguration message
// -- ASN1START
;

struct SCPTMConfiguration_r13 {
SCMTCHInfoList_r13 scmtchInfoList_r13;
SCPTMNeighbourCellList_r13 scptmNeighbourCellList_r13;

;
};
;
// -- ASN1STOP
//
// SCPTMConfiguration field descriptions
// sc-mtch-InfoList
// Provides the configuration of each SC-MTCH in the current cell.
// scptmNeighbourCellList
// List of neighbour cells providing MBMS services via SC-MRB. Absence of this field means that MBMS services listed in the SCPTMConfiguration message are not provided via SC-MRB in any neighbour cell.
//
// -	SecurityModeCommand
// The SecurityModeCommand message is used to command the activation of AS security.
// Signalling radio bearer: SRB1
// RLC-SAP: AM
// Logical channel: DCCH
// Direction: E-UTRAN to UE
// SecurityModeCommand message
// -- ASN1START
;

struct SecurityModeCommand {
RRCTransactionIdentifier rrcTransactionIdentifier;

enum criticalExtensions {

    criticalExtensions
    enum c1 {
        c1
        SecurityModeCommandr8IEs securityModeCommandr8;
        spare3 typedef spare2 typedef spare1 typedef;
    };
    struct
        } criticalExtensionsFuture {
};
};
;

struct SecurityModeCommandr8IEs {
SecurityConfigSMC securityConfigSMC;
SecurityModeCommandv8a0IEs nonCriticalExtension;
};
;

SecurityModeCommandv8a0IEs struct {
char T string lateNonCriticalExtension;
struct
    } nonCriticalExtension {
};
;

struct SecurityConfigSMC {
SecurityAlgorithmConfig securityAlgorithmConfig;

;
};
;
// -- ASN1STOP
//
// -	SecurityModeComplete
// The SecurityModeComplete message is used to confirm the successful completion of a security mode command.
// Signalling radio bearer: SRB1
// RLC-SAP: AM
// Logical channel: DCCH
// Direction: UE to E-UTRAN
// SecurityModeComplete message
// -- ASN1START
;

struct SecurityModeComplete {
RRCTransactionIdentifier rrcTransactionIdentifier;

enum criticalExtensions {
    criticalExtensions
    SecurityModeCompleter8IEs securityModeCompleter8;
    struct
        } criticalExtensionsFuture {
};
};
;

struct SecurityModeCompleter8IEs {
SecurityModeCompletev8a0IEs nonCriticalExtension;
};
;

SecurityModeCompletev8a0IEs struct {
char T string lateNonCriticalExtension;
struct
    } nonCriticalExtension {
};
;
// -- ASN1STOP
//
// -	SecurityModeFailure
// The SecurityModeFailure message is used to indicate an unsuccessful completion of a security mode command.
// Signalling radio bearer: SRB1
// RLC-SAP: AM
// Logical channel: DCCH
// Direction: UE to E-UTRAN
// SecurityModeFailure message
// -- ASN1START
;

struct SecurityModeFailure {
RRCTransactionIdentifier rrcTransactionIdentifier;

enum criticalExtensions {
    criticalExtensions
    SecurityModeFailurer8IEs securityModeFailurer8;
    struct
        } criticalExtensionsFuture {
};
};
;

struct SecurityModeFailurer8IEs {
SecurityModeFailurev8a0IEs nonCriticalExtension;
};
;

SecurityModeFailurev8a0IEs struct {
char T string lateNonCriticalExtension;
struct
    } nonCriticalExtension {
};


// The SidelinkUEInformation message is used for the indication of sidelink information to the eNB.
// Signalling radio bearer: SRB1
// RLC-SAP: AM
// Logical channel: DCCH
// Direction: UE to E-UTRAN

// Moved to lte-rrc-sidelink.h


//
// -	SystemInformation
// The SystemInformation message is used to convey one or more System Information Blocks. All the SIBs included are transmitted with the same periodicity.
// Signalling radio bearer: N/A
// RLC-SAP: TM
// Logical channel: BCCH
// Direction: E-UTRAN to UE
// SystemInformation message
// -- ASN1START
;

struct SystemInformation {

enum criticalExtensions {
    criticalExtensions
    SystemInformationr8IEs systemInformationr8;
    struct
        } criticalExtensionsFuture {
};
};

struct SystemInformationr8IEs {
SEQU sibTypeAndInfo;
SystemInformationBlockType2 sib2;
SystemInformationBlockType3 sib3;
SystemInformationBlockType4 sib4;
SystemInformationBlockType5 sib5;
SystemInformationBlockType6 sib6;
SystemInformationBlockType7 sib7;
SystemInformationBlockType8 sib8;
SystemInformationBlockType9 sib9;
SystemInformationBlockType10 sib10;
SystemInformationBlockType11 sib11;

;
SystemInformationBlockType12r9 sib12v920;
SystemInformationBlockType13r9 sib13v920;
SystemInformationBlockType14r11 sib14v1130;
SystemInformationBlockType15r11 sib15v1130;
SystemInformationBlockType16r11 sib16v1130;
SystemInformationBlockType17 sib17v1250;
SystemInformationBlockType18 sib18v1250;
SystemInformationBlockType19 sib19v1250;
SystemInformationBlockType20_r13 sib20v13x0;
};
SystemInformationv8a0IEs nonCriticalExtension;
};
;

SystemInformationv8a0IEs struct {
char T string lateNonCriticalExtension;
struct
    } nonCriticalExtension {
};

// SystemInformationBlockType1 contains information relevant when evaluating if a UE is allowed to access a cell and defines the scheduling of other system information.
// Signalling radio bearer: N/A
// RLC-SAP: TM
// Logical channel: BCCH
// Direction: E-UTRAN to UE
// SystemInformationBlockType1 message

struct SystemInformationBlockType1 {

struct cellAccessRelatedInfo {
    PLMNIdentityList plmnIdentityList;
    TrackingAreaCode trackingAreaCode;
    CellIdentity cellIdentity;

    enum cellBarred {

        barred,
        enum intraFreqReselection {
            allowed,
            bool csgIndication;
            Need OR CSGIdentity csgIdentity;
        };

        struct cellSelectionInfo {
            QRxLevMin qRxLevMin;
            int qRxLevMinOffset;
        };
        PMax pMax;
        FreqBandIndicator freqBandIndicator;
        SchedulingInfoList schedulingInfoList;
        Cond TDD TDDConfig tddConfig;

        enum siWindowLength {
            siWindowLength,
            ms1 ms2 ms5 ms10 ms15 ms20;
            ms40
        };
        int systemInfoValueTag;
        SystemInformationBlockType1v890IEs nonCriticalExtension;
    };
    ;

    struct SystemInformationBlockType1v890IEs {
        char lateNonCriticalExtension;
        SystemInformationBlockType1v920IEs nonCriticalExtension;
    };
    ;
    Late non critical extensions;

    struct SystemInformationBlockType1v8h0IEs {
        Need OR MultiBandInfoList multiBandInfoList;
        SystemInformationBlockType1v9e0IEs nonCriticalExtension;
    };
    ;

    SystemInformationBlockType1v9e0IEs struct {
        Cond FBImax FreqBandIndicatorv9e0 freqBandIndicatorv9e0;
        Cond mFBImax MultiBandInfoListv9e0 multiBandInfoListv9e0;
        SystemInformationBlockType1v10j0IEs nonCriticalExtension;
    };
    ;

    SystemInformationBlockType1v10j0IEs struct {
        Need OR NSPmaxListr10 freqBandInfor10;
        Need OR MultiBandInfoListv10j0 multiBandInfoListv10j0;
        struct
            } nonCriticalExtension {
    };
    ;
    Regular non critical extensions;

    struct SystemInformationBlockType1v920IEs {
        Need OR, enum true
    } imsEmergencySupportr9 {
        true
    }
    imsEmergencySupportr9,
    Cond RSRQ CellSelectionInfov920 cellSelectionInfov920;
    SystemInformationBlockType1v1130IEs nonCriticalExtension;
};
;

struct SystemInformationBlockType1v1130IEs {
    Cond TDDOR TDDConfigv1130 tddConfigv1130;
    Cond WBRSRQ CellSelectionInfov1130 cellSelectionInfov1130;
    SystemInformationBlockType1v1250IEs nonCriticalExtension;
};
;

struct SystemInformationBlockType1v1250IEs {

    struct cellAccessRelatedInfov1250 {
        enum true
    } category0Allowed {
        true
    }
    category0Allowed,
};
Cond RSRQ2 CellSelectionInfov1250 cellSelectionInfov1250;
Cond mFBI, enum true
} freqBandIndicatorPriority {
true
}
freqBandIndicatorPriority,
SystemInformationBlockType1v13xyIEs nonCriticalExtension;
};
;

struct SystemInformationBlockType1v13xyIEs {
std::bitset<10> hyperSFN;
, enum true
} eDRXAllowed {
true
}
eDRXAllowed,
struct
}
nonCriticalExtension {
};
;
SEQU PLMNIdentityList;
;

struct PLMNIdentityInfo {
PLMNIdentity plmnIdentity;
enum notReserved
} cellReservedForOperatorUse {
reserved,
};
;
Sche;
;

struct SchedulingInfo {

enum siPeriodicity {
    siPeriodicity,
    rf8 rf16 rf32 rf64 rf128 rf256 rf512
};
SIBMappingInfo sibMappingInfo;
};
;
SIB;
;

enum SIBType {
SIBType,
sibType3 sibType4 sibType5 sibType6;
sibType7 sibType8 sibType9 sibType10;
sibType11 sibType12v920 sibType13v920;
sibType14v1130 sibType15v1130;
sibType16v1130 sibType17v1250 sibType18v1250;

sibType19v1250 sibType20v13xy
};
;

struct CellSelectionInfov920 {
QQualMinr9 qQualMinr9;
int qQualMinOffsetr9;
};
;

struct CellSelectionInfov1130 {
QQualMinr9 qQualMinWBr11;
};
;

struct CellSelectionInfov1250 {
QQualMinr9 qQualMinRSRQOnAllSymbols;
};
;
// -- ASN1STOP
//
// SystemInformationBlockType1 field descriptions
// category0Allowed
// The presence of this field indicates category 0 UEs are allowed to access the cell.
// cellBarred
// barred means the cell is barred, as defined in TS 36.304 [4].
// cellReservedForOperatorUse
// As defined in TS 36.304 [4].
// csg-Identity
// Identity of the Closed Subscriber Group the cell belongs to.
// csg-Indication
// If set to TRUE the UE is only allowed to access the cell if it is a CSG member cell, if selected during manual CSG selection or to obtain limited service, see TS 36.304 [4].
// eDRXAllowed
// Indicates if idle mode extended DRX is allowed in the cell. The UE shall not operate in idle mode extended DRX unless eDRXAllowed  is set to TRUE.
// freqBandIndicatorPriority
// If the field is present and supported by the UE, the UE shall prioritize the frequency bands in the multiBandInfoList IE in decreasing priority order. Only if the UE does not support any of the frequency band in multiBandInfoList, the UE shall use the value in freqBandIndicator IE. Otherwise, the UE applies frequency band according to the rules defined in multiBandInfoList.
// freqBandInfo
// A list of additionalPmax and additionalSpectrumEmission values as defined in TS 36.101 [42, table 6.2.4-1] for the frequency band in freqBandIndicator.
// hyperSFN
// Indicates hyper SFN which increments by one when the SFN wraps around.
// ims-EmergencySupport
// Indicates whether the cell supports IMS emergency bearer services for UEs in limited service mode. If absent, IMS emergency call is not supported by the network in the cell for UEs in limited service mode.
// intraFreqReselection
// Used to control cell reselection to intra-frequency cells when the highest ranked cell is barred, or treated as barred by the UE, as specified in TS 36.304 [4].
// multiBandInfoList
// A list of additional frequency band indicators, as defined in TS 36.101 [42, table 5.5-1] that the cell belongs to. If the UE supports the frequency band in the freqBandIndicator IE it shall apply that frequency band. Otherwise, the UE shall apply the first listed band which it supports in the multiBandInfoList IE. If E-UTRAN includes multiBandInfoList-v9e0 it includes the same number of entries, and listed in the same order, as in multiBandInfoList (i.e. without suffix). See Annex D for more descriptions. The UE shall ignore the rule defined in this field description if freqBandIndicatorPriority is present and supported by the UE.
// multiBandInfoList-v10j0
// A list of additionalPmax and additionalSpectrumEmission values as defined in TS 36.101 [42, table 6.2.4-1] for the frequency bands in multiBandInfoList (i.e. without suffix) and multiBandInfoList-v9e0. If E-UTRAN includes multiBandInfoList-v10j0, it includes the same number of entries, and listed in the same order, as in multiBandInfoList (i.e. without suffix).
// plmn-IdentityList
// List of PLMN identities. The first listed PLMN-Identity is the primary PLMN.
// p-Max
// Value applicable for the cell. If absent the UE applies the maximum power according to the UE capability.
// q-QualMin
// Parameter "Qqualmin" in TS 36.304 [4]. If cellSelectionInfo-v920 is not present, the UE applies the (default) value of negative infinity for Qqualmin. NOTE 1.
// q-QualMinRSRQ-OnAllSymbols
// If this field is present and supported by the UE, the UE shall, when performing RSRQ measurements, perform RSRQ measurement on all OFDM symbols in accordance with TS 36.214 [48]. NOTE 1.
// q-QualMinOffset
// Parameter "Qqualminoffset" in TS 36.304 [4]. Actual value Qqualminoffset = IE value [dB]. If cellSelectionInfo-v920 is not present or the field is not present, the UE applies the (default) value of 0 dB for Qqualminoffset. Affects the minimum required quality level in the cell.
// q-QualMinWB
// If this field is present and supported by the UE, the UE shall, when performing RSRQ measurements, use a wider bandwidth in accordance with TS 36.133 [16]. NOTE 1.
// q-RxLevMinOffset
// Parameter Qrxlevminoffset in TS 36.304 [4]. Actual value Qrxlevminoffset = IE value * 2 [dB]. If absent, the UE applies the (default) value of 0 dB for Qrxlevminoffset. Affects the minimum required Rx level in the cell.
// sib-MappingInfo
// List of the SIBs mapped to this SystemInformation message.There is no mapping information of SIB2; it is always present in the first SystemInformation message listed in the schedulingInfoList list.
// si-Periodicity
// Periodicity of the SI-message in radio frames, such that rf8 denotes 8 radio frames, rf16 denotes 16 radio frames, and so on.
// si-WindowLength
// Common SI scheduling window for all SIs. Unit in milliseconds, where ms1 denotes 1 millisecond, ms2 denotes 2 milliseconds and so on.
// systemInfoValueTag
// Common for all SIBs other than MIB, SIB1, SIB10, SIB11, SIB12 and SIB14. Change of MIB and SIB1 is detected by acquisition of the corresponding message.
// trackingAreaCode
// A trackingAreaCode that is common for all the PLMNs listed.
//
// NOTE 1:	The value the UE applies for parameter "Qqualmin" in TS 36.304 [4] depends on the q-QualMin fields signalled by E-UTRAN and supported by the UE. In case multiple candidate options are available, the UE shall select the highest priority candidate option according to the priority order indicated by the following table (top row is highest priority).
// q-QualMinRSRQ-OnAllSymbols
// q-QualMinWB
// Value of parameter "Qqualmin" in TS 36.304 [4]
// Included
// Included
// q-QualMinRSRQ-OnAllSymbols - (q-QualMin - q-QualMinWB)
// Included
// Not included
// q-QualMinRSRQ-OnAllSymbols
// Not included
// Included
// q-QualMinWB
// Not included
// Not included
// q-QualMin
//
// Conditional presence
// Explanation
// FBI-max
// The field is mandatory present if freqBandIndicator (i.e. without suffix) is set to maxFBI. Otherwise the field is not present.
// mFBI
// The field is optional present, Need OR, if multiBandInfoList is present. Otherwise the field is not present.
// mFBI-max
// The field is mandatory present if one or more entries in multiBandInfoList (i.e. without suffix, introduced in -v8h0) is set to maxFBI. Otherwise the field is not present.
// RSRQ
// The field is mandatory present if SIB3 is being broadcast and threshServingLowQ is present in SIB3; otherwise optionally present, Need OP.
// RSRQ2
// The field is mandatory present if q-QualMinRSRQ-OnAllSymbols is present in SIB3; otherwise it is not present and the UE shall delete any existing value for this field.
// TDD
// This field is mandatory present for TDD; it is not present for FDD and the UE shall delete any existing value for this field.
// TDD-OR
// The field is optional present for TDD, need OR; it is not present for FDD.
// WB-RSRQ
// The field is optionally present, need OP if the measurement bandwidth indicated by allowedMeasBandwidth in systemInformationBlockType3 is 50 resource blocks or larger; otherwise it is not present.
//
// -	UEAssistanceInformation
// The UEAssistanceInformation message is used for the indication of UE assistance information to the eNB.
// Signalling radio bearer: SRB1
// RLC-SAP: AM
// Logical channel: DCCH
// Direction: UE to E-UTRAN
// UEAssistanceInformation message
// -- ASN1START
;

struct UEAssistanceInformationr11 {

enum criticalExtensions {

    criticalExtensions
    enum c1 {
        c1
        UEAssistanceInformationr11IEs ueAssistanceInformationr11;
        spare3 typedef spare2 typedef spare1 typedef;
    };
    struct
        } criticalExtensionsFuture {
};
};
;

struct UEAssistanceInformationr11IEs {
, lowPowerConsumption
}

enum powerPrefIndicationr11 {
normal,
char T string lateNonCriticalExtension;
struct
    } nonCriticalExtension {
};
;
// -- ASN1STOP
//
// UEAssistanceInformation field descriptions
// powerPrefIndication
// Value lowPowerConsumption indicates the UE prefers a configuration that is primarily optimised for power saving. Otherwise the value is set to normal.
//
// -	UECapabilityEnquiry
// The UECapabilityEnquiry message is used to request the transfer of UE radio access capabilities for E-UTRA as well as for other RATs.
// Signalling radio bearer: SRB1
// RLC-SAP: AM
// Logical channel: DCCH
// Direction: E-UTRAN to UE
// UECapabilityEnquiry message
// -- ASN1START
;

struct UECapabilityEnquiry {
RRCTransactionIdentifier rrcTransactionIdentifier;

enum criticalExtensions {

    criticalExtensions
    enum c1 {
        c1
        UECapabilityEnquiryr8IEs ueCapabilityEnquiryr8;
        spare3 typedef spare2 typedef spare1 typedef;
    };
    struct
        } criticalExtensionsFuture {
};
};
;

struct UECapabilityEnquiryr8IEs {
UECapabilityRequest ueCapabilityRequest;
UECapabilityEnquiryv8a0IEs nonCriticalExtension;
};
;

UECapabilityEnquiryv8a0IEs struct {
char T string lateNonCriticalExtension;
UECapabilityEnquiryv1180IEs nonCriticalExtension;
};
;

UECapabilityEnquiryv1180IEs struct {
SEQU requestedFrequencyBandsr11;
UECapabilityEnquiryv13xyIEs nonCriticalExtension;
};
;

UECapabilityEnquiryv13xyIEs struct {
, enum true
} requestReducedFormat_r13 {
true
}
requestReducedFormat_r13,
, enum true
}
skipFallbackCombinations_r13 {
true
}
skipFallbackCombinations_r13,
int requestedMaxCCsDL_r13;
int requestedMaxCCsUL_r13;
struct
}
nonCriticalExtension {
};
;
SEQU UECapabilityRequest;
;
// -- ASN1STOP
//
// UECapabilityEnquiry field descriptions
// requestReducedFormat
// Indicates that the UE if supported is requested to provide supported CA band combinations in the supportedBandCombinations-r13 instead of the supportedBandCombination-r10.
// skipFallbackCombinations
// Indicates that the UE shall explicitly exclude fallback CA band combinations in capability signalling.
// ue-CapabilityRequest
// List of the RATs for which the UE is requested to transfer the UE radio access capabilities i.e. E-UTRA, UTRA, GERAN-CS, GERAN-PS, CDMA2000.
// requestedFrequencyBands
// List of frequency bands for which the UE is requested to provide supported CA band combinations and non CA bands.
// requestedMaxCCsDL, requestedMaxCCsUL
// Indicates the maximum number of CCs for which the UE is requested to provide supported CA band combinations and non-CA bands.
//
// -	UECapabilityInformation
// The UECapabilityInformation message is used to transfer of UE radio access capabilities requested by the E-UTRAN.
// Signalling radio bearer: SRB1
// RLC-SAP: AM
// Logical channel: DCCH
// Direction: UE to E-UTRAN
// UECapabilityInformation message
// -- ASN1START
;

struct UECapabilityInformation {
RRCTransactionIdentifier rrcTransactionIdentifier;

enum criticalExtensions {

    criticalExtensions
    enum c1 {
        c1
        UECapabilityInformationr8IEs ueCapabilityInformationr8;
        spare7 typedef;
        spare6 typedef spare5 typedef spare4 typedef;
        spare3 typedef spare2 typedef spare1 typedef;
    };
    struct
        } criticalExtensionsFuture {
};
};
;

struct UECapabilityInformationr8IEs {
UECapabilityRATContainerList ueCapabilityRATContainerList;
UECapabilityInformationv8a0IEs nonCriticalExtension;
};
;

UECapabilityInformationv8a0IEs struct {
char T string lateNonCriticalExtension;
UECapabilityInformationv1250IEs nonCriticalExtension;
};
;

UECapabilityInformationv1250IEs struct {
UERadioPagingInfo ueRadioPagingInfo;
struct
    } nonCriticalExtension {
};
;
// -- ASN1STOP
//
// UECapabilityInformation field descriptions
// ue-RadioPagingInfo
// This field contains information used for paging of category 0 UEs.
//
// -	UEInformationRequest
// The UEInformationRequest is the command used by E-UTRAN to retrieve information from the UE.
// Signalling radio bearer: SRB1
// RLC-SAP: AM
// Logical channel: DCCH
// Direction: E-UTRAN to UE
// UEInformationRequest message
// -- ASN1START
;

struct UEInformationRequestr9 {
RRCTransactionIdentifier rrcTransactionIdentifier;

enum criticalExtensions {

    criticalExtensions
    enum c1 {
        c1
        UEInformationRequestr9IEs ueInformationRequestr9;
        spare3 typedef spare2 typedef spare1 typedef;
    };
    struct
        } criticalExtensionsFuture {
};
};
;

struct UEInformationRequestr9IEs {
bool rachReportReqr9;
bool rlfReportReqr9;
UEInformationRequestv930IEs nonCriticalExtension;
};
;

UEInformationRequestv930IEs struct {
char T string lateNonCriticalExtension;
UEInformationRequestv1020IEs nonCriticalExtension;
};
;

struct UEInformationRequestv1020IEs {
, enum true
} logMeasReportReqr10 {
true
}
logMeasReportReqr10,
UEInformationRequestv1130IEs nonCriticalExtension;
};
;

UEInformationRequestv1130IEs struct {
, enum true
} connEstFailReportReqr11 {
true
}
connEstFailReportReqr11,
UEInformationRequestv1250IEs nonCriticalExtension;
};
;

UEInformationRequestv1250IEs struct {
, enum true
} mobilityHistoryReportReq {
true
}
mobilityHistoryReportReq,
struct
}
nonCriticalExtension {
};
;
// -- ASN1STOP
//
// UEInformationRequest field descriptions
// rach-ReportReq
// This field is used to indicate whether the UE shall report information about the random access procedure.
//
// -	UEInformationResponse
// The UEInformationResponse message is used by the UE to transfer the information requested by the E-UTRAN.
// Signalling radio bearer: SRB1 or SRB2 (when logged measurement information is included)
// RLC-SAP: AM
// Logical channel: DCCH
// Direction: UE to E-UTRAN
// UEInformationResponse message
// -- ASN1START
;

struct UEInformationResponser9 {
RRCTransactionIdentifier rrcTransactionIdentifier;

enum criticalExtensions {

    criticalExtensions
    enum c1 {
        c1
        UEInformationResponser9IEs ueInformationResponser9;
        spare3 typedef spare2 typedef spare1 typedef;
    };
    struct
        } criticalExtensionsFuture {
};
};
;

struct UEInformationResponser9IEs {

struct rachReportr9 {
    NumberOfPreamblesSentr11 numberOfPreamblesSentr9;
    bool contentionDetectedr9;
};
RLFReportr9 rlfReportr9;
UEInformationResponsev930IEs nonCriticalExtension;
};
;
Late non critical extensions;

UEInformationResponsev9e0IEs struct {
RLFReportv9e0 rlfReportv9e0;
struct
    } nonCriticalExtension {
};
;
Regular non critical extensions;

struct UEInformationResponsev930IEs {
char lateNonCriticalExtension;
UEInformationResponsev1020IEs nonCriticalExtension;
};
;

UEInformationResponsev1020IEs struct {
LogMeasReportr10 logMeasReportr10;
UEInformationResponsev1130IEs nonCriticalExtension;
};
;

UEInformationResponsev1130IEs struct {
ConnEstFailReportr11 connEstFailReportr11;
UEInformationResponsev1250IEs nonCriticalExtension;
};
;

UEInformationResponsev1250IEs struct {
MobilityHistoryReport mobilityHistoryReport;
struct
    } nonCriticalExtension {
};
;

struct RLFReportr9 {

struct measResultLastServCellr9 {
    RSRPRange rsrpResultr9;
    RSRQRange rsrqResultr9;
};

struct measResultNeighCellsr9 {
    MeasResultList2EUTRAr9 measResultListEUTRAr9;
    MeasResultList2UTRAr9 measResultListUTRAr9;
    MeasResultListGERAN measResultListGERANr9;
    MeasResultList2CDMA2000r9 measResultsCDMA2000r9;
};

;
LocationInfor10 locationInfor10;

enum failedPCellIdr10 {
    failedPCellIdr10
    CellGlobalIdEUTRA cellGlobalIdr10;

    struct pciarfcnr10 {
        PhysCellId physCellIdr10;
        ARFCNValueEUTRA carrierFreqr10;
    };
};
CellGlobalIdEUTRA reestablishmentCellIdr10;
int timeConnFailurer10;
, enum hof
} connectionFailureTyper10 {
rlf,
CellGlobalIdEUTRA previousPCellIdr10;
;

struct failedPCellIdv1090 {
    ARFCNValueEUTRAv9e0 carrierFreqv1090;
};
;

struct basicFieldsr11 {
    CRNTI cRNTIr11;

    enum rlfCauser11 {
        rlfCauser11,
        t310Expiry randomAccessProblem;
        rlcMaxNumRetx t312Expiry
    };
    TimeSinceFailurer11 timeSinceFailurer11;
};

struct previousUTRACellIdr11 {
    ARFCNValueUTRA carrierFreqr11;

    enum physCellIdr11 {
        physCellIdr11
        PhysCellIdUTRAFDD fddr11;
        PhysCellIdUTRATDD tddr11;
    };
    CellGlobalIdUTRA cellGlobalIdr11;
};

struct selectedUTRACellIdr11 {
    ARFCNValueUTRA carrierFreqr11;

    enum physCellIdr11 {
        physCellIdr11
        PhysCellIdUTRAFDD fddr11;
        PhysCellIdUTRATDD tddr11;
    };
};
;

struct failedPCellIdv1250 {
    TrackingAreaCode tacFailedPCell;
};
RSRQRangev1250 measResultLastServCellv1250;
RSRQType lastServCellRSRQType;
MeasResultList2EUTRAv1250 measResultListEUTRAv1250;
;
enum qci1
}
droppedQCI_r13 {
qci1
}
droppedQCI_r13
,

;
};
;

struct RLFReportv9e0 {
MeasResultList2EUTRAv9e0 measResultListEUTRAv9e0;
};
;
SEQU MeasResultList2EUTRAr9;
;
SEQU MeasResultList2EUTRAv9e0;
;
SEQU MeasResultList2EUTRAv1250;
;

struct MeasResult2EUTRAr9 {
ARFCNValueEUTRA carrierFreqr9;
MeasResultListEUTRA measResultListr9;
};
;

struct MeasResult2EUTRAv9e0 {
ARFCNValueEUTRAv9e0 carrierFreqv9e0;
};
;

struct MeasResult2EUTRAv1250 {
RSRQType rsrqType;
};
;
SEQU MeasResultList2UTRAr9;
;

struct MeasResult2UTRAr9 {
ARFCNValueUTRA carrierFreqr9;
MeasResultListUTRA measResultListr9;
};
;
SEQU MeasResultList2CDMA2000r9;
;

struct MeasResult2CDMA2000r9 {
CarrierFreqCDMA2000 carrierFreqr9;
MeasResultsCDMA2000 measResultListr9;
};
;

struct LogMeasReportr10 {
AbsoluteTimeInfor10 absoluteTimeStampr10;
TraceReferencer10 traceReferencer10;
char traceRecordingSessionRefr10;
char tceIdr10;
LogMeasInfoListr10 logMeasInfoListr10;
, enum true
} logMeasAvailabler10 {
true
}
logMeasAvailabler10,

;
};
;
SEQU LogMeasInfoListr10;
;

struct LogMeasInfor10 {
LocationInfor10 locationInfor10;
int relativeTimeStampr10;
CellGlobalIdEUTRA servCellIdentityr10;

struct measResultServCellr10 {
    RSRPRange rsrpResultr10;
    RSRQRange rsrqResultr10;
};

struct measResultNeighCellsr10 {
    MeasResultList2EUTRAr9 measResultListEUTRAr10;
    MeasResultList2UTRAr9 measResultListUTRAr10;
    MeasResultList2GERANr10 measResultListGERANr10;
    MeasResultList2CDMA2000r9 measResultListCDMA2000r10;
};

;
MeasResultList2EUTRAv9e0 measResultListEUTRAv1090;
;
MeasResultListMBSFN measResultListMBSFN;
RSRQRangev1250 measResultServCellv1250;
RSRQType servCellRSRQType;
MeasResultList2EUTRAv1250 measResultListEUTRAv1250;
;
enum true
} inDeviceCoexDetected_r13 {
true
}
inDeviceCoexDetected_r13
,

;
};
;
SEQU MeasResultListMBSFN;
;

struct MeasResultMBSFN {

struct mbsfnArea {
    MBSFNAreaId mbsfnAreaId;
    ARFCNValueEUTRAr9 carrierFreq;
};
RSRPRange rsrpResultMBSFN;
MBSFNRSRQRange rsrqResultMBSFN;
BLERResult signallingBLERResult;
DataBLERMCHResultList dataBLERMCHResultList;

;
};
;
SEQU DataBLERMCHResultList;
;

struct DataBLERMCHResult {
int mchIndex;
BLERResult dataBLERResult;
};
;

struct BLERResult {
BLERRange bler;

struct blocksReceived {
    std::bitset<3> n;
    std::bitset<8> m;
};
};
;
int BLERRange;
;
SEQU MeasResultList2GERANr10;
;

struct ConnEstFailReportr11 {
CellGlobalIdEUTRA failedCellIdr11;
LocationInfor10 locationInfor11;

struct measResultFailedCellr11 {
    RSRPRange rsrpResultr11;
    RSRQRange rsrqResultr11;
};

struct measResultNeighCellsr11 {
    MeasResultList2EUTRAr9 measResultListEUTRAr11;
    MeasResultList2UTRAr9 measResultListUTRAr11;
    MeasResultListGERAN measResultListGERANr11;
    MeasResultList2CDMA2000r9 measResultsCDMA2000r11;
};
NumberOfPreamblesSentr11 numberOfPreamblesSentr11;
bool contentionDetectedr11;
bool maxTxPowerReachedr11;
TimeSinceFailurer11 timeSinceFailurer11;
MeasResultList2EUTRAv9e0 measResultListEUTRAv1130;

;
RSRQRangev1250 measResultFailedCellv1250;
RSRQType failedCellRSRQType;
MeasResultList2EUTRAv1250 measResultListEUTRAv1250;

;
};
;
int NumberOfPreamblesSentr11;
;
int TimeSinceFailurer11;
;
VisitedCellInfoList MobilityHistoryReport;
;
// -- ASN1STOP
//
// UEInformationResponse field descriptions
// absoluteTimeStamp
// Indicates the absolute time when the logged measurement configuration logging is provided, as indicated by E-UTRAN within absoluteTimeInfo.
// bler
// Indicates the measured BLER value.
// The coding of BLER value is defined in TS 36.133 [16].
// blocksReceived
// Indicates total number of MCH blocks, which were received by the UE and used for the corresponding BLER calculation, within the measurement period as defined in TS 36.133 [16].
// carrierFreq
// In case the UE includes carrierFreq-v9e0 and/ or carrierFreq-v1090, the UE shall set the corresponding entry of carrierFreq-r9 and/ or carrierFreq-r10 respectively to maxEARFCN. For E-UTRA and UTRA frequencies, the UE sets the ARFCN according to the band used when obtaining the concerned measurement results.
// connectionFailureType
// This field is used to indicate whether the connection failure is due to radio link failure or handover failure.
// contentionDetected
// This field is used to indicate that contention was detected for at least one of the transmitted preambles, see TS 36.321 [6].
// c-RNTI
// This field indicates the C-RNTI used in the PCell upon detecting radio link failure or the C-RNTI used in the source PCell upon handover failure.
// droppedQCI
// This field is used to indicate the radio link failure concerned bearer with QCI value equal 1, see TS 24.301 [35].
// dataBLER-MCH-ResultList
// Includes a BLER result per MCH on subframes using dataMCS, with the applicable MCH(s) listed in the same order as in pmch-InfoList within MBSFNAreaConfiguration.
// failedCellId
// This field is used to indicate the cell in which connection establishment failed.
// failedPCellId
// This field is used to indicate the PCell in which RLF is detected or the target PCell of the failed handover. The UE sets the EARFCN according to the band used for transmission/ reception when the failure occurred.
// inDeviceCoexDetected
// Indicates the reason for measurement logging suspension due to IDC problem detection.
// maxTxPowerReached
// This field is used to indicate whether or not the maximum power level was used for the last transmitted preamble, see TS 36.321 [6].
// mch-Index
// Indicates the MCH by referring to the entry as listed in pmch-InfoList within MBSFNAreaConfiguration.
// measResultFailedCell
// This field refers to the last measurement results taken in the cell, where connection establishment failure happened.
// measResultLastServCell
// This field refers to the last measurement results taken in the PCell, where radio link failure or handover failure happened.
// measResultListEUTRA
// If measResultListEUTRA-v9e0, measResultListEUTRA-v1090 or measResultListEUTRA-v1130 is included, the UE shall include the same number of entries, and listed in the same order, as in measResultListEUTRA-r9, measResultListEUTRA-r10 and/ or measResultListEUTRA-r11 respectively.
// measResultListEUTRA-v1250
// If included in RLF-Report-r9 the UE shall include the same number of entries, and listed in the same order, as in measResultListEUTRA-r9;
// If included in LogMeasInfo-r10 the UE shall include the same number of entries, and listed in the same order, as in measResultListEUTRA-r10;
// If included in ConnEstFailReport-r11 the UE shall include the same number of entries, and listed in the same order, as in measResultListEUTRA-r11;
// mobilityHistoryReport
// This field is used to indicate the time of stay in 16 most recently visited E-UTRA cells or of stay out of E-UTRA.
// numberOfPreamblesSent
// This field is used to indicate the number of RACH preambles that were transmitted. Corresponds to parameter PREAMBLE_TRANSMISSION_COUNTER in TS 36.321 [6].
// previousPCellId
// This field is used to indicate the source PCell of the last handover (source PCell when the last RRC-Connection-Reconfiguration message including mobilityControlInfowas received).
// previousUTRA-CellId
// This field is used to indicate the source UTRA cell of the last successful handover to E-UTRAN, when RLF occurred at the target PCell. The UE sets the ARFCN according to the band used for transmission/ reception on the concerned cell.
// reestablishmentCellId
// This field is used to indicate the cell in which the re-establishment attempt was made after connection failure.
// relativeTimeStamp
// Indicates the time of logging measurement results, measured relative to the absoluteTimeStamp. Value in seconds.
// rlf-Cause
// This field is used to indicate the cause of the last radio link failure that was detected. In case of handover failure information reporting (i.e., the connectionFailureType is set to 'hof'), the UE is allowed to set this field to any value.
// selectedUTRA-CellId
// This field is used to indicate the UTRA cell that the UE selects after RLF is detected, while T311 is running. The UE sets the ARFCN according to the band selected for transmission/ reception on the concerned cell.
// signallingBLER-Result
// Includes a BLER result of MBSFN subframes using signallingMCS.
// tac-FailedPCell
// This field is used to indicate the Tracking Area Code of the PCell in which RLF is detected.
// tce-Id
// Parameter Trace Collection Entity Id: See TS 32.422 [58].
// timeConnFailure
// This field is used to indicate the time elapsed since the last HO initialization until connection failure. Actual value = IE value * 100ms. The maximum value 1023 means 102.3s or longer.
// timeSinceFailure
// This field is used to indicate the time that elapsed since the connection (establishment) failure. Value in seconds. The maximum value 172800 means 172800s or longer.
// traceRecordingSessionRef
// Parameter Trace Recording Session Reference: See TS 32.422 [58].
//
// -	ULHandoverPreparationTransfer (CDMA2000)
// The ULHandoverPreparationTransfer message is used for the uplink transfer of handover related CDMA2000 information when requested by the higher layers.
// Signalling radio bearer: SRB1
// RLC-SAP: AM
// Logical channel: DCCH
// Direction: UE to E-UTRAN
// ULHandoverPreparationTransfer message
// -- ASN1START
;

struct ULHandoverPreparationTransfer {

enum criticalExtensions {

    criticalExtensions
    enum c1 {
        c1
        ULHandoverPreparationTransferr8IEs ulHandoverPreparationTransferr8;
        spare3 typedef spare2 typedef spare1 typedef;
    };
    struct
        } criticalExtensionsFuture {
};
};
;

ULHandoverPreparationTransferr8IEs struct {
CDMA2000Type cdma2000Type;
std::bitset<56> meid;
DedicatedInfoCDMA2000 dedicatedInfo;
ULHandoverPreparationTransferv8a0IEs nonCriticalExtension;
};
;

ULHandoverPreparationTransferv8a0IEs struct {
char T string lateNonCriticalExtension;
struct
    } nonCriticalExtension {
};
;
// -- ASN1STOP
//
// ULHandoverPreparationTransfer field descriptions
// meid
// The 56 bit mobile identification number provided by the CDMA2000 Upper layers.
//
// -	ULInformationTransfer
// The ULInformationTransfer message is used for the uplink transfer of NAS or non-3GPP dedicated information.
// Signalling radio bearer: SRB2 or SRB1(only if SRB2 not established yet). If SRB2 is suspended, the UE does not send this message until SRB2 is resumed
// RLC-SAP: AM
// Logical channel: DCCH
// Direction: UE to E-UTRAN
// ULInformationTransfer message
// -- ASN1START
;

struct ULInformationTransfer {

enum criticalExtensions {

    criticalExtensions
    enum c1 {
        c1
        ULInformationTransferr8IEs ulInformationTransferr8;
        spare3 typedef spare2 typedef spare1 typedef;
    };
    struct
        } criticalExtensionsFuture {
};
};
;

struct ULInformationTransferr8IEs {

enum dedicatedInfoType {
    dedicatedInfoType
    DedicatedInfoNAS dedicatedInfoNAS;
    DedicatedInfoCDMA2000 dedicatedInfoCDMA20001XRTT;
    DedicatedInfoCDMA2000 dedicatedInfoCDMA2000HRPD;
};
ULInformationTransferv8a0IEs nonCriticalExtension;
};
;

ULInformationTransferv8a0IEs struct {
char T string lateNonCriticalExtension;
struct
    } nonCriticalExtension {
};
;
// -- ASN1STOP
//
// 6.3	RRC information elements
// 6.3.1	System information blocks
// -	SystemInformationBlockType2
// The IE SystemInformationBlockType2 contains radio resource configuration information that is common for all UEs.
// NOTE:	UE timers and constants related to functionality for which parameters are provided in another SIB are included in the corresponding SIB.
// SystemInformationBlockType2 information element
// -- ASN1START
;

struct SystemInformationBlockType2 {

struct acBarringInfo {
    bool acBarringForEmergency;
    ACBarringConfig acBarringForMOSignalling;
    ACBarringConfig acBarringForMOData;
};
RadioResourceConfigCommonSIB radioResourceConfigCommon;
UETimersAndConstants ueTimersAndConstants;

struct freqInfo {
    ARFCNValueEUTRA ulCarrierFreq;
    enum n100
} ulBandwidth {
    n6, n15, n25, n50
    ;
    AdditionalSpectrumEmission additionalSpectrumEmission;
};
Need OR MBSFNSubframeConfigList mbsfnSubframeConfigList;
TimeAlignmentTimer timeAlignmentTimerCommon;

;
char lateNonCriticalExtension;
ACBarringConfig ssacBarringForMMTELVoicer9;
ACBarringConfig ssacBarringForMMTELVideor9;
;
ACBarringConfig acBarringForCSFBr10;
;
, enum true
} acBarringSkipForMMTELVoice {
true
}
acBarringSkipForMMTELVoice
,
, enum true
}
acBarringSkipForMMTELVideo {
true
}
acBarringSkipForMMTELVideo,
, enum true
}
acBarringSkipForSMS {
true
}
acBarringSkipForSMS,
ACBarringPerPLMNList acBarringPerPLMNList;
;
enum true
}
voiceServiceCauseIndication {
true
}
voiceServiceCauseIndication
,
;
ACDCBarringForCommon_r13 acdcBarringForCommon_r13;
ACDCBarringPerPLMNList_r13 acdcBarringPerPLMNList_r13;

;
};
;

struct SystemInformationBlockType2v8h0IEs {
Need OR SEQU multiBandInfoList;
SystemInformationBlockType2v9e0IEs nonCriticalExtension;
};
;

SystemInformationBlockType2v9e0IEs struct {
Cond ulFreqMax ARFCNValueEUTRAv9e0 ulCarrierFreqv9e0;
struct
    } nonCriticalExtension {
};
;

struct ACBarringConfig {

enum acBarringFactor {
    acBarringFactor,
    p00 p05 p10 p15 p20 p25 p30 p40;
    p50 p60 p70 p75 p80 p85 p90 p95
};

enum acBarringTime {
    s4, s8, s16, s32, s64, s128, s256
    std::bitset<5> acBarringForSpecialAC;
};
;
SEQU MBSFNSubframeConfigList;
;
SEQU ACBarringPerPLMNList;
;

struct ACBarringPerPLMN {
    int plmnIdentityIndex;

    struct acBarringInfo {
        bool acBarringForEmergency;
        ACBarringConfig acBarringForMOSignalling;
        ACBarringConfig acBarringForMOData;
    };
    , enum true
} acBarringSkipForMMTELVoice {
    true
}
acBarringSkipForMMTELVoice,
, enum true
} acBarringSkipForMMTELVideo {
true
}
acBarringSkipForMMTELVideo,
, enum true
}
acBarringSkipForSMS {
true
}
acBarringSkipForSMS,
ACBarringConfig acBarringForCSFB;
ACBarringConfig ssacBarringForMMTELVoice;
ACBarringConfig ssacBarringForMMTELVideo;
};
;

struct ACDCBarringForCommon_r13 {
bool acdcHPLMNonly_r13;
BarringPerACDCCategoryList_r13 barringPerACDCCategoryList_r13;
};
;
SEQU ACDCBarringPerPLMNList_r13;
;

struct ACDCBarringPerPLMN_r13 {
int plmnIdentityIndex_r13;
bool acdcHPLMNonly_r13;
BarringPerACDCCategoryList_r13 barringPerACDCCategoryList_r13;
};
;
Barr;
;

BarringPerACDCCategory_r13 struct {
int acdcCatValue_r13;

struct acdcBarringConfig_r13 {

    enum acBarringFactor_r13 {
        acBarringFactor_r13,
        p00 p05 p10 p15 p20 p25 p30 p40;
        p50 p60 p70 p75 p80 p85 p90 p95
    };
    enum s512
} acBarringTime_r13 {
    s4, s8, s16, s32, s64, s128
};
};
;
// -- ASN1STOP
//
// SystemInformationBlockType2 field descriptions
// ac-BarringFactor
// If the random number drawn by the UE is lower than this value, access is allowed. Otherwise the access is barred. The values are interpreted in the range [0,1): p00 = 0, p05 = 0.05, p10 = 0.10,?, p95 = 0.95. Values other than p00 can only be set if all bits of the corresponding ac-BarringForSpecialAC are set to 0.
// ac-BarringForCSFB
// Access class barring for mobile originating CS fallback.
// ac-BarringForEmergency
// Access class barring for AC 10.
// ac-BarringForMO-Data
// Access class barring for mobile originating calls.
// ac-BarringForMO-Signalling
// Access class barring for mobile originating signalling.
// ac-BarringForSpecialAC
// Access class barring for AC 11-15. The first/ leftmost bit is for AC 11, the second bit is for AC 12, and so on.
// ac-BarringTime
// Mean access barring time value in seconds.
// acdc-BarringConfig
// Barring configuration for an ACDC category. If the field is absent, access to the cell is considered as not barred for the ACDC category in accordance with subclause 5.3.3.13.
// acdc-CatValue
// Indicates the ACDC category as defined in TS 24.105 [72].
// acdc-HPLMNonly
// Indicates whether ACDC is applicable for UEs not in their HPLMN for the corresponding PLMN. TRUE indicates that ACDC is applicable only for UEs in their HPLMN for the corresponding PLMN. FALSE indicates that ACDC is applicable for both UEs in their HPLMN and UEs not in their HPLMN for the corresponding PLMN.
// additionalSpectrumEmission
// The UE requirements related to IE AdditionalSpectrumEmission are defined in TS 36.101 [42, table 6.2.4.1].
// barringPerACDC-CategoryList
// A list of barring information per ACDC category according to the order defined in TS 22.011 [10]. The first entry in the list corresponds to the highest ACDC category of which applications are the least restricted in access attempts at a cell, the second entry in the list corresponds to the ACDC category of which applications are restricted more than applications of the highest ACDC category in access attempts at a cell, and so on. The last entry in the list corresponds to the lowest ACDC category of which applications are the most restricted in access attempts at a cell.
// mbsfn-SubframeConfigList
// Defines the subframes that are reserved for MBSFN in downlink.
// multiBandInfoList
// A list of additionalSpectrumEmission i.e. one for each additional frequency band included in multiBandInfoList in SystemInformationBlockType1, listed in the same order.
// plmn-IdentityIndex
// Index of the PLMN in plmn-IdentityList included in SIB1. Value 1 indicates the PLMN listed 1st in plmn-IdentityList included in SIB1. Value 2 indicates the PLMN listed 2nd in plmn-IdentityList included in SIB1 and so on.
// ssac-BarringForMMTEL-Video
// Service specific access class barring for MMTEL video originating calls.
// ssac-BarringForMMTEL-Voice
// Service specific access class barring for MMTEL voice originating calls.
// ul-Bandwidth
// Parameter: transmission bandwidth configuration, NRB, in uplink, see TS 36.101 [42, table 5.6-1]. Value n6 corresponds to 6 resource blocks, n15 to 15 resource blocks and so on. If for FDD this parameter is absent, the uplink bandwidth is equal to the downlink bandwidth. For TDD this parameter is absent and it is equal to the downlink bandwidth.
// ul-CarrierFreq
// For FDD: If absent, the (default) value determined from the default TX-RX frequency separation defined in TS 36.101 [42, table 5.7.3-1] applies.
// For TDD: This parameter is absent and it is equal to the downlink frequency.
// voiceServiceCauseIndication
// Indicates whether UE is requested to use the establishment cause mo-VoiceCall for mobile originating MMTEL voice calls.
//
// Conditional presence
// Explanation
// ul-FreqMax
// The field is mandatory present if ul-CarrierFreq (i.e. without suffix) is present and set to maxEARFCN. Otherwise the field is not present.
//
// -	SystemInformationBlockType3
// The IE SystemInformationBlockType3 contains cell re-selection information common for intra-frequency, inter-frequency and/ or inter-RAT cell re-selection (i.e. applicable for more than one type of cell re-selection but not necessarily all) as well as intra-frequency cell re-selection information other than neighbouring cell related.
// SystemInformationBlockType3 information element
// -- ASN1START
;

struct SystemInformationBlockType3 {

struct cellReselectionInfoCommon {

    enum qHyst {
        qHyst,
        dB0 dB1 dB2 dB3 dB4 dB5 dB6 dB8 dB10;
        dB12 dB14 dB16 dB18 dB20 dB22 dB24
    };

    struct speedStateReselectionPars {
        MobilityStateParameters mobilityStateParameters;

        struct qHystSF {

            enum sfMedium {
                sfMedium,
                dB6 dB4 dB2 dB0
            };

            enum sfHigh {
                sfHigh,
                dB6 dB4 dB2 dB0
            };
        };
    };
};

struct cellReselectionServingFreqInfo {
    ReselectionThreshold sNonIntraSearch;
    ReselectionThreshold threshServingLow;
    CellReselectionPriority cellReselectionPriority;
};

struct intraFreqCellReselectionInfo {
    QRxLevMin qRxLevMin;
    PMax pMax;
    ReselectionThreshold sIntraSearch;
    AllowedMeasBandwidth allowedMeasBandwidth;
    PresenceAntennaPort1 presenceAntennaPort1;
    NeighCellConfig neighCellConfig;
    TReselection tReselectionEUTRA;
    SpeedStateScaleFactors tReselectionEUTRASF;
};

;
char lateNonCriticalExtension;

struct sIntraSearchv920 {
    ReselectionThreshold sIntraSearchPr9;
    ReselectionThresholdQr9 sIntraSearchQr9;
};

struct sNonIntraSearchv920 {
    ReselectionThreshold sNonIntraSearchPr9;
    ReselectionThresholdQr9 sNonIntraSearchQr9;
};
QQualMinr9 qQualMinr9;
ReselectionThresholdQr9 threshServingLowQr9;
;
Cond WBRSRQ QQualMinr9 qQualMinWBr11;
;
Cond RSRQ QQualMinr9 qQualMinRSRQOnAllSymbols;
;
cellReselectionServingFreqInfov13xy CellReselectionServingFreqInfov13xy;
Need OR RedistributionServingInfo_r13 redistributionServingInfo_r13;

;
};
;

struct RedistributionServingInfo_r13 {
int redistributionFactorServing_r13;
, enum true
} redistributionFactorCell_r13 {
true
}

redistributionFactorCell_r13,
enum t360_r13 {
min4, min8, min16, min32
spare3 spare2 spare1
};
enum true
}
redistrOnPagingOnly_r13 {
true
}
redistrOnPagingOnly_r13,
};
;

struct CellReselectionServingFreqInfov13xy {
CellReselectionSubPriority_r13 cellReselectionSubPriority_r13;
};
;
Late non critical extensions;

SystemInformationBlockType3v10j0IEs struct {
Need OR NSPmaxListr10 freqBandInfor10;
Need OR MultiBandInfoListv10j0 multiBandInfoListv10j0;
struct
    } nonCriticalExtension {
};
;
// -- ASN1STOP
//
// SystemInformationBlockType3 field descriptions
// allowedMeasBandwidth
// If absent, the value corresponding to the downlink bandwidth indicated by the dl-Bandwidth included in MasterInformationBlock applies.
// cellReselectionInfoCommon
// Cell re-selection information common for cells.
// cellReselectionServingFreqInfo
// Information common for Cell re-selection to inter-frequency and inter-RAT cells.
// cellReselectionSubPriority
// Fractional priority value added to cellReselectionPriority for the carrier.
// freqBandInfo
// A list of additionalPmax and additionalSpectrumEmission values as defined in TS 36.101 [42, table 6.2.4-1] applicable for the intra-frequency neighouring E-UTRA cells if the UE selects the frequenby band from freqBandIndicator in SystemInformationBlockType1.
// intraFreqcellReselectionInfo
// Cell re-selection information common for intra-frequency cells.
// multiBandInfoList-v10j0
// A list of additionalPmax and additionalSpectrumEmission values as defined in TS 36.101 [42, table 6.2.4-1] applicable for the intra-frequency neighouring E-UTRA cells if the UE selects the frequenby bands in multiBandInfoList (i.e. without suffix) or multiBandInfoList-v9e0. If E-UTRAN includes multiBandInfoList-v10j0, it includes the same number of entries, and listed in the same order, as in multiBandInfoList (i.e. without suffix).
// p-Max
// Value applicable for the intra-frequency neighbouring E-UTRA cells. If absent the UE applies the maximum power according to the UE capability.
// redistrOnPagingOnly
// If this field is present and the UE is redistribution capable, the UE shall only wait for the paging message to trigger E-UTRAN Inter-frequency Redistribution procedure as specified in 5.2.4.10 of 36.304[4].
// q-Hyst
// Parameter Qhyst in 36.304 [4], Value in dB. Value dB1 corresponds to 1 dB, dB2 corresponds to 2 dB and so on.
// q-HystSF
// Parameter "Speed dependent ScalingFactor for Qhyst" in TS 36.304 [4]. The sf-Medium and sf-High concern the additional hysteresis to be applied, in Medium and High Mobility state respectively, to Qhyst as defined in TS 36.304 [4]. In dB. Value dB-6 corresponds to -6dB, dB-4 corresponds to -4dB and so on.
// q-QualMin
// Parameter "Qqualmin" in TS 36.304 [4], applicable for intra-frequency neighrbour cells. If the field is not present, the UE applies the (default) value of negative infinity for Qqualmin. NOTE 1.
// q-QualMinRSRQ-OnAllSymbols
// If this field is present and supported by the UE, the UE shall, when performing RSRQ measurements, perform RSRQ measurement on all OFDM symbols in accordance with TS 36.214 [48]. NOTE 1.
// q-QualMinWB
// If this field is present and supported by the UE, the UE shall, when performing RSRQ measurements, use a wider bandwidth in accordance with TS 36.133 [16]. NOTE 1.
// q-RxLevMin
// Parameter "Qrxlevmin" in TS 36.304 [4], applicable for intra-frequency neighbour cells.
// redistributionFactorCell
// If redistributionFactorCell is present, this value is only applicable for the serving cell otherwise it is applicable for serving frequency
// redistributionFactorServing
// Parameter redistributionFactorServing in TS 36.304 [4].
// s-IntraSearch
// Parameter "SIntraSearchP" in TS 36.304 [4]. If the field s-IntraSearchP is present, the UE applies the value of s-IntraSearchP instead. Otherwise if neither s-IntraSearch nor s-IntraSearchP is present, the UE applies the (default) value of infinity for SIntraSearchP.
// s-IntraSearchP
// Parameter "SIntraSearchP" in TS 36.304 [4]. See descriptions under s-IntraSearch.
// s-IntraSearchQ
// Parameter "SIntraSearchQ" in TS 36.304 [4]. If the field is not present, the UE applies the (default) value of 0 dB for SIntraSearchQ.
// s-NonIntraSearch
// Parameter "SnonIntraSearchP" in TS 36.304 [4]. If the field s-NonIntraSearchP is present, the UE applies the value of s-NonIntraSearchP instead. Otherwise if neither s-NonIntraSearch nor s-NonIntraSearchP is present, the UE applies the (default) value of infinity for SnonIntraSearchP.
// s-NonIntraSearchP
// Parameter "SnonIntraSearchP" in TS 36.304 [4]. See descriptions under s-NonIntraSearch.
// s-NonIntraSearchQ
// Parameter "SnonIntraSearchQ" in TS 36.304 [4]. If the field is not present, the UE applies the (default) value of 0 dB for SnonIntraSearchQ.
// speedStateReselectionPars
// Speed dependent reselection parameters, see TS 36.304 [4]. If this field is absent, i.e, mobilityStateParameters is also not present, UE behaviour is specified in TS 36.304 [4].
// t360
// Parameter "T360" in TS 36.304 [4].
// threshServingLow
// Parameter "ThreshServing, LowP" in TS 36.304 [4].
// threshServingLowQ
// Parameter "ThreshServing, LowQ" in TS 36.304 [4].
// t-ReselectionEUTRA
// Parameter "TreselectionEUTRA" in TS 36.304 [4].
// t-ReselectionEUTRA-SF
// Parameter "Speed dependent ScalingFactor for TreselectionEUTRA" in TS 36.304 [4]. If the field is not present, the UE behaviour is specified in TS 36.304 [4].
//
// NOTE 1:	The value the UE applies for parameter "Qqualmin" in TS 36.304 [4] depends on the q-QualMin fields signalled by E-UTRAN and supported by the UE. In case multiple candidate options are available, the UE shall select the highest priority candidate option according to the priority order indicated by the following table (top row is highest priority).
// q-QualMinRSRQ-OnAllSymbols
// q-QualMinWB
// Value of parameter "Qqualmin" in TS 36.304 [4]
// Included
// Included
// q-QualMinRSRQ-OnAllSymbols - (q-QualMin - q-QualMinWB)
// Included
// Not included
// q-QualMinRSRQ-OnAllSymbols
// Not included
// Included
// q-QualMinWB
// Not included
// Not included
// q-QualMin
//
// Conditional presence
// Explanation
// RSRQ
// The field is optionally present, Need OR, if threshServingLowQ is present in SIB3; otherwise it is not present.
// WB-RSRQ
// The field is optionally present, need OP if the measurement bandwidth indicated by allowedMeasBandwidth is 50 resource blocks or larger; otherwise it is not present.
//
// -	SystemInformationBlockType4
// The IE SystemInformationBlockType4 contains neighbouring cell related information relevant only for intra-frequency cell re-selection. The IE includes cells with specific re-selection parameters as well as blacklisted cells.
// SystemInformationBlockType4 information element
// -- ASN1START
;

struct SystemInformationBlockType4 {
Need OR IntraFreqNeighCellList intraFreqNeighCellList;
Need OR IntraFreqBlackCellList intraFreqBlackCellList;
Cond CSG PhysCellIdRange csgPhysCellIdRange;

;
char T string lateNonCriticalExtension;
};
;
SEQU IntraFreqNeighCellList;
;

struct IntraFreqNeighCellInfo {
PhysCellId physCellId;
QOffsetRange qOffsetCell;

;
};
;
SEQU IntraFreqBlackCellList;
;
// -- ASN1STOP
//
// SystemInformationBlockType4 field descriptions
// csg-PhysCellIdRange
// Set of physical cell identities reserved for CSG cells on the frequency on which this field was received. The received csg-PhysCellIdRange applies if less than 24 hours has elapsed since it was received and the UE is camped on a cell of the same primary PLMN where this field was received. The 3 hour validity restriction (section 5.2.1.3) does not apply to this field. The UE shall not apply any stored csg-PhysCellIdRange when it is in any cell selection state defined in TS 36.304 [4].
// intraFreqBlackCellList
// List of blacklisted intra-frequency neighbouring cells.
// intraFreqNeighbCellList
// List of intra-frequency neighbouring cells with specific cell re-selection parameters.
// q-OffsetCell
// Parameter "Qoffsets,n" in TS 36.304 [4].
//
// Conditional presence
// Explanation
// CSG
// This field is optional, need OP, for non-CSG cells, and mandatory for CSG cells.
//
// -	SystemInformationBlockType5
// The IE SystemInformationBlockType5 contains information relevant only for inter-frequency cell re-selection i.e. information about other E-UTRA frequencies and inter-frequency neighbouring cells relevant for cell re-selection. The IE includes cell re-selection parameters common for a frequency as well as cell specific re-selection parameters.
// SystemInformationBlockType5 information element
// -- ASN1START
;

struct SystemInformationBlockType5 {
InterFreqCarrierFreqList interFreqCarrierFreqList;

;
(
    CON char T string lateNonCriticalExtension;
    Need OR InterFreqCarrierFreqListv1250 interFreqCarrierFreqListv1250;
    Need OR InterFreqCarrierFreqListExt interFreqCarrierFreqListExt;
    ;
    Need OR InterFreqCarrierFreqListExtv1280 interFreqCarrierFreqListExtv1280;
    ;
    Need OR InterFreqCarrierFreqListv13xy interFreqCarrierFreqListv13xy;
    Need OR InterFreqCarrierFreqListExtv13xy interFreqCarrierFreqListExtv13xy;

    ;
};
;
struct SystemInformationBlockType5v8h0IEs {

inte;
SystemInformationBlockType5v9e0IEs nonCriticalExtension;
};
;
struct SystemInformationBlockType5v9e0IEs {

Need OR SEQU interFreqCarrierFreqListv9e0;
SystemInformationBlockType5v10j0IEs nonCriticalExtension;
};
;
struct SystemInformationBlockType5v10j0IEs {

Need OR SEQU interFreqCarrierFreqListv10j0;
struct
    } nonCriticalExtension {
};
;
SEQU InterFreqCarrierFreqList;
;
SEQU InterFreqCarrierFreqListv1250;
;
SEQU InterFreqCarrierFreqListExt;
;
SEQU InterFreqCarrierFreqListExtv1280;
;
SEQU InterFreqCarrierFreqListv13xy;
;
SEQU InterFreqCarrierFreqListExtv13xy;
;
struct InterFreqCarrierFreqInfo {

ARFCNValueEUTRA dlCarrierFreq;
QRxLevMin qRxLevMin;
PMax pMax;
TReselection tReselectionEUTRA;
SpeedStateScaleFactors tReselectionEUTRASF;
ReselectionThreshold threshXHigh;
ReselectionThreshold threshXLow;
AllowedMeasBandwidth allowedMeasBandwidth;
PresenceAntennaPort1 presenceAntennaPort1;
CellReselectionPriority cellReselectionPriority;
NeighCellConfig neighCellConfig;
DEFAULT dB0 QOffsetRange qOffsetFreq;
Need OR InterFreqNeighCellList interFreqNeighCellList;
Need OR InterFreqBlackCellList interFreqBlackCellList;

;
QQualMinr9 qQualMinr9;
struct threshXQr9 {

    ReselectionThresholdQr9 threshXHighQr9;
    ReselectionThresholdQr9 threshXLowQr9;
    Cond RSRQ
};
;
Cond WBRSRQ QQualMinr9 qQualMinWBr11;

;
};
;
struct InterFreqCarrierFreqInfov8h0 {

Need OR MultiBandInfoList multiBandInfoList;
};
;
struct InterFreqCarrierFreqInfov9e0 {

Cond dlFreqMax ARFCNValueEUTRAv9e0 dlCarrierFreqv9e0;
Need OR MultiBandInfoListv9e0 multiBandInfoListv9e0;
};
;
struct InterFreqCarrierFreqInfov10j0 {

Need OR NSPmaxListr10 freqBandInfor10;
Need OR MultiBandInfoListv10j0 multiBandInfoListv10j0;
};
;
struct InterFreqCarrierFreqInfov1250 {

, enum true
} reducedMeasPerformance {
true
}
reducedMeasPerformance,
Cond RSRQ2 QQualMinr9 qQualMinRSRQOnAllSymbols;
};
;
struct InterFreqCarrierFreqInfo {

ARFCNValueEUTRAr9 dlCarrierFreq;
QRxLevMin qRxLevMin;
PMax pMax;
TReselection tReselectionEUTRA;
SpeedStateScaleFactors tReselectionEUTRASF;
ReselectionThreshold threshXHigh;
ReselectionThreshold threshXLow;
AllowedMeasBandwidth allowedMeasBandwidth;
PresenceAntennaPort1 presenceAntennaPort1;
CellReselectionPriority cellReselectionPriority;
NeighCellConfig neighCellConfig;
DEFAULT dB0 QOffsetRange qOffsetFreq;
Need OR InterFreqNeighCellList interFreqNeighCellList;
Need OR InterFreqBlackCellList interFreqBlackCellList;
QQualMinr9 qQualMin;
struct threshXQ {

    ReselectionThresholdQr9 threshXHighQ;
    ReselectionThresholdQr9 threshXLowQ;
    Cond RSRQ
};
Cond WBRSRQ QQualMinr9 qQualMinWB;
Need OR MultiBandInfoListr11 multiBandInfoList;
, enum true
} reducedMeasPerformance {
true
}
reducedMeasPerformance,
Cond RSRQ2 QQualMinr9 qQualMinRSRQOnAllSymbols;

;
};
;
struct InterFreqCarrierFreqInfov13xy {

CellReselectionSubPriority_r13 cellReselectionSubPriority_r13;
RedistributionInterFreqInfo_r13 redistributionInterFreqInfo_r13;
};
;
SEQU InterFreqNeighCellList;
;
struct InterFreqNeighCellInfo {

PhysCellId physCellId;
QOffsetRange qOffsetCell;
};
;
SEQU InterFreqBlackCellList;
;
struct RedistributionInterFreqInfo_r13 {

RedistributionFactor_r13 redistributionFactorFreq_r13;
RedistributionNeighCellList_r13 redistributionNeighCellList_r13;
};
;
SEQU RedistributionNeighCellList_r13;
;
struct RedistributionNeighCell_r13 {

PhysCellId physCellId;
RedistributionFactor_r13 redistributionFactorCell_r13;
};
;
int RedistributionFactor_r13;
;
// -- ASN1STOP
//
// SystemInformationBlockType5 field descriptions
// cellReselectionSubPriority
// Fractional priority value added to cellReselectionPriority for the carrier.
// freqBandInfo
// A list of additionalPmax and additionalSpectrumEmission values as defined in TS 36.101 [42, table 6.2.4-1] for the frequency band represented by dl-CarrierFreq for which cell reselection parameters are common.
// interFreqBlackCellList
// List of blacklisted inter-frequency neighbouring cells.
// interFreqCarrierFreqList
// List of neighbouring inter-frequencies. E-UTRAN does not configure more than one entry for the same physical frequency regardless of the E-ARFCN used to indicate this. If E-UTRAN includes interFreqCarrierFreqList-v8h0, interFreqCarrierFreqList-v9e0, InterFreqCarrierFreqList-v1250 and/or InterFreqCarrierFreqList-v13xy, it includes the same number of entries, and listed in the same order, as in interFreqCarrierFreqList (i.e. without suffix). See Annex D for more descriptions.
// interFreqCarrierFreqListExt
// List of additional neighbouring inter-frequencies, i.e. extending the size of the inter-frequency carrier list using the general principles specified in 5.1.2. E-UTRAN does not configure more than one entry for the same physical frequency regardless of the E-ARFCN used to indicate this. EUTRAN may include interFreqCarrierFreqListExt even if interFreqCarrierFreqList (i.e without suffix) does not include maxFreq entries. If E-UTRAN includes InterFreqCarrierFreqListExt-v13xy it includes the same number of entries, and listed in the same order, as in interFreqCarrierFreqListExt-r12.
// interFreqNeighCellList
// List of inter-frequency neighbouring cells with specific cell re-selection parameters.
// multiBandInfoList
// Indicates the list of frequency bands in addition to the band represented by dl-CarrierFreq for which cell reselection parameters are common. E-UTRAN indicates at most maxMultiBands frequency bands (i.e. the total number of entries across both multiBandInfoList and multiBandInfoList-v9e0 is below this limit).
// multiBandInfoList-v10j0
// A list of additionalPmax and additionalSpectrumEmission values as defined in TS 36.101 [42, table 6.2.4-1] for the frequency bands in multiBandInfoList (i.e. without suffix) and multiBandInfoList-v9e0. If E-UTRAN includes multiBandInfoList-v10j0, it includes the same number of entries, and listed in the same order, as in multiBandInfoList (i.e. without suffix).
// p-Max
// Value applicable for the neighbouring E-UTRA cells on this carrier frequency. If absent the UE applies the maximum power according to the UE capability.
// q-OffsetCell
// Parameter "Qoffsets,n" in TS 36.304 [4].
// q-OffsetFreq
// Parameter "Qoffsetfrequency" in TS 36.304 [4].
// q-QualMin
// Parameter "Qqualmin" in TS 36.304 [4]. If the field is not present, the UE applies the (default) value of negative infinity for Qqualmin. NOTE 1.
// q-QualMinRSRQ-OnAllSymbols
// If this field is present and supported by the UE, the UE shall, when performing RSRQ measurements, perform RSRQ measurement on all OFDM symbols in accordance with TS 36.214 [48]. NOTE 1.
// q-QualMinWB
// If this field is present and supported by the UE, the UE shall, when performing RSRQ measurements, use a wider bandwidth in accordance with TS 36.133 [16]. NOTE 1.
// redistributionFactorFreq
// Parameter redistributionFactorFreq in TS 36.304 [4].
// redistributionFactorCell
// Parameter redistributionFactorCell in TS 36.304 [4].
// reducedMeasPerformance
// Value TRUE indicates that the neighbouring inter-frequency is configured for reduced measurement performance, see TS 36.133 [16]. If the field is not included, the neighbouring inter-frequency is configured for normal measurement performance, see TS 36.133 [16].
// threshX-High
// Parameter "ThreshX, HighP" in TS 36.304 [4].
// threshX-HighQ
// Parameter "ThreshX, HighQ" in TS 36.304 [4].
// threshX-Low
// Parameter "ThreshX, LowP" in TS 36.304 [4].
// threshX-LowQ
// Parameter "ThreshX, LowQ" in TS 36.304 [4].
// t-ReselectionEUTRA
// Parameter "TreselectionEUTRA" in TS 36.304 [4].
// t-ReselectionEUTRA-SF
// Parameter "Speed dependent ScalingFactor for TreselectionEUTRA" in TS 36.304 [4]. If the field is not present, the UE behaviour is specified in TS 36.304 [4].
//
// NOTE 1:	The value the UE applies for parameter "Qqualmin" in TS 36.304 [4] depends on the q-QualMin fields signalled by E-UTRAN and supported by the UE. In case multiple candidate options are available, the UE shall select the highest priority candidate option according to the priority order indicated by the following table (top row is highest priority).
// q-QualMinRSRQ-OnAllSymbols
// q-QualMinWB
// Value of parameter "Qqualmin" in TS 36.304 [4]
// Included
// Included
// q-QualMinRSRQ-OnAllSymbols - (q-QualMin - q-QualMinWB)
// Included
// Not included
// q-QualMinRSRQ-OnAllSymbols
// Not included
// Included
// q-QualMinWB
// Not included
// Not included
// q-QualMin
//
// Conditional presence
// Explanation
// dl-FreqMax
// The field is mandatory present if, for the corresponding entry in InterFreqCarrierFreqList (i.e. without suffix), dl-CarrierFreq (i.e. without suffix) is set to maxEARFCN. Otherwise the field is not present.
// RSRQ
// The field is mandatory present if threshServingLowQ is present in systemInformationBlockType3; otherwise it is not present.
// RSRQ2
// The field is mandatory present for all EUTRA carriers listed in SIB5 if q-QualMinRSRQ-OnAllSymbols is present in SIB3; otherwise it is not present and the UE shall delete any existing value for this field.
// WB-RSRQ
// The field is optionally present, need OP if the measurement bandwidth indicated by allowedMeasBandwidth is 50 resource blocks or larger; otherwise it is not present.
//
// -	SystemInformationBlockType6
// The IE SystemInformationBlockType6 contains information relevant only for inter-RAT cell re-selection i.e. information about UTRA frequencies and UTRA neighbouring cells relevant for cell re-selection. The IE includes cell re-selection parameters common for a frequency.
// SystemInformationBlockType6 information element
// -- ASN1START
;
struct SystemInformationBlockType6 {

Need OR CarrierFreqListUTRAFDD carrierFreqListUTRAFDD;
Need OR CarrierFreqListUTRATDD carrierFreqListUTRATDD;
TReselection tReselectionUTRA;
SpeedStateScaleFactors tReselectionUTRASF;

;
(CON char T string lateNonCriticalExtension;
 carr
 ;
 Cond UTRAFDD CarrierFreqInfoUTRAv1250;
 carr;
 Cond UTRATDD CarrierFreqInfoUTRAv1250;
 Cond UTRAFDD CarrierFreqListUTRAFDDExt carrierFreqListUTRAFDDExt;
 Cond UTRATDD CarrierFreqListUTRATDDExt carrierFreqListUTRATDDExt;

 ;
};
;
struct SystemInformationBlockType6v8h0IEs {
Cond UTRAFDD carr;
nonCriticalExtension;
struct
    }
{
};
;

struct CarrierFreqInfoUTRAv1250 {

enum true
} reducedMeasPerformance {
true
}
reducedMeasPerformance,
};
;
SEQU CarrierFreqListUTRAFDD;
;
struct CarrierFreqUTRAFDD {

ARFCNValueUTRA carrierFreq;
CellReselectionPriority cellReselectionPriority;
ReselectionThreshold threshXHigh;
ReselectionThreshold threshXLow;
int qRxLevMin;
int pMaxUTRA;
int qQualMin;

;
struct threshXQr9 {

    ReselectionThresholdQr9 threshXHighQr9;
    ReselectionThresholdQr9 threshXLowQr9;
    Cond RSRQ
};

;
};
;
struct CarrierFreqInfoUTRAFDDv8h0 {

Need OR SEQU multiBandInfoList;
};
;
SEQU CarrierFreqListUTRAFDDExt;
CarrierFreqUTRAFDDExt;
;
struct CarrierFreqUTRAFDDExt {

ARFCNValueUTRA carrierFreq;
CellReselectionPriority cellReselectionPriority;
ReselectionThreshold threshXHigh;
ReselectionThreshold threshXLow;
int qRxLevMin;
int pMaxUTRA;
int qQualMin;
struct threshXQ {

    ReselectionThresholdQr9 threshXHighQ;
    ReselectionThresholdQr9 threshXLowQ;
    Cond RSRQ
};
Need OR SEQU multiBandInfoList;
, enum true
} reducedMeasPerformance {
true
}
reducedMeasPerformance,

;
};
;
SEQU CarrierFreqListUTRATDD;
;
struct CarrierFreqUTRATDD {

ARFCNValueUTRA carrierFreq;
CellReselectionPriority cellReselectionPriority;
ReselectionThreshold threshXHigh;
ReselectionThreshold threshXLow;
int qRxLevMin;
int pMaxUTRA;

;
};
;
SEQU CarrierFreqListUTRATDDExt;
CarrierFreqUTRATDD;
;
struct CarrierFreqUTRATDD {

ARFCNValueUTRA carrierFreq;
CellReselectionPriority cellReselectionPriority;
ReselectionThreshold threshXHigh;
ReselectionThreshold threshXLow;
int qRxLevMin;
int pMaxUTRA;
, enum true
} reducedMeasPerformance {
true
}
reducedMeasPerformance,

;
};
;
int FreqBandIndicatorUTRAFDD;
;
// -- ASN1STOP
//
// SystemInformationBlockType6 field descriptions
// carrierFreqListUTRA-FDD
// List of carrier frequencies of UTRA FDD. E-UTRAN does not configure more than one entry for the same physical frequency regardless of the ARFCN used to indicate this. If E-UTRAN includes carrierFreqListUTRA-FDD-v8h0 and/or carrierFreqListUTRA-FDD-v1250, it includes the same number of entries, and listed in the same order, as in carrierFreqListUTRA-FDD (i.e. without suffix). See Annex D for more descriptions.
// carrierFreqListUTRA-FDD-Ext
// List of additional carrier frequencies of UTRA FDD. E-UTRAN does not configure more than one entry for the same physical frequency regardless of the ARFCN used to indicate this. EUTRAN may include carrierFreqListUTRA-FDD-Ext even if carrierFreqListUTRA-FDD (i.e without suffix) does not include maxUTRA-FDD-Carrier entries.
// carrierFreqListUTRA-TDD
// List of carrier frequencies of UTRA TDD. E-UTRAN does not configure more than one entry for the same physical frequency regardless of the ARFCN used to indicate this If E-UTRAN includes carrierFreqListUTRA-TDD-v1250, it includes the same number of entries, and listed in the same order, as in carrierFreqListUTRA-TDD (i.e. without suffix).
// carrierFreqListUTRA-TDD-Ext
// List of additional carrier frequencies of UTRA TDD. E-UTRAN does not configure more than one entry for the same physical frequency regardless of the ARFCN used to indicate this. EUTRAN may include carrierFreqListUTRA-TDD-Ext even if carrierFreqListUTRA-TDD (i.e without suffix) does not include maxUTRA-TDD-Carrier entries.
// multiBandInfoList
// Indicates the list of frequency bands in addition to the band represented by carrierFreq in the CarrierFreqUTRA-FDD for which UTRA cell reselection parameters are common.
// p-MaxUTRA
// The maximum allowed transmission power on the (uplink) carrier frequency, see TS 25.304 [40]. In dBm
// q-QualMin
// Parameter "Qqualmin" in TS 25.304 [40]. Actual value = IE value [dB].
// q-RxLevMin
// Parameter "Qrxlevmin" in TS 25.304 [40]. Actual value = IE value * 2+1 [dBm].
// reducedMeasPerformance
// Value TRUE indicates that the UTRA carrier frequency is configured for reduced measurement performance, see TS 36.133 [16]. If the field is not included, the UTRA carrier frequency is configured for normal measurement performance, see TS 36.133 [16].
// t-ReselectionUTRA
// Parameter "TreselectionUTRAN" in TS 36.304 [4].
// t-ReselectionUTRA-SF
// Parameter "Speed dependent ScalingFactor for TreselectionUTRA" in TS 36.304 [4]. If the field is not present, the UE behaviour is specified in TS 36.304 [4].
// threshX-High
// Parameter "ThreshX, HighP" in TS 36.304 [4].
// threshX-HighQ
// Parameter "ThreshX, HighQ" in TS 36.304 [4].
// threshX-Low
// Parameter "ThreshX, LowP" in TS 36.304 [4].
// threshX-LowQ
// Parameter "ThreshX, LowQ" in TS 36.304 [4].
//
// Conditional presence
// Explanation
// RSRQ
// The field is mandatory present if the threshServingLowQ is present in systemInformationBlockType3; otherwise it is not present.
// UTRA-FDD
// The field is optionally present, need OR, if the carrierFreqListUTRA-FDD is present. Otherwise it is not present.
// UTRA-TDD
// The field is optionally present, need OR, if the carrierFreqListUTRA-TDD is present. Otherwise it is not present.
//
// -	SystemInformationBlockType7
// The IE SystemInformationBlockType7 contains information relevant only for inter-RAT cell re-selection i.e. information about GERAN frequencies relevant for cell re-selection. The IE includes cell re-selection parameters for each frequency.
// SystemInformationBlockType7 information element
// -- ASN1START
;
struct SystemInformationBlockType7 {

TReselection tReselectionGERAN;
Need OR SpeedStateScaleFactors tReselectionGERANSF;
Need OR CarrierFreqsInfoListGERAN carrierFreqsInfoList;

;
char T string lateNonCriticalExtension;
};
;
SEQU CarrierFreqsInfoListGERAN;
;
struct CarrierFreqsInfoGERAN {

CarrierFreqsGERAN carrierFreqs;
struct commonInfo {

    CellReselectionPriority cellReselectionPriority;
    std::bitset<8> nccPermitted;
    int qRxLevMin;
    int pMaxGERAN;
    ReselectionThreshold threshXHigh;
    ReselectionThreshold threshXLow;
};

;
};
;
// -- ASN1STOP
//
// SystemInformationBlockType7 field descriptions
// carrierFreqs
// The list of GERAN carrier frequencies organised into one group of GERAN carrier frequencies.
// carrierFreqsInfoList
// Provides a list of neighbouring GERAN carrier frequencies, which may be monitored for neighbouring GERAN cells. The GERAN carrier frequencies are organised in groups and the cell reselection parameters are provided per group of GERAN carrier frequencies.
// commonInfo
// Defines the set of cell reselection parameters for the group of GERAN carrier frequencies.
// ncc-Permitted
// Field encoded as a bit map, where bit N is set to "0" if a BCCH carrier with NCC = N-1 is not permitted for monitoring and set to "1" if the BCCH carrier with NCC = N-1 is permitted for monitoring; N = 1 to 8; bit 1 of the bitmap is the leading bit of the bit string.
// p-MaxGERAN
// Maximum allowed transmission power for GERAN on an uplink carrier frequency, see TS 45.008 [28]. Value in dBm. Applicable for the neighbouring GERAN cells on this carrier frequency. If pmaxGERAN is absent, the maximum power according to the UE capability is used.
// q-RxLevMin
// Parameter "Qrxlevmin" in TS 36.304 [1], minimum required RX level in the GSM cell. The actual value of Qrxlevmin in dBm = (IE value * 2) ? 115.
// threshX-High
// Parameter "ThreshX, HighP" in TS 36.304 [4].
// threshX-Low
// Parameter "ThreshX, LowP" in TS 36.304 [4].
// t-ReselectionGERAN
// Parameter "TreselectionGERAN" in TS 36.304 [4].
// t-ReselectionGERAN-SF
// Parameter "Speed dependent ScalingFactor for TreselectionGERAN" in TS 36.304 [4]. If the field is not present, the UE behaviour is specified in TS 36.304 [4].
//
// -	SystemInformationBlockType8
// The IE SystemInformationBlockType8 contains information relevant only for inter-RAT cell re-selection i.e. information about CDMA2000 frequencies and CDMA2000 neighbouring cells relevant for cell re-selection. The IE includes cell re-selection parameters common for a frequency as well as cell specific re-selection parameters.
// SystemInformationBlockType8 information element
// -- ASN1START
;
struct SystemInformationBlockType8 {

Need OR SystemTimeInfoCDMA2000 systemTimeInfo;
Need OR int searchWindowSize;
struct parametersHRPD {

    PreRegistrationInfoHRPD preRegistrationInfoHRPD;
    Need OR CellReselectionParametersCDMA2000 cellReselectionParametersHRPD;
    Need OR
};
struct parameters1XRTT {

    CSFBRegistrationParam1XRTT csfbRegistrationParam1XRTT;
    Need OR std::bitset<42> longCodeState1XRTT;
    Need OR CellReselectionParametersCDMA2000 cellReselectionParameters1XRTT;
    Need OR
};

;
char T string lateNonCriticalExtension;
Need OR bool csfbSupportForDualRxUEsr9;
Cond NCLHRPD CellReselectionParametersCDMA2000v920 cellReselectionParametersHRPDv920;
Cond NCL1XRTT CellReselectionParametersCDMA2000v920 cellReselectionParameters1XRTTv920;
Cond REG1XRTT CSFBRegistrationParam1XRTTv920 csfbRegistrationParam1XRTTv920;
Cond REG1XRTT ACBarringConfig1XRTTr9 acBarringConfig1XRTTr9;
;
Cond REG1XRTT enum true
} csfbDualRxTxSupportr10 {
true
}
csfbDualRxTxSupportr10
,
;
Need OR SIB8PerPLMNListr11 sib8PerPLMNListr11;

;
};
;
CellReselectionParametersCDMA2000 struct {

BandClassListCDMA2000 bandClassList;
NeighCellListCDMA2000 neighCellList;
TReselection tReselectionCDMA2000;
SpeedStateScaleFactors tReselectionCDMA2000SF;
};
;
CellReselectionParametersCDMA2000r11 struct {

BandClassListCDMA2000 bandClassList;
SEQU neighCellListr11;
TReselection tReselectionCDMA2000;
SpeedStateScaleFactors tReselectionCDMA2000SF;
};
;
CellReselectionParametersCDMA2000v920 struct {

NeighCellListCDMA2000v920 neighCellListv920;
};
;
SEQU NeighCellListCDMA2000;
;
struct NeighCellCDMA2000 {

BandclassCDMA2000 bandClass;
NeighCellsPerBandclassListCDMA2000 neighCellsPerFreqList;
};
;
struct NeighCellCDMA2000r11 {

BandclassCDMA2000 bandClass;
SEQU neighFreqInfoListr11;
};
;
Neig;
;
struct NeighCellsPerBandclassCDMA2000 {

ARFCNValueCDMA2000 arfcn;
PhysCellIdListCDMA2000 physCellIdList;
};
;
struct NeighCellsPerBandclassCDMA2000r11 {

ARFCNValueCDMA2000 arfcn;
SEQU physCellIdListr11;
};
;
SEQU NeighCellListCDMA2000v920;
;
struct NeighCellCDMA2000v920 {

NeighCellsPerBandclassListCDMA2000v920 neighCellsPerFreqListv920;
};
;
Neig;
;
struct NeighCellsPerBandclassCDMA2000v920 {

PhysCellIdListCDMA2000v920 physCellIdListv920;
};
;
SEQU PhysCellIdListCDMA2000;
;
SEQU PhysCellIdListCDMA2000v920;
;
SEQU BandClassListCDMA2000;
;
struct BandClassInfoCDMA2000 {

BandclassCDMA2000 bandClass;
CellReselectionPriority cellReselectionPriority;
int threshXHigh;
int threshXLow;

;
};
;
struct ACBarringConfig1XRTTr9 {

int acBarring0to9r9;
int acBarring10r9;
int acBarring11r9;
int acBarring12r9;
int acBarring13r9;
int acBarring14r9;
int acBarring15r9;
int acBarringMsgr9;
int acBarringRegr9;
int acBarringEmgr9;
};
;
SEQU SIB8PerPLMNListr11;
;
struct SIB8PerPLMNr11 {

int plmnIdentityr11;
enum parametersCDMA2000r11 {

    parametersCDMA2000r11
    ParametersCDMA2000r11 explicitValue;
    typedef defaultValue;
};
};
;
struct ParametersCDMA2000r11 {

enum systemTimeInfor11 {

    enum systemTimeInfor11
    SystemTimeInfoCDMA2000 explicitValue;
    typedef defaultValue;
    Need OR
};
int searchWindowSizer11;
struct parametersHRPDr11 {

    PreRegistrationInfoHRPD preRegistrationInfoHRPDr11;
} CellReselectionParametersCDMA2000r11 cellReselectionParametersHRPDr11;
;
struct parameters1XRTTr11 {
    CSFBRegistrationParam1XRTT csfbRegistrationParam1XRTTr11;
    Cond REG1XRTTPerPLMN CSFBRegistrationParam1XRTTv920 csfbRegistrationParam1XRTTExtr11;
    Cond PerPLMNLC std::bitset<42> longCodeState1XRTTr11;
    Need OR CellReselectionParametersCDMA2000r11 cellReselectionParameters1XRTTr11;
    Cond REG1XRTTPerPLMN ACBarringConfig1XRTTr9 acBarringConfig1XRTTr11;
    Need OR bool csfbSupportForDualRxUEsr11;
    Cond REG1XRTTPerPLMN enum true
} csfbDualRxTxSupportr11 {
    true
}
csfbDualRxTxSupportr11,
Need OR
};

;
};
;
// -- ASN1STOP
//
// SystemInformationBlockType8 field descriptions
// ac-BarringConfig1XRTT
// Contains the access class barring parameters the UE uses to calculate the access class barring factor, see C.S0097 [53].
// ac-Barring0to9
// Parameter used for calculating the access class barring factor for access overload classes 0 through 9. It is the parameter "PSIST" in C.S0004 [34] for access overload classes 0 through 9.
// ac-BarringEmg
// Parameter used for calculating the access class barring factor for emergency calls and emergency message transmissions for access overload classes 0 through 9. It is the parameter "PSIST_EMG" in C.S0004 [34].
// ac-BarringMsg
// Parameter used for modifying the access class barring factor for message transmissions. It is the parameter "MSG_PSIST" in C.S0004 [34].
// ac-BarringN
// Parameter used for calculating the access class barring factor for access overload class N (N = 10 to 15). It is the parameter "PSIST" in C.S0004 [34] for access overload class N.
// ac-BarringReg
// Parameter used for modifying the access class barring factor for autonomous registrations. It is the parameter "REG_PSIST" in C.S0004 [34].
// bandClass
// Identifies the Frequency Band in which the Carrier can be found. Details can be found in C.S0057 [24, Table 1.5].
// bandClassList
// List of CDMA2000 frequency bands.
// cellReselectionParameters1XRTT
// Cell reselection parameters applicable only to CDMA2000 1xRTT system.
// cellReselectionParameters1XRTT-Ext
// Cell reselection parameters applicable for cell reselection to CDMA2000 1XRTT system.
// cellReselectionParameters1XRTT-v920
// Cell reselection parameters applicable for cell reselection to CDMA2000 1XRTT system. The field is not present if cellReselectionParameters1XRTT is not present; otherwise it is optionally present.
// cellReselectionParametersHRPD
// Cell reselection parameters applicable for cell reselection to CDMA2000 HRPD system
// cellReselectionParametersHRPD-Ext
// Cell reselection parameters applicable for cell reselection to CDMA2000 HRPD system.
// cellReselectionParametersHRPD-v920
// Cell reselection parameters applicable for cell reselection to CDMA2000 HRPD system. The field is not present if cellReselectionParametersHRPD is not present; otherwise it is optionally present.
// csfb-DualRxTxSupport
// Value TRUE indicates that the network supports dual Rx/Tx enhanced 1xCSFB, which enables UEs capable of dual Rx/Tx enhanced 1xCSFB to switch off their 1xRTT receiver/transmitter while camped in E-UTRAN [51].
// csfb-RegistrationParam1XRTT
// Contains the parameters the UE will use to determine if it should perform a CDMA2000 1xRTT Registration/Re-Registration. This field is included if either CSFB or enhanced CS fallback to CDMA2000 1xRTT is supported.
// csfb-SupportForDualRxUEs
// Value TRUE indicates that the network supports dual Rx CSFB [51].
// longCodeState1XRTT
// The state of long code generation registers in CDMA2000 1XRTT system as defined in C.S0002 [12, Section 1.3] at ms, where t equals to the cdma-SystemTime. This field is required for 1xRTT Inter-RAT ANR, SRVCC handover and enhanced CS fallback to CDMA2000 1xRTT operation. Otherwise this IE is not needed. This field is excluded when estimating changes in system information, i.e. changes of longCodeState1XRTT should neither result in system information change notifications nor in a modification of systemInfoValueTag in SIB1.
// neighCellList
// List of CDMA2000 neighbouring cells. The total number of neighbouring cells in neighCellList for each RAT (1XRTT or HRPD) is limited to 32.
// neighCellList-v920
// Extended List of CDMA2000 neighbouring cells. The combined total number of CDMA2000 neighbouring cells in both neighCellList and neighCellList-v920 is limited to 32 for HRPD and 40 for 1xRTT.
// neighCellsPerFreqList
// List of carrier frequencies and neighbour cell ids in each frequency within a CDMA2000 Band, see C.S0002 [12] or C.S0024 [26].
// neighCellsPerFreqList-v920
// Extended list of neighbour cell ids, in the same CDMA2000 Frequency Band as the corresponding instance in "NeighCellListCDMA2000".
// parameters1XRTT
// Parameters applicable for interworking with CDMA2000 1XRTT system.
// parametersCDMA2000
// Error line: 4125