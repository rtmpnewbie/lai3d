#pragma once
#ifndef RAKNET_RAKNETPACKETPRIORITY_H
#define RAKNET_RAKNETPACKETPRIORITY_H

#include "extlibs/raknet/source/packetpriority.h"
namespace RakNet
{
class RakNetPacketPriority
{
public:
    // not really necessary this wrapping?!
    enum Priority
    {
        LOW = LOW_PRIORITY,
        MEDIUM = MEDIUM_PRIORITY,
        HIGH = HIGH_PRIORITY
    };
}
}
#endif // RAKNET_RAKNETPACKETPRIORITY_H