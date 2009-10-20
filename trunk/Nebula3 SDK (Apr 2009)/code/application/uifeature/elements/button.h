#ifndef UI_BUTTON_H
#define UI_BUTTON_H
//------------------------------------------------------------------------------
/**
    @class UI::Button

    A simple graphical button UI element.

    (C) 2005 Radon Labs GmbH
*/
#include "uifeature/elements/element.h"
#include "util/fixedarray.h"

//------------------------------------------------------------------------------
namespace UI
{
class Button : public Element
{
	__DeclareClass(Button);
public:

    /// control states
    enum VisualState
    {
        Normal,
        Pressed,
        MouseOver,
        Disabled,

        NumVisualStates,
        InvalidVisualState,
    };

    /// constructor
    Button();
    /// destructor
    virtual ~Button();

    /// called before the gui hierarchy is rendered
    virtual void OnRender();

    /// called when left mouse button is pressed over element
    virtual void OnLeftButtonDown(const Math::float2& mousePos);
    /// called when left mouse button is released over element
    virtual void OnLeftButtonUp(const Math::float2& mousePos);   

    /// called when a gamepad button was pressed
    virtual void OnGamePadButtonDown(Input::GamePad::Button btn);
    /// called when a gamepad button was released
    virtual void OnGamePadButtonUp(Input::GamePad::Button btn);
    
	/// change texture of a visual state
	void SetStateUV(VisualState visualState, const Math::float4& uv);    
    /// get the texture of a visual state
	const Math::float4& GetStateUV(VisualState visualState);

    /// convert visual state to a string
    static Util::String VisualStateToString(VisualState visualState);

    /// called when input focus is gained
    virtual void OnFocusGain();
    /// called when input focus is lost
    virtual void OnFocusLost(); 

protected:
    /// set the active visual state
    virtual void SetActiveVisualState(VisualState state);
    VisualState activeState;

	/// array for all textures for all visual states
	Util::FixedArray<Math::float4> stateUV;

    bool pressed;
};

//------------------------------------------------------------------------------
/**
*/
inline 
void
Button::SetStateUV(VisualState visualState, const Math::float4& uv)
{
    this->stateUV[(int)visualState] = uv;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::float4& 
Button::GetStateUV(VisualState visualState)
{   
    return this->stateUV[(int)visualState];
}

}; // namespace UI
//------------------------------------------------------------------------------
#endif