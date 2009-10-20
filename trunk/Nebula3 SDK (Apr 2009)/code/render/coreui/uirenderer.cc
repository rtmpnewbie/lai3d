//------------------------------------------------------------------------------
//  uirenderer.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coreui/uirenderer.h"
#include "coreui/uirenderprotocol.h"
#include "graphics/graphicsinterface.h"
#include "threading/thread.h"

namespace UI
{
__ImplementClass(UIRenderer, 'UIRR', Core::RefCounted);
__ImplementSingleton(UIRenderer);

using namespace Util;
using namespace Math;
using namespace Threading;

//------------------------------------------------------------------------------
/**
*/
UIRenderer::UIRenderer()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
UIRenderer::~UIRenderer()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
UIRenderer::DrawElement(IndexT id, 
                        const Math::rectangle<float>& rect, 
                        const Math::float4& uv, 
                        int& depth, 
                        const Math::float4& color, 
                        Util::String texture, 
                        bool blinking,
                        Timing::Time blinkTime,
                        const Math::float4& blinkColor,
                        Timing::Time fadeInTime,
                        Timing::Time fadeOutTime,
                        bool dirty)
{
    // create new
	CoreUI::UIElement el(id, color, rect, uv, depth, Thread::GetMyThreadId(), texture);
    el.SetBlinking(blinking);
    el.SetBlinkColor(blinkColor);
    el.SetBlinkTime(blinkTime);      
    el.SetFadeInTime(fadeInTime);
    el.SetFadingIn(fadeInTime == 0 ? false : true);
    el.SetFadeOutTime(fadeOutTime);
    el.SetFadingOut(false);

    // check if element was dirty and should be rendered new, but skip fading in
    if (dirty)
    {
        if(this->elements.Contains(id))
        {
            // clear fadein
            el.SetFadingIn(false);
            el.SetFadeInTime(0);

            this->updatedElements.Add(id, el);
            this->elements.Erase(id);
        }
    }

    // add
    if (!this->elements.Contains(id))
    {
	    this->elements.Add(id, el);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
UIRenderer::DrawDebugView( IndexT id,
                          const Math::rectangle<float>& rect, 
                          int& depth, 
                          const Math::float4& color)
{
    // create new
    CoreUI::UIElement el(id, color, rect, Math::float4(0,0,0,0), depth, Thread::GetMyThreadId(), "");

    // add
    this->debugShapes.Append(el);
}

//------------------------------------------------------------------------------
/**
*/
void
UIRenderer::OnFrame()
{
    // flush removed elements
    if (this->updatedElements.Size() > 0)
    {
        Ptr<CoreUI::UpdateUIElement> msg = CoreUI::UpdateUIElement::Create();
        msg->SetThreadId(Thread::GetMyThreadId());
        msg->SetUIElements(this->updatedElements.ValuesAsArray());
        Graphics::GraphicsInterface::Instance()->Send(msg.cast<Messaging::Message>());	        	
        this->updatedElements.Clear();
    }
    // send a RenderUIElements message to the render thread, send this
    // every frame, whether there are any ui elements or not as this will
    // also cleanup the ui elements from the previous frame in the
    // render thread
	if(this->elements.Size() > 0)
	{        
        // afterwards also send the debug shapes for rendering and delete afterwards immediately
        if(this->debugShapes.Size() > 0)
        {
            Util::Array<CoreUI::UIElement> all = this->debugShapes;
            all.AppendArray(this->elements.ValuesAsArray());
                             
            Ptr<CoreUI::RenderUIElements> msg = CoreUI::RenderUIElements::Create();
            msg->SetThreadId(Thread::GetMyThreadId());
            msg->SetUIElements(all);
            Graphics::GraphicsInterface::Instance()->Send(msg.cast<Messaging::Message>());
            this->debugShapes.Clear();
        }
        else
        {
            Ptr<CoreUI::RenderUIElements> msg = CoreUI::RenderUIElements::Create();
		    msg->SetThreadId(Thread::GetMyThreadId());
            msg->SetUIElements(this->elements.ValuesAsArray());
		    Graphics::GraphicsInterface::Instance()->Send(msg.cast<Messaging::Message>());		    
        }
	}  
}

//------------------------------------------------------------------------------
/**
*/
void
UIRenderer::FlushAllElements()
{
	Ptr<CoreUI::FlushAllUIElements> msg = CoreUI::FlushAllUIElements::Create();
	msg->SetThreadId(Thread::GetMyThreadId());
	Graphics::GraphicsInterface::Instance()->Send(msg.cast<Messaging::Message>());	
	// clear all pending elements	
    this->elements.Clear();
    this->updatedElements.Clear();
}

//------------------------------------------------------------------------------
/**
*/
void
UIRenderer::FlushElement(IndexT id)
{     
    if (this->elements.Size() > 0)
    {
	    Ptr<CoreUI::FlushUIElement> msg = CoreUI::FlushUIElement::Create();
	    msg->SetThreadId(Thread::GetMyThreadId());
        msg->SetUIElement(this->elements[id]);
	    Graphics::GraphicsInterface::Instance()->Send(msg.cast<Messaging::Message>());	
        this->elements.Erase(id);
    }
}

} // namespace Debug
