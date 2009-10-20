#ifndef MULTIPLAYER_MULTIPLAYERMANAGER_H
#define MULTIPLAYER_MULTIPLAYERMANAGER_H
//------------------------------------------------------------------------------
/**
    @class Multiplayer::MultiplayerManager



    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "io/uri.h"
#include "corenetwork/netserver.h"
#include "corenetwork/netclient.h"
#include "util/dictionary.h"
#include "core/ptr.h"
#include "multiplayer/netmessagehandler.h"
#include "corenetwork/ipaddress.h"

namespace Messaging
{
    class Message;
}

// macro for registering messages
#define RECEIVE_MSG_FROMSERVER(port, msgId) { n_assert(Multiplayer::MultiplayerManager::HasInstance());\
                                         Multiplayer::MultiplayerManager* mmanger = Multiplayer::MultiplayerManager::Instance();\
                                         mmanger->RegisterNetMessage(port, msgId, MultiplayerManager::ClientMode);}

//------------------------------------------------------------------------------
namespace Multiplayer
{    

class MultiplayerManager : public Core::RefCounted
{
	__DeclareClass(MultiplayerManager);
    __DeclareSingleton(MultiplayerManager);

public:
    enum NetMode
    {
        ClientMode = 0,
        ServerMode = 1,

        NumNetModes
    };

    /// constructor
    MultiplayerManager();
    /// destructor
    virtual ~MultiplayerManager();
    /// open multiplayer manager 
    void Open();
    /// close 
    void Close();
    /// is open?
    bool IsOpen() const;
    /// on frame
    bool OnFrame();
    
    /// start default client service
    void StartDefaultClientService();
    /// start default server service
    void StartDefaultServerService();
    /// stop default service
    void StopDefaultService();
    
    /// attach handler for client mode
    void AttachClientHandler(const Ptr<NetMessageHandler>& handler);
    /// attach handler for server mode
    void AttachServerHandler(const Ptr<NetMessageHandler>& handler);
    /// register port for receiving messages from
    void RegisterNetMessage(Messaging::Port* port, const Messaging::Id& msgId, NetMode mode); 

    /// handle system message by client or server handler
    void HandleSystemMessageBy(const Ptr<Messaging::Message>& msg, NetMode mode);
    /// send message to client over server
    void SendToClient(const Ptr<Messaging::Message>& msg, const Ptr<CoreNetwork::IpAddress>& address);
    /// broadcast msg to all clients
    void BroadcastToClients(const Ptr<Messaging::Message>& msg, CoreNetwork::PacketReliability::Reliability packetReliability);
    /// send to server
    void SendToServer(const Ptr<Messaging::Message>& msg);

private: 
    /// get net message handler
    const Ptr<NetMessageHandler>& GetNetMessageHandler(NetMode mode) const;
    /// remove all handlers
    void RemoveAllNetHandlers();

    Util::FixedArray<Ptr<NetMessageHandler> > netHandler;
    bool isOpen;
};

//------------------------------------------------------------------------------
/**
*/
inline bool 
MultiplayerManager::IsOpen() const
{
    return this->isOpen;
}

}; // namespace Multiplayer
//------------------------------------------------------------------------------
#endif

