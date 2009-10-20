#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreUI::UIElement
    
    This is the container for the message wich is sent from the client side
	UI::UIRenderer to the CoreUI::UIGraphicsHandler and so on
    
    (C) 2008 Radon Labs GmbH
*/
#include "math/rectangle.h"
#include "util/string.h"
#include "threading/thread.h"
#include "timing/time.h"

//------------------------------------------------------------------------------
namespace CoreUI
{
class UIElement
{
public:
	/// constructor
	UIElement();
	/// color|rect constructor
	UIElement(IndexT eId, const Math::float4& c, const Math::rectangle<float>& r, const Math::float4& u, int& depth, Threading::ThreadId id, Util::String tex);

	/// deconstructor
	virtual ~UIElement();
    
    /// compare operator
    bool operator == (const UIElement& other) const;

	/// set the color
	void SetColor(const Math::float4& c);
	/// get the color
	const Math::float4& GetColor() const;
	
	/// set the uv coordinates
	void SetUV(const Math::float4& uv);
	/// get the uv coordinates
	const Math::float4& GetUV() const;

	/// set the dimension
	void SetDimension(const Math::rectangle<float>& r);
	/// get the dimension
	const Math::rectangle<float>& GetDimension() const;

    /// set the dimension
	void SetGraphicsDimension(const Math::rectangle<float>& r);
	/// get the dimension
	const Math::rectangle<float>& GetGraphicsDimension() const;
	
	/// get thread id
    Threading::ThreadId GetThreadId() const;
	/// set the thread id
	void SetThreadId(Threading::ThreadId id);

	/// set the depth 
	void SetDepth(const int& d);
	/// get the depth
	const int& GetDepth() const;

    /// get id
    IndexT GetElementId() const;

    /// get texture path
    const Util::String& GetTexturePath() const;

    /// get BlinkTime	
    Timing::Time GetBlinkTime() const;
    /// set BlinkTime
    void SetBlinkTime(Timing::Time val);
    /// get BlinkColor	
    const Math::float4& GetBlinkColor() const;
    /// set BlinkColor
    void SetBlinkColor(const Math::float4& val);
    /// get Blinking	
    bool GetBlinking() const;
    /// set Blinking
    void SetBlinking(bool val);



    /// get fadein time	
    Timing::Time GetFadeInTime() const;
    /// set fadein time (ALSO SET THE CURRENT VALUES, READY TO FADE WITH BOOL FLAG AND FRAMETIME)
    void SetFadeInTime(Timing::Time val);
    /// get time of creation
    Timing::Time GetCurrentFadeInTime() const;
    /// set time of creation
    void SetCurrentFadeInTime(Timing::Time val);
    /// get fading	
    bool IsFadingIn() const;
    /// set fading
    void SetFadingIn(bool val);


    /// get fadeout time	
    Timing::Time GetFadeOutTime() const;
    /// set fadeout time (ALSO SET THE CURRENT VALUES, READY TO FADE WITH BOOL FLAG AND FRAMETIME)
    void SetFadeOutTime(Timing::Time val);  
    /// get time of hiding
    Timing::Time GetCurrentFadeOutTime() const;
    /// set time of hiing
    void SetCurrentFadeOutTime(Timing::Time val);
    /// get fading	
    bool IsFadingOut() const;
    /// set fading
    void SetFadingOut(bool val);

private:
	Threading::ThreadId threadId;
	Math::float4 color;	
	Math::float4 uv;
	Math::rectangle<float> dimension;
    Math::rectangle<float> graphicsDimension;
	int depth;
    IndexT elementId;
    Util::String texture;
    Timing::Time blinkTime;
    Math::float4 blinkColor;
    bool blinking;

    Timing::Time fadeInTime;
    Timing::Time currentFadeInTime;
    bool fadingIn;
    
