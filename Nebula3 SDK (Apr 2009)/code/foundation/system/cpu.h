#pragma once
//------------------------------------------------------------------------------
/**
    @class System::Cpu
    
    Provides information about the system's CPU(s).
    
    (C) 2007 Radon Labs GmbH
*/
#if __WIN32__
#include "system/win32/win32cpu.h"
namespace System
{
typedef Win32::Win32Cpu Cpu;
}
#elif __XBOX360__
#include "system/xbox360/xbox360cpu.h"
namespace System
{
typedef Xbox360::Xbox360Cpu Cpu;
}
#elif __WII__
#include "system/wii/wiicpu.h"
namespace System
{
typedef Wii::WiiCpu Cpu;
}
#endif
//------------------------------------------------------------------------------
    