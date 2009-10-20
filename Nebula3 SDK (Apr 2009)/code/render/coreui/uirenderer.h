#pragma once
//------------------------------------------------------------------------------
/**
    @class UI::UIRenderer
    
    Client-side proxy for rendering the user interface. Packs ui render requests
    into a message which is sent to the render thread once per frame.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "math/rectangle.h"
#include "coreui/uielement.h"

//------------------------------------------------------------------------------
namespace UI
{
class UIRenderer : public Core::RefCounted
{
    __DeclareClass(UIRenderer);
    __DeclareSingleton(UIRenderer);

public:
	/// constructor
    UIRenderer();
    /// destructor
    virtual ~UIRenderer();
    /// draw a simple element
    void DrawElement(   IndexT id, 
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
                        bool dirty = false);

    /// draw a debug element
    void DrawDebugView( IndexT id,
                        const Math::rectangle<float>& rect, 
                        int& depth, 
                        const Math::float4& color);

    /// call once per frame to send of accumulated draw commands
    void OnFrame();

    /// Flush all elements
    void FlushAllElements();
    /// flush one element
    void FlushElement(IndexT id);

private:
    Util::Array<CoreUI::UIElement> debugShapes;
	Util::Dictionary<IndexT, CoreUI::UIElement> elements;    
    Util::Dictionary<IndexT, CoreUI::UIElement> updatedElements;
}; 

} // namespace UI
//------------------------------------------------------------------------------