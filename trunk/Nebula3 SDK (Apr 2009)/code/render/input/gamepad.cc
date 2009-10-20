//------------------------------------------------------------------------------
//  gamepad.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "input/gamepad.h"
#if (__WIN32__ || __XBOX360__)
namespace Input
{
__ImplementClass(Input::GamePad, 'GMPD', XInput::XInputGamePad);
}
#elif (__WII__)
namespace Input
{
__ImplementClass(Input::GamePad, 'GMPD', Wii::Wiimote);
}
#else
#error "GamePad class not implemented on this platform!"
#endif
