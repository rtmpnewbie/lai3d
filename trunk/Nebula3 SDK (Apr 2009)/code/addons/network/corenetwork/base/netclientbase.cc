//------------------------------------------------------------------------------
//  netclientbase.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "corenetwork/base/netclientbase.h"

namespace Base
{
__ImplementClass(Base::NetClientBase, 'NCLB', Core::RefCounted);
__ImplementSingleton(Base::NetClientBase);

using namespace Util;

//------------------------------------------------------------------------------
/**
*/
NetClientBase::NetClientBase():
isOpen(false),
isConnected(false),
connectionFailed(false),
connectionLost(false),
connectionSucceeded(false)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
NetClientBase::~NetClientBase()
{
    __DestructSingleton;
}


//------------------------------------------------------------------------------
/**
*/
bool
NetClientBase::Open()
{
    n_assert(!this->IsOpen());
    this->isOpen = true;

    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
NetClientBase::Close()
{
    n_assert(this->IsOpen());

    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
bool
NetClientBase::IsOpen() const
{
    return this->isOpen;
}

//------------------------------------------------------------------------------
/**
*/
bool 
NetClientBase::IsConnected() const
{
    return this->isConnected;
}

//------------------------------------------------------------------------------
/**
*/
bool NetClientBase::Connect()
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void NetClientBase::Disconnect()
{    
}

//------------------------------------------------------------------------------
/**
*/
bool 
NetClientBase::Update()
{
    return false;
}
} // namespace Base
