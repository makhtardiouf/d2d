/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
//
// Copyright (c) 2006 Georgia Tech Research Corporation
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Author: George F. Riley<riley@ece.gatech.edu>
// George F. Riley, Georgia Tech, Spring 2007
// Adapted from ApplicationOnOff in GTNetS.

// Extended for LTE ProSe D2D: Makhtar Diouf 
// $Id: http-client.cc, 2016-02-23 21:40 +0900 makhtar $

#include "ns3/log.h"
#include "ns3/address.h"
#include "ns3/node.h"
#include "ns3/nstime.h"
#include "ns3/data-rate.h"

#include "ns3/socket.h"
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/tcp-socket-factory.h"
#include "ns3/callback.h"

#include "http-client.h"

NS_LOG_COMPONENT_DEFINE ("HttpClient");

using namespace std;

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (HttpClient);

TypeId
HttpClient::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::HttpClient")
          .SetParent<Application> ()
          .AddConstructor<HttpClient> ()
          .AddAttribute ("Remote", "The address of the destination",
                         AddressValue (),
                         MakeAddressAccessor (&HttpClient::m_peer),
                         MakeAddressChecker ())
          .AddAttribute ("Protocol", "The type of protocol to use.",
                         TypeIdValue (TcpSocketFactory::GetTypeId ()),
                         MakeTypeIdAccessor (&HttpClient::m_tid),
                         MakeTypeIdChecker ())
          ;
  return tid;
}

HttpClient::HttpClient ()
{
  //    NS_LOG_FUNCTION_NOARGS();
  m_socket = 0;
  m_connected = false;
}

HttpClient::~ HttpClient ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
HttpClient::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  m_socket = 0;
  // chain up
  Application::DoDispose ();
}

// Application Methods

void HttpClient::StartApplication ()
{
  NS_LOG_FUNCTION_NOARGS ();

  // Create the socket if not already
  if (! m_socket)
  {
    m_socket = Socket::CreateSocket (GetNode (), m_tid);
    if (Inet6SocketAddress::IsMatchingType (m_peer))
      m_socket->Bind6 ();
    else
      m_socket->Bind ();

    // Minimum retransmission timeout
    m_socket->SetAttribute ("MinRto", TimeValue (Seconds (4.0)));
    m_socket->SetAllowBroadcast (true);
 
    Callback<void, Ptr<Socket> > successCB = MakeCallback (&ns3::HttpClient::ConnectionSucceeded, this);
    Callback<void, Ptr<Socket> > failCB = MakeCallback (&ns3::HttpClient::ConnectionFailed, this);
    m_socket->SetConnectCallback (successCB, failCB);
        
    int ok = m_socket->Connect (m_peer);
    NS_ASSERT_MSG (ok == 0, "Failed to connect to peer: " << m_peer);   
  }
}

void HttpClient::StopApplication ()
{
  if (m_socket)
  {
    m_socket->Close ();

  }
}

void HttpClient::ConnectionSucceeded (Ptr<Socket> s)
{
  NS_LOG_DEBUG (s << "\tHttpClient connected");
  m_connected = true;
  NS_ASSERT (s == m_socket);

  // Train Tcp windows scaling
  // HttpGet ("/");
  // Triggers WebServer::HandleRead to test Page Not Found
  // HttpGet("/favicon.ico");
}

void HttpClient::ConnectionFailed (Ptr<Socket> s)
{
  NS_LOG_DEBUG (s << "\tHttpClient failed to connect");
  m_connected = false;
}

int HttpClient::HttpGet (std::string url)
{
  NS_LOG_FUNCTION (this);
  std::string request = std::string ("GET ").append (url).append (" HTTP/1.1\r\n\r\n");
  const uint8_t* data = reinterpret_cast<const uint8_t*> (request.data ());
  return m_socket->Send (data, request.length (), 0);
}

/**
 * Ref http://www.w3.org/Protocols/rfc2616/rfc2616-sec3.html
 * transfer-coding  = "chunked" | transfer-extension
 * chunk-data       = chunk-size (OCTET)
 * Spec says: HTTP/1.1 client must not preface or follow a request with an extra CRLF
 * but wireshark seem to expect it
 * @param dataStr
 */
int HttpClient::HttpPost (std::string url, std::string dataStr)
{
  std::ostringstream pageSize;
  pageSize.flags (ios::hex);
  pageSize << dataStr.length ();

  std::string request = std::string ("POST ").append (url).append (" HTTP/1.1\r\n")
          .append ("Transfer-Encoding: chunked\r\n")
          // .append("chunk: ").append(pageSize.str()).append("\r\n")
          .append ("Content-Type: application/x-www-form-url-encoded\r\n")
          .append ("Content-Length: ").append (pageSize.str ()).append ("\r\n")
          //.append("Content: ")
          .append (dataStr)
          .append ("\r\n\r\n")
          // .append("0\r\n\r\b"); // CRLF ends chunk
          ;

  const uint8_t* data = reinterpret_cast<const uint8_t*> (request.data ());
  return m_socket->Send (data, request.length (), 0);
}


} // Namespace ns3
