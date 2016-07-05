/* 
 * File:   lte-ue-mac-d2d.cc
 * Author: makhtar
 *
 * Created on October 15, 2014, 5:01 PM
 */

#include <cstdlib>
#include "lte-ue-mac.h"
#include <ns3/object.h>
#include <ns3/log.h>

using namespace std;
using namespace ns3;

NS_LOG_COMPONENT_DEFINE("LteUeMacD2d");

//void LteUeMac::SendDiscoveryMsg(Ptr<Node> n, string msg)
//{
//    //NS_LOG_FUNCTION(this);
//    NS_LOG_INFO("Ue is sending discovery msg...");
//    Ptr<LteUeNetDevice> dev = CreateObject<LteUeNetDevice>();
//    dev->SetNode(n);
//    Ptr<LteUeMac> mac = dev->GetMac();
//    if(mac == NULL) {
//        NS_LOG_ERROR("Node's MAC object is null"); 
//        return;    
//    }
//    
//    DiscoveryMsgElement el = new DiscoveryMsgElement();
//    el.payload = msg.c_str(); // limit the size to avoid buffer over run
//    
//    LteMacSapProvider::TransmitPduParameters pduParams;
//    Ptr<Packet> pdu = Create<Packet>(reinterpret_cast<const uint8_t*>(msg.c_str()), msg.size());
//    pduParams.pdu = pdu;
//
//}
//
//void LteUeMac::ReceiveDiscoveryMsg(void) {
//
//
//}
