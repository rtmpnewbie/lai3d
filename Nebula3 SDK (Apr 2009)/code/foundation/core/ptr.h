#pragma once
//------------------------------------------------------------------------------
/**
    @class Ptr

    Nebula3's smart pointer class which manages the life time of RefCounted
    objects. Can be used like a normal C++ pointer in most cases.

    NOTE: the Ptr class is not part of the Core namespace for
    convenience reasons.

    (C) 2006 RadonLabs GmbH
*/
#include "core/types.h"
//------------------------------------------------------------------------------
template<class TYPE>
class Ptr
{
public:
    /// constructor
    Ptr();
    /// construct from C++ pointer
    Ptr(TYPE* p);
    /// construct from smart pointer
    Ptr(const Ptr<TYPE>& p);
    /// destructor
    ~Ptr();
    /// assignment operator
    void operator=(const Ptr<TYPE>& rhs);
    /// assignment operator
    void operator=(TYPE* rhs);
    /// equality operator
    bool operator==(const Ptr<TYPE>& rhs) const;
    /// inequality operator
    bool operator!=(const Ptr<TYPE>& rhs) const;
    /// shortcut equality operator
    bool operator==(const TYPE* rhs) const;
    /// shortcut inequality operator
    bool operator!=(const TYPE* rhs) const;
    /// safe -> operator
    TYPE* operator->() const;
    /// safe dereference operator
    TYPE& operator*() const;
    /// safe pointer cast operator
    operator TYPE*() const;
    /// safe downcast operator to other smart pointer
    template<class OTHERTYPE> const Ptr<OTHERTYPE>& downcast() const;
    /// safe upcast operator to other smart pointer
    template<class OTHERTYPE> const Ptr<OTHERTYPE>& upcast() const;
    /// unsafe(!) cast to anything
    template<class OTHERTYPE> const Ptr<OTHERTYPE>& cast() const;
    /// check if pointer is valid
    bool isvalid() const;
    /// return direct pointer (asserts if null pointer)
    TYPE* get() const;
    /// return direct pointer (returns null pointer)
    TYPE* get_unsafe() const;

private:
    TYPE* ptr;
};

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
Ptr<TYPE>::Ptr() : 
    ptr(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
Ptr<TYPE>::Ptr(TYPE* p) : 
    ptr(p)
{
    if (0 != this->ptr)
    {
        this->ptr->AddRef();
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
Ptr<TYPE>::Ptr(const Ptr<TYPE>& p) : 
    ptr(p.ptr)
{
    if (0 != this->ptr)
    {
        this->ptr->AddRef();
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
Ptr<TYPE>::~Ptr()
{
    if (0 != this->ptr)
    {
        this->ptr->Release();
        this->ptr = 0;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void
Ptr<TYPE>::operator=(const Ptr<TYPE>& rhs)
{
    if (this->ptr != rhs.ptr)
    {
        if (this->ptr)
        {
            this->ptr->Release();
        }
        this->ptr = rhs.ptr;
        if (this->ptr)
        {
            this->ptr->AddRef();
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void
Ptr<TYPE>::operator=(TYPE* rhs)
{
    if (this->ptr != rhs)
    {
        if (this->ptr)
        {
            this->ptr->Release();
        }
        this->ptr = rhs;
        if (this->ptr)
        {
            this->ptr->AddRef();
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
bool
Ptr<TYPE>::operator==(const Ptr<TYPE>& rhs) const
{
    return (this->ptr == rhs.ptr);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
bool
Ptr<TYPE>::operator!=(const Ptr<TYPE>& rhs) const
{
    return (this->ptr != rhs.ptr);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
bool
Ptr<TYPE>::operator==(const TYPE* rhs) const
{
    return (this->ptr == rhs);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
bool
Ptr<TYPE>::operator!=(const TYPE* rhs) const
{
    return (this->ptr != rhs);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
TYPE*
Ptr<TYPE>::operator->() const
{
    n_assert2(this->ptr, "NULL pointer access in Ptr::operator->()!");
    return this->ptr;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
TYPE&
Ptr<TYPE>::operator*() const
{
    n_assert2(this->ptr, "NULL pointer access in Ptr::operator*()!");
    return *this->ptr;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
Ptr<TYPE>::operator TYPE*() const
{
    n_assert2(this->ptr, "NULL pointer access in Ptr::operator TYPE*()!");
    return this->ptr;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
template<class OTHERTYPE> const Ptr<OTHERTYPE>&
Ptr<TYPE>::downcast() const
{
    // only allow casts to base classes
    n_assert(OTHERTYPE::RTTI.IsDerivedFrom(TYPE::RTTI));
    return *(Ptr<OTHERTYPE>*)this;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
template<class OTHERTYPE> const Ptr<OTHERTYPE>&
Ptr<TYPE>::upcast() const
{
    // only allow casts to base classes
    n_assert(TYPE::RTTI.IsDerivedFrom(OTHERTYPE::RTTI));
    return *(Ptr<OTHERTYPE>*)this;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
template<class OTHERTYPE> const Ptr<OTHERTYPE>&
Ptr<TYPE>::cast() const
{
    // note: this is an unsafe cast!
    return *(Ptr<OTHERTYPE>*)this;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
bool 
Ptr<TYPE>::isvalid() const
{
    return (0 != this->ptr);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
TYPE*
Ptr<TYPE>::get() const
{
    n_assert2(this->ptr, "NULL pointer access in Ptr::get()!");
    return this->ptr;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
TYPE*
Ptr<TYPE>::get_unsafe() const
{
    return this->ptr;
}
//------------------------------------------------------------------------------



    
    