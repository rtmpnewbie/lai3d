#pragma once
#ifndef RESOURCES_RESOURCEMANAGER_H
#define RESOURCES_RESOURCEMANAGER_H
//------------------------------------------------------------------------------
/**
    @class Resources::ResourceManager
    
    The ResourceManager adds a management layer between resource using clients
    and actual resource objects. The main purpose of the manager is
    resource streaming for large seamless worlds. Resource users request 
    a ManagedResource object from the ResourceManager. A ManagedResource is 
    a wrapper around an actual Resource object, which may change based on 
    the resource management strategy implemented by the manager. The 
    main job of a resource manager is to provide all the resource required for 
    rendering while making the best use of limited resource memory. It will also 
    care about background loading of resources, and provide placeholder
    resources if a resource hasn't finished loading.
    
    The actual resource management strategies for different resource types
    are customizable by attaching ResourceMapper objects to the 
    ResourceManager. A ResourceMapper analyzes the usage statistics of existing 
    ManagedResource objects and implements a specific resource management pattern 
    using the following basic operations:

    - Load(pri, lod): asynchronously load a resource from external 
      storage into memory given a priority and a level-of-detail.
    - Discard: completely unload a resource, freeing up limited
      resource memory.
    - Upgrade(lod): upgrade a loaded resource to a higher level-of-detail
    - Degrade(lod): degrade a loaded resource to a lower level-of-detail
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "resources/resourceid.h"
#include "resources/resourceloader.h"
#include "timing/time.h"

//------------------------------------------------------------------------------
namespace Resources
{
class ResourceMapper;
class ManagedResource;

class ResourceManager : public Core::RefCounted
{
    __DeclareClass(ResourceManager);
    __DeclareSingleton(ResourceManager);
public:
    /// constructor
    ResourceManager();
    /// destructor
    virtual ~ResourceManager();

    /// open the resource manager
    void Open();
    /// close the resource manager
    void Close();
    /// return true if resource manager is open
    bool IsOpen() const;

    /// register a resource mapper (resource type is defined by mapper)
    void AttachMapper(const Ptr<ResourceMapper>& mapper);
    /// unregister a resource mapper by resource type
    void RemoveMapper(const Core::Rtti& resourceType);
    /// unregister all mappers
    void RemoveAllMappers();
    /// return true if a mapper has been registered for the given resource type
    bool HasMapper(const Core::Rtti& resourceType) const;
    /// get the resource mapper registered with a resource type
    const Ptr<ResourceMapper>& GetMapperByResourceType(const Core::Rtti& resourceType) const;

    /// create a ManagedResource object (bumps usecount on existing resource)
    Ptr<ManagedResource> CreateManagedResource(const Core::Rtti& resType, const ResourceId& id, const Ptr<ResourceLoader>& optResourceLoader=0);
    /// unregister a ManagedResource object
    void DiscardManagedResource(const Ptr<ManagedResource>& managedResource);
    /// return true if a managed resource exists
    bool HasManagedResource(const ResourceId& id) const;
    /// lookup a managed resource (does not change usecount of resource)
    const Ptr<ManagedResource>& LookupManagedResource(const ResourceId& id) const;

    /// prepare stats gathering, call per frame
    void Prepare();
    /// perform actual resource management, call per frame
    void Update();
    /// test if any resources are pending, returns true if not resources are pending
    bool CheckPendingResources();
    /// wait until pending resources are loaded, or time-out is reached (returns false if time-out)
    bool WaitForPendingResources(Timing::Time timeOut);

private:
    bool isOpen;
    Util::Dictionary<const Core::Rtti*,Ptr<ResourceMapper> > mappers;        //> resource schedulers by resource type
    Util::Dictionary<ResourceId,Ptr<ManagedResource> > managedResources;     //> managed resources by name
};

//------------------------------------------------------------------------------
/**
*/
inline bool
ResourceManager::IsOpen() const
{
    return this->isOpen;
}

} // namespace Resources
//------------------------------------------------------------------------------
#endif

