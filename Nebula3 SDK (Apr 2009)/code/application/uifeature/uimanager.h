#ifndef UI_UIMANAGER_H
#define UI_UIMANAGER_H
//------------------------------------------------------------------------------
/**
    @class UI::UIManager

    Server for the Nebula 3 GUI subsystem.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "util/array.h"
#include "util/stack.h"
#include "timing/time.h"
#include "math/float2.h"
#include "uifeature/uieventhandler.h"
#include "uifeature/elements/window.h"

//------------------------------------------------------------------------------
namespace UI
{
class UIRenderer;
class UIFactory;
class UIEvent;
class UIXmlParser;
class Element;

class UIManager : public Core::RefCounted
{
    __DeclareClass(UIManager);
    __DeclareSingleton(UIManager);

    static const int WINDOW_Z_OFFSET            = 10;               // offset between 2 windows
public:
    /// constructor
    UIManager();
    /// destructor
    virtual ~UIManager();        

    /// open the ui subsystem
    bool Open();
    /// close the ui subsystem
    void Close();
    /// return true if currently open
    bool IsOpen() const;

	/// trigger the server
    void Trigger();
    /// render the user interface
    void Render();
	/// debug rendering (just the shapes)
	void RenderDebug();

	/// return the active window (can be invalid)
    Ptr<Window> GetActiveWindow() const;
    /// set the active window 
    void SetActiveWindow(const Ptr<Window>& w);
    /// get the current pointer to the focus element (might be invalid!)
    const Ptr<Element>& GetFocusElement() const;    
    
	/// set current time
	void SetTime(Timing::Time t);
    /// get current time
    Timing::Time GetTime() const;

    /// get mouse position
	const Math::float2& GetMousePosition() const;
    /// get mouse postion in pixels
	const Math::float2& GetPixelMousePosition() const;

    /// is mouse over a UI element
    bool IsMouseOverGui() const;
    /// return the window, under the mouse
    Ptr<Window> GetWindowUnderMouse() const;

    /// enable input handling
    void EnableInput();
    /// disable input handling
    void DisableInput();
    /// return true if input handling is enabled
    bool IsInputEnabled() const;

	/// set the event handler
	void SetEventHandler(const Ptr<UIEventHandler>& handler);
	/// get the event handler
	const Ptr<UIEventHandler>& GetEventHandler() const;

	/// redirects the event to the handler
	void PutEvent(const Ptr<UIEvent>& event);

	/// preload the specified ui file ATTENTION: does NOT attach the windows
	Util::Array<Ptr<Window>> LoadWindows(const Util::String& fileName, bool attachAndOpen = false);

	/// finds the window with the specified UNIQUE window id (=title)
	Ptr<Window> FindWindow(const Util::String& title) const;

    /// close all windows
    void CloseAllWindows();

    /// show and load loading screen
    void ShowLoadingScreen(const Util::String& res);
    /// hide and remove loading screen
    void HideLoadingScreen();

private:
    friend class Window;
    friend class Element;

    /// set current focus object DON'T CALL DIRECTLY IN CONTROL
    void SetFocusElement(const Ptr<Element>& focusElement, bool reg);
    
	/// attach a window to the UI
    void AttachWindow(const Ptr<Window>& window);
    /// remove a window from the UI
    void RemoveWindow(const Ptr<Window>& window);

	/// handles each frame the imput
	void HandleInput();
    /// handle focus element selection
    void SelectFocusElement(const Math::float2& cursorDir);

    /// get new element id
    IndexT GetNewElementId();
    /// free id
    void FreeElementId(IndexT id);

    bool isOpen;
    bool inTrigger;
    bool globalHide;
	bool inputEnabled;

    Util::Array<Ptr<Window> > windows;
    Util::Array<Ptr<Window> > loadingWindows;
    Ptr<Window> activeWindow;
    
    Ptr<UIFactory> uiFactory;    
	Ptr<Element> focusElement; 
    Ptr<UIEventHandler> curEventHandler;
	Ptr<UIXmlParser> parser;

	Math::float2 mousePosition;
    Math::float2 pixelMousePosition;   

	Timing::Time time;

    Util::Stack<IndexT> freeIds;
    IndexT nextElementId;
};

//------------------------------------------------------------------------------
/**
*/
inline
Ptr<Window>
UIManager::FindWindow(const Util::String& title) const
{
	IndexT i;
	for(i = 0; i < this->windows.Size(); i++)
	{
		if(this->windows[i]->GetWindowTitle() == title)
		{
			return this->windows[i];
		}
	}
	return Ptr<Window>(0);
}
//------------------------------------------------------------------------------
/**
*/
inline
const Ptr<UIEventHandler>&
UIManager::GetEventHandler() const
{
	return this->curEventHandler;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
UIManager::SetEventHandler(const Ptr<UIEventHandler>& handler)
{
	this->curEventHandler = handler;
}

//------------------------------------------------------------------------------
/**
*/
inline
Ptr<Window>
UIManager::GetActiveWindow() const
{
    if (activeWindow.isvalid())
    {
        return this->activeWindow;
    }
    // FIXME!
    // find the first valid window
    IndexT i;
    for (i = this->windows.Size()-1; i >= 0; i--)
    {
        if (this->windows[i].isvalid())
        {
            return this->windows[i];
        }
    }
    return Ptr<Window>(0);
}

//------------------------------------------------------------------------------
/**
*/
inline void 
UIManager::SetActiveWindow(const Ptr<Window>& w)
{    
    this->activeWindow = w;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
UIManager::IsOpen() const
{
    return this->isOpen;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::float2&
UIManager::GetMousePosition() const
{
    return this->mousePosition;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::float2&
UIManager::GetPixelMousePosition() const
{
    return this->pixelMousePosition;
}

//-----------------------------------------------------------------------------
/**
*/
inline
const Ptr<Element>&
UIManager::GetFocusElement() const
{
    return this->focusElement;
}

//-----------------------------------------------------------------------------
/**
*/
inline
void
UIManager::EnableInput()
{
    this->inputEnabled = true;
}

//-----------------------------------------------------------------------------
/**
*/
inline
void
UIManager::DisableInput()
{
    this->inputEnabled = false;
}

//-----------------------------------------------------------------------------
/**
*/
inline
bool
UIManager::IsInputEnabled() const
{
    return this->inputEnabled;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
UIManager::SetTime(Timing::Time t)
{
    this->time = t;
}

//------------------------------------------------------------------------------
/**
*/
inline
Timing::Time
UIManager::GetTime() const
{
    return this->time;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT 
UIManager::GetNewElementId()
{
    if (this->freeIds.Size() > 0)
    {
        return this->freeIds.Pop();
    }
    return this->nextElementId++;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
UIManager::FreeElementId(IndexT id)
{
    this->freeIds.Push(id);
}

}; // namespace UI
//------------------------------------------------------------------------------
#endif
