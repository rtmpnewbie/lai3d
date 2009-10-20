#pragma once
//------------------------------------------------------------------------------
/**
    @class Util::Proxy
    
    A Proxy holds an embedded object of any class and offers compare
    operators which are routed to the target object. Several Proxies 
    may point to the same target object, a refcount is associated with
    the target object, and it will automatically be destroyed when all
    Proxies have gone. Proxies are useful in some situations when
    working with container classes, since they guarantee that the
    target object doesn't move in memory even when the proxy itself
    is moved around by the container class.

    (C) 2007 Radon Labs GmbH
*/
#include "core/types.h"
#include "core/ptr.h"
#include "threading/interlocked.h"

//------------------------------------------------------------------------------
namespace Util
{
template<class TYPE> class Proxy
{
public:
    /// default constructor
    Proxy();
    /// construct from type
    explicit Proxy(const TYPE& rhs);
    /// copy constructor
    explicit Proxy(const Proxy<TYPE>& rhs);

    /// assignment operator from type
    void operator=(const TYPE& rhs);
    /// assignment operator from proxy
    void operator=(const Proxy<TYPE>& rhs);

    /// equality operator with type
    bool operator==(const TYPE& rhs) const;
    /// inequality operator with type
    bool operator!=(const TYPE& rhs) const;
    /// greater-then operator with type
    bool operator>(const TYPE& rhs) const;
    /// less-then operator with type
    bool operator<(const TYPE& rhs) const;
    /// greater-or-equal operator with type
    bool operator>=(const TYPE& rhs) const;
    /// less-or-equal operator with type
    bool operator<=(const TYPE& rhs) const;

    /// equality operator with proxy
    bool operator==(const Proxy<TYPE>& rhs) const;
    /// inequality operator with proxy
    bool operator!=(const Proxy<TYPE>& rhs) const;
    /// greater-then operator with proxy
    bool operator>(const Proxy<TYPE>& rhs) const;
    /// less-then operator with proxy
    bool operator<(const Proxy<TYPE>& rhs) const;
    /// greater-or-equal operator with proxy
    bool operator>=(const Proxy<TYPE>& rhs) const;
    /// less-or-equal operator with proxy
    bool operator<=(const Proxy<TYPE>& rhs) const;

    /// access to contained object
    const TYPE& GetObject() const;
    /// get refcount of object
    int GetObjectRefCount() const;
    /// return true if the proxy contains a valid object
    bool IsValid() const;
    /// clear content
    void Clear();

private:
    /// embedded object associated with refcount (compatible with Ptr<>)
    class Object
    {
    public:
        /// static create method
        static Object* Create(const TYPE& rhs);
        /// constructor
        Object(const TYPE& rhs);
        /// increment refcount
        void AddRef();
        /// decrement refcount, destroy self if refcount reaches zero
        void Release();

        int refCount;
        TYPE obj;
    };

    Ptr<Object> ptr;
};

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
Proxy<TYPE>::Object::Object(const TYPE& rhs) :
    refCount(0),
    obj(rhs)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> typename Proxy<TYPE>::Object*
Proxy<TYPE>::Object::Create(const TYPE& rhs)
{
    return new Object(rhs);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
Proxy<TYPE>::Object::AddRef()
{
    Threading::Interlocked::Increment(this->refCount);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
Proxy<TYPE>::Object::Release()
{
    if (0 == Threading::Interlocked::Decrement(this->refCount))
    {
        n_delete(this);
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
Proxy<TYPE>::Proxy()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    Constructing a proxy from TYPE always creates a new target object.
*/
template<class TYPE>
Proxy<TYPE>::Proxy(const TYPE& rhs)
{
    this->ptr = Object::Create(rhs);
}

//------------------------------------------------------------------------------
/**
    Constructing a proxy from another proxy shares the target object
    of the right-hand-side proxy and increments its refcount.
*/
template<class TYPE>
Proxy<TYPE>::Proxy(const Proxy<TYPE>& rhs)
{
    this->ptr = rhs.ptr;
}

//------------------------------------------------------------------------------
/**
    Assigning a TYPE to the proxy will throw away the previous target
    object and create a new one.
*/
template<class TYPE> void
Proxy<TYPE>::operator=(const TYPE& rhs)
{
    this->ptr = Object::Create(rhs);
}

//------------------------------------------------------------------------------
/**
    Assigning a proxy to this proxy will share the right-hand-side's target
    object and increment its refcount.
*/
template<class TYPE> void
Proxy<TYPE>::operator=(const Proxy<TYPE>& rhs)
{
    this->ptr = rhs.ptr;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> bool
Proxy<TYPE>::operator==(const TYPE& rhs) const
{
    return this->ptr->obj == rhs;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> bool
Proxy<TYPE>::operator!=(const TYPE& rhs) const
{
    return this->ptr->obj != rhs;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> bool
Proxy<TYPE>::operator>(const TYPE& rhs) const
{
    return this->ptr->obj > rhs;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> bool
Proxy<TYPE>::operator<(const TYPE& rhs) const
{
    return this->ptr->obj < rhs;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> bool
Proxy<TYPE>::operator>=(const TYPE& rhs) const
{
    return this->ptr->obj >= rhs;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> bool
Proxy<TYPE>::operator<=(const TYPE& rhs) const
{
    return this->ptr->obj <= rhs;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> bool
Proxy<TYPE>::operator==(const Proxy<TYPE>& rhs) const
{
    // shortcut
    if (this->ptr == rhs.ptr)
    {
        return true;
    }
    else
    {
        return this->ptr->obj == rhs.ptr->obj;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> bool
Proxy<TYPE>::operator!=(const Proxy<TYPE>& rhs) const
{
    // shortcut
    if (this->ptr == rhs.ptr)
    {
        return false;
    }
    else
    {
        return this->ptr->obj != rhs.ptr->obj;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> bool
Proxy<TYPE>::operator>(const Proxy<TYPE>& rhs) const
{
    return this->ptr->obj > rhs.ptr->obj;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> bool
Proxy<TYPE>::operator<(const Proxy<TYPE>& rhs) const
{
    return this->ptr->obj < rhs.ptr->obj;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> bool
Proxy<TYPE>::operator>=(const Proxy<TYPE>& rhs) const
{
    return this->ptr->obj >= rhs.ptr->obj;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> bool
Proxy<TYPE>::operator<=(const Proxy<TYPE>& rhs) const
{
    return this->ptr->obj <= rhs.ptr->obj;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> const TYPE&
Proxy<TYPE>::GetObject() const
{
    return this->ptr->obj;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> int
Proxy<TYPE>::GetObjectRefCount() const
{
    return this->ptr->refCount;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> bool
Proxy<TYPE>::IsValid() const
{
    return this->ptr.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
Proxy<TYPE>::Clear()
{
    this->ptr = 0;
}

} // namespace Util
//------------------------------------------------------------------------------
