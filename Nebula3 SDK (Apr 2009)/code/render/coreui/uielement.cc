//------------------------------------------------------------------------------
//  uirenderer.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coreui/uielement.h"

namespace CoreUI
{

//------------------------------------------------------------------------------
/**
*/
UIElement::UIElement() : elementId(0), 
                         color(1.0f, 1.0f, 1.0f, 1.0f), 
						 uv(0.0f, 0.0f, 1.0f, 1.0f), 
						 dimension(10, 10, 20, 20),
						 threadId(Threading::InvalidThreadId),
						 depth(0),
                         blinkTime(0),
                         currentFadeInTime(0),
                         currentFadeOutTime(0),
                         blinkColor(1, 1, 0, 1), 
                         blinking(false),
                         fadingIn(false),
                         fadingOut(false),
                         fadeInTime(0.0),
                         fadeOutTime(0.0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
UIElement::~UIElement()
{	
	// empty
}

//------------------------------------------------------------------------------
/**
*/
UIElement::UIElement(IndexT eId, const Math::float4& c, const Math::rectangle<float>& r, const Math::float4& u, int& depth, Threading::ThreadId id, Util::String tex)
                        :	elementId(eId),
                            color(c),
                            dimension(r),
                            threadId(id),
                            depth(depth),
                            uv(u),
                            texture(tex),
                            blinking(false),
                            fadingIn(false),
                            fadingOut(false),
                            fadeInTime(0.0),
                            fadeOutTime(0.0),
                            currentFadeInTime(0),
                            currentFadeOutTime(0)
{
    graphicsDimension.left = r.left * 2 - 1;
    graphicsDimension.top = (r.top * 2 - 1) * -1;
    graphicsDimension.bottom = (r.bottom * 2 - 1) * -1;
    graphicsDimension.right = r.right * 2 - 1;
}

} // namespace Debug
