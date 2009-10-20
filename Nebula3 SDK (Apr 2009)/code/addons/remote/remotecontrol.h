#pragma once
#ifndef REMOTE_REMOTECONTROL_H
#define REMOTE_REMOTECONTROL_H
//------------------------------------------------------------------------------
/**
    @class Remote::RemoteControl
    
    Implements an Remote server with attached RemoteRequestHandlers. This is 
    used to remote control the application over a TCP/IP connection.
    Its target application is to control nebula games from external
    tools like animation or cut scene editors.

    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "net/tcpserver.h"
#include "net/socket/ipaddress.h"
#include "io/textreader.h"
#include "io/textwriter.h"
#include "remote/remoteresponsewriter.h"
#include "remote/remoterequesthandler.h"
#include "remote/defaultremoterequesthandler.h"

//------------------------------------------------------------------------------
namespace Remote
{
class RemoteControl : public Core::RefCounted
{
    __DeclareClass(RemoteControl);
    __DeclareSingleton(RemoteControl);
public:
    /// constructor
    RemoteControl();
    /// destructor
    virtual ~RemoteControl();
    
    /// set port number for remote service
    void SetPort(ushort p);
    /// get port number of remote service
    ushort GetPort() const;
    /// open the remote server
    bool Open();
    /// close the remote server
    void Close();
    /// return true if server is open
    bool IsOpen() const;
    /// attach a request handler to the server
    void AttachRequestHandler(const Ptr<RemoteRequestHandler>& h);
    /// remove a request handler from the server
    void RemoveRequestHandler(const Ptr<RemoteRequestHandler>& h);
    /// get registered request handlers
    Util::Array<Ptr<RemoteRequestHandler> > GetRequestHandlers() const;
    /// call this method frequently to serve http connections
    void OnFrame();

private:
    /// handle an RemoteRequest
    bool HandleRemoteRequest(const Ptr<Net::TcpClientConnection>& clientConnection);
    /// build an RemoteResponse for a handled remote request
    bool BuildRemoteResponse(const Ptr<Net::TcpClientConnection>& clientConnection, 
        const Ptr<RemoteRequest>& remoteRequest);

    struct PendingRequest
    {
        Ptr<Net::TcpClientConnection> clientConnection;
        Ptr<RemoteRequest> remoteRequest;
    };

    Util::Dictionary<Util::String, Ptr<RemoteRequestHandler> > requestHandlers;
    Ptr<DefaultRemoteRequestHandler> defaultRequestHandler;    
    Net::IpAddress ipAddress;
    Ptr<Net::TcpServer> tcpServer;
    Util::Array<PendingRequest> pendingRequests;
    bool isOpen;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
RemoteControl::IsOpen() const
{
    return this->isOpen;
}

//------------------------------------------------------------------------------
/**
*/
inline void
RemoteControl::SetPort(ushort p)
{
    this->ipAddress.SetPort(p);
}

//------------------------------------------------------------------------------
/**
*/
inline ushort
RemoteControl::GetPort() const
{
    return this->ipAddress.GetPort();
}

//------------------------------------------------------------------------------
/**
*/
inline Util::Array<Ptr<RemoteRequestHandler> >
RemoteControl::GetRequestHandlers() const
{
    return this->requestHandlers.ValuesAsArray();
}

} // namespace Remote
//------------------------------------------------------------------------------
#endif
    