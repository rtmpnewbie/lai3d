//------------------------------------------------------------------------------
//  netserverbase.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "corenetwork/netserver.h"

namespace CoreNetwork
{
#if __WIN32__
__ImplementClass(CoreNetwork::NetServer, 'NESV', RakNet::RaknetServer);
#else
#error "NetServer class not implemented on this platform!"
#endif

__ImplementSingleton(CoreNetwork::NetServer);

//------------------------------------------------------------------------------
/**
*/
NetServer::NetServer()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
NetServer::~NetServer()
{
    __DestructSingleton;
}

} // namespace CoreNetwork
