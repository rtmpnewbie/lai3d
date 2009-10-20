//------------------------------------------------------------------------------
//  resourcemanager.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "resources/resourcemanager.h"
#include "resources/resourcemapper.h"
#include "resources/managedresource.h"
#include "timing/timer.h"

namespace Resources
{
__ImplementClass(Resources::ResourceManager, 'RMGR', Core::RefCounted);
__ImplementSingleton(Resources::ResourceManager);

using namespace Core;
using namespace Util;
using namespace Timing;

//------------------------------------------------------------------------------
/**
*/
ResourceManager::ResourceManager() :
    isOpen(false)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
ResourceManager::~ResourceManager()
{
    if (this->IsOpen())
    {
        this->Close();
    }
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
ResourceManager::Open()
{
    n_assert(!this->IsOpen());
    this->isOpen = true;
}

//------------------------------------------------------------------------------
/**
*/
void
ResourceManager::Close()
{
    n_assert(this->IsOpen());
    this->RemoveAllMappers();
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
void
ResourceManager::AttachMapper(const Ptr<ResourceMapper>& mapper)
{
    n_assert(this->IsOpen());
    n_assert(mapper.isvalid());
    n_assert(!this->mappers.Contains(&(mapper->GetResourceType())));
    this->mappers.Add(&(mapper->GetResourceType()), mapper);
    mapper->OnAttachToResourceManager();
}

//------------------------------------------------------------------------------
/**
*/
void
ResourceManager::RemoveMapper(const Core::Rtti& resType)
{
    n_assert(this->IsOpen());
    n_assert(this->HasMapper(resType));
    this->mappers[&resType]->OnRemoveFromResourceManager();
    this->mappers.Erase(&resType);
}
   
//------------------------------------------------------------------------------
/**
*/
void
ResourceManager::RemoveAllMappers()
{
    n_assert(this->IsOpen());
    while (this->mappers.Size() > 0)
    {
        this->mappers.ValueAtIndex(0)->OnRemoveFromResourceManager();
        this->mappers.EraseAtIndex(0);
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
ResourceManager::HasMapper(const Rtti& resType) const
{
    n_assert(this->IsOpen());
    return this->mappers.Contains(&resType);
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<ResourceMapper>&
ResourceManager::GetMapperByResourceType(const Rtti& resType) const
{
    n_assert(this->IsOpen());
    n_assert(this->HasMapper(resType));
    return this->mappers[&resType];
}

//------------------------------------------------------------------------------
/**
    This method must be called per-frame before rendering begins. This will 
    call the OnPrepare() method on all attached resource mappers, which will 
    at least reset the render statistics in the managed resource.
*/
void
ResourceManager::Prepare()
{
    n_assert(this->IsOpen());
    IndexT i;
    for (i = 0; i < this->mappers.Size(); i++)
    {
        this->mappers.ValueAtIndex(i)->OnPrepare();
    }
}

//------------------------------------------------------------------------------
/**
    This method must be called by the application after render statistics 
    have been gathered and before the actual rendering. The ResourceManager
    will call the OnUpdate() method on all attached resource mappers. This is 
    the place where the actual resource management will happen. 
*/
void
ResourceManager::Update()
{
    n_assert(this->IsOpen());
    IndexT i;
    for (i = 0; i < this->mappers.Size(); i++)
    {
        this->mappers.ValueAtIndex(i)->OnUpdate();
    }
}

//------------------------------------------------------------------------------
/**
    Create a shared ResourceManager object. If a managed resource with the same
    resource name already exists, its client count will be incremented
    and it will be returned. Otherwise the right ResourceMapper will
    be asked to create a new managed resource.
*/
Ptr<ManagedResource>
ResourceManager::CreateManagedResource(const Core::Rtti& resType, const ResourceId& resId, const Ptr<ResourceLoader>& optResourceLoader)
{
    n_assert(this->IsOpen());
    // check if managed resource already exists
    IndexT i = this->managedResources.FindIndex(resId);
    if (InvalidIndex != i)
    {
        // yes exists, increment client count and return existing managed resource
        const Ptr<ManagedResource>& managedResource = this->managedResources.ValueAtIndex(i);
        n_assert(managedResource.isvalid());
        n_assert(&resType == managedResource->GetResourceType());
        managedResource->IncrClientCount();
        return managedResource;
    }
    else
    {
        // managed resource doesn't exist yet, ask the right resource mapper to create a new one
        n_assert(this->HasMapper(resType));
        Ptr<ManagedResource> managedResource = this->mappers[&resType]->OnCreateManagedResource(resType, resId, optResourceLoader);
        n_assert(managedResource.isvalid());
        this->managedResources.Add(resId, managedResource);
        return managedResource;
    }
}

//------------------------------------------------------------------------------
/**
    Discard a shared ManagedResource object. This will decrement the
    client count. If the client count reaches zero, the ManagedResource
    object will be released as well.
*/
void
ResourceManager::DiscardManagedResource(const Ptr<ManagedResource>& managedResource)
{
    n_assert(this->IsOpen());
    n_assert(this->managedResources.Contains(managedResource->GetResourceId()));
    this->mappers[managedResource->GetResourceType()]->OnDiscardManagedResource(managedResource);
    if (managedResource->GetClientCount() == 0)
    {
        this->managedResources.Erase(managedResource->GetResourceId());
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
ResourceManager::HasManagedResource(const ResourceId& resId) const
{
    n_assert(this->IsOpen());
    return this->managedResources.Contains(resId);
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<ManagedResource>&
ResourceManager::LookupManagedResource(const ResourceId& resId) const
{
    n_assert(this->IsOpen());
    return this->managedResources[resId];
}

//------------------------------------------------------------------------------
/**
    This method tests if there are pending resource which haven't been loaded
    yet. Returns true if there are no pending resources (everything has 
    loaded), false if pending resources exist.
*/
bool
ResourceManager::CheckPendingResources()
{
    SizeT numPending = 0;
    IndexT i;
    for (i = 0; i < this->mappers.Size(); i++)
    {
        numPending += this->mappers.ValueAtIndex(i)->GetNumPendingResources();
    }
    if (0 == numPending)
    {
        // everything has been loaded
        return true;
    }
    else
    {
        // pending resources exist
        return false;
    }
}

//------------------------------------------------------------------------------
/**
    This method blocks until all pending resources are loaded, or until
    a time-out occurs. If a time-out occurs the method will return false,
    otherwise true.
*/
bool
ResourceManager::WaitForPendingResources(Time timeOut)
{
    // this basically runs a local resource update loop until 
    // everything is loaded or the timeout is reached
    Timer timer;                                    // not necessary to use slavetime here, just a local timer
    timer.Start();
    bool timedOut = false;
    bool allLoaded = false;
    do
    {
        this->Prepare();
        this->Update();
        if (this->CheckPendingResources())
        {
            allLoaded = true;
        }
        timedOut = timer.GetTime() > timeOut;
        Timing::Sleep(0.01);
    }
    while (!(allLoaded || timedOut));
    return allLoaded;
}

} // namespace Resources