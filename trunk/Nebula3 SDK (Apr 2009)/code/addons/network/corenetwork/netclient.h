#pragma once
#ifndef CORENETWORK_NETCLIENT_H
#define CORENETWORK_NETCLIENT_H
//------------------------------------------------------------------------------
/**
    @class CoreNetwork::NetClient
      
    (C) 2008 Radon Labs GmbH
*/    
#if __WIN32__
#include "corenetwork/raknet/raknetclient.h"

namespace CoreNetwork
{
class NetClient : public RakNet::RaknetClient
{
    __DeclareClass(NetClient);
    __DeclareSingleton(NetClient);
public:
    /// constructor
    NetClient();
    /// destructor
    virtual ~NetClient();
};
}
#else
#error "NetClient class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
