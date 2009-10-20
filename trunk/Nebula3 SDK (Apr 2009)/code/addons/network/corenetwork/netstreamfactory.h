#pragma once
#ifndef CORENETWORK_NETSTREAMFACTORY_H
#define CORENETWORK_NETSTREAMFACTORY_H

#if __WIN32__
#include "corenetwork/raknet/raknetstreamfactory.h"
namespace CoreNetwork
{
    typedef RakNet::RaknetStreamFactory NetStreamFactory;
}
#else
n_error("Netstream factory not implemented on this platform!");
#endif
#endif // CORENETWORK_PACKETPRIORITY_H