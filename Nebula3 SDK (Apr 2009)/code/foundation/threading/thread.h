#pragma once
//------------------------------------------------------------------------------
/**
    @class Threading::Thread
    
    @todo describe Thread class
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/config.h"
#if (__WIN32__ || __XBOX360__)
#include "threading/win360/win360thread.h"
namespace Threading
{
class Thread : public Win360::Win360Thread
{ 
    __DeclareClass(Thread);
};
}
#elif __WII__
#include "threading/wii/wiithread.h"
namespace Threading
{
class Thread : public Wii::WiiThread
{
    __DeclareClass(Thread);
};
}
#else
#error "Threading::Thread not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
