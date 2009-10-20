//------------------------------------------------------------------------------
//  inputserver.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "input/inputserver.h"

namespace Input
{
#if __WIN32__
__ImplementClass(Input::InputServer, 'INPS', Win32::Win32InputServer);
__ImplementSingleton(Input::InputServer);
#elif __XBOX360__
// FIXME!
__ImplementClass(Input::InputServer, 'INPS', Xbox360::Xbox360InputServer);
__ImplementSingleton(Input::InputServer);
#elif __WII__
__ImplementClass(Input::InputServer, 'INPS', Wii::WiiInputServer);
__ImplementSingleton(Input::InputServer);
#else
#error "InputServer class not implemented on this platform!"
#endif

//------------------------------------------------------------------------------
/**
*/
InputServer::InputServer()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
InputServer::~InputServer()
{
    __DestructSingleton;
}

} // namespace Input
