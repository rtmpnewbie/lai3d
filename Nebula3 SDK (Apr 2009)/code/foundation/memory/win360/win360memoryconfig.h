#pragma once
//------------------------------------------------------------------------------
/**
    @file memory/win360/win360memoryconfig.h
    
    Central config file for memory setup on the Win32 and Xbox360 platform.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/config.h"

namespace Memory
{

//------------------------------------------------------------------------------
/**
    Heap types are defined here. The main purpose for the different heap
    types is to decrease memory fragmentation and to improve cache
    usage by grouping similar data together. Platform ports may define
    platform-specific heap types, as long as only platform specific
    code uses those new heap types.
*/
enum HeapType
{
    DefaultHeap = 0,            // for stuff that doesn't fit into any category
    ObjectHeap,                 // standard heap for C++ objects
    ObjectArrayHeap,            // for objects that use the array new/delete operator
    ResourceHeap,               // heap for resource data (like animation buffers)
    ScratchHeap,                // for short-lived scratch memory (encode/decode buffers, etc...)
    StringDataHeap,             // special heap for string data
    StringObjectHeap,           // special heap for string objects
    StreamDataHeap,             // special heap for stream data like memory streams, zip file streams, etc...
    PoolHeap,                   // special heap for all allocations going through MemoryPools 
    PhysicsHeap,                // physics engine allocations go here
    AppHeap,                    // for general Application layer stuff
        
    Xbox360GraphicsHeap,        // defines special Xbox360 graphical memory
    Xbox360AudioHeap,           // defines special Xbox360 audio memory
    
    NumHeapTypes,
};

//------------------------------------------------------------------------------
/**
    Heap pointers are defined here. Call ValidateHeap() to check whether
    a heap already has been setup, and to setup the heap if not.
*/
extern HANDLE volatile Heaps[NumHeapTypes];

//------------------------------------------------------------------------------
/**
    This method is called by SysFunc::Setup() to setup the different heap
    types. This method can be tuned to define the start size of the 
    heaps and whether the heap may grow or not (non-growing heaps may
    be especially useful on console platforms without memory paging).
*/
extern void SetupHeaps();

//------------------------------------------------------------------------------
/**
    Returns a human readable name for a heap type.
*/
extern const char* GetHeapTypeName(HeapType heapType);

} // namespace Memory    
//------------------------------------------------------------------------------
