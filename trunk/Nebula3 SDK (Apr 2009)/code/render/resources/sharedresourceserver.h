#pragma once
#ifndef RESOURCES_SHAREDRESOURCESERVER_H
#define RESOURCES_SHAREDRESOURCESERVER_H
//------------------------------------------------------------------------------
/**
    @class Resources::SharedResourceServer
    
    The SharedResourceServer manages the pool of shared resources.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "resources/resource.h"
#include "resources/resourcedictionary.h"

//------------------------------------------------------------------------------
namespace Resources
{
class SharedResourceServer : public Core::RefCounted
{
    __DeclareClass(SharedResourceServer);
    __DeclareSingleton(SharedResourceServer);
public:
    /// constructor
    SharedResourceServer();
    /// destructor
    virtual ~SharedResourceServer();
    
    /// open the resource server
    bool Open();
    /// close the resource server, discards resources
    void Close();
    /// return true if resource server is open
    bool IsOpen() const;
    
    /// return true if a shared resource exists
    bool HasSharedResource(const ResourceId& id) const;
    /// lookup a shared resource
    const Ptr<Resource>& LookupSharedResource(const ResourceId& id) const;
    /// create and register a shared resource
    Ptr<Resource> CreateSharedResource(const ResourceId& id, const Core::Rtti& resClass);
    /// create and register shared resource with associated loader
    Ptr<Resource> CreateSharedResource(const ResourceId& id, const Core::Rtti& resClass, const Ptr<ResourceLoader>& loader);
    /// create and register shared resource with associated loader and saver
    Ptr<Resource> CreateSharedResource(const ResourceId& id, const Core::Rtti& resClass, const Ptr<ResourceLoader>& loader, const Ptr<ResourceSaver>& saver);
    /// register an existing resource object as shared resource
    void RegisterSharedResource(const Ptr<Resource>& res);
    /// unregister a shared resource (necessary for managing the use count)
    void UnregisterSharedResource(const Ptr<Resource>& res);
    /// unregister a shared resource by resource name
    void UnregisterSharedResource(const ResourceId& id);
    /// read-only access to shared resources
    const Util::Dictionary<ResourceId, Ptr<Resource> >& GetSharedResources() const;
    /// get shared resources by type (slow)
    Util::Array<Ptr<Resource> > GetSharedResourcesByType(const Core::Rtti& type) const;

private:
    bool isOpen;
    Util::Dictionary<ResourceId, Ptr<Resource> > resources;
    Ptr<ResourceDictionary> resourceDictionary;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
SharedResourceServer::IsOpen() const
{
    return this->isOpen;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Dictionary<ResourceId, Ptr<Resource> >&
SharedResourceServer::GetSharedResources() const
{
    return this->resources;
}

} // namespace Resources
//------------------------------------------------------------------------------
#endif
