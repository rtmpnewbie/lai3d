#pragma once
#ifndef COREUI_UIGRAPHICSHANDLER_H
#define COREUI_UIGRAPHICSHANDLER_H
//------------------------------------------------------------------------------
/**
    @class CoreUI::UIGraphicsHandler
    
    Handles visualization of the UI elements.
    
    (C) 2008 Radon Labs GmbH
*/
#include "interface/interfacehandlerbase.h"
#include "messaging/message.h"
#include "math/rectangle.h"

//------------------------------------------------------------------------------
namespace CoreUI
{
class RenderUIElements;
class FlushAllUIElements;
class FlushUIElement;
class LoadUITexture;
class UpdateUIElement;

class UIGraphicsHandler : public Interface::InterfaceHandlerBase
{
    __DeclareClass(UIGraphicsHandler);

public:
    /// constructor
    UIGraphicsHandler();
    /// destructor
    virtual ~UIGraphicsHandler();    
    /// handle a message, return true if handled
    virtual bool HandleMessage(const Ptr<Messaging::Message>& msg);

private:
    /// handle RenderDebugText message
    void OnRenderUIElement(const Ptr<RenderUIElements>& msg);
    /// handle FlushDebugText message
    void OnFlushAllUIElements(const Ptr<FlushAllUIElements>& msg);
    /// handle flush element message
    void OnFlushUIElement(const Ptr<FlushUIElement>& msg);
    /// handle SetTexture message
    void OnLoadUITexture(const Ptr<LoadUITexture>& msg);
    /// handle updateuielement message
    void OnUpdateUIElement(const Ptr<UpdateUIElement>& msg);
};

} // namespace Debug
//------------------------------------------------------------------------------
#endif
