#pragma once
//------------------------------------------------------------------------------
/**
    @class Memory::MemoryPool
    
    A simple thread-safe memory pool. Memory pool items are 16-byte aligned.

    (C) 2009 Radon Labs GmbH
*/
#include "core/config.h"
#if (__XBOX360__ || __WIN32__)
#include "memory/win360/win360memorypool.h"
namespace Memory
{
typedef Win360::Win360MemoryPool MemoryPool;
}
#elif __WII__
#include "memory/wii/wiimemorypool.h"
namespace Memory
{
typedef Wii::WiiMemoryPool MemoryPool;
}
#else
#error "IMPLEMENT ME!"
#endif
    