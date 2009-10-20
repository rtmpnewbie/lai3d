#pragma once
//------------------------------------------------------------------------------
/**
    @class System::SystemInfo
    
    Provides information about the host system.
    
    (C) 2008 Radon Labs GmbH
*/
#if __WIN32__
#include "system/win32/win32systeminfo.h"
namespace System
{
class SystemInfo : public Win32::Win32SystemInfo {};
}
#elif __XBOX360__
#include "system/xbox360/xbox360systeminfo.h"
namespace System
{
class SystemInfo : public Xbox360::Xbox360SystemInfo {};
}
#elif __WII__
#include "system/wii/wiisysteminfo.h"
namespace System
{
class SystemInfo : public Wii::WiiSystemInfo {};
}
#else
#error "System::SystemInfo not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
