//------------------------------------------------------------------------------
//  netclient.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "corenetwork/netclient.h"

namespace CoreNetwork
{
#if __WIN32__
__ImplementClass(CoreNetwork::NetClient, 'NECL', RakNet::RaknetClient);
#else
#error "NetClient class not implemented on this platform!"
#endif

__ImplementSingleton(CoreNetwork::NetClient);

//------------------------------------------------------------------------------
/**
*/
NetClient::NetClient()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
NetClient::~NetClient()
{
    __DestructSingleton;
}

} // namespace CoreNetwork
