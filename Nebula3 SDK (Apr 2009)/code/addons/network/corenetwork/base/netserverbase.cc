//------------------------------------------------------------------------------
//  netserverbase.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "corenetwork/base/netserverbase.h"

namespace Base
{
__ImplementClass(Base::NetServerBase, 'NSRB', Core::RefCounted);
__ImplementSingleton(Base::NetServerBase);

using namespace Util;

//------------------------------------------------------------------------------
/**
*/
NetServerBase::NetServerBase() :
isOpen(false),
maxConnections(16),
portNumber(1234)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
NetServerBase::~NetServerBase()
{
    n_assert(!this->IsOpen());    
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
bool
NetServerBase::Open()
{
    n_assert(!this->IsOpen());
    this->isOpen = true;

    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
NetServerBase::Close()
{
    n_assert(this->IsOpen());

    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
bool
NetServerBase::IsOpen() const
{
    return this->isOpen;
}

//------------------------------------------------------------------------------
/**
*/
bool 
NetServerBase::Update()
{
    return false;
}

} // namespace Base
