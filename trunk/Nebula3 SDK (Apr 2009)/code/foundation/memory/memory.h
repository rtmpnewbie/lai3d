#pragma once
//------------------------------------------------------------------------------
/**
    @class Memory::Memory
  
    Implements a private heap.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/config.h"

#if (__WIN32__ || __XBOX360__)
#include "memory/win360/win360memory.h"
#elif __WII__
#include "memory/wii/wiimemory.h"
#else
    #error "UNKNOWN PLATFORM"
#endif
