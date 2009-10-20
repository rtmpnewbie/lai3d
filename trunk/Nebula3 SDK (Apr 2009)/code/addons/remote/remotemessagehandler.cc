//------------------------------------------------------------------------------
//  remotemessagehandler.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "remote/remotemessagehandler.h"

namespace Remote
{
__ImplementClass(Remote::RemoteMessageHandler, 'REMH', Interface::InterfaceHandlerBase);

using namespace Interface;
using namespace Messaging;

//------------------------------------------------------------------------------
/**
*/
RemoteMessageHandler::RemoteMessageHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
RemoteMessageHandler::~RemoteMessageHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
RemoteMessageHandler::Open()
{   
    n_assert(!this->IsOpen());
    InterfaceHandlerBase::Open();

    // setup core runtime and central remote server
    this->remoteControl = RemoteControl::Create();
    this->remoteControl->Open();
}

//------------------------------------------------------------------------------
/**
*/
void
RemoteMessageHandler::Close()
{
    n_assert(this->IsOpen());

    this->remoteControl->Close();
    this->remoteControl = 0;

    InterfaceHandlerBase::Close();
}

//------------------------------------------------------------------------------
/**
    Triggers the remote server from time to time.
*/
void
RemoteMessageHandler::DoWork()
{
    n_assert(this->IsOpen());

    // only process remote requests once in a while
    this->remoteControl->OnFrame();
    n_sleep(0.1f);
}

//------------------------------------------------------------------------------
/**
*/
bool
RemoteMessageHandler::HandleMessage(const Ptr<Message>& msg)
{
    n_assert(msg.isvalid());

    if (msg->CheckId(AttachRequestHandler::Id))
    {
        this->OnAttachRequestHandler(msg.cast<AttachRequestHandler>());
    }
    else if (msg->CheckId(RemoveRequestHandler::Id))
    {
        this->OnRemoveRequestHandler(msg.cast<RemoveRequestHandler>());
    }
    else
    {
        // unknown message
        return false;
    }
    // fallthrough: message was handled
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
RemoteMessageHandler::OnAttachRequestHandler(const Ptr<AttachRequestHandler>& msg)
{
    n_printf("RemoteMessageHandler::OnAttachRequestHandler() called!\n");
    this->remoteControl->AttachRequestHandler(msg->GetRequestHandler());
}

//------------------------------------------------------------------------------
/**
*/
void
RemoteMessageHandler::OnRemoveRequestHandler(const Ptr<RemoveRequestHandler>& msg)
{
    n_printf("RemoteMessageHandler::OnRemoveRequestHandler() called!\n");
    this->remoteControl->RemoveRequestHandler(msg->GetRequestHandler());
}

} // namespace Remote
