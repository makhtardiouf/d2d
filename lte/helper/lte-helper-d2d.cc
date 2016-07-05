
#include "lte-helper.h"
#include "ns3/lte-ue-phy-d2d.h"

#include <ns3/string.h>
#include <ns3/log.h>
#include <ns3/abort.h>
#include <ns3/pointer.h>
#include <ns3/lte-enb-rrc.h>
#include <ns3/epc-ue-nas.h>
#include <ns3/epc-enb-application.h>
#include <ns3/lte-ue-rrc.h>
#include <ns3/lte-ue-mac.h>
#include <ns3/lte-enb-mac.h>
#include <ns3/lte-enb-net-device.h>
#include <ns3/lte-enb-phy.h>
#include <ns3/lte-ue-phy.h>
#include "ns3/lte-spectrum-phy.h"
#include <ns3/lte-chunk-processor.h>
#include <ns3/multi-model-spectrum-channel.h>
#include <ns3/friis-spectrum-propagation-loss.h>
#include <ns3/trace-fading-loss-model.h>
#include <ns3/isotropic-antenna-model.h>
#include <ns3/lte-enb-net-device.h>
#include <ns3/lte-ue-net-device.h>
#include <ns3/ff-mac-scheduler.h>
#include <ns3/lte-ffr-algorithm.h>
#include <ns3/lte-handover-algorithm.h>
#include <ns3/lte-anr.h>
#include <ns3/lte-rlc.h>
#include <ns3/lte-rlc-um.h>
#include <ns3/lte-rlc-am.h>
#include <ns3/epc-enb-s1-sap.h>
#include <ns3/lte-rrc-protocol-ideal.h>
#include <ns3/lte-rrc-protocol-real.h>
#include <ns3/mac-stats-calculator.h>
#include <ns3/phy-stats-calculator.h>
#include <ns3/phy-tx-stats-calculator.h>
#include <ns3/phy-rx-stats-calculator.h>
#include <ns3/epc-helper.h>
#include <iostream>
#include <ns3/buildings-propagation-loss-model.h>
#include <ns3/lte-spectrum-value-helper.h>
#include <ns3/epc-x2.h>

// Adaptation for LTE ProSe D2D: Makhtar Diouf 
// $Id: lte-helper-d2d.cc, 2016-02-23 21:40 +0900 makhtar $

NS_LOG_COMPONENT_DEFINE("LteHelperD2d");