    Timing::Time fadeOutTime;
    Timing::Time currentFadeOutTime;
    bool fadingOut;
};

//------------------------------------------------------------------------------
/**
*/
inline
bool
UIElement::operator == (const UIElement& other) const
{
    // FIMXE: reduce to elementID and creation time?
    if( this->color == other.color &&
        this->depth == other.depth &&
        this->dimension.bottom == other.dimension.bottom &&
        this->dimension.top == other.dimension.top &&
        this->dimension.left == other.dimension.left &&
        this->dimension.right == other.dimension.right &&
        this->elementId == other.elementId &&
        this->graphicsDimension.bottom == other.graphicsDimension.bottom &&
        this->graphicsDimension.top == other.graphicsDimension.top &&
        this->graphicsDimension.left == other.graphicsDimension.left &&
        this->graphicsDimension.right == other.graphicsDimension.right &&
        this->texture == other.texture &&
        this->threadId == other.threadId &&
        this->uv == other.uv)
    {
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
inline 
const int& 
UIElement::GetDepth() const
{
    return this->depth;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
UIElement::SetDepth(const int& d)
{
	this->depth = d;
}

//------------------------------------------------------------------------------
/**
*/
inline 
Threading::ThreadId
UIElement::GetThreadId() const
{
    return this->threadId;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
UIElement::SetThreadId(Threading::ThreadId id)
{
	this->threadId = id;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
UIElement::SetUV(const Math::float4& u)
{
	this->uv = u;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::float4&
UIElement::GetUV() const
{
	return this->uv;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
UIElement::SetColor(const Math::float4& c)
{
	this->color = c;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::float4&
UIElement::GetColor() const
{
	return this->color;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
UIElement::SetDimension(const Math::rectangle<float>& r)
{
	this->dimension = r;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::rectangle<float>&
UIElement::GetDimension() const
{
	return this->dimension;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT 
UIElement::GetElementId() const
{
    return this->elementId;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
UIElement::SetGraphicsDimension(const Math::rectangle<float>& r)
{
	this->graphicsDimension = r;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::rectangle<float>&
UIElement::GetGraphicsDimension() const
{
	return this->graphicsDimension;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::String&
UIElement::GetTexturePath() const
{
	return this->texture;
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Time
UIElement::GetBlinkTime() const
{
    return this->blinkTime;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
UIElement::SetBlinkTime(Timing::Time val)
{
    this->blinkTime = val;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::float4& 
UIElement::GetBlinkColor() const
{
    return this->blinkColor;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
UIElement::SetBlinkColor(const Math::float4& val)
{
    this->blinkColor = val;
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
UIElement::GetBlinking() const
{
    return this->blinking;
}

//------------------------------------------------------------------------------
/**
*/
inline
void UIElement::SetBlinking(bool val)
{
    this->blinking = val;
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Time 
UIElement::GetFadeInTime() const
{
    return this->fadeInTime;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
UIElement::SetFadeInTime(Timing::Time val)
{
    this->fadeInTime = val;
    this->currentFadeInTime = 0;
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Time 
UIElement::GetFadeOutTime() const
{
    return this->fadeOutTime;
}

//------------------------------------------------------------------------------
/**
*/
inline
void UIElement::SetFadeOutTime(Timing::Time val)
{
    this->fadeOutTime = val;
    this->currentFadeOutTime = val;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool UIElement::IsFadingIn() const
{
    return this->fadingIn;
}

//------------------------------------------------------------------------------
/**
*/
inline
void UIElement::SetFadingIn(bool val)
{
    this->fadingIn = val;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool UIElement::IsFadingOut() const
{
    return this->fadingOut;
}

//------------------------------------------------------------------------------
/**
*/
inline
void UIElement::SetFadingOut(bool val)
{
    this->fadingOut = val;
}
//------------------------------------------------------------------------------
/**
*/
inline Timing::Time 
UIElement::GetCurrentFadeOutTime() const
{
    return this->currentFadeOutTime;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
UIElement::SetCurrentFadeOutTime(Timing::Time val)
{
    this->currentFadeOutTime = val;
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Time 
UIElement::GetCurrentFadeInTime() const
{
    return this->currentFadeInTime;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
UIElement::SetCurrentFadeInTime(Timing::Time val)
{
    this->currentFadeInTime = val;
}
} // namespace CoreUI
//------------------------------------------------------------------------------
    