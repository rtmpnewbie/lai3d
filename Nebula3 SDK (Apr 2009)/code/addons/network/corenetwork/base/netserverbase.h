#pragma once
#ifndef BASE_NETSERVERBASE_H
#define BASE_NETSERVERBASE_H
//------------------------------------------------------------------------------
/**
    @class Base::NetServerBase

    A server base class who can get multiple connections to clients.
         
    (C) 2008 Radon Labs GmbH
*/    
#include "core/refcounted.h"
#include "core/singleton.h"
#include "timing/time.h"

//------------------------------------------------------------------------------
namespace Base
{
class NetServerBase : public Core::RefCounted
{
    __DeclareClass(NetServerBase);
    __DeclareSingleton(NetServerBase);
public:
    /// constructor
    NetServerBase();
    /// destructor
    virtual ~NetServerBase();

    /// open the device
    bool Open();
    /// close the device
    void Close();
    /// return true if currently open
    bool IsOpen() const;
    /// get maximum connections allowed
    uint GetMaxConnections() const;
    /// set maximum connections
    void SetMaxConnections(uint val);
    /// get port number
    uint GetPortNumber() const;
    /// get port number
    void SetPortNumber(uint port);
    /// update state of client from received streams
    virtual bool Update();
   
protected:
    bool isOpen;
    uint maxConnections;
    uint portNumber;        
};

//------------------------------------------------------------------------------
/**
*/
inline uint 
NetServerBase::GetMaxConnections() const
{
    return this->maxConnections;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
NetServerBase::SetMaxConnections(uint val) 
{ 
    maxConnections = val; 
}

//------------------------------------------------------------------------------
/**
*/
inline uint 
NetServerBase::GetPortNumber() const
{
    return this->portNumber;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
NetServerBase::SetPortNumber(uint port)
{
    this->portNumber = port;
}

} // namespace Base
//------------------------------------------------------------------------------
#endif
