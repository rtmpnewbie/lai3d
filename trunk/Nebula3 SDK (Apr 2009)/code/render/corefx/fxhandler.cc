//------------------------------------------------------------------------------
//  uigraphicshandler.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "corefx/fxhandler.h"
#include "corefx/corefxprotocol.h"
#include "corefx/fxserver.h"

namespace CoreFX
{
__ImplementClass(FxHandler, 'CFXH', Interface::InterfaceHandlerBase);

//------------------------------------------------------------------------------
/**
*/
FxHandler::FxHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
FxHandler::~FxHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
FxHandler::HandleMessage(const Ptr<Messaging::Message>& msg)
{
    n_assert(msg.isvalid());
    if (msg->CheckId(AttachEffectMixer::Id))
    {
        this->OnAttachEffectMixer(msg.cast<AttachEffectMixer>());
    }
    else if (msg->CheckId(AttachEffect::Id))
    {
        this->OnAttachEffect(msg.cast<AttachEffect>());
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
FxHandler::OnAttachEffect(const Ptr<CoreFX::AttachEffect> &msg)
{
    FxServer::Instance()->AttachEffect(msg->GetEffect());
}

//------------------------------------------------------------------------------
/**
*/
void
FxHandler::OnAttachEffectMixer(const Ptr<CoreFX::AttachEffectMixer>& msg)
{
    FxServer::Instance()->AttachEffectMixer(msg->GetEffectMixer());
}
} // namespace Debug
