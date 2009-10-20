#pragma once
//------------------------------------------------------------------------------
/**
    @class Threading::CriticalSection

    Critical section objects are used to protect a portion of code from parallel
    execution. Define a static critical section object and use its Enter() 
    and Leave() methods to protect critical sections of your code.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/config.h"
#if (__WIN32__ || __XBOX360__)
#include "threading/win360/win360criticalsection.h"
namespace Threading
{
class CriticalSection : public Win360::Win360CriticalSection
{ };
}
#elif __WII__
#include "threading/wii/wiicriticalsection.h"
namespace Threading
{
class CriticalSection : public Wii::WiiCriticalSection
{ };
}
#else
#error "Threading::CriticalSection not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
    