#pragma once
#ifndef CORENETWORK_PACKETPRIORITY_H
#define CORENETWORK_PACKETPRIORITY_H

#if __WIN32__
#include "corenetwork/raknet/raknetpacketpriority.h"
namespace CoreNetwork
{
typedef RakNet::RakNetPacketPriority PacketPriority;
}
#else
n_error("Packet priority not implemented on this platform!");
#endif
#endif // CORENETWORK_PACKETPRIORITY_H