#pragma once
//------------------------------------------------------------------------------
/**
    @class Base::GamePadBase

    An input handler which represents one of at most 4 game pads.

    (C) 2007 Radon Labs GmbH
*/
#include "input/inputhandler.h"

//------------------------------------------------------------------------------
namespace Base
{
class GamePadBase : public Input::InputHandler
{
    __DeclareClass(GamePadBase);
public:
    /// gamepad buttons
    enum Button
    {
        DPadUpButton = 0,           // all platforms
        DPadDownButton,             // all platforms        
        DPadLeftButton,             // all platforms
        DPadRightButton,            // all platforms
        StartButton,                // all platforms
        BackButton,                 // Xbox360
        LeftThumbButton,            // Xbox360, PS3
        RightThumbButton,           // Xbox360, PS3
        LeftShoulderButton,         // Xbox360, PS3, Wii: ClassicController ZL
        RightShoulderButton,        // Xbox360, PS3, Wii: ClassicController ZR
        
        AButton,                    // Xbox360: A, Wii: A/a, PS3: Cross
        BButton,                    // Xbox360: B, Wii: b(pad), PS3: Circle
        XButton,                    // Xbox360: X, Wii: x(pad), PS3: Square
        YButton,                    // Xbox360: Y, Wii: y(pad), PS3: Triangle
        
        // Wii specific buttons
        HomeButton,                 // Xbox360: /, Wii: Home, PS3: /
        PlusButton,                 // Xbox360: /, Wii: +, PS3: /
        MinusButton,                // Xbox360: /, Wii: -, PS3: /
        OneButton,                  // Xbox360: /, Wii: 1, PS3: /
        TwoButton,                  // Xbox360: /, Wii: 2, PS3: /
        ZButton,                    // Xbox360: /, Wii: Z(Nunchuk), PS3: /
        CButton,                    // Xbox360: /, Wii: C(Nunchuk), PS3: /

        NumButtons,
        InvalidButton,
    };

    /// gamepad axis
    enum Axis
    {
        // general axi
        LeftTriggerAxis = 0,    // 0.0f .. 1.0f
        RightTriggerAxis,       // 0.0f .. 1.0f
        LeftThumbXAxis,         // -1.0f .. 1.0f
        LeftThumbYAxis,         // -1.0f .. 1.0f
        RightThumbXAxis,        // -1.0f .. 1.0f
        RightThumbYAxis,        // -1.0f .. 1.0f
        
        NumAxes,
        InvalidAxis,
    };

    /// constructor
    GamePadBase();
    /// destructor
    virtual ~GamePadBase();
    
    /// convert button code to string
    static Util::String ButtonAsString(Button btn);
    /// convert axis to string
    static Util::String AxisAsString(Axis a);

    /// return true if this game pad is currently connected
    bool IsConnected() const;
    /// set player index
    void SetPlayerIndex(IndexT i);
    /// get the player index of this game pad
    IndexT GetPlayerIndex() const;
    /// get maximum number of players
    static SizeT GetMaxNumPlayers();

    /// return true if a button is currently pressed
    bool ButtonPressed(Button btn) const;
    /// return true if button was down at least once in current frame
    bool ButtonDown(Button btn) const;
    /// return true if button was up at least once in current frame
    bool ButtonUp(Button btn) const;
    /// get current axis value
    float GetAxisValue(Axis axis) const;

    /// set low-frequency vibration effect (0.0f .. 1.0f)
    void SetLowFrequencyVibrator(float f);
    /// get low-frequency vibration
    float GetLowFrequencyVibrator() const;
    /// set high-frequency vibration effect (0.0f .. 1.0f)
    void SetHighFrequencyVibrator(float f);
    /// get high-frequency vibration
    float GetHighFrequencyVibrator() const;

    /// get current state as an array of input events (override in subclass!)
    Util::Array<Input::InputEvent> GetStateAsInputEvents() const;

protected:   
    /// called when the handler is attached to the input server
    virtual void OnAttach();
    /// reset the input handler
    virtual void OnReset();

    struct ButtonState
    {
    public:
        /// constructor
        ButtonState() : pressed(false), down(false), up(false) {};

        bool pressed;
        bool down;
        bool up;
    };

    IndexT playerIndex;
    bool isConnected;
    Util::FixedArray<ButtonState> buttonStates;
    Util::FixedArray<float> axisValues;
    bool vibratorsDirty;
    float lowFreqVibrator;
    float highFreqVibrator;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
GamePadBase::IsConnected() const
{
    return this->isConnected;
}

//------------------------------------------------------------------------------
/**
*/
inline void
GamePadBase::SetPlayerIndex(IndexT i)
{
    n_assert(i < this->GetMaxNumPlayers());
    this->playerIndex = i;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT
GamePadBase::GetPlayerIndex() const
{
    return this->playerIndex;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
GamePadBase::GetMaxNumPlayers()
{
    return 4;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
GamePadBase::ButtonPressed(Button btn) const
{
    return this->buttonStates[btn].pressed;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
GamePadBase::ButtonDown(Button btn) const
{
    return this->buttonStates[btn].down;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
GamePadBase::ButtonUp(Button btn) const
{
    return this->buttonStates[btn].up;
}

//------------------------------------------------------------------------------
/**
*/
inline float
GamePadBase::GetAxisValue(Axis axis) const
{
    return this->axisValues[axis];
}

//------------------------------------------------------------------------------
/**
*/
inline void
GamePadBase::SetLowFrequencyVibrator(float f)
{
    this->vibratorsDirty = true;
    this->lowFreqVibrator = Math::n_clamp(f, 0.0f, 1.0f);
}

//------------------------------------------------------------------------------
/**
*/
inline float
GamePadBase::GetLowFrequencyVibrator() const
{
    return this->lowFreqVibrator;
}

//------------------------------------------------------------------------------
/**
*/
inline void
GamePadBase::SetHighFrequencyVibrator(float f)
{
    this->vibratorsDirty = true;
    this->highFreqVibrator = Math::n_clamp(f, 0.0f, 1.0f);;
}

//------------------------------------------------------------------------------
/**
*/
inline float
GamePadBase::GetHighFrequencyVibrator() const
{
    return this->highFreqVibrator;
}

} // namespace Base
//------------------------------------------------------------------------------

