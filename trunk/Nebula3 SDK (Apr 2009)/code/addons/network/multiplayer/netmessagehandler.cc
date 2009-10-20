//------------------------------------------------------------------------------
//  debughandler.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "multiplayer/netmessagehandler.h"
#include "corenetwork/netstream.h"


namespace Multiplayer
{
__ImplementClass(Multiplayer::NetMessageHandler, 'NEMH', Core::RefCounted);

using namespace Util;
using namespace CoreNetwork;
using namespace Messaging;
//------------------------------------------------------------------------------
/**
*/
NetMessageHandler::NetMessageHandler():
    isOpen(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
NetMessageHandler::~NetMessageHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This method runs in the Debug thread's context and sets up the required
    runtime.
*/
void
NetMessageHandler::Open()
{
    n_assert(!this->IsOpen());    

    this->isOpen = true;
}

//------------------------------------------------------------------------------
/**
*/
void
NetMessageHandler::Close()
{
    n_assert(this->IsOpen());    

    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
    The per-frame method just checks periodically whether there are
    any pending HttpRequests to process...
*/
void
NetMessageHandler::DoWork()
{
    n_assert(this->IsOpen());
}

//------------------------------------------------------------------------------
/**
*/
void 
NetMessageHandler::ProcessIncomingStreams()
{
    // overwrite in derived class
}

//------------------------------------------------------------------------------
/**
*/
bool 
NetMessageHandler::HandleSystemMessage(const Ptr<Messaging::Message>& msg)
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void 
NetMessageHandler::RegisterNetMessage(Messaging::Port* port, const Messaging::Id& msgId)
{
    n_assert(port != 0);
    if (this->portMessageIdMapping.Contains(&msgId))
    {
        if (InvalidIndex == this->portMessageIdMapping[&msgId].BinarySearchIndex(port))
        {
            this->portMessageIdMapping[&msgId].InsertSorted(port);
        }
    }
    else
    {
        Util::Array<Messaging::Port*> ports;
        ports.Append(port);
        this->portMessageIdMapping.Add(&msgId, ports);
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
NetMessageHandler::SendTo(const Ptr<Message>& msg, const Ptr<CoreNetwork::IpAddress>& address)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void 
NetMessageHandler::Broadcast(const Ptr<Messaging::Message>& msg, CoreNetwork::PacketReliability::Reliability packetReliability)
{
    // empty
}
} // namespace Multiplayer
