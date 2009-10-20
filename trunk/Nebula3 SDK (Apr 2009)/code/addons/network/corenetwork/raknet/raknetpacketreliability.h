#pragma once
#ifndef RAKNET_RAKNETPACKETRELIABILITY_H
#define RAKNET_RAKNETPACKETRELIABILITY_H

#include "extlibs/raknet/source/packetpriority.h"
namespace RakNet
{
class RakNetPacketReliability
{
public:
    enum Reliability
    {
	    /// Same as regular UDP, except that it will also discard duplicate datagrams.  RakNet adds (6 to 17) + 21 bits of overhead, 16 of which is used to detect duplicate packets and 6 to 17 of which is used for message length.
	    Unreliable = UNRELIABLE,

	    /// Regular UDP with a sequence counter.  Out of order messages will be discarded.  This adds an additional 13 bits on top what is used for UNRELIABLE.
	    UnreliableSequenced = UNRELIABLE_SEQUENCED,

	    /// The message is sent reliably, but not necessarily in any order.  Same overhead as UNRELIABLE.
	    Reliable = RELIABLE,

	    /// This message is reliable and will arrive in the order you sent it.  Messages will be delayed while waiting for out of order messages.  Same overhead as UNRELIABLE_SEQUENCED.
	    ReliableOrdered = RELIABLE_ORDERED,

	    /// This message is reliable and will arrive in the sequence you sent it.  Out or order messages will be dropped.  Same overhead as UNRELIABLE_SEQUENCED.
	    ReliableSequenced = RELIABLE_SEQUENCED
    };
};
}
#endif // RAKNET_RAKNETPACKETRELIABILITY_H