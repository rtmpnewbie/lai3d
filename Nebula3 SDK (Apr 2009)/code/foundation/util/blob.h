#pragma once
//------------------------------------------------------------------------------
/**
    @class Util::Blob
    
    The Util::Blob class encapsulates a chunk of raw memory into 
    a C++ object which can be copied, compared and hashed.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/types.h"
#include "memory/heap.h"

//------------------------------------------------------------------------------
namespace Util
{
class Blob
{
public:
    /// static Setup method, called by SysFunc::Setup()
    static void Setup();
    /// static Shutdown method called by SysFunc::Exit
    static void Shutdown();
    /// overloaded operator new
    void* operator new(size_t size);
    /// overloaded operator delete
    void operator delete(void* p);

    /// default constructor
    Blob();
    /// constructor
    Blob(const void* ptr, SizeT size);
    /// reserve N bytes
    Blob(SizeT size);
    /// copy constructor
    Blob(const Blob& rhs);
    /// destructor
    ~Blob();
    /// assignment operator
    void operator=(const Blob& rhs);
    
    /// equality operator
    bool operator==(const Blob& rhs) const;
    /// inequality operator
    bool operator!=(const Blob& rhs) const;
    /// greater operator
    bool operator>(const Blob& rhs) const;
    /// less operator
    bool operator<(const Blob& rhs) const;
    /// greater-equal operator
    bool operator>=(const Blob& rhs) const;
    /// less-eqial operator
    bool operator<=(const Blob& rhs) const;
    
    /// return true if the blob contains data
    bool IsValid() const;
    /// reserve N bytes
    void Reserve(SizeT size);
    /// trim the size member (without re-allocating!)
    void Trim(SizeT size);
    /// set blob contents
    void Set(const void* ptr, SizeT size);
    /// get blob ptr
    void* GetPtr() const;
    /// get blob size
    SizeT Size() const;
    /// get a hash code (compatible with Util::HashTable)
    IndexT HashCode() const;

private:
    /// delete content
    void Delete();
    /// allocate internal buffer
    void Allocate(SizeT size);
    /// copy content
    void Copy(const void* ptr, SizeT size);
    /// do a binary comparison between this and other blob
    int BinaryCompare(const Blob& rhs) const;

    static Memory::Heap* DataHeap;
    static Memory::Heap* ObjectHeap;

    void* ptr;
    SizeT size;
    SizeT allocSize;
};

//------------------------------------------------------------------------------
/**
*/
inline void
Blob::Setup()
{
    n_assert(0 == DataHeap);
    n_assert(0 == ObjectHeap);
    ObjectHeap = n_new(Memory::Heap("Util.Blob.ObjectHeap"));
    DataHeap = n_new(Memory::Heap("Util.Blob.DataHeap"));
}

//------------------------------------------------------------------------------
/**
*/
inline void
Blob::Shutdown()
{
    n_assert(0 != DataHeap);
    n_assert(0 != ObjectHeap);
    n_delete(ObjectHeap);
    n_delete(DataHeap);
    ObjectHeap = 0;
    DataHeap = 0;
}

//------------------------------------------------------------------------------
/**
*/
inline void*
Blob::operator new(size_t size)
{
    n_assert(0 != ObjectHeap);
    return ObjectHeap->Alloc(size);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Blob::operator delete(void* p)
{
    n_assert(0 != ObjectHeap);
    ObjectHeap->Free(p);
}

//------------------------------------------------------------------------------
/**
*/
inline
Blob::Blob() :
    ptr(0),
    size(0),
    allocSize(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Blob::IsValid() const
{
    return (0 != this->ptr);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Blob::Delete()
{
    if (this->IsValid())
    {
        n_assert(0 != DataHeap);
        DataHeap->Free((void*)this->ptr);
        this->ptr = 0;
        this->size = 0;
        this->allocSize = 0;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline
Blob::~Blob()
{
    this->Delete();
}

//------------------------------------------------------------------------------
/**
*/
inline void
Blob::Allocate(SizeT s)
{
    n_assert(!this->IsValid());
    n_assert(0 != DataHeap);
    this->ptr = DataHeap->Alloc(s);
    this->allocSize = s;
    this->size = s;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Blob::Copy(const void* fromPtr, SizeT fromSize)
{
    n_assert((0 != fromPtr) && (fromSize > 0));

    // only re-allocate if not enough space
    if ((0 == this->ptr) || (this->allocSize < fromSize))
    {
        this->Delete();
        this->Allocate(fromSize);
    }
    this->size = fromSize;
    Memory::Copy(fromPtr, (void*) this->ptr, fromSize);
}

//------------------------------------------------------------------------------
/**
*/
inline
Blob::Blob(const void* fromPtr, SizeT fromSize) :
    ptr(0),
    size(0),
    allocSize(0)
{    
    this->Copy(fromPtr, fromSize);
}

//------------------------------------------------------------------------------
/**
*/
inline
Blob::Blob(const Blob& rhs) :
    ptr(0),
    size(0),
    allocSize(0)
{
    if (rhs.IsValid())
    {
        this->Copy(rhs.ptr, rhs.size);
    }
}

//------------------------------------------------------------------------------
/**
*/
inline
Blob::Blob(SizeT s) :
    ptr(0),
    size(0),
    allocSize(0)
{
    this->Allocate(s);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Blob::operator=(const Blob& rhs)
{
    if (rhs.IsValid())
    {
        this->Copy(rhs.ptr, rhs.size);
    }
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Blob::operator==(const Blob& rhs) const
{
    return (this->BinaryCompare(rhs) == 0);
}
            
//------------------------------------------------------------------------------
/**
*/
inline bool
Blob::operator!=(const Blob& rhs) const
{
    return (this->BinaryCompare(rhs) != 0);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Blob::operator>(const Blob& rhs) const
{
    return (this->BinaryCompare(rhs) > 0);
}
            
//------------------------------------------------------------------------------
/**
*/
inline bool
Blob::operator<(const Blob& rhs) const
{
    return (this->BinaryCompare(rhs) < 0);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Blob::operator>=(const Blob& rhs) const
{
    return (this->BinaryCompare(rhs) >= 0);
}
            
//------------------------------------------------------------------------------
/**
*/
inline bool
Blob::operator<=(const Blob& rhs) const
{
    return (this->BinaryCompare(rhs) <= 0);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Blob::Reserve(SizeT s)
{
    if (this->allocSize < s)
    {
        this->Delete();
        this->Allocate(s);
    }
    this->size = s;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Blob::Trim(SizeT trimSize)
{
    n_assert(trimSize <= this->size);
    this->size = trimSize;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Blob::Set(const void* fromPtr, SizeT fromSize)
{
    this->Copy(fromPtr, fromSize);
}

//------------------------------------------------------------------------------
/**
*/
inline void*
Blob::GetPtr() const
{
    n_assert(this->IsValid());
    return this->ptr;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
Blob::Size() const
{
    n_assert(this->IsValid());
    return this->size;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT
Blob::HashCode() const
{
    IndexT hash = 0;
    const char* charPtr = (const char*) this->ptr;
    IndexT i;
    for (i = 0; i < this->size; i++)
    {
        hash += charPtr[i];
        hash += hash << 10;
        hash ^= hash >>  6;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    hash &= ~(1<<31);       // don't return a negative number (in case IndexT is defined signed)
    return hash;
}

} // namespace Util
//------------------------------------------------------------------------------
    