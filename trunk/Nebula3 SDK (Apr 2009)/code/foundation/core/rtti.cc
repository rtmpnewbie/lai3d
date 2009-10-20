//------------------------------------------------------------------------------
//  rtti.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "core/rtti.h"
#include "core/refcounted.h"
#include "core/sysfunc.h"

namespace Core
{
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
Rtti::Rtti(const char* className, FourCC fcc, Creator creatorFunc, const Rtti* parentClass, SizeT instSize) :
    fourCC(fcc),
    parent(parentClass),
    creator(creatorFunc),
    instanceSize(instSize)
{
    // make sure String, etc... is working correctly
    Core::SysFunc::Setup();

    n_assert(0 != className);
    n_assert(fourCC.IsValid() != 0);
    n_assert(parentClass != this);

    // register class with factory
    this->name = className;
    if (!Factory::Instance()->ClassExists(fcc))
    {
        Factory::Instance()->Register(this, this->name, fcc);        
    }
    
    // make a debug check that no name/fcc collission occured, but only in debug mode
    #if NEBULA3_DEBUG
    const Rtti* checkRtti = Factory::Instance()->GetClassRtti(fcc);
    n_assert(0 != checkRtti);
    if (checkRtti != this)
    {
        String err;
        err.Format("Class registry collision: (%s, %s) collides with (%s, %s)!",
            this->name.AsCharPtr(), this->fourCC.AsString().AsCharPtr(),
            checkRtti->name.AsCharPtr(), checkRtti->fourCC.AsString().AsCharPtr());
        n_error(err.AsCharPtr());
    }
    #endif
}

//------------------------------------------------------------------------------
/**
*/
RefCounted*
Rtti::Create() const
{
    if (0 == this->creator)
    {
        n_error("Rtti::Create(): Trying to create instance of abstract class '%s'!", this->name.AsCharPtr());
        return 0;
    }
    else
    {
        return this->creator();
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
Rtti::IsDerivedFrom(const Rtti& other) const
{
    const Rtti* cur;
    for (cur = this; cur != 0; cur = cur->GetParent())
    {
        if (cur == &other)
        {
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
Rtti::IsDerivedFrom(const Util::String& otherClassName) const
{
    const Rtti* cur;
    for (cur = this; cur != 0; cur = cur->GetParent())
    {
        if (cur->name == otherClassName)
        {
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
Rtti::IsDerivedFrom(const Util::FourCC& otherClassFourCC) const
{
    const Rtti* cur;
    for (cur = this; cur != 0; cur = cur->GetParent())
    {
        if (cur->fourCC == otherClassFourCC)
        {
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void*
Rtti::AllocInstanceMemory()
{
    n_assert(this->instanceSize > 0);
    return Memory::Alloc(Memory::ObjectHeap, this->instanceSize);
}

//------------------------------------------------------------------------------
/**
*/
void
Rtti::FreeInstanceMemory(void* ptr)
{
    n_assert(this->instanceSize > 0);
    Memory::Free(Memory::ObjectHeap, ptr);
}

} // namespace Core
