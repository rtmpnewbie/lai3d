//------------------------------------------------------------------------------
//  resourcemapper.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "resources/resourcemapper.h"
#include "resources/managedresource.h"

namespace Resources
{
__ImplementClass(Resources::ResourceMapper, 'RSMP', Core::RefCounted);

using namespace Core;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
ResourceMapper::ResourceMapper() :
    isAttached(false),
    asyncEnabled(true)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ResourceMapper::~ResourceMapper()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
const Rtti&
ResourceMapper::GetResourceType() const
{
    n_error("ResourceMapper::GetResourceType() called!");
    return RefCounted::RTTI;
}

//------------------------------------------------------------------------------
/**
*/
void
ResourceMapper::OnAttachToResourceManager()
{
    n_assert(!this->isAttached);
    this->isAttached = true;
}

//------------------------------------------------------------------------------
/**
*/
void
ResourceMapper::OnRemoveFromResourceManager()
{
    n_assert(this->isAttached);
    this->isAttached = false;
}

//------------------------------------------------------------------------------
/**
*/
Ptr<ManagedResource>
ResourceMapper::OnCreateManagedResource(const Rtti& resType, const ResourceId& id, const Ptr<ResourceLoader>& optResourceLoader)
{
    n_error("ResourceMapper::OnCreateManagedResource() called!");
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
void
ResourceMapper::OnDiscardManagedResource(const Ptr<ManagedResource>& managedResource)
{
    n_error("ResourceMapper::OnDiscardManagedResource() called!");
}

//------------------------------------------------------------------------------
/**
*/
void
ResourceMapper::OnPrepare()
{
    n_error("ResourceMapper::OnPrepare() called!");
}

//------------------------------------------------------------------------------
/**
*/
void
ResourceMapper::OnUpdate()
{
    n_error("ResourceMapper::OnUpdate() called!");
}

//------------------------------------------------------------------------------
/**
    This method must return the number of currently pending resources
    (resource which have been requested but are not loaded yet).
*/
SizeT
ResourceMapper::GetNumPendingResources() const
{
    // override in subclass!
    n_error("ResourceMapper::GetNumPendingResources() called!");
    return 0;
}

} // namespace Resources