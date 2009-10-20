#pragma once
#ifndef BASE_NETCLIENTBASE_H
#define BASE_NETCLIENTBASE_H
//------------------------------------------------------------------------------
/**
    @class Base::NetClientBase

    A network client which can connect to one server.

    (C) 2008 Radon Labs GmbH
*/    
#include "core/refcounted.h"
#include "core/singleton.h"
#include "corenetwork/netstream.h"

//------------------------------------------------------------------------------
namespace Base
{
class NetClientBase : public Core::RefCounted
{
    __DeclareClass(NetClientBase);
    __DeclareSingleton(NetClientBase);
public:
    /// constructor
    NetClientBase();
    /// destructor
    virtual ~NetClientBase();

    /// open client thread
    virtual bool Open();
    /// close client thread
    virtual void Close();
    /// check if client thread is running
    bool IsOpen() const;

    /// update client state from received packets, returns true for correct connection
    virtual bool Update();
    /// set server address
    const Util::String& GetServerAdress() const;
    /// get server address
    void SetServerAdress(const Util::String& val);
    /// set host port of server
    void SetHostPort(uint port);
    /// set host port of server
    uint GetHostPort() const;
    /// connect, returns true if client goes into connecting attempt
    virtual bool Connect();
    /// disconnect from server
    virtual void Disconnect();
    /// return true if connected
    virtual bool IsConnected() const;

    /// get connection lost notification
    bool PopConnectionSucceededFlag();
    /// get connection lost notification
    bool PopConnectionLostFlag();
    /// pop connection failed flag
    bool PopConnectionFailedFlag();
    
protected:
    bool isOpen;
    Util::String serverAdress;
    bool isConnected;
    bool connectionFailed;
    bool connectionLost;
    bool connectionSucceeded;
    uint hostPort;
};

//------------------------------------------------------------------------------
/**
*/
inline const Util::String& 
NetClientBase::GetServerAdress() const 
{ 
    return serverAdress; 
}

//------------------------------------------------------------------------------
/**
*/
inline void 
NetClientBase::SetServerAdress(const Util::String& val) 
{ 
    serverAdress = val; 
}

//------------------------------------------------------------------------------
/**
*/
inline void 
NetClientBase::SetHostPort(uint port)
{
    this->hostPort = port;
}

//------------------------------------------------------------------------------
/**
*/
inline uint
NetClientBase::GetHostPort() const
{
    return this->hostPort;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
NetClientBase::PopConnectionSucceededFlag()
{
    bool result = this->connectionSucceeded;
    this->connectionSucceeded = false;
    return result;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
NetClientBase::PopConnectionLostFlag()
{
    bool result = this->connectionLost;
    this->connectionLost = false;
    return result;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
NetClientBase::PopConnectionFailedFlag()
{
    bool result = this->connectionFailed;
    this->connectionFailed = false;
    return result;
}

} // namespace Base
//------------------------------------------------------------------------------
#endif
