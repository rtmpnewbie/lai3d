#pragma once
#ifndef REMOTE_REMOTEMESSAGEHANDLER_H
#define REMOTE_REMOTEMESSAGEHANDLER_H
//------------------------------------------------------------------------------
/**
    @class Remote::RemoteMessageHandler
    
    Runs the RemoteServer thread, and owns the central remote server. Processes
    messages sent to the RemoteInterface from other threads.

    (C) 2008 Radon Labs GmbH
*/
#include "interface/interfacehandlerbase.h"
#include "messaging/message.h"
#include "io/console.h"
#include "remote/remotecontrol.h"
#include "remote/remoteprotocol.h"

//------------------------------------------------------------------------------
namespace Remote
{
class RemoteMessageHandler : public Interface::InterfaceHandlerBase
{
    __DeclareClass(RemoteMessageHandler);
public:
    /// constructor
    RemoteMessageHandler();
    /// destructor
    virtual ~RemoteMessageHandler();

    /// open the handler
    virtual void Open();
    /// close the handler
    virtual void Close();
    /// handle a message, return true if handled
    virtual bool HandleMessage(const Ptr<Messaging::Message>& msg);
    /// do per-frame work
    virtual void DoWork();

private:
    /// handle AttachRequestHandler message
    void OnAttachRequestHandler(const Ptr<AttachRequestHandler>& msg);
    /// handle RemoveRequestHandler message
    void OnRemoveRequestHandler(const Ptr<RemoveRequestHandler>& msg);

    Ptr<RemoteControl> remoteControl;
};

} // namespace Remote
//------------------------------------------------------------------------------
#endif
