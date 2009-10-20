#include "stdneb.h"
#include "corenetwork/raknet/raknetstreamfactory.h"
#include "corenetwork/netstream.h"
#include "corenetwork/packetid.h"
#include "messaging/messagewriter.h"
#include "messaging/messagereader.h"

namespace RakNet
{
using namespace CoreNetwork;
using namespace Messaging;

//------------------------------------------------------------------------------
/**
*/
Ptr<NetStream> 
RaknetStreamFactory::CreateStreamFromString(const Util::String &str)
{
    Ptr<NetStream> newStream = NetStream::Create();
    newStream->Open();
    newStream->Write(PacketId::ID_NEBULA3_STRING);
    // copy string with ending zero
    newStream->Write(str.AsCharPtr(), str.Length()+1);
    newStream->Close();

    return newStream;
}

//------------------------------------------------------------------------------
/**
*/
Ptr<CoreNetwork::NetStream> 
RaknetStreamFactory::CreateNetStreamFromMessage(const Ptr<Message>& msg)
{
    Ptr<NetStream> netStream = NetStream::Create();
    netStream->Open();
    netStream->Write(PacketId::ID_NEBULA3_MESSAGE);
    netStream->Close();
    Ptr<MessageWriter> writer = MessageWriter::Create();
    writer->SetStream(netStream.cast<IO::Stream>());
    writer->Open();
    writer->WriteMessage(msg);
    writer->Close();

    return netStream;
}

//------------------------------------------------------------------------------
/**
*/
Ptr<Message> 
RaknetStreamFactory::CreateMessageFromStream(const Ptr<CoreNetwork::NetStream>& stream)
{    
    Ptr<MessageReader> reader = MessageReader::Create();
    reader->SetStream(stream.cast<IO::Stream>());
    reader->Open();
    Ptr<Message> msg = reader->ReadMessage();
    reader->Close();

    return msg;
}

//------------------------------------------------------------------------------
/**
*/
Util::String 
RaknetStreamFactory::CreateStringFromStream(const Ptr<CoreNetwork::NetStream>& stream)
{
    n_assert(stream->GetMessageId() == PacketId::ID_NEBULA3_STRING);

    Util::String str;
    str.Reserve(stream->GetSize());
    stream->Read((void*)str.AsCharPtr(), stream->GetSize());

    return str;
}
} // namespace RakNet