namespace ns3 {

Ptr<NetDevice>
    LteHelper::InstallSingleD2dUeDevice(Ptr<Node> n) {
    NS_LOG_FUNCTION(this);
    Ptr<LteSpectrumPhy> dlPhy = CreateObject<LteSpectrumPhy> ();
    Ptr<LteSpectrumPhy> ulPhy = CreateObject<LteSpectrumPhy> ();

    /** 
     * Add support for ProSe D2D 
     * Templating this function is tricky
     */
    Ptr<D2dUePhy> phy = CreateObject<D2dUePhy> (dlPhy, ulPhy);

    Ptr<LteHarqPhy> harq = Create<LteHarqPhy> ();
    dlPhy->SetHarqPhyModule(harq);
    ulPhy->SetHarqPhyModule(harq);
    phy->SetHarqPhyModule(harq);

    Ptr<LteChunkProcessor> pRs = Create<LteChunkProcessor> ();
    pRs->AddCallback(MakeCallback(&LteUePhy::ReportRsReceivedPower, phy));
    dlPhy->AddRsPowerChunkProcessor(pRs);

    Ptr<LteChunkProcessor> pInterf = Create<LteChunkProcessor> ();
    pInterf->AddCallback(MakeCallback(&LteUePhy::ReportInterference, phy));
    dlPhy->AddInterferenceCtrlChunkProcessor(pInterf); // for RSRQ evaluation of UE Measurements

    Ptr<LteChunkProcessor> pCtrl = Create<LteChunkProcessor> ();
    pCtrl->AddCallback(MakeCallback(&LteUePhy::GenerateCtrlCqiReport, phy));
    pCtrl->AddCallback(MakeCallback(&LteSpectrumPhy::UpdateSinrPerceived, dlPhy));
    dlPhy->AddCtrlSinrChunkProcessor(pCtrl);

    Ptr<LteChunkProcessor> pData = Create<LteChunkProcessor> ();
    pData->AddCallback(MakeCallback(&LteSpectrumPhy::UpdateSinrPerceived, dlPhy));
    dlPhy->AddDataSinrChunkProcessor(pData);

    Ptr<LteChunkProcessor> pDataInterf = Create<LteChunkProcessor> ();
    if (m_usePdschForCqiGeneration)
    {
        pDataInterf->AddCallback(MakeCallback(&LteUePhy::ReportDataInterference, phy));
    }

    dlPhy->AddInterferenceDataChunkProcessor(pDataInterf);

    dlPhy->SetChannel(m_downlinkChannel);
    ulPhy->SetChannel(m_uplinkChannel);

    Ptr<MobilityModel> mm = n->GetObject<MobilityModel> ();
    NS_ASSERT_MSG(mm, "MobilityModel needs to be set on node before calling LteHelper::InstallD2dUeDevice ()");
    dlPhy->SetMobility(mm);
    ulPhy->SetMobility(mm);

    Ptr<AntennaModel> antenna = (m_ueAntennaModelFactory.Create())->GetObject<AntennaModel> ();
    NS_ASSERT_MSG(antenna, "error in creating the AntennaModel object");
    dlPhy->SetAntenna(antenna);
    ulPhy->SetAntenna(antenna);

    Ptr<LteUeMac> mac = CreateObject<LteUeMac> ();
    Ptr<LteUeRrc> rrc = CreateObject<LteUeRrc> ();

    if (m_useIdealRrc)
    {
        Ptr<LteUeRrcProtocolIdeal> rrcProtocol = CreateObject<LteUeRrcProtocolIdeal> ();
        rrcProtocol->SetUeRrc(rrc);
        rrc->AggregateObject(rrcProtocol);
        rrcProtocol->SetLteUeRrcSapProvider(rrc->GetLteUeRrcSapProvider());
        rrc->SetLteUeRrcSapUser(rrcProtocol->GetLteUeRrcSapUser());
        } else {
        Ptr<LteUeRrcProtocolReal> rrcProtocol = CreateObject<LteUeRrcProtocolReal> ();
        rrcProtocol->SetUeRrc(rrc);
        rrc->AggregateObject(rrcProtocol);
        rrcProtocol->SetLteUeRrcSapProvider(rrc->GetLteUeRrcSapProvider());
        rrc->SetLteUeRrcSapUser(rrcProtocol->GetLteUeRrcSapUser());
    }

        if (m_epcHelper != 0) {
        rrc->SetUseRlcSm(false);
    }
    Ptr<EpcUeNas> nas = CreateObject<EpcUeNas> ();

    nas->SetAsSapProvider(rrc->GetAsSapProvider());
    rrc->SetAsSapUser(nas->GetAsSapUser());

    rrc->SetLteUeCmacSapProvider(mac->GetLteUeCmacSapProvider());
    mac->SetLteUeCmacSapUser(rrc->GetLteUeCmacSapUser());
    rrc->SetLteMacSapProvider(mac->GetLteMacSapProvider());

    phy->SetLteUePhySapUser(mac->GetLteUePhySapUser());
    mac->SetLteUePhySapProvider(phy->GetLteUePhySapProvider());

    phy->SetLteUeCphySapUser(rrc->GetLteUeCphySapUser());
    rrc->SetLteUeCphySapProvider(phy->GetLteUeCphySapProvider());

    NS_ABORT_MSG_IF(m_imsiCounter >= 0xFFFFFFFF, "max num UEs exceeded");
    uint64_t imsi = ++m_imsiCounter;

    Ptr<LteUeNetDevice> dev = m_ueNetDeviceFactory.Create<LteUeNetDevice> ();
    dev->SetNode(n);
    dev->SetAttribute("Imsi", UintegerValue(imsi));
    
    // If set to D2dUePhy  generate error
    // Attribute name=D2dUePhy does not exist for this object:
    dev->SetAttribute("LteUePhy", PointerValue(phy));
    dev->SetAttribute("LteUeMac", PointerValue(mac));
    dev->SetAttribute("LteUeRrc", PointerValue(rrc));
    dev->SetAttribute("EpcUeNas", PointerValue(nas));

    phy->SetDevice(dev);
    dlPhy->SetDevice(dev);
    ulPhy->SetDevice(dev);
    nas->SetDevice(dev);

    n->AddDevice(dev);
    dlPhy->SetLtePhyRxDataEndOkCallback(MakeCallback(&LteUePhy::PhyPduReceived, phy));
    dlPhy->SetLtePhyRxCtrlEndOkCallback(MakeCallback(&LteUePhy::ReceiveLteControlMessageList, phy));

    // ProSe D2D - should mind the case of hybrid Ue
  //  dlPhy->SetLtePhyRxPssCallback(MakeCallback(&D2dUePhy::ReceivePd2dss, phy));
    dlPhy->SetLtePhyRxPssCallback(MakeCallback(&LteUePhy::ReceivePss, phy));
    
        //    ulPhy->SetLtePhyTxDiscoveryCallback(MakeCallback(&D2dUePhy::SendDiscoveryMsg, phy));
        //    dlPhy->SetLtePhyRxDiscoveryCallback(MakeCallback(&D2dUePhy::ReceiveDiscoveryMsg, phy));
    
    dlPhy->SetLtePhyDlHarqFeedbackCallback(MakeCallback(&LteUePhy::ReceiveLteDlHarqFeedback, phy));
    nas->SetForwardUpCallback(MakeCallback(&LteUeNetDevice::Receive, dev));

        if (m_epcHelper != 0) {
        m_epcHelper->AddUe(dev, dev->GetImsi());
    }

    dev->Initialize();
    return dev;
}

}
