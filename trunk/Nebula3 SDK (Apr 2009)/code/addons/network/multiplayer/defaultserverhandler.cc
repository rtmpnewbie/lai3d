//------------------------------------------------------------------------------
//  debughandler.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "multiplayer/defaultserverhandler.h"
#include "corenetwork/netstream.h"
#include "messaging/messagewriter.h"
#include "messaging/messagereader.h"
#include "corenetwork/netstreamfactory.h"
#include "corenetwork/packetid.h"
#include "multiplayer/multiplayerprotocol.h"
#include "corenetwork/netconfig.h"

namespace Multiplayer
{
    __ImplementClass(Multiplayer::DefaultServerHandler, 'DSMH', Multiplayer::NetMessageHandler);

using namespace Util;
using namespace CoreNetwork;
using namespace Messaging;
//------------------------------------------------------------------------------
/**
*/
DefaultServerHandler::DefaultServerHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
DefaultServerHandler::~DefaultServerHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This method runs in the Debug thread's context and sets up the required
    runtime.
*/
void
DefaultServerHandler::Open()
{
    n_assert(!this->IsOpen());
    NetMessageHandler::Open();

    n_assert(!this->server.isvalid());
    this->server = CoreNetwork::NetServer::Create();
    this->server->SetPortNumber(SERVER_DEFAULT_PORT);
    this->server->Open();
}

//------------------------------------------------------------------------------
/**
*/
void
DefaultServerHandler::Close()
{
    n_assert(this->IsOpen());
    n_assert(this->server.isvalid());
    this->server->Close();

    NetMessageHandler::Close();
}

//------------------------------------------------------------------------------
/**
    The per-frame method just checks periodically whether there are
    any pending HttpRequests to process...
*/
void
DefaultServerHandler::DoWork()
{
    n_assert(this->IsOpen());
    n_assert(this->server.isvalid());

    this->server->Update();

    if (this->server->HasNewStreams())
    {
        this->ProcessIncomingStreams();
    }
}

//------------------------------------------------------------------------------
/**
    
*/
void 
DefaultServerHandler::ProcessIncomingStreams()
{
    n_assert(this->server.isvalid());
    n_assert(this->server->HasNewStreams());
    IndexT j;
    for (j = 0; j < (SizeT)this->server->GetConnectionCount(); j++)
    {
        Ptr<IpAddress> address = this->server->GetClientAddress(j);    
        IndexT streamIdx;
        for (streamIdx = 0; streamIdx < this->server->GetNumberRecvStreams(address); streamIdx++)
        {
            const Ptr<NetStream>& stream = this->server->GetStreamAtIndex(address, streamIdx);
            Ptr<Messaging::Message> msg = NetStreamFactory::CreateMessageFromStream(stream);

            // example of handling a message
            if (msg->CheckId(Chatmsg::Id))
            {
                Ptr<Chatmsg> chatMsg = msg.cast<Chatmsg>();
                // just print message text to console with client ip
                n_printf("Client with ip %s: %s\n", address->AsString().AsCharPtr(), chatMsg->GetText().AsCharPtr());
                this->server->RemoveStreamAtIndex(address, streamIdx);
            }
            else
            {
                this->incomingMsgs.Append(msg);
            }            
        }
    }

    /// send new created msgs to ports which want this type of message
    const Array<Ptr<Messaging::Message> >& msgs = this->incomingMsgs;
    IndexT msgIdx;
    for (msgIdx = 0; msgIdx < msgs.Size(); msgIdx++)
    {
        const Messaging::Id& mId = msgs[msgIdx]->GetId();
        if (this->portMessageIdMapping.Contains(&mId))
        {
            // send message to all registered port for this type of msg
            IndexT portIdx;
            for (portIdx = 0; portIdx < this->portMessageIdMapping[&mId].Size(); portIdx++)
            {
                this->portMessageIdMapping[&mId][portIdx]->Send(msgs[msgIdx]);
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
Util::Array<Util::String> 
DefaultServerHandler::GetIncomingStrings() const
{
    Util::Array<Util::String> result;
    if (this->server.isvalid() && this->server->HasNewStreams())
    {
        IndexT j;
        for (j = 0; j < (SizeT)this->server->GetConnectionCount(); j++)
        {
            Ptr<IpAddress> address = this->server->GetClientAddress(j);
            IndexT i;
            for (i = 0; i < this->server->GetNumberRecvStreams(address); i++)
            {
                Ptr<NetStream> stream = this->server->GetStreamAtIndex(address, i);
                stream->Open();
                if (stream->GetMessageId() == PacketId::ID_NEBULA3_STRING)
                {
                    String str = NetStreamFactory::CreateStringFromStream(stream);
                    result.Append(str);
                }
                stream->Close();
            }
        }
    }

    return result;
}
} // namespace Multiplayer
