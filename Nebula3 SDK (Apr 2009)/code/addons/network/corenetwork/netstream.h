#pragma once
#ifndef CORENETWORK_NETSTREAM_H
#define CORENETWORK_NETSTREAM_H

#if __WIN32__
#include "corenetwork/raknet/raknetstream.h"
namespace CoreNetwork
{
    class NetStream : public RakNet::RaknetStream
    {
        __DeclareClass(NetStream);
    };
}
#else
n_error("NetStream not implemented for this platform!");
#endif

#endif // CORENETWORK_NETSTREAM_H