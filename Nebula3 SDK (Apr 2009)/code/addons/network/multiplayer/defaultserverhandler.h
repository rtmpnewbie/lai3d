#pragma once
#ifndef MULTIPLAYER_DEFAULTSERVERHANDLER_H
#define MULTIPLAYER_DEFAULTSERVERHANDLER_H
//------------------------------------------------------------------------------
/**
    @class Debug::DefaultServerHandler
        
    (C) 2008 Radon Labs GmbH
*/
#include "multiplayer/netmessagehandler.h"
#include "corenetwork/netserver.h"

//------------------------------------------------------------------------------
namespace Multiplayer
{

class DefaultServerHandler : public NetMessageHandler
{
    __DeclareClass(DefaultServerHandler);
public:
    /// constructor
    DefaultServerHandler();
    /// destructor
    virtual ~DefaultServerHandler();

protected:    
    friend class MultiplayerManager;
    /// open the handler
    virtual void Open();
    /// close the handler
    virtual void Close();
    /// do per-frame work
    virtual void DoWork();    
    /// process incoming streams
    virtual void ProcessIncomingStreams();
    /// process incoming strings
    Util::Array<Util::String> DefaultServerHandler::GetIncomingStrings() const;

private:
    Ptr<CoreNetwork::NetServer> server;
};

} // namespace Multiplayer
//------------------------------------------------------------------------------
#endif
    