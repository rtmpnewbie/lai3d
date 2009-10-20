#pragma once
#ifndef RAKNET_RAKNETSTREAMFACTORY_H
#define RAKNET_RAKNETSTREAMFACTORY_H

#include "util/string.h"
#include "corenetwork/netstream.h"
#include "messaging/message.h"

namespace RakNet
{
class RaknetStreamFactory
{
public:
    /// create stream with string
    static Ptr<CoreNetwork::NetStream> CreateStreamFromString(const Util::String& str);
    /// create string from stream
    static Util::String CreateStringFromStream(const Ptr<CoreNetwork::NetStream>& stream);
    /// convert messaging msg to netstream
    static Ptr<CoreNetwork::NetStream> CreateNetStreamFromMessage(const Ptr<Messaging::Message>& msg);
    /// convert netstream to msg
    static Ptr<Messaging::Message> CreateMessageFromStream(const Ptr<CoreNetwork::NetStream>& stream);
};
}
#endif // RAKNET_RAKNETSTREAMFACTORY_H