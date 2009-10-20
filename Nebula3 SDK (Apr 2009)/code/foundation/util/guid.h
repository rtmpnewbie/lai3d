#pragma once
//------------------------------------------------------------------------------
/**
    @class Util::Guid
    
    Implements a GUID.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/config.h"
#if __WIN32__
#include "util/win32/win32guid.h"
namespace Util
{
typedef Win32::Win32Guid Guid;
}
#elif __XBOX360__
#include "util/xbox360/xbox360guid.h"
namespace Util
{
typedef Xbox360::Xbox360Guid Guid;
}
#elif __WII__
#include "util/wii/wiiguid.h"
namespace Util
{
typedef Wii::WiiGuid Guid;
}
#else
#error "IMPLEMENT ME!"
#endif
//------------------------------------------------------------------------------
    