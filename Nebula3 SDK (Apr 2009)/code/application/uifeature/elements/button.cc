//------------------------------------------------------------------------------
//  uifeature/elements/button.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "uifeature/elements/button.h"
#include "uifeature/uimanager.h"
#include "uifeature/uievent.h"
#include "audio/audioplayer.h"
namespace UI
{
__ImplementClass(Button, 'UIBT', Element);

//------------------------------------------------------------------------------
/**
*/
Button::Button() :   activeState(Normal),
                     stateUV(NumVisualStates),
                     pressed(false)
{
}

//------------------------------------------------------------------------------
/**
*/
Button::~Button()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void 
Button::OnRender()
{
    // set correct UV for rendering
    if (!this->enabled)
    {
        this->SetActiveVisualState(Disabled);
    }
    else if(this->IsMouseOver())
    {
        if(this->pressed)
        {
            this->SetActiveVisualState(Pressed);
        }
        else
        {
            this->SetActiveVisualState(MouseOver);
        }
        // if mouse is over element, switch focus element
        this->RegisterAsFocusElement();
    }
    else
    {        
        // reset pressed state if mouse outside
        this->pressed = false;
    }

    // now call parent to render
    Element::OnRender();    
}

//------------------------------------------------------------------------------
/**
    Called on left mouse button down. This sets the pressed state of the
    button. Also set the pressed state.
*/
void
Button::OnLeftButtonDown(const Math::float2& mousePos)
{
	// build event
	Ptr<Button> ptr(this);
	Ptr<UIEvent> event = UIEvent::Create();
	event->SetElement(ptr.cast<Element>());
	event->SetEventType(Input::InputEvent::MouseButtonDown);
	event->SetMouseButton(Input::MouseButton::LeftButton);
	UIManager::Instance()->PutEvent(event);

    // set state
    this->pressed = true;
}

//------------------------------------------------------------------------------
/**
    Called on left mouse button up. This sends the button event.
    Also reset the pressed state.
*/
void
Button::OnLeftButtonUp(const Math::float2& mousePos)
{
	// build event
	Ptr<Button> ptr(this);
	Ptr<UIEvent> event = UIEvent::Create();
	event->SetElement(ptr.cast<Element>());
	event->SetEventType(Input::InputEvent::MouseButtonUp);
	event->SetMouseButton(Input::MouseButton::LeftButton);
	UIManager::Instance()->PutEvent(event);    

    // reset state
    this->pressed = false;
}

//------------------------------------------------------------------------------
/**
    Called on gamepad button down. This sets the pressed state of the
    button.
*/
void
Button::OnGamePadButtonDown(Input::GamePad::Button btn)
{
	// build event
	Ptr<Button> ptr(this);
	Ptr<UIEvent> event = UIEvent::Create();
	event->SetElement(ptr.cast<Element>());
    event->SetEventType(Input::InputEvent::GamePadButtonDown);
    event->SetGamePadButton(btn);
	UIManager::Instance()->PutEvent(event);
}

//------------------------------------------------------------------------------
/**
    Called on left mouse button up. This sends the button event.
*/
void
Button::OnGamePadButtonUp(Input::GamePad::Button btn)
{
	// build event
	Ptr<Button> ptr(this);
	Ptr<UIEvent> event = UIEvent::Create();
	event->SetElement(ptr.cast<Element>());
    event->SetEventType(Input::InputEvent::GamePadButtonUp);
    event->SetGamePadButton(btn);
	UIManager::Instance()->PutEvent(event);
}

//------------------------------------------------------------------------------
/**
*/    
Util::String
Button::VisualStateToString(VisualState visualState)
{
    n_assert(visualState < NumVisualStates);
    switch (visualState)
    {
        case Normal:
            return "Normal";
        case Pressed:
            return "Pressed";
        case MouseOver:
            return "MouseOver";
        case Disabled:
            return "Disabled";
        default:
            return "Normal";
    }
}

//------------------------------------------------------------------------------
/**
*/
void
Button::SetActiveVisualState(VisualState state)
{
    n_assert(state < NumVisualStates);

    if(this->activeState == state)
    {
        return;
    }

    // set new uv coord
    this->activeState = state;
    this->defaultUV = this->stateUV[(int)this->activeState];

    // set dirty
    this->dirty = true;
}

//------------------------------------------------------------------------------
/**
*/
void 
Button::OnFocusGain()
{
    Audio::AudioPlayer::Instance()->PlayCue(CoreAudio::CueId("ui_switch"));
    this->SetActiveVisualState(MouseOver);
}

//------------------------------------------------------------------------------
/**
*/
void 
Button::OnFocusLost()
{
    this->SetActiveVisualState(Normal);
}
}; // namespace UI

