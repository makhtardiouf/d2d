/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014-2016 Gyeongsang N. University
 * InfoCom Engineering Dept, South Korea
 *
 * $Id: lte-utils.h, 2016-02-23 21:40 +0900 makhtar $
 * Author: Makhtar Diouf 
 *
 * This program is open source software released under the terms of the
 * GNU General Public License version 2 as published by the Free Software Foundation.
 * It is distributed WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <string>
#include <cmath>
#include <ns3/log.h>
#include <ns3/config-store-module.h>
#include <ns3/command-line.h>
#include <ns3/node-list.h>
#include <ns3/global-value.h>
#include <ns3/pointer.h>

#include <ns3/ipv6.h>
#include <ns3/ipv6-address.h>
#include <ns3/net-device-container.h>
#include <ns3/lte-ue-net-device.h>
#include <ns3/lte-ue-rrc.h>
#include <ns3/lte-ue-phy.h>
#include <ns3/lte-enb-phy.h>

#include <ns3/animation-interface.h>
#include <ns3/gnuplot-helper.h>
#include <ns3/file-helper.h>
#include <ns3/radio-environment-map-helper.h>
#include <ns3/constant-velocity-mobility-model.h>

#include "ns3/lte-common.h"
#include "ns3/lte-prose-helper.h"

#ifndef LTE_UTILS_H
#define	LTE_UTILS_H

namespace ns3
{

/**
 * @brief SaveDiscoveryStats output discovery ratio stats
 * @param line contain Time, num of dUes Rxing data, num of D2D candidates
 */
void SaveDiscoveryStats(std::string line);

void SaveWifiDataStats(string line);

void SaveLteDataStats(string line);

/**
 * Save nodes positions in file for plotting
 * @param filename
 */
void TrackNodes(std::string filename);

void GenRadioMap(Ptr<ProSeHelper> d2dCtrl);

/**
 * Annotate nodes and trace movements for Netanim
 */
//void NetanimTrace(Ptr<ProSeHelper> d2dCtrl);

void ProcessUeSinr(uint16_t cellId, uint64_t imsi, uint16_t rnti, double sinrLinear);

void PrintUeAttrListToFile(std::string filename);

void PrintEnbAttrListToFile(std::string filename);

} // ns-3 namespace


#endif	/* LTE_UTILS_H */
