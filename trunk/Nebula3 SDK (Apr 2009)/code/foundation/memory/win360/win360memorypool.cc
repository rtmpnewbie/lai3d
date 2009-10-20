//------------------------------------------------------------------------------
//  win360memorypool.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "memory/win360/win360memorypool.h"

namespace Win360
{

//------------------------------------------------------------------------------
/**
    NOTE: name must be a static string!
*/
Win360MemoryPool::Win360MemoryPool(const char* name_, uint blockSize_, uint numBlocks_)
{
    n_assert(numBlocks_ < MaxNumBlocks);
    this->name = name_;
    this->blockSize = blockSize_;
    this->numBlocks = numBlocks_;

    // compute block size with sizeof(*) header
    const int align = 16;
    uint blockSizeWithHeader = this->blockSize + sizeof(PSLIST_ENTRY);
    uint padding = (align - (blockSizeWithHeader % align)) % align;
    this->alignedBlockSize = blockSizeWithHeader + padding;
    this->poolSize = (this->numBlocks * this->alignedBlockSize) + align;

    // setup pool memory block, each entry in the block has a header
    // with a pointer to the next block, the actual start of the memory block
    // is 16-byte-aligned
    this->poolStart = (ubyte*) Memory::Alloc(Memory::PoolHeap, this->poolSize);
    this->poolEnd = this->poolStart + this->poolSize;
    
    // setup forward-linked free-block-list
    Memory::Clear(&this->listHead, sizeof(listHead));
    PSLIST_ENTRY listEntry = (PSLIST_ENTRY) (this->poolStart + (align - sizeof(SLIST_ENTRY)));
    uint i;
    for (i = 0; i < this->numBlocks; i++)
    {
        InterlockedPushEntrySList(&this->listHead, listEntry);
        listEntry += this->alignedBlockSize / sizeof(SLIST_ENTRY);
    }
}

//------------------------------------------------------------------------------
/**
*/
Win360MemoryPool::~Win360MemoryPool()
{
    // discard memory pool
    Memory::Free(Memory::PoolHeap, this->poolStart);
    this->poolStart = 0;
    this->poolEnd = 0;
}

//------------------------------------------------------------------------------
/**
*/
void*
Win360MemoryPool::Alloc()
{
    // get the next free block from the free list and fixup the free list
    PSLIST_ENTRY entry = InterlockedPopEntrySList(&this->listHead);
    void* ptr = (void*) (((ubyte*)entry) + sizeof(SLIST_ENTRY));
    return ptr;
}

//------------------------------------------------------------------------------
/**
*/
void
Win360MemoryPool::Free(void* ptr)
{
    // get pointer to header and fixup free list
    PSLIST_ENTRY entry = (PSLIST_ENTRY) (((ubyte*)ptr) - sizeof(SLIST_ENTRY));
    InterlockedPushEntrySList(&this->listHead, entry);
}

} // namespace Win360
