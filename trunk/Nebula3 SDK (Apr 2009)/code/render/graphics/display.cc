//------------------------------------------------------------------------------
//  display.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/display.h"
#include "graphics/graphicsprotocol.h"
#include "graphics/graphicsinterface.h"

namespace Graphics
{
__ImplementClass(Graphics::Display, 'DISP', Core::RefCounted);
__ImplementSingleton(Graphics::Display);

using namespace Util;
using namespace CoreGraphics;
using namespace Messaging;

//------------------------------------------------------------------------------
/**
*/
Display::Display() :
    displayMode(0, 0, 1024, 768, PixelFormat::X8R8G8B8),
    antiAliasQuality(AntiAliasQuality::None),
    adapter(Adapter::Primary),
    fullscreen(false),
    modeSwitchEnabled(true),
    tripleBufferingEnabled(false),
    alwaysOnTop(false),
    verticalSync(false),
    isOpen(false),
    iconName("Icon")
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
Display::~Display()
{
    n_assert(!this->isOpen);
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
Display::Open()
{
    n_assert(!this->isOpen);

    Ptr<Graphics::SetupGraphics> msg = Graphics::SetupGraphics::Create();
    msg->SetAdapter(this->adapter);
    msg->SetDisplayMode(this->displayMode);
    msg->SetAntiAliasQuality(this->antiAliasQuality);
    msg->SetFullscreen(this->fullscreen);
    msg->SetDisplayModeSwitchEnabled(this->modeSwitchEnabled);
    msg->SetTripleBufferingEnabled(this->tripleBufferingEnabled);
    msg->SetAlwaysOnTop(this->alwaysOnTop);
    msg->SetVerticalSyncEnabled(this->verticalSync);
    msg->SetIconName(this->iconName);
    GraphicsInterface::Instance()->SendWait(msg.cast<Message>());
    n_assert(msg->Handled());
    
    // update our display mode with the actual display mode
    this->displayMode = msg->GetActualDisplayMode();
    
    this->isOpen = true;
}

//------------------------------------------------------------------------------
/**
*/
void
Display::Close()
{
    n_assert(this->isOpen);

    // FIXME: hmm, closing/reopening the display not currently supported
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
bool
Display::AdapterExists(CoreGraphics::Adapter::Code adapter)
{
    Ptr<Graphics::AdapterExists> msg = Graphics::AdapterExists::Create();
    msg->SetAdapter(adapter);
    GraphicsInterface::Instance()->SendWait(msg.cast<Message>());
    n_assert(msg->Handled());
    return msg->GetResult();
}

//------------------------------------------------------------------------------
/**
*/
Array<DisplayMode>
Display::GetAvailableDisplayModes(Adapter::Code adapter, PixelFormat::Code pixelFormat)
{
    Ptr<Graphics::GetAvailableDisplayModes> msg = Graphics::GetAvailableDisplayModes::Create();
    msg->SetAdapter(adapter);
    msg->SetPixelFormat(pixelFormat);
    GraphicsInterface::Instance()->SendWait(msg.cast<Message>());
    n_assert(msg->Handled());
    return msg->GetResult();
}

//------------------------------------------------------------------------------
/**
*/
bool
Display::SupportsDisplayMode(Adapter::Code adapter, const DisplayMode& requestedMode)
{
    Ptr<Graphics::SupportsDisplayMode> msg = Graphics::SupportsDisplayMode::Create();
    msg->SetAdapter(adapter);
    msg->SetDisplayMode(requestedMode);
    GraphicsInterface::Instance()->SendWait(msg.cast<Message>());
    n_assert(msg->Handled());
    return msg->GetResult();
}

//------------------------------------------------------------------------------
/**
*/
DisplayMode
Display::GetCurrentAdapterDisplayMode(Adapter::Code adapter)
{
    Ptr<Graphics::GetCurrentAdapterDisplayMode> msg = Graphics::GetCurrentAdapterDisplayMode::Create();
    msg->SetAdapter(adapter);
    GraphicsInterface::Instance()->SendWait(msg.cast<Message>());
    n_assert(msg->Handled());
    return msg->GetResult();
}

//------------------------------------------------------------------------------
/**
*/
AdapterInfo
Display::GetAdapterInfo(Adapter::Code adapter)
{
    Ptr<Graphics::GetAdapterInfo> msg = Graphics::GetAdapterInfo::Create();
    msg->SetAdapter(adapter);
    GraphicsInterface::Instance()->SendWait(msg.cast<Message>());
    n_assert(msg->Handled());
    return msg->GetResult();
}

//------------------------------------------------------------------------------
/**
*/
void
Display::AttachDisplayEventHandler(const Ptr<ThreadSafeDisplayEventHandler>& handler)
{
    n_assert(this->isOpen);
    Ptr<Graphics::AttachDisplayEventHandler> msg = Graphics::AttachDisplayEventHandler::Create();
    msg->SetHandler(handler);
    GraphicsInterface::Instance()->Send(msg.cast<Message>());
}

//------------------------------------------------------------------------------
/**
*/
void
Display::RemoveDisplayEventHandler(const Ptr<ThreadSafeDisplayEventHandler>& handler)
{
    n_assert(this->isOpen);
    Ptr<Graphics::RemoveDisplayEventHandler> msg = Graphics::RemoveDisplayEventHandler::Create();
    msg->SetHandler(handler);
    GraphicsInterface::Instance()->Send(msg.cast<Message>());
}

//------------------------------------------------------------------------------
/**
*/
void
Display::AttachRenderEventHandler(const Ptr<ThreadSafeRenderEventHandler>& handler)
{
    n_assert(this->isOpen);
    Ptr<Graphics::AttachRenderEventHandler> msg = Graphics::AttachRenderEventHandler::Create();
    msg->SetHandler(handler);
    GraphicsInterface::Instance()->Send(msg.cast<Message>());
}

//------------------------------------------------------------------------------
/**
*/
void
Display::RemoveRenderEventHandler(const Ptr<ThreadSafeRenderEventHandler>& handler)
{
    n_assert(this->isOpen);
    Ptr<Graphics::RemoveRenderEventHandler> msg = Graphics::RemoveRenderEventHandler::Create();
    msg->SetHandler(handler);
    GraphicsInterface::Instance()->Send(msg.cast<Message>());
}

} // namespace Graphics
