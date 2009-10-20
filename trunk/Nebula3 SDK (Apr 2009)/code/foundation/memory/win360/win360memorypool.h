#pragma once
//------------------------------------------------------------------------------
/**
    @class Win360::Win360MemoryPool
    
    A simple thread-safe memory pool. Memory pool items are 16-byte aligned.
    
    FIXME:
    - debug: overwrite memory blocks with pattern 
    - debug: check for double-free
    - debug: check for mem-leaks
    - debug: list memory pools in Debug HTML page!

    (C) 2009 Radon Labs GmbH
*/
#include "core/types.h"

//------------------------------------------------------------------------------
namespace Win360
{
class Win360MemoryPool
{
public:
    /// constructor
    Win360MemoryPool(const char* name, uint blockSize, uint numBlocks);
    /// destructor
    ~Win360MemoryPool();
    /// allocate a block from the pool
    void* Alloc();
    /// deallocate a block from the pool
    void Free(void* ptr);
    /// return true if block is owned by this pool
    bool IsPoolBlock(void* ptr) const;
    /// get memory pool name
    const char* GetName() const;
    /// get number of allocated blocks in pool
    uint GetNumBlocks() const;
    /// get block size
    uint GetBlockSize() const;
    /// get aligned block size
    uint GetAlignedBlockSize() const;
    /// get pool size
    uint GetPoolSize() const;
    
private:
    static const uint FreeBlockPattern = 0xDEADBEEF;
    static const uint NewBlockPattern = 0xDEADF00D;
    static const uint MaxNumBlocks = 0xFFFFFFFF;

    const char* name;
    uint blockSize;
    uint alignedBlockSize;
    uint poolSize;
    uint numBlocks;
    
    SLIST_HEADER listHead;
    ubyte* poolStart;       // first valid block header is at poolStart + (align - sizeof(SLIST_ENTRY))
    ubyte* poolEnd;
};

//------------------------------------------------------------------------------
/**
*/
inline const char*
Win360MemoryPool::GetName() const
{
    return this->name;
}

//------------------------------------------------------------------------------
/**
*/
inline uint
Win360MemoryPool::GetNumBlocks() const
{
    return this->numBlocks;
}

//------------------------------------------------------------------------------
/**
*/
inline uint
Win360MemoryPool::GetBlockSize() const
{
    return this->blockSize;
}

//------------------------------------------------------------------------------
/**
*/
inline uint
Win360MemoryPool::GetAlignedBlockSize() const
{
    return this->alignedBlockSize;
}

//------------------------------------------------------------------------------
/**
*/
inline uint
Win360MemoryPool::GetPoolSize() const
{
    return this->poolSize;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Win360MemoryPool::IsPoolBlock(void* ptr) const
{
    return (ptr >= this->poolStart) && (ptr < this->poolEnd);
}

} // namespace Win360
//------------------------------------------------------------------------------
    