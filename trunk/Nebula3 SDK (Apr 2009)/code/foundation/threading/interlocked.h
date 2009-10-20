#pragma once
//------------------------------------------------------------------------------
/**
    @class Threading::Interlocked
    
    Provide simple atomic operations on memory variables.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/config.h"
#if (__WIN32__ || __XBOX360__)
#include "threading/win360/win360interlocked.h"
namespace Threading
{
class Interlocked : public Win360::Win360Interlocked
{ };
}
#elif __WII__
#include "threading/wii/wiiinterlocked.h"
namespace Threading
{
class Interlocked : public Wii::WiiInterlocked
{ };
}
#else
#error "Threading::Interlocked not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
