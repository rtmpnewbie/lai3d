#pragma once
#ifndef CORENETWORK_RELIABILITY_H
#define CORENETWORK_RELIABILITY_H

#if __WIN32__
#include "corenetwork/raknet/raknetpacketreliability.h"
namespace CoreNetwork
{
typedef RakNet::RakNetPacketReliability PacketReliability;
}
#else
n_error("Packet Reliability not implemented on this platform!");
#endif
#endif // CORENETWORK_PACKETRELIABILITY_H