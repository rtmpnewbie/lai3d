#pragma once
#ifndef RAKNET_PACKETID_H
#define RAKNET_PACKETID_H

#include "extlibs/raknet/source/messageidentifiers.h"

namespace RakNet
{
#define PACKET_DATA_BYTEOFFSET 1

class RakNetPackedId
{
public:

enum PacketIdCode
{
    ID_NEBULA3_MESSAGE = ID_USER_PACKET_ENUM,
    ID_NEBULA3_ATTRLIST = ID_NEBULA3_MESSAGE +1,
    ID_NEBULA3_STRING,
    
    ID_NUM_NEBULA3_PACKET,
    ID_NEBULA3_INVALID_ID
};

};
}
#endif // RakNet_PACKETID_H