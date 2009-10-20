#ifndef UI_WINDOW_H
#define UI_WINDOW_H
//------------------------------------------------------------------------------
/**
    @class UI::Window

    Base class for windows. A window generally has exactly one canvas
    attached.

    Override this class to implement windows with new behaviour.
    The window class generally implements only functionality, not layout!
	The virtual position of the window is in the center of the screen.
	The alignment descripes the center position of the canvas.

    //If Activate() is called before Open() (the window has already been attached
    //to server for receiving messages) it will not be deactivated on Close(). So
    //the window is able to receive further messages after closing.

    (C) 2008 Radon Labs GmbH
*/
#include "messaging/port.h"
#include "uifeature/elements/canvas.h"

//------------------------------------------------------------------------------
namespace UI
{
class Window : public Core::RefCounted
{
    __DeclareClass(Window);
public:
	
    enum ScreenAlignmentX
    {
        UnalignedX = 0,
        LeftSide,
        CenterX,
        RightSide
    };
    enum ScreenAlignmentY
    {
        UnalignedY = 0,
        UpSide,
        CenterY,
        DownSide
    };

    /// constructor
    Window();
    /// destructor
    virtual ~Window();

    /// open the window at position(will attach the window to the UI server)
	virtual void Open();
    /// close the window (will remove the window from the UI server)
    virtual void Close();
    /// return true if window is open
    bool IsOpen() const;
    /// hide the window
    virtual void Hide();
    /// shows the window (will open if needed)
    virtual void Show();
    /// return true if window is hidden
    bool IsHidden() const;
    /// return true if window is open and not hidden
    bool IsShown() const;

    /// get the canvas of the window
    const Ptr<Canvas>& GetCanvas() const;    

    /// called once per frame after input has been delivered
    virtual void OnFrame();
    /// called before the gui hierarchy is rendered
    virtual void OnRender();
	/// render debug (just the shapes of window and all elements)
	virtual void OnRenderDebug();

	/// return true if mouse is within element area (just checks the canvas)
	virtual bool Inside(const Math::float2& mousePos);
	/// returns all elements at the position
	virtual void GetElementsAt(const Math::float2& mousePos, Util::Array<Ptr<Element>>& elements_out);

	/// set screen alignment
    void SetScreenAlignmentX(ScreenAlignmentX align);
    /// set screen alignment
    ScreenAlignmentX GetScreenAlignmentX() const;
    /// set screen alignment
    void SetScreenAlignmentY(ScreenAlignmentY align);
    /// set screen alignment
    ScreenAlignmentY GetScreenAlignmentY() const;

	/// convert x align to string
	static Util::String AlignmentXToString(const ScreenAlignmentX& align);
	/// convert y align to string
	static Util::String AlignmentYToString(const ScreenAlignmentY& align);
	/// convert string to x align
	static ScreenAlignmentX StringToAlignmentX(const Util::String& align);
	/// convert string to y align
	static ScreenAlignmentY StringToAlignmentY(const Util::String& align);

	/// set the window title
	void SetWindowTitle(const Util::String& title);
	/// get the window title
	const Util::String& Window::GetWindowTitle() const;

    /// set the windowtexture
    void SetWindowTexture(const Util::String& texture);
    /// get the windowtexture
    const Util::String& Window::GetWindowTexture() const;

    /// set the additional z offset
    void SetAdditionalZOffset(int z);
    /// get the additional z offset
    int GetAdditionalZOffset() const;

    /// set fadein time
    void SetFadeInTime(Timing::Time time);
    /// get fadein time
    Timing::Time GetFadeInTime() const;
    /// set fadeout time
    void SetFadeOutTime(Timing::Time time);
    /// get fadeout time
    Timing::Time GetFadeOutTime() const;
    
protected:
	friend class UIXmlParser;
	friend class UIManager;

	/// set the canvas of the window
    void SetCanvas(const Ptr<Canvas>& canvas);
  
	/// update all screen rect's of all child elements (recursivly)
	/// ATTENTION, call this just ONCE after creating! afterwards, 
	/// just move other methods, like Move() etc.
	virtual void InitAbsolutePosition();

	
    bool isOpen;    
    bool isHidden;

	Util::String windowTitle;
    Util::String windowTexture;

    Ptr<Canvas> canvas;

    ScreenAlignmentX screenAlignmentX;
    ScreenAlignmentY screenAlignmentY;

    /// the additional offset position (for moving windows in background...)
    int zOffset;
    Timing::Time fadeOutTime;
    Timing::Time fadeInTime;
};

//------------------------------------------------------------------------------
/**
*/
inline
void
Window::SetAdditionalZOffset(int z)
{
    this->zOffset = z;
}

//------------------------------------------------------------------------------
/**
*/
inline
int
Window::GetAdditionalZOffset() const
{
    return this->zOffset;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
Window::SetWindowTitle(const Util::String& title)
{
	this->windowTitle = title;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::String& 
Window::GetWindowTitle() const
{
    return this->windowTitle;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
Window::SetCanvas(const Ptr<Canvas>& canvas)
{
	this->canvas = canvas;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
Window::IsOpen() const
{
    return this->isOpen;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Ptr<Canvas>&
Window::GetCanvas() const
{
    return this->canvas;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
Window::IsHidden() const
{
    if (this->isOpen && this->isHidden)
    {
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
Window::IsShown() const
{
    if (this->isOpen && !this->isHidden)
    {
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
/**
*/
inline
void 
Window::SetScreenAlignmentX(Window::ScreenAlignmentX align)
{
    this->screenAlignmentX = align;
}

//------------------------------------------------------------------------------
/**
*/
inline
Window::ScreenAlignmentX 
Window::GetScreenAlignmentX() const
{
    return this->screenAlignmentX;
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
Window::SetScreenAlignmentY(Window::ScreenAlignmentY align)
{
    this->screenAlignmentY = align;
}

//------------------------------------------------------------------------------
/**
*/
inline
Window::ScreenAlignmentY 
Window::GetScreenAlignmentY() const
{
    return this->screenAlignmentY;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
Window::SetWindowTexture(const Util::String& texture)
{
	this->windowTexture = texture;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::String& 
Window::GetWindowTexture() const
{
    return this->windowTexture;
}

//------------------------------------------------------------------------------
/**
*/
inline 
void 
Window::SetFadeInTime(Timing::Time time)
{
    this->fadeInTime = time;
    // pass to canvas and its children
    this->canvas->SetFadeInTime(time);
}

//------------------------------------------------------------------------------
/**
*/
inline 
void 
Window::SetFadeOutTime(Timing::Time time)
{
    this->fadeOutTime = time;
    // pass to canvas and its children
    this->canvas->SetFadeOutTime(time);
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Time 
Window::GetFadeInTime() const
{
    return this->fadeInTime;
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Time 
Window::GetFadeOutTime() const
{
    return this->fadeOutTime;
}
}; // namespace UI
//------------------------------------------------------------------------------
#endif