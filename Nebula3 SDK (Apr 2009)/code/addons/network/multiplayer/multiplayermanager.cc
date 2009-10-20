//------------------------------------------------------------------------------
//  multiplayer/multiplayermanager.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "multiplayer/multiplayermanager.h"
#include "corenetwork/netconfig.h"
#include "messaging/message.h"
#include "corenetwork/netstreamfactory.h"
#include "corenetwork/netstream.h"
#include "multiplayer/defaultclienthandler.h"
#include "multiplayer/defaultserverhandler.h"

namespace Multiplayer
{
__ImplementClass(MultiplayerManager, 'MULM', Core::RefCounted);
__ImplementSingleton(MultiplayerManager);

using namespace CoreNetwork;
using namespace Util;
//------------------------------------------------------------------------------
/**
*/
MultiplayerManager::MultiplayerManager()
{
    __ConstructSingleton;

    this->netHandler.SetSize(NumNetModes);
}

//------------------------------------------------------------------------------
/**
*/
MultiplayerManager::~MultiplayerManager()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void 
MultiplayerManager::Open()
{
    this->isOpen = true;    
}

//------------------------------------------------------------------------------
/**
*/
void MultiplayerManager::Close()
{
    this->RemoveAllNetHandlers();

    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
bool
MultiplayerManager::OnFrame()
{
    bool success = true;
    
    IndexT i;
    for (i = 0; i < this->netHandler.Size(); i++)
    {   
        if (this->netHandler[i].isvalid())
        {
            // receive net streams and send as messages
            this->netHandler[i]->DoWork();
        }        
    }

    return success;
}

//------------------------------------------------------------------------------
/**
*/
void 
MultiplayerManager::StartDefaultClientService()
{
    Ptr<DefaultClientHandler> handler = DefaultClientHandler::Create();
    this->AttachClientHandler(handler.cast<NetMessageHandler>());    
}

//------------------------------------------------------------------------------
/**
*/
void 
MultiplayerManager::StartDefaultServerService()
{
    Ptr<DefaultServerHandler> handler = DefaultServerHandler::Create();    
    this->AttachServerHandler(handler.cast<NetMessageHandler>());    
}

//------------------------------------------------------------------------------
/**
*/
void 
MultiplayerManager::StopDefaultService()
{
    this->RemoveAllNetHandlers();
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<NetMessageHandler>& 
MultiplayerManager::GetNetMessageHandler(NetMode mode) const
{
    return this->netHandler[mode];
}

//------------------------------------------------------------------------------
/**
*/
void 
MultiplayerManager::AttachClientHandler(const Ptr<NetMessageHandler>& handler)
{
    this->netHandler[ClientMode] = handler;
    handler->Open();
}

//------------------------------------------------------------------------------
/**
*/
void 
MultiplayerManager::AttachServerHandler(const Ptr<NetMessageHandler>& handler)
{
    this->netHandler[ServerMode] = handler;
    handler->Open();
}

//------------------------------------------------------------------------------
/**
*/
void 
MultiplayerManager::RemoveAllNetHandlers()
{
    IndexT handlerIdx;
    for (handlerIdx = 0; handlerIdx < this->netHandler.Size() ; handlerIdx++)
    {
        if (this->netHandler[handlerIdx].isvalid())
        {
            if (this->netHandler[handlerIdx]->IsOpen())
            {
                this->netHandler[handlerIdx]->Close();
            }
            this->netHandler[handlerIdx] = 0;
        }        
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
MultiplayerManager::RegisterNetMessage(Messaging::Port* port, const Messaging::Id& msgId, NetMode mode)
{
    n_assert2(this->netHandler[mode].isvalid(), "No handler for this network mode exists!!!");
    this->netHandler[mode]->RegisterNetMessage(port, msgId);
}

//------------------------------------------------------------------------------
/**
*/
void 
MultiplayerManager::SendToClient(const Ptr<Messaging::Message>& msg,  const Ptr<CoreNetwork::IpAddress>& address)
{
    n_assert(this->netHandler[ServerMode].isvalid());
    this->netHandler[ServerMode]->SendTo(msg, address);

    //n_printf("Send %s to %s \n", msg->GetRtti()->GetName().AsCharPtr(), address->AsString().AsCharPtr());
}

//------------------------------------------------------------------------------
/**
*/
void 
MultiplayerManager::BroadcastToClients(const Ptr<Messaging::Message>& msg, CoreNetwork::PacketReliability::Reliability packetReliability)
{
    n_assert(this->netHandler[ServerMode].isvalid());
    this->netHandler[ServerMode]->Broadcast(msg, packetReliability);
    
    //n_printf("Broadcast %s to all clients \n", msg->GetRtti()->GetName().AsCharPtr());
}

//------------------------------------------------------------------------------
/**
*/
void 
MultiplayerManager::SendToServer(const Ptr<Messaging::Message>& msg)
{
    n_assert(this->netHandler[ClientMode].isvalid());
    this->netHandler[ClientMode]->Broadcast(msg, CoreNetwork::PacketReliability::ReliableSequenced);
    
    //n_printf("Send %s to server \n", msg->GetRtti()->GetName().AsCharPtr());
}

//------------------------------------------------------------------------------
/**
*/
void 
MultiplayerManager::HandleSystemMessageBy(const Ptr<Messaging::Message>& msg, NetMode mode)
{
    n_assert(this->netHandler[mode].isvalid());
    this->netHandler[mode]->HandleSystemMessage(msg);
}
}; // namespace Managers
