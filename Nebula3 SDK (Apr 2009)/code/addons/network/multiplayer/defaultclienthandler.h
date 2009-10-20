#pragma once
#ifndef MULTIPLAYER_DEFAULTCLIENTHANDLER_H
#define MULTIPLAYER_DEFAULTCLIENTHANDLER_H
//------------------------------------------------------------------------------
/**
    @class Debug::DefaultClientHandler
        
    (C) 2008 Radon Labs GmbH
*/
#include "multiplayer/netmessagehandler.h"
#include "corenetwork/netclient.h"

//------------------------------------------------------------------------------
namespace Multiplayer
{
class DefaultClientHandler : public NetMessageHandler
{
    __DeclareClass(DefaultClientHandler);
public:
    /// constructor
    DefaultClientHandler();
    /// destructor
    virtual ~DefaultClientHandler();

protected:    
    friend class MultiplayerManager;
    /// open the handler
    virtual void Open();
    /// close the handler
    virtual void Close();
    /// do per-frame work
    virtual void DoWork(); 
    /// handle a message, return true if handled
    virtual bool HandleSystemMessage(const Ptr<Messaging::Message>& msg);
    /// broadcast, send to server
    virtual void Broadcast(const Ptr<Messaging::Message>& msg);
    /// process incoming streams
    virtual void ProcessIncomingStreams();

private:
    /// connect client to server 
    void ConnectToServer(const IO::URI& uri);
    /// disconnect from server
    void DisconnectFromServer();
    /// send a string to server
    void SendStringToServer(const Util::String& str);

    Ptr<CoreNetwork::NetClient> client;
};

} // namespace Multiplayer
//------------------------------------------------------------------------------
#endif
    