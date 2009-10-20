#pragma once
#ifndef MULTIPLAYER_NETMESSAGEHANDLER_H
#define MULTIPLAYER_NETMESSAGEHANDLER_H
//------------------------------------------------------------------------------
/**
    @class Debug::NetMessageHandler
        
    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "messaging/message.h"
#include "messaging/port.h"
#include "util/dictionary.h"
#include "corenetwork/ipaddress.h"
#include "corenetwork/packetreliability.h"

namespace CoreNetwork
{
    class NetStream; 
}

//------------------------------------------------------------------------------
namespace Multiplayer
{
class MultiplayerManager;

class NetMessageHandler : public Core::RefCounted
{
    __DeclareClass(NetMessageHandler);
public:
    /// constructor
    NetMessageHandler();
    /// destructor
    virtual ~NetMessageHandler();
    /// open the handler
    virtual void Open();
    /// close the handler
    virtual void Close();
    /// return true if open
    bool IsOpen() const;
    /// do per-frame work
    virtual void DoWork();    
    /// handle a message, return true if handled
    virtual bool HandleSystemMessage(const Ptr<Messaging::Message>& msg);
    /// send message to specified address
    virtual void SendTo(const Ptr<Messaging::Message>& msg, const Ptr<CoreNetwork::IpAddress>& address);
    /// broadcast to all connected clients
    virtual void Broadcast(const Ptr<Messaging::Message>& msg, CoreNetwork::PacketReliability::Reliability packetReliability);
    /// process incoming streams
    virtual void ProcessIncomingStreams();
    /// register message 
    void RegisterNetMessage(Messaging::Port* port, const Messaging::Id& msgId);

protected:   
    // incoming
    Util::Array<Ptr<Messaging::Message> > incomingMsgs;
    Util::Dictionary<const Messaging::Id*, Util::Array<Messaging::Port*> > portMessageIdMapping;

    bool isOpen;
};

//-----------------------------------------------------------------------------
/**
*/
inline bool
NetMessageHandler::IsOpen() const
{
    return this->isOpen;
}

} // namespace Multiplayer
//------------------------------------------------------------------------------
#endif
    