#pragma once
#ifndef DEBUGRENDER_DEBUGGRAPHICSHANDLER_H
#define DEBUGRENDER_DEBUGGRAPHICSHANDLER_H
//------------------------------------------------------------------------------
/**
    @class Debug::DebugGraphicsHandler
    
    Handles debug-visualization messages in the graphics thread.
    
    (C) 2008 Radon Labs GmbH
*/
#include "interface/interfacehandlerbase.h"
#include "messaging/message.h"
#include "debugrender/debugrenderprotocol.h"

//------------------------------------------------------------------------------
namespace Debug
{
class DebugGraphicsHandler : public Interface::InterfaceHandlerBase
{
    __DeclareClass(DebugGraphicsHandler);
public:
    /// constructor
    DebugGraphicsHandler();
    /// destructor
    virtual ~DebugGraphicsHandler();    
    /// handle a message, return true if handled
    virtual bool HandleMessage(const Ptr<Messaging::Message>& msg);

private:
    /// handle RenderDebugText message
    void OnRenderDebugText(const Ptr<RenderDebugText>& msg);
    /// handle RenderDebugShapes message
    void OnRenderDebugShapes(const Ptr<RenderDebugShapes>& msg);
    /// handle render debug message
    void OnRenderDebugView(const Ptr<Debug::RenderDebugView>& msg);
    /// handle increment layer index of debug shader
    void OnIncrDebugShaderLayer(const Ptr<Debug::IncrDebugShaderLayer>& msg);

    int shaderDebugLayer;
};

} // namespace Debug
//------------------------------------------------------------------------------
#endif
