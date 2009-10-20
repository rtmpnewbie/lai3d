#ifndef UI_ELEMENT_H
#define UI_ELEMENT_H
//------------------------------------------------------------------------------
/**
    @class UI::Element

    The base class for all 2D user interface elements describes a rectangle
    and implements a hierarchy. The position describes the center of the window.
	Sizes are set around this center. All screen values are in the (0|1) space.

						|------------------------------------|--- top
						|				  .					 |
						|				  .					 |
						|.................x..................|...... centerY
						|				  .					 |
						|				  .					 |					
						|------------------------------------|--- bottom
						|				  .					 |
					   left				  .					right				
										centerX

    
	Until you call the InitAbsolutePositions() all positions of each element 
	are relative position to the parent element.

	The Method InitAbsolutePositions() convert them to absolute position.

	If the parent element is invalid, it seems to be the canvas.
	
	If the parent window is not screen aligned, the canvas position is the absolute
	position of the window.

	

    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "math/rectangle.h"
#include "input/key.h"
#include "input/gamepad.h"
#include "math/float2.h"
#include "math/rectangle.h"
#include "graphics/display.h"

//------------------------------------------------------------------------------
namespace UI
{

class Window;

class Element : public Core::RefCounted
{
    __DeclareClass(Element);
public:
    enum NeighbourDirection
    {
        Top = 0,
        Bottom,
        Left,
        Right,

        NumDirections
    };
    /// constructor
    Element();
    /// destructor
    virtual ~Element();
    
    /// set id
    void SetId(const Util::String& i);
    /// get id
    const Util::String& GetId() const;

    /// get element id
    IndexT GetElementId() const;

    /// set visible flag of the element
    virtual void SetVisible(bool b);
    /// get visible flag of the element
    bool IsVisible() const;   	

	/// set relativ position
	void SetPixelPosition(const Math::float2& relPos);
    /// returns the screenspace position of an element
    const Math::float2 GetPixelPosition() const;
	/// set relativ position
	void SetScreenPosition(const Math::float2& relPos);
    /// returns the screenspace position of an element
    const Math::float2 GetScreenPosition() const;

	/// set relativ position
	void SetPixelSize(const Math::float2& size);
    /// returns the screenspace position of an element
    const Math::float2 GetPixelSize() const;
	/// set relativ position
	void SetScreenSize(const Math::float2& size);
    /// returns the screenspace position of an element
    const Math::float2 GetScreenSize() const;

	/// set the default color
	void SetDefaultColor(const Math::float4& color);
	/// get the default color
	const Math::float4& GetDefaultColor() const;

	/// set the default uv coord
	void SetDefaultUV(const Math::float4& uv);
	/// get the default uv coord
	const Math::float4& GetDefaultUV() const;

	/// called when gui hierarchy is created
	virtual void OnCreate();
    /// called when gui hierarchy is destroyed
    virtual void OnDestroy();
	/// return true if element is valid (between OnCreate() and OnDestroy())
    bool IsValid() const;

    /// attach child element
    void AttachElement(const Ptr<Element>& elm);
    /// remove child element
    void RemoveElement(const Ptr<Element>& elm);
    /// erase the childelements array
    void EraseElement(const Ptr<Element>& elm);

    /// get the number of childelements
    int GetNumChildElements() const;
	/// access to child elements
	const Util::Array<Ptr<Element>>& GetChildElements() const;
    /// acccess to a single child element
    const Ptr<Element>& GetChildElementAt(int index) const;
    /// find a direct child element by name
    Ptr<Element> FindChildElement(const Util::String& name) const;

    /// get pointer to parent element (can be 0)
    const Ptr<Element>& GetParentElement() const;    
    /// get window that contains this element
    const Ptr<Window>& GetParentWindow() const;
    
    /// called once per frame after input has been delivered
    virtual void OnFrame();
    /// called before the gui hierarchy is rendered
    virtual void OnRender();
	/// render debug yourself (just the shape)
	virtual void OnRenderDebug();

	/// return true if mouse is within element area
    virtual bool Inside(const Math::float2& mousePos);
	/// returns all child elements at the position
	virtual void GetElementsAt(const Math::float2& mousePos, Util::Array<Ptr<Element>>& elements_out);

    /// enable/disable the element (won't call OnX() methods if disabled)
    void SetEnabled(bool b);
    /// get enabled state of element
    const bool IsEnabled() const;

    /// set the mouseover state
    void SetMouseOver(bool b);
    /// get mouse over
    bool IsMouseOver() const;

    /// set the mouseover state
    void SetIgnoreInput(bool b);
    /// get mouse over
    bool GetIgnoreInput() const;

    /// called when mouse is moved
    virtual void OnMouseMove(const Math::float2& mousePos);
    /// called when mousewheel is moved (false = down, true = up)
    virtual void OnMouseWheelMoved(const bool& up);
    
	/// called when left mouse button is pressed over element
    virtual void OnLeftButtonDown(const Math::float2& mousePos);
    /// called when left mouse button is held pressed
    virtual void OnLeftButtonPressed(const Math::float2& mousePos);
    /// called when left mouse button is released over element
    virtual void OnLeftButtonUp(const Math::float2& mousePos);
    /// called when left mouse button is double clicked
    virtual void OnLeftButtonDoubleClicked(const Math::float2& mousePos);
	
	/// called when left mouse button is pressed over element
    virtual void OnMiddleButtonDown(const Math::float2& mousePos);
    /// called when left mouse button is held pressed
    virtual void OnMiddleButtonPressed(const Math::float2& mousePos);
    /// called when left mouse button is released over element
    virtual void OnMiddleButtonUp(const Math::float2& mousePos);
    /// called when left mouse button is double clicked
    virtual void OnMiddleButtonDoubleClicked(const Math::float2& mousePos);

    /// called when right mouse button is pressed over element
    virtual void OnRightButtonDown(const Math::float2& mousePos);
    /// called when right mouse button is held pressed
    virtual void OnRightButtonPressed(const Math::float2& mousePos);
    /// called when right mouse button is released over element
    virtual void OnRightButtonUp(const Math::float2& mousePos);
    /// called when right mouse button is double clicked
    virtual void OnRightButtonDoubleClicked(const Math::float2& mousePos);

    /// called when a character is input
    virtual void OnChar(const uchar& code);
    /// called when a key is pressed
    virtual void OnKeyDown(Input::Key::Code key);
    /// called when a key is released
    virtual void OnKeyUp(Input::Key::Code key);

    /// called when a gamepad button was pressed
    virtual void OnGamePadButtonDown(Input::GamePad::Button btn);
    /// called when a gamepad button was released
    virtual void OnGamePadButtonUp(Input::GamePad::Button btn);

    /// called when input focus is gained
    virtual void OnFocusGain();
    /// called when input focus is lost
    virtual void OnFocusLost();    

    /// register control as input focus element
    void RegisterAsFocusElement();
    /// unregister control as input focus element
    void UnregisterAsFocusElement();
    /// return true if this is currently the focus element
    bool IsFocusElement() const;

	/// moves this element and all childs
    void Move(const Math::float2& vec);

    /// get BlinkingEnabled	
    bool GetBlinkingEnabled() const;
    /// set BlinkingEnabled
    void SetBlinkingEnabled(bool val);
    /// get BlinkTime	
    float GetBlinkTime() const;
    /// set BlinkTime
    void SetBlinkTime(float val);
    /// get BlinkColor	
    const Math::float4& GetBlinkColor() const;
    /// set BlinkColor
    void SetBlinkColor(const Math::float4& val);

    /// set fadein time
    void SetFadeInTime(Timing::Time time);
    /// get fadein time
    Timing::Time GetFadeInTime() const;
    /// set fadeout time
    void SetFadeOutTime(Timing::Time time);
    /// get fadeout time
    Timing::Time GetFadeOutTime() const;

    /// link neighbours
    void LinkNeighbours();
    /// get neighbor by direction (can return invalid ptr)
    const Ptr<Element>& GetNeighbour(NeighbourDirection dir) const;

protected:
	friend class Window;
	friend class UIXmlParser;

    /// appends an exiting child element to the childs array
    void AppendChildElement(const Ptr<Element>& elem);

	/// set parent element
	void SetParentElement(const Ptr<Element>& parentElement);
	/// set parent window
	void SetParentWindow(const Ptr<Window>& parentWindow);

	/// update all screen rect's of all child elements (recursivly)
	/// ATTENTION, call this just ONCE after creating! afterwards, 
	/// just move other methods, like Move() etc.
	void InitAbsolutePositions();

	/// calculate how deep this element is in the hierarchy
	int CalculateHierarchyDepth() const;
    /// get ScreenSpaceRect	
    const Math::rectangle<float>& GetScreenSpaceRect() const;

    Util::String id;    
    bool visible;
    bool enabled;
    Ptr<Element> parentElement;
    Ptr<Window> parentWindow;
	Util::Array<Ptr<Element> > childElements;    
    bool isValid;     
	bool isConvertedToAbsolutPositions;
    IndexT elementId;

    Ptr<Element> neighbours[NumDirections];

	Math::float4 defaultColor;
	Math::float4 defaultUV;
	Math::rectangle<float> screenSpaceRect;

    bool blinkingEnabled;
    float blinkTime;
    Math::float4 blinkColor;
    Timing::Time fadeOutTime;
    Timing::Time fadeInTime;

    bool mouseOver;
    bool ignoreInput;
    bool dirty;
};

//------------------------------------------------------------------------------
/**
*/
inline 
void
Element::SetIgnoreInput(bool b)
{
    this->ignoreInput = b;
}

