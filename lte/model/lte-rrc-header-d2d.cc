
#include "lte-rrc-header.h"
#include "ns3/lte-asn1-header.h"
#include "lte-rrc-sidelink.h"
namespace ns3 {

//*********** D2D SideLink

void SerializeSystemInformationBlockType18 (LteRrcSap::SystemInformationBlockType18 systemInformationBlockType18) const
{
    // SLCommRxPoolList
    SLCommRxPoolList commRxPool;
    commRxPool.push_back (SLCommResourcePool());
    SerializeSequence(commRxPool, false);
    
    SLCommTxPoolList commTxPoolNormalCommon;
    commTxPoolNormalCommon.push_back (SLCommResourcePool());
    SerializeSequence(commTxPoolNormalCommon, false);
    
    
    
    std::bitset<3> sysInfoBlk1Opts;
    sysInfoBlk1Opts.set (2, 0); // p-Max absent
    sysInfoBlk1Opts.set (1, 0); // tdd-Config absent
    sysInfoBlk1Opts.set (0, 0); // nonCriticalExtension absent
    SerializeSequence (sysInfoBlk1Opts, false);

    // Serialize cellAccessRelatedInfo
    // 1 optional field (csgIdentity) which is present, no extension marker.
    SerializeSequence (std::bitset<1> (1), false);

    // Serialize plmn-IdentityList
    SerializeSequenceOf (1, 6, 1);

    // PLMN-IdentityInfo 
    SerializeSequence (std::bitset<0> (), false);

    SerializePlmnIdentity (systemInformationBlockType1.cellAccessRelatedInfo.plmnIdentityInfo.plmnIdentity);

    // Serialize si-WindowLength
    SerializeEnum (7, 0);

}

void SerializeSystemInformationBlockType19 (LteRrcSap::SystemInformationBlockType19 systemInformationBlockType19) const
{

    ;
}
}


