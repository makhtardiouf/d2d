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
 * $Id: http-server.cc, 2016-03-15 21:07 +0900 makhtar $
 */
#include "ns3/address.h"
#include "ns3/log.h"
#include "ns3/inet-socket-address.h"
#include "ns3/node.h"
#include "ns3/socket.h"
#include "ns3/ipv4-header.h"

#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/packet.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/tcp-socket-factory.h"
#include "ns3/fatal-error.h"

#include "http-server.h"
#include <ostream>
#include <map>
#include <thread>

using namespace std;

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("HttpServer");
NS_OBJECT_ENSURE_REGISTERED (HttpServer);

TypeId
HttpServer::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::HttpServer")
          .SetParent<Application> ()
          .AddConstructor<HttpServer> ()
          .AddAttribute ("Local", "The Address on which to Bind the rx socket.",
                         AddressValue (),
                         MakeAddressAccessor (&HttpServer::m_local),
                         MakeAddressChecker ())
          .AddAttribute ("Protocol", "The type id of the protocol to use for the rx socket.",
                         TypeIdValue (TcpSocketFactory::GetTypeId ()),
                         MakeTypeIdAccessor (&HttpServer::m_tid),
                         MakeTypeIdChecker ())
          ;
  return tid;
}

HttpServer::HttpServer ()
: m_stopped (true), m_connections (0), m_maxConnections (20), m_secondRead (false)
{
  //*** Warning: 1 socket seems not enough for multiple concurrent connections

  m_socket = 0;
}

HttpServer::~ HttpServer ()
{
}

void
HttpServer::DoDispose (void)
{
  m_socket = 0;
  m_rxPostCb = MakeNullCallback< void, std::map<std::string, std::string> > ();
  m_responseCb = MakeNullCallback< void, HttpStatus_t > ();
  m_responseStrCb = MakeNullCallback< void, std::string > ();

  // chain up
  Application::DoDispose ();
}


// Application Methods

void HttpServer::StartApplication () // Called at time specified by Start
{
  NS_LOG_FUNCTION ("\n" << this);

  // Create the socket if not already
  if (! m_socket)
  {
    m_socket = Socket::CreateSocket (GetNode (), m_tid);
    m_socket->Bind (m_local);
    m_socket->SetAllowBroadcast (true);
    m_socket->Listen ();
  }

  m_socket->SetRecvCallback (MakeCallback (&HttpServer::HandleRead, this));
  m_socket->SetAcceptCallback (MakeCallback (&HttpServer::ConnectionRequested, this),
                               MakeCallback (&HttpServer::ConnectionAccepted, this));
}

void HttpServer::StopApplication ()
{
  NS_LOG_FUNCTION ("\n" << this);

  if (m_socket)
  {
    m_socket->Close ();
    m_socket->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
  }
}

bool HttpServer::ConnectionRequested (Ptr<Socket> s, const Address& a)
{
  NS_LOG_FUNCTION ("\n" << this << s << a);
  return m_connections < m_maxConnections;
}

void HttpServer::ConnectionAccepted (Ptr<Socket> s, const Address& a)
{
  // We have a new socket. We need to set the receive callback 
  NS_LOG_FUNCTION ("\n" << this << s << a);
  m_connections ++;
  m_socket = s;
  Ptr<Packet> p;
  uint32_t size;

  //read until it "blocks"
  while ((p = m_socket->Recv ()))
  {
    size = p->GetSize ();
    uint8_t data[size];

    p->CopyData (data, size);
    m_data.append (reinterpret_cast<const char*> (data), size);
  }
  //now we've blocked, so register the upcall for later
  s->SetRecvCallback (MakeCallback (&HttpServer::HandleRead, this));
}

void HttpServer::HandleRead (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION ("\n" << this << socket);

  Ptr<Packet> p;
  uint32_t size = 0;
  uint32_t totalBytes = 0;
  string content(m_data);
  
  //read until it "blocks"
  while ((p = socket->Recv ()))
  {
    size = p->GetSize ();
    uint8_t data[size];

    p->CopyData (data, size);
    content.append (reinterpret_cast<const char*> (data), size);
    totalBytes += size;
  }
  
  // Parse out any HTTP we got
  if (content.length () > 0)
  {
    NS_LOG_DEBUG ("\tHttpServer RXed " << totalBytes << " bytes "
                  //<< "Content: " << content
                  //<< " from " << head.GetSource()
                  );

    // Simulator::ScheduleNow(&HttpServer::ParseHttp, this, content);
    // ParseHttp (content, socket);
    std::thread parserThread(&HttpServer::ParseHttp, this, content, socket); 
    parserThread.detach();
    content.clear ();
  }
}

