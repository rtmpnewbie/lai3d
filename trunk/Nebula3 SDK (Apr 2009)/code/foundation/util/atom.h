#pragma once
//------------------------------------------------------------------------------
/**
    @class Util::Atom
  
    An Atom is a compact, shared reference of a constant object. A unique
    object is guaranteed to exist only once, no matter how many Atoms are
    pointing to it. Copying Atoms and comparing against Atoms is very fast,
    since these are actually pointer operations.

    Note that comparing 2 Atoms against each other will yield a different
    result then comparing the contents of the same Atoms. This is because
    comparing 2 Atoms actually compares the addresses of the content without
    actually invoking the compare operators of the content. So, comparing
    2 Atom<String>'s will never do a strcmp(), instead the string pointers
    are compared against each other.

    Atoms are consistent across threads.

    The Atom table will *not* be garbage collected automatically, this
    means, entries for which no more Atoms exist will not be removed
    automatically. This is to prevent excessive allocations/deallocations
    for some usage scenarios. Instead, if you want to remove orphaned
    entries, call the PerformGarbageCollection() method manually.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/types.h"
#include "util/proxy.h"
#include "util/array.h"
#include "threading/criticalsection.h"

//------------------------------------------------------------------------------
namespace Util
{
template<class TYPE> class Atom
{
public:
    /// default constructor
    Atom();
    /// construct from type
    Atom(const TYPE& rhs);
    /// copy constructor
    Atom(const Atom<TYPE>& rhs);
    /// destructor
    ~Atom();

    /// assignment from type
    void operator=(const TYPE& rhs);
    /// assignment from other atom
    void operator=(const Atom<TYPE>& rhs);

    /// equality operator with atom content
    bool operator==(const TYPE& rhs) const;
    /// inequality operator with atom content
    bool operator!=(const TYPE& rhs) const;
    /// greater-then operator with atom content
    bool operator>(const TYPE& rhs) const;
    /// less-then operator with atom content
    bool operator<(const TYPE& rhs) const;
    /// greater-or-equal operator with atom content
    bool operator>=(const TYPE& rhs) const;
    /// less-or-equal operator with atom content
    bool operator<=(const TYPE& rhs) const;

    /// equality operator between atoms
    bool operator==(const Atom<TYPE>& rhs) const;
    /// inequality operator between atoms
    bool operator!=(const Atom<TYPE>& rhs) const;
    /// greater-then operator between atoms
    bool operator>(const Atom<TYPE>& rhs) const;
    /// less-then operator between atoms
    bool operator<(const Atom<TYPE>& rhs) const;
    /// greater-or-equal operator between atoms
    bool operator>=(const Atom<TYPE>& rhs) const;
    /// less-or-equal operator between atoms
    bool operator<=(const Atom<TYPE>& rhs) const;

    /// clear content
    void Clear();
    /// return true if the Atom is valid
    bool IsValid() const;
    /// access to referenced object
    const TYPE& Value() const;

    /// perform garbage collection on atom table
    static void PerformGarbageCollection();
    /// get current atom table size (for debugging)
    static SizeT GetAtomTableSize();

private:
    /// own binary search, saves on implicitely created object
    IndexT BinarySearch(const TYPE& elm) const;

    static Threading::CriticalSection critSect;
    static Array<Proxy<TYPE> > atomTable;
    Proxy<TYPE> proxy;
};

template<class TYPE> Threading::CriticalSection Atom<TYPE>::critSect;
template<class TYPE> Array<Proxy<TYPE> > Atom<TYPE>::atomTable;

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
Atom<TYPE>::Atom()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
Atom<TYPE>::Atom(const TYPE& rhs)
{
    critSect.Enter();
    IndexT i = this->BinarySearch(rhs);
    if (InvalidIndex == i)
    {
        // value doesn't exist yet, create new
        this->proxy = rhs;
        atomTable.InsertSorted(this->proxy);
    }
    else
    {
        // value already exists
        this->proxy = atomTable[i];
    }
    critSect.Leave();
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
Atom<TYPE>::Atom(const Atom<TYPE>& rhs)
{
    // copy from existing atom
    this->proxy = rhs.proxy;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
Atom<TYPE>::~Atom()
{
    this->proxy.Clear();
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
Atom<TYPE>::operator=(const TYPE& rhs)
{
    critSect.Enter();
    IndexT i = this->BinarySearch(rhs);
    if (InvalidIndex == i)
    {
        // value doesn't exist yet, create new
        this->proxy = rhs;
        atomTable.InsertSorted(this->proxy);
    }
    else
    {
        // value already exists
        this->proxy = atomTable[i];
    }
    critSect.Leave();
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
Atom<TYPE>::operator=(const Atom<TYPE>& rhs)
{
    this->proxy = rhs.proxy;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> bool
Atom<TYPE>::operator==(const TYPE& rhs) const
{
    return this->proxy.GetObject() == rhs;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> bool
Atom<TYPE>::operator!=(const TYPE& rhs) const
{
    return this->proxy.GetObject() != rhs;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> bool
Atom<TYPE>::operator>(const TYPE& rhs) const
{
    return this->proxy.GetObject() > rhs;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> bool
Atom<TYPE>::operator<(const TYPE& rhs) const
{
    return this->proxy.GetObject() < rhs;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> bool
Atom<TYPE>::operator>=(const TYPE& rhs) const
{
    return this->proxy.GetObject() >= rhs;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> bool
Atom<TYPE>::operator<=(const TYPE& rhs) const
{
    return this->proxy.GetObject() <= rhs;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> bool
Atom<TYPE>::operator==(const Atom<TYPE>& rhs) const
{
    return &(this->proxy.GetObject()) == &(rhs.proxy.GetObject());
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> bool
Atom<TYPE>::operator!=(const Atom<TYPE>& rhs) const
{
    return &(this->proxy.GetObject()) != &(rhs.proxy.GetObject());
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> bool
Atom<TYPE>::operator>(const Atom<TYPE>& rhs) const
{
    return &(this->proxy.GetObject()) > &(rhs.proxy.GetObject());
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> bool
Atom<TYPE>::operator<(const Atom<TYPE>& rhs) const
{
    return &(this->proxy.GetObject()) < &(rhs.proxy.GetObject());
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> bool
Atom<TYPE>::operator>=(const Atom<TYPE>& rhs) const
{
    return &(this->proxy.GetObject()) >= &(rhs.proxy.GetObject());
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> bool
Atom<TYPE>::operator<=(const Atom<TYPE>& rhs) const
{
    return &(this->proxy.GetObject()) <= &(rhs.proxy.GetObject());
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
Atom<TYPE>::Clear()
{
    this->proxy.Clear();
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> bool
Atom<TYPE>::IsValid() const
{
    return this->proxy.IsValid();
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> const TYPE&
Atom<TYPE>::Value() const
{
    return this->proxy.GetObject();
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
Atom<TYPE>::PerformGarbageCollection()
{
    critSect.Enter();
    Array<Proxy<TYPE> >::Iterator iter;
    for (iter = atomTable.Begin(); iter != atomTable.End();)
    {
        if ((*iter).GetObjectRefCount() == 1)
        {
            // no more Atoms exist for this entry
            iter = atomTable.Erase(iter);
        }
        else
        {
            iter++;
        }
    }
    critSect.Leave();
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> SizeT
Atom<TYPE>::GetAtomTableSize()
{
    return atomTable.Size();
}

//------------------------------------------------------------------------------
/**
    Specialized version of Array::BinarySearchIndex() which searches
    the atom table directly by TYPE, not by Proxy<TYPE>. This saves an
    implicit construction of a Proxy object.

    NOTE: This method depends on the critical section being taken. Since
    the method is private, this is not a problem (the caller must care 
    about this).
*/
template<class TYPE> IndexT
Atom<TYPE>::BinarySearch(const TYPE& elm) const
{
    SizeT num = atomTable.Size();
    if (num > 0)
    {
        IndexT half;
        IndexT lo = 0;
	    IndexT hi = num - 1;
	    IndexT mid;
        while (lo <= hi) 
        {
            if (0 != (half = num/2)) 
            {
                mid = lo + ((num & 1) ? half : (half - 1));
                if (atomTable[mid] > elm)
                {
                    hi = mid - 1;
                    num = num & 1 ? half : half - 1;
                } 
                else if (atomTable[mid] < elm) 
                {
                    lo = mid + 1;
                    num = half;
                } 
                else
                {
                    return mid;
                }
            } 
            else if (num) 
            {
                if (atomTable[lo] != elm)
                {
                    return InvalidIndex;
                }
                else      
                {
                    return lo;
                }
            } 
            else 
            {
                break;
            }
        }
    }
    return InvalidIndex;
}

} // namespace Util
//------------------------------------------------------------------------------
