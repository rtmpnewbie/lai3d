#pragma once
//------------------------------------------------------------------------------
/**
    @class Threading::Event
  
    @todo describe Event class
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/config.h"
#if (__WIN32__ || __XBOX360__)
#include "threading/win360/win360event.h"
namespace Threading
{
class Event : public Win360::Win360Event
{ };
}
#elif __WII__
#include "threading/wii/wiievent.h"
namespace Threading
{
class Event : public Wii::WiiEvent
{ };
}
#else
#error "Threading::Event not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
