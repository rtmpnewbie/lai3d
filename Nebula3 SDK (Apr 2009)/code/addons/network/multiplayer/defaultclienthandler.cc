//------------------------------------------------------------------------------
//  debughandler.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "multiplayer/defaultclienthandler.h"
#include "corenetwork/netstream.h"
#include "messaging/messagewriter.h"
#include "messaging/messagereader.h"
#include "corenetwork/netstreamfactory.h"
#include "multiplayer/multiplayerprotocol.h"

namespace Multiplayer
{
    __ImplementClass(Multiplayer::DefaultClientHandler, 'DCMH', Multiplayer::NetMessageHandler);

using namespace Util;
using namespace CoreNetwork;
using namespace Messaging;
//------------------------------------------------------------------------------
/**
*/
DefaultClientHandler::DefaultClientHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
DefaultClientHandler::~DefaultClientHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This method runs in the Debug thread's context and sets up the required
    runtime.
*/
void
DefaultClientHandler::Open()
{
    n_assert(!this->IsOpen());
    NetMessageHandler::Open();

    n_assert(!this->client.isvalid());
    this->client = NetClient::Create();
    // set default port for client        
    this->client->Open();
}

//------------------------------------------------------------------------------
/**
*/
void
DefaultClientHandler::Close()
{
    n_assert(this->IsOpen());
    if (this->client->IsConnected())
    {
        this->client->Disconnect();
    }
    this->client->Close();
    
    NetMessageHandler::Close();
}

//------------------------------------------------------------------------------
/**
    The per-frame method just checks periodically whether there are
    any pending HttpRequests to process...
*/
void
DefaultClientHandler::DoWork()
{
    n_assert(this->IsOpen());
    n_assert(this->client.isvalid());

    this->client->Update();

    if (this->client->GetNumberRecvStreams() > 0)
    {
        this->ProcessIncomingStreams();
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
DefaultClientHandler::ProcessIncomingStreams()
{
    n_assert(this->client.isvalid());
    n_assert(this->client->GetNumberRecvStreams() > 0);
    IndexT streamIdx;
    for (streamIdx = 0; streamIdx < this->client->GetNumberRecvStreams(); streamIdx++)
    {
        const Ptr<NetStream>& stream = this->client->GetStreamAtIndex(streamIdx);
        Ptr<Messaging::Message> msg = NetStreamFactory::CreateMessageFromStream(stream);
        this->incomingMsgs.Append(msg);
    }

    /// send new created msgs to ports which want this typ of message
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
bool 
DefaultClientHandler::HandleSystemMessage(const Ptr<Messaging::Message>& msg)
{
    // check for system msgs
    if (msg->CheckId(Connect::Id))
    {
        Ptr<Connect> connectMsg = msg.cast<Connect>();
        this->ConnectToServer(connectMsg->GetServerAddress());
        msg->SetHandled(true);

        return true;
    }
    else if (msg->CheckId(Disconnect::Id))
    {
        this->DisconnectFromServer();
        msg->SetHandled(true);

        return true;
    }
    else if (msg->CheckId(IsClientConnected::Id))
    {
        Ptr<IsClientConnected> isConnected = msg.cast<IsClientConnected>();
        isConnected->SetIsConnected(this->client->IsConnected());
        isConnected->SetHandled(true);

        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void 
DefaultClientHandler::ConnectToServer(const IO::URI& uri)
{
    n_assert(this->IsOpen());
    n_assert(this->client.isvalid());
    this->client->SetHostPort(uri.Port().AsInt());
    this->client->SetServerAdress(uri.Host());
    this->client->Connect(); 
}

//------------------------------------------------------------------------------
/**
*/
void 
DefaultClientHandler::DisconnectFromServer()
{
    n_assert(this->IsOpen());
    n_assert(this->client.isvalid());
    this->client->Disconnect();
}

//------------------------------------------------------------------------------
/**
*/
void 
DefaultClientHandler::SendStringToServer(const Util::String& str)
{
    n_assert(this->client.isvalid());
    Ptr<NetStream> stream = NetStreamFactory::CreateStreamFromString(str);
    this->client->SendStream(stream);
}

//------------------------------------------------------------------------------
/**
*/
void 
DefaultClientHandler::Broadcast(const Ptr<Messaging::Message>& msg)
{
    // convert message to stream
    Ptr<NetStream> stream = NetStreamFactory::CreateNetStreamFromMessage(msg);

    // send msg to server
    this->client->SendStream(stream);
}
} // namespace Multiplayer