void HttpServer::ParseHttp (std::string content, Ptr<Socket> s)
{
  NS_LOG_FUNCTION ("\n" << this << " Thread ID: " << std::this_thread::get_id());

  std::istringstream in (content);
  std::string verb;
  in >> verb;

  if (verb.compare ("HEAD") == 0)
  {
  }
  else if (verb.compare ("GET") == 0)
  {
    SendResponse (OK, s); //NOT_FOUND);
  }
  else if (verb.compare ("POST") == 0)
  {
    // NS_LOG_DEBUG("RXing POST...");

    std::map<string, string> keyValues;
    uint32_t keyIndex = 0;
    // trap npos, subtile on OsX
    size_t valIndex = 0;

    while (in >> verb)
    {
      // Parse POST parameters from here
      if (verb.compare ("Content-Length:") == 0)
      {
        in >> verb;
        in >> verb;
        do
        {
          keyIndex = verb.find ('=');
          valIndex = verb.find ('&');

          keyValues.insert (std::pair<string, string> (verb.substr (0, keyIndex),
                            verb.substr (keyIndex + 1, valIndex - keyIndex - 1)));

          verb = verb.substr (valIndex + 1);
          if (valIndex == string::npos)
            break;
        }
        while (1 > 0); //valIndex < content.length());

        // for (auto i = keyValues.begin(); i != keyValues.end(); i++)
        //     NS_LOG_DEBUG((string) i->first << " <-> " << (string) i->second);
      }
    }
    if (! m_rxPostCb.IsNull ())
    {
      // m_rxPostCb (keyValues);
        std::thread rxThread(m_rxPostCb, keyValues);
        rxThread.detach();
    }
     std::thread respT(&HttpServer::SendResponse, this, OK, s);
     respT.detach();
  }
  else if (verb.compare ("PUT") == 0) //**** Adjust below as needed
  {
    SendResponse (UNAUTHORIZED, s);
  }
  else if (verb.compare ("DELETE") == 0)
  {
    SendResponse (UNAUTHORIZED, s);
  }
  else if (verb.compare ("TRACE") == 0)
  {
    SendResponse (NO_CONTENT, s);
  }
  else if (verb.compare ("OPTIONS") == 0)
  {
    SendResponse (UNAUTHORIZED, s);
  }
  else if (verb.compare ("CONNECT") == 0)
  {
    SendResponse (OK, s);
  }
  else
  {
    SendResponse (BAD_REQUEST, s);
  }
}

void HttpServer::SendResponse (HttpStatus_t status, Ptr<Socket> socket)
{
  NS_LOG_FUNCTION ("\n" << this);

  std::string response;
  std::string page ("");
  std::ostringstream pageSize;
  pageSize.flags (ios::hex);

  if (! m_responseCb.IsNull ())
    m_responseCb (status);

  switch (status)
  {
    case NOT_FOUND:
    {
      response.append ("HTTP/1.1 404 Not Found\r\n");
      page = GenerateNotFoundPage ("favicon.ico");

      break;
    }

    case OK:
    {
      response.append ("HTTP/1.1 200 OK\r\n");
      break;
    }

    case BAD_REQUEST:
    {
      response.append ("HTTP/1.1 400 Bad Request\r\n");
      break;
    }

    default:
    {
      response.append ("HTTP/1.1 204 No Content\r\n");
      break;
    }
  }
  // should add "Content-Length" ..
  pageSize << page.length ();
  response.append ("Transfer-Encoding: chunked\r\n\r\n");
  response.append ("Content-Type: text/html; charset=utf-8\r\n");

  response.append (pageSize.str ()).append ("\r\n").append (page).append ("\r\n");
  response.append ("0\r\n\r\b"); // footer ends chunk  CRLF

  const uint8_t* data = reinterpret_cast<const uint8_t*> (response.data ());
  socket->Send (data, response.length (), 0);
  response.clear ();
}

std::string HttpServer::GenerateNotFoundPage (std::string url)
{
  std::string page = "<!DOCTYPE html>\n\
            <HTML><HEAD>\n\
            <TITLE>404 Not Found</TITLE>\n\
            </HEAD><BODY>\n\
            <H1>Not Found</H1>\n\
            The requested URL ";
  page.append (url);
  page.append (" was not found on this server.<P>\n </BODY></HTML>\n");
  return page;
}

void HttpServer::SetRecvPostCallback (Callback<void, std::map<std::string, std::string> > keyValuesCb)
{
  NS_LOG_FUNCTION ("\n" << this);
  m_rxPostCb = keyValuesCb;

}

void HttpServer::SetResponseCallback (Callback<void, HttpStatus_t> respCb)
{
  NS_LOG_FUNCTION ("\n" << this);
  m_responseCb = respCb;
}

// e.g. for LTE-D AppCode feedback

void HttpServer::SetResponseCallback (Callback<void, std::string> respCb)
{

  NS_LOG_FUNCTION ("\n" << this);

}

} // Namespace ns3
