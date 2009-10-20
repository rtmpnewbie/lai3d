#pragma once
//------------------------------------------------------------------------------
/**
    @class Win32::Win32Guid
    
    Win32 implementation of the Util::Guid class. GUIDs can be
    compared and provide a hash code, so they can be used as keys in
    most collections.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/types.h"
#include "util/string.h"
#include "core/sysfunc.h"

//------------------------------------------------------------------------------
namespace Win32
{
class Win32Guid
{
public:
    /// static setup method, called by SysFunc::Setup
    static void Setup();
    /// static shutdown method, called by SysFunc::Exit
    static void Shutdown();
    /// overloaded operator new
    void* operator new(size_t size);
    /// overloaded operator delete
    void operator delete(void* p);

    /// constructor
    Win32Guid();
    /// copy constructor
    Win32Guid(const Win32Guid& rhs);
    /// construct from raw binary data as returned by AsBinary()
    Win32Guid(const unsigned char* ptr, SizeT size);
    /// assignement operator
    void operator=(const Win32Guid& rhs);
    /// assignment operator from string
    void operator=(const Util::String& rhs);
    /// equality operator
    bool operator==(const Win32Guid& rhs) const;
    /// inequlality operator
    bool operator!=(const Win32Guid& rhs) const;
    /// less-then operator
    bool operator<(const Win32Guid& rhs) const;
    /// less-or-equal operator
    bool operator<=(const Win32Guid& rhs) const;
    /// greater-then operator
    bool operator>(const Win32Guid& rhs) const;
    /// greater-or-equal operator
    bool operator>=(const Win32Guid& rhs) const;
    /// return true if the contained guid is valid (not NIL)
    bool IsValid() const;
    /// generate a new guid
    void Generate();
    /// construct from string representation
    static Win32Guid FromString(const Util::String& str);
    /// construct from binary representation
    static Win32Guid FromBinary(const unsigned char* ptr, SizeT numBytes);
    /// get as string
    Util::String AsString() const;
    /// get pointer to binary data
    SizeT AsBinary(const unsigned char*& outPtr) const;
    /// get a hash code (compatible with Util::HashTable)
    IndexT HashCode() const;

private:
    static Memory::Heap* ObjectHeap;
    UUID uuid;
};

//------------------------------------------------------------------------------
/**
*/
inline void
Win32Guid::Setup()
{
    n_assert(0 == ObjectHeap);
    ObjectHeap = n_new(Memory::Heap("Util.Win32Guid.ObjectHeap"));
}

//------------------------------------------------------------------------------
/**
*/
inline void
Win32Guid::Shutdown()
{
    n_assert(0 != ObjectHeap);
    n_delete(ObjectHeap);
    ObjectHeap = 0;
}

//------------------------------------------------------------------------------
/**
*/
inline
void*
Win32Guid::operator new(size_t size)
{
    Core::SysFunc::Setup();
    n_assert(0 != ObjectHeap);
    return ObjectHeap->Alloc(size);
}

//------------------------------------------------------------------------------
/**
*/
inline
void
Win32Guid::operator delete(void* p)
{
    n_assert(0 != ObjectHeap);
    ObjectHeap->Free(p);
}

//------------------------------------------------------------------------------
/**
*/
inline
Win32Guid::Win32Guid()
{
    Memory::Clear(&(this->uuid), sizeof(this->uuid));
}

//------------------------------------------------------------------------------
/**
*/
inline
Win32Guid::Win32Guid(const Win32Guid& rhs)
{
    this->uuid = rhs.uuid;
}

//------------------------------------------------------------------------------
/**
*/
inline
Win32Guid::Win32Guid(const unsigned char* ptr, SizeT size)
{
    n_assert((0 != ptr) && (size == sizeof(UUID)));
    Memory::Copy(ptr, &this->uuid, sizeof(UUID));
}

} // namespace Win32
//------------------------------------------------------------------------------