//------------------------------------------------------------------------------
/**
*/
inline 
bool
Element::GetIgnoreInput() const
{
    return this->ignoreInput;
}

//------------------------------------------------------------------------------
/**
*/
inline 
void
Element::SetMouseOver(bool b)
{
    this->mouseOver = b;
}

//------------------------------------------------------------------------------
/**
*/
inline 
bool
Element::IsMouseOver() const
{
    return this->mouseOver;
}

//------------------------------------------------------------------------------
/**
*/
inline 
void
Element::SetDefaultColor(const Math::float4& color)
{
	this->defaultColor = color;
}

//------------------------------------------------------------------------------
/**
*/
inline 
const Math::float4&
Element::GetDefaultColor() const
{
	return this->defaultColor;
}

//------------------------------------------------------------------------------
/**
*/
inline 
void
Element::SetDefaultUV(const Math::float4& uv)
{
	this->defaultUV = uv;
}

//------------------------------------------------------------------------------
/**
*/
inline 
const Math::float4&
Element::GetDefaultUV() const
{
	return this->defaultUV;
}

//------------------------------------------------------------------------------
/**
*/
inline 
void 
Element::SetPixelSize(const Math::float2& size)
{
	// transform to screen coord
	Ptr<Graphics::Display> display = Graphics::Display::Instance();
	Math::float2 screenSize(size.x() / display->GetDisplayMode().GetWidth(),
							size.y() / display->GetDisplayMode().GetHeight());
	// set new
	this->SetScreenSize(screenSize);
}

