#pragma once
#ifndef CORENETWORK_PACKETID_H
#define CORENETWORK_PACKETID_H

#if __WIN32__
#include "corenetwork/raknet/raknetpacketid.h"
namespace CoreNetwork
{
    typedef RakNet::RakNetPackedId PacketId;
}
#else
n_error("PacketId not implemented on this platform!");
#endif

#endif // RakNet_PACKETID_H