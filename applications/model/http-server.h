/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright 2007 University of Washington
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author:  Tom Henderson (tomhend@u.washington.edu)
 *
 * Updated for the latest ns-3-dev Makhtar Diouf 
 * $Id: http-server.h, 2016-02-23 21:40 +0900 makhtar $
 */

#ifndef __http_server_h__
#define __http_server_h__

#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/callback.h"
#include "ns3/traced-callback.h"
#include "ns3/address.h"
#include "ns3/http-status-codes.h"

#include <vector>
#include <map>

namespace ns3 {

class Address;
class Socket;
class Packet;

class HttpServer : public Application 
{
public:
  static TypeId GetTypeId (void);
  HttpServer ();

  virtual ~HttpServer ();

  void SetRecvPostCallback (Callback<void, std::map<std::string, std::string> > keyValues);

  void SetResponseCallback(Callback<void, HttpStatus_t> respCb) ;
    
  void SetResponseCallback(Callback<void, std::string> respCb) ; // e.g. for AppCode feedback

protected:
  virtual void DoDispose (void);

private:
  // inherited from Application base class.
  virtual void StartApplication (void);    // Called at time specified by Start
  virtual void StopApplication (void);     // Called at time specified by Stop

  void HandleRead (Ptr<Socket> socket);

  virtual bool ConnectionRequested(Ptr<Socket>, const Address&);
  virtual void ConnectionAccepted (Ptr<Socket>, const Address&);

  //Private Helpers - need the socket param for handling multiple concurrent requests
  void ParseHttp(std::string data, Ptr<Socket>);
  void SendResponse (HttpStatus_t, Ptr<Socket>);

  static std::string GenerateNotFoundPage (std::string);
  static std::string CalculateNotFoundPageSizeString (std::string);

  Ptr<Socket>     m_socket;       // Associated socket
  Address         m_local;        // Local address to bind to
  TypeId          m_tid;          // Protocol TypeId
  std::string m_data;
  bool        m_stopped;        // Rejects all connection requests if true
  uint32_t    m_connections;    // Current active connection count
  uint32_t    m_maxConnections; // Maximum simultaneous connections allowed
  bool m_secondRead;
  
  // Callback to feedback POSTed data to parent/caller
  Callback<void, std::map<std::string, std::string> > m_rxPostCb;

  // Feedback the status of the response, so that the caller can customize the message
  Callback<void, HttpStatus_t> m_responseCb;

  Callback<void, std::string> m_responseStrCb;

};

} // namespace ns3

#endif