//------------------------------------------------------------------------------
/**
*/
inline     
const Math::float2 
Element::GetPixelSize() const
{
	// get screen size
	Math::float2 screenSize = this->GetScreenSize();
	// transform to pixels
	Ptr<Graphics::Display> display = Graphics::Display::Instance();
	return Math::float2(display->GetDisplayMode().GetWidth() * screenSize.x(),
						display->GetDisplayMode().GetHeight() * screenSize.y());
}

//------------------------------------------------------------------------------
/**
*/
inline 
void 
Element::SetScreenSize(const Math::float2& size)
{
	// get size and pos
	Math::float2 screenPos = this->GetScreenPosition();
	Math::float2 sizeHalf = size * 0.5f;
	// set new
	this->screenSpaceRect.set(	screenPos.x() - sizeHalf.x(),
								screenPos.y() - sizeHalf.y(),
								screenPos.x() + sizeHalf.x(),
								screenPos.y() + sizeHalf.y());
}

//------------------------------------------------------------------------------
/**
*/
inline 
const Math::float2 
Element::GetScreenSize() const
{
	return Math::float2(this->screenSpaceRect.width(), this->screenSpaceRect.height());
}

//------------------------------------------------------------------------------
/**
*/
inline 
void
Element::SetPixelPosition(const Math::float2& relPos)
{
	// transform to screen coord
	Ptr<Graphics::Display> display = Graphics::Display::Instance();
	Math::float2 screenPos(	relPos.x() / display->GetDisplayMode().GetWidth(),
							relPos.y() / display->GetDisplayMode().GetHeight());
	// set new
	this->SetScreenPosition(screenPos);
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::float2
Element::GetPixelPosition() const
{
	// get screen pos
	Math::float2 screenPos = this->GetScreenPosition();
	// transform to pixels
	Ptr<Graphics::Display> display = Graphics::Display::Instance();
	return Math::float2(display->GetDisplayMode().GetWidth() * screenPos.x(),
						display->GetDisplayMode().GetHeight() * screenPos.y());
}

//------------------------------------------------------------------------------
/**
*/
inline
void
Element::SetScreenPosition(const Math::float2& relPos)
{
	// get actuall size
	float widthHalf = this->screenSpaceRect.width() / 2.0f;
	float heightHalf = this->screenSpaceRect.height() / 2.0f;

	// set new pos with old size
	this->screenSpaceRect.set(	relPos.x() - widthHalf,
								relPos.y() - heightHalf,
								relPos.x() + widthHalf,
								relPos.y() + heightHalf);											
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::float2
Element::GetScreenPosition() const
{
	return Math::float2((this->screenSpaceRect.left + this->screenSpaceRect.right) / 2.0f,
						(this->screenSpaceRect.top + this->screenSpaceRect.bottom) / 2.0f);
}

//------------------------------------------------------------------------------
/**
*/
inline 
bool
Element::IsValid() const
{
    return this->isValid;
}

//------------------------------------------------------------------------------
/**
*/
inline 
void
Element::SetId(const Util::String& i)
{
    this->id = i;
}

//------------------------------------------------------------------------------
/**
*/
inline 
const Util::String&
Element::GetId() const
{
    return this->id;
}

//------------------------------------------------------------------------------
/**
    Get pointer to parent element.
*/
inline 
const Ptr<Element>&
Element::GetParentElement() const
{
    return this->parentElement;
}


//------------------------------------------------------------------------------
/**
    Set the pointer to the parent element.
*/
inline
void
Element::SetParentElement(const Ptr<Element>& parent)
{
	n_assert(parent.isvalid());
	n_assert(!this->parentElement.isvalid());	

	this->parentElement = parent;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Element::SetEnabled(bool b)
{
    this->enabled = b;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Element::AppendChildElement(const Ptr<Element>& elem)
{
	n_assert(elem.isvalid());
    this->childElements.Append(elem);
}

//------------------------------------------------------------------------------
/**
    Provides access to the child element array.
*/
inline 
const Util::Array<Ptr<Element>>&
Element::GetChildElements() const
{
    return this->childElements;
}

//------------------------------------------------------------------------------
/**
    Returns the number of child elements
*/
inline
int
Element::GetNumChildElements() const
{
    return this->childElements.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
Element::GetBlinkingEnabled() const
{
    return this->blinkingEnabled;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
Element::SetBlinkingEnabled(bool val)
{
    this->blinkingEnabled = val;
}

//------------------------------------------------------------------------------
/**
*/
inline float 
Element::GetBlinkTime() const
{
    return this->blinkTime;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
Element::SetBlinkTime(float val)
{
    this->blinkTime = val;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::float4& 
Element::GetBlinkColor() const
{
    return this->blinkColor;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
Element::SetBlinkColor(const Math::float4& val)
{
    this->blinkColor = val;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT 
Element::GetElementId() const
{   
    return this->elementId;
}


//------------------------------------------------------------------------------
/**
*/
inline 
void 
Element::SetFadeInTime(Timing::Time time)
{
    this->fadeInTime = time;
    // pass to children
    IndexT i;
    for (i = 0; i < this->childElements.Size(); ++i)
    {
    	this->childElements[i]->SetFadeInTime(time);
    }
}

//------------------------------------------------------------------------------
/**
*/
inline 
void 
Element::SetFadeOutTime(Timing::Time time)
{
    this->fadeOutTime = time;
    // pass to children
    IndexT i;
    for (i = 0; i < this->childElements.Size(); ++i)
    {
        this->childElements[i]->SetFadeOutTime(time);
    }
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Time 
Element::GetFadeInTime() const
{
    return this->fadeInTime;
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Time 
Element::GetFadeOutTime() const
{
    return this->fadeOutTime;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::rectangle<float>& 
Element::GetScreenSpaceRect() const
{
    return this->screenSpaceRect;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Element>& 
Element::GetNeighbour(NeighbourDirection dir) const
{
    return this->neighbours[dir];
}
}; // namespace
//------------------------------------------------------------------------------
#endif
