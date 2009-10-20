#pragma once
#ifndef CORENETWORK_NETSERVER_H
#define CORENETWORK_NETSERVER_H
//------------------------------------------------------------------------------
/**
    @class CoreNetwork::NetServer
    
    (C) 2008 Radon Labs GmbH
*/    
#if __WIN32__
#include "corenetwork/raknet/raknetserver.h"

namespace CoreNetwork
{
class NetServer : public RakNet::RaknetServer
{
    __DeclareClass(NetServer);
    __DeclareSingleton(NetServer);
public:
    /// constructor
    NetServer();
    /// destructor
    virtual ~NetServer();
};
} // namespace CoreNetwork
#else
#error "NetServer class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
