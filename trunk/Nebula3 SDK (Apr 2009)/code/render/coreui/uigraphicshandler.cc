//------------------------------------------------------------------------------
//  uigraphicshandler.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coreui/uigraphicshandler.h"
#include "coreui/uirenderprotocol.h"
#include "internalgraphics/internalgraphicsserver.h"
#include "coreui/coreuirenderer.h"

namespace CoreUI
{
__ImplementClass(UIGraphicsHandler, 'UIGH', Interface::InterfaceHandlerBase);

using namespace IO;
using namespace Util;
using namespace Messaging;
using namespace CoreGraphics;
using namespace InternalGraphics;

//------------------------------------------------------------------------------
/**
*/
UIGraphicsHandler::UIGraphicsHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
UIGraphicsHandler::~UIGraphicsHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
UIGraphicsHandler::HandleMessage(const Ptr<Messaging::Message>& msg)
{
    n_assert(msg.isvalid());
    if (msg->CheckId(RenderUIElements::Id))
    {
        this->OnRenderUIElement(msg.cast<RenderUIElements>());
    }
    else if (msg->CheckId(FlushAllUIElements::Id))
    {
        this->OnFlushAllUIElements(msg.cast<FlushAllUIElements>());
    }
    else if (msg->CheckId(FlushUIElement::Id))
    {
        this->OnFlushUIElement(msg.cast<FlushUIElement>());
    }
    else if(msg->CheckId(LoadUITexture::Id))
    {
        this->OnLoadUITexture(msg.cast<LoadUITexture>());
    }
    else if(msg->CheckId(UpdateUIElement::Id))
    {
        this->OnUpdateUIElement(msg.cast<UpdateUIElement>());
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
UIGraphicsHandler::OnRenderUIElement(const Ptr<RenderUIElements>& msg)
{	
	CoreUI::CoreUIRenderer::Instance()->AddUIElements(msg->GetUIElements());
}

//------------------------------------------------------------------------------
/**
*/
void
UIGraphicsHandler::OnFlushAllUIElements(const Ptr<FlushAllUIElements>& msg)
{	
	CoreUI::CoreUIRenderer::Instance()->DeleteUIElementsByThreadId(msg->GetThreadId());
}

//------------------------------------------------------------------------------
/**
*/
void
UIGraphicsHandler::OnFlushUIElement(const Ptr<FlushUIElement>& msg)
{
    CoreUI::CoreUIRenderer::Instance()->RemoveUIElement(msg->GetUIElement());
}

//------------------------------------------------------------------------------
/**
*/
void 
UIGraphicsHandler::OnUpdateUIElement(const Ptr<UpdateUIElement>& msg)
{
    CoreUI::CoreUIRenderer::Instance()->UpdateUIElements(msg->GetUIElements());
}

//------------------------------------------------------------------------------
/**
*/
void
UIGraphicsHandler::OnLoadUITexture(const Ptr<LoadUITexture>& msg)
{
    CoreUI::CoreUIRenderer::Instance()->LoadUITexture(msg->GetResourceName());
}

} // namespace Debug
