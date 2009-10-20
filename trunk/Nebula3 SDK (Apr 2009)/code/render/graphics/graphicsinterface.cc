//------------------------------------------------------------------------------
//  graphicsinterface.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/graphicsinterface.h"
#include "graphics/graphicshandler.h"
#include "debugrender/debuggraphicshandler.h"

#if !__WII__
#include "coreui/uigraphicshandler.h"
#include "posteffect/posteffecthandler.h"
#include "corefx/fxhandler.h"
#include "video/videohandler.h"
#endif

namespace Graphics
{
__ImplementClass(Graphics::GraphicsInterface, 'GRIF', Interface::InterfaceBase);
__ImplementInterfaceSingleton(Graphics::GraphicsInterface);

using namespace Threading;
using namespace Interface;
using namespace Messaging;

//------------------------------------------------------------------------------
/**
*/
GraphicsInterface::GraphicsInterface()
{
    __ConstructSingleton;
    this->SetThreadCpuCoreId(System::Cpu::RenderThreadCore);
    this->SetThreadStackSize(65536);
    this->mainThreadId = Thread::GetMyThreadId();
}

//------------------------------------------------------------------------------
/**
*/
GraphicsInterface::~GraphicsInterface()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsInterface::Open()
{
    // we need to run continously, not wait for messages
    this->SetBehaviour(DoNotWait);
    this->SetName("GraphicsInterface Thread");
    this->batchMessage = BatchMessage::Create();
    InterfaceBase::Open();
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsInterface::Close()
{
    InterfaceBase::Close();
    this->batchMessage = 0;
    this->graphicsHandler = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsInterface::OnCreateHandlers()
{
    this->graphicsHandler = GraphicsHandler::Create();
    this->AttachHandler(this->graphicsHandler.upcast<Messaging::Handler>());
    
	Ptr<Debug::DebugGraphicsHandler> debugGraphicsHandler = Debug::DebugGraphicsHandler::Create();
    this->AttachHandler(debugGraphicsHandler.upcast<Messaging::Handler>());

    #if !__WII__
	Ptr<CoreUI::UIGraphicsHandler> uiGraphicsHandler = CoreUI::UIGraphicsHandler::Create();
	this->AttachHandler(uiGraphicsHandler.upcast<Messaging::Handler>());
    
	Ptr<PostEffect::PostEffectHandler> postEffectHandler = PostEffect::PostEffectHandler::Create();
    this->AttachHandler(postEffectHandler.upcast<Messaging::Handler>());

    Ptr<CoreFX::FxHandler> fxHandler = CoreFX::FxHandler::Create();
    this->AttachHandler(fxHandler.upcast<Messaging::Handler>());

    Ptr<Video::VideoHandler> videoHandler = Video::VideoHandler::Create();
    this->AttachHandler(videoHandler.upcast<Messaging::Handler>());
    #endif
}

//------------------------------------------------------------------------------
/**
    This method is called by the GraphicsHandler once per frame
    right after pending messages are processed and before starting
    frame rendering. This can be used to synchronize other threads
    with rendering, for instance to prevent the game thread from running
    ahead of the graphics thread and thus spamming its message queue.
*/
void
GraphicsInterface::SignalFrameEvent()
{
    this->frameEvent.Signal();
}

//------------------------------------------------------------------------------
/**
    This method can be called by any thread who wishes to synchronize itself
    with the graphics thread. The event will become signalled once per
    graphics frame after messages are processed and before rendering starts.
    The best place for a thread to call this method is right before graphics
    messages for the next frame are produced.
*/
void
GraphicsInterface::WaitForFrameEvent()
{
    this->frameEvent.Wait();
}

//------------------------------------------------------------------------------
/**
    This method will wait until the graphics handler signals that all pending
    resources have been loaded.
*/
void
GraphicsInterface::WaitForPendingResources()
{
    this->graphicsHandler->WaitForPendingResources();
}    

//------------------------------------------------------------------------------
/**
    Send a batched message. The message will not be sent to the render thread
    immediately, but instead will be collected in a batch message for later
    sending. Use this method to reduce communication overhead
    between the main and render thread by sending only one batch message
    instead of many messages. This method may only be called from the 
    main thread!
*/
void
GraphicsInterface::SendBatched(const Ptr<Message>& msg)
{
    #if NEBULA3_DEBUG
    n_assert(Thread::GetMyThreadId() == this->mainThreadId);
    #endif
    this->batchMessage->AddMessage(msg);
}

//------------------------------------------------------------------------------
/**
    Send off the batched messages to the render thread. This reduces 
    thread synchronization overhead dramatically if many messages must
    be sent per-frame. Only the main thread may call this method.
*/
void
GraphicsInterface::FlushBatchedMessages()
{
    #if NEBULA3_DEBUG
    n_assert(Thread::GetMyThreadId() == this->mainThreadId);
    #endif

    // send current batch message
    this->Send(this->batchMessage.cast<Message>());

    // create a new, empty batch message
    this->batchMessage = BatchMessage::Create();
}

} // namespace Graphics
