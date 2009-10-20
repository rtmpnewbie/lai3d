//------------------------------------------------------------------------------
//  sharedresourceserver.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "resources/sharedresourceserver.h"
#include "resources/resourceloader.h"
#include "resources/resourcesaver.h"
#include "resources/resourcedictionary.h"
#include "io/ioserver.h"

namespace Resources
{
__ImplementClass(Resources::SharedResourceServer, 'SRSV', Core::RefCounted);
__ImplementSingleton(Resources::SharedResourceServer);

using namespace Util;
using namespace Core;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
SharedResourceServer::SharedResourceServer() :
    isOpen(false)
{
    __ConstructSingleton;
    this->resourceDictionary = ResourceDictionary::Create();
}

//------------------------------------------------------------------------------
/**
*/
SharedResourceServer::~SharedResourceServer()
{
    if (this->IsOpen())
    {
        this->Close();
    }
    this->resourceDictionary = 0;
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
bool
SharedResourceServer::Open()
{
    n_assert(!this->IsOpen());

    // setup shared resource server
    // FIXME: resource dictionary handling may have to be more
    // advanced in the future, when there are many thousand resources
    // in a project it may be useful to have several smaller resource dictionaries
    // (i.e. one per level)
    URI resDictUri("root:export/resources.dic");
    if (IoServer::Instance()->FileExists(resDictUri))
    {
        this->resourceDictionary->SetURI(resDictUri);
        if (!this->resourceDictionary->Load())
        {
            n_error("Failed to load resource dictionary ('%s')", resDictUri.AsString().AsCharPtr());
        }
    }
    else
    {
        n_printf("WARNING: no resource dictionary file found ('%s')\n", resDictUri.AsString().AsCharPtr());
    }
    this->isOpen = true;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
SharedResourceServer::Close()
{
    n_assert(this->IsOpen());
    this->isOpen = false;

    // discard all shared resource
    IndexT i;
    for (i = 0; i < this->resources.Size(); i++)
    {
        const Ptr<Resource>& res = this->resources.ValueAtIndex(i);
        res->Unload();
        res->SetLoader(0);
        res->SetSaver(0);
    }
    this->resources.Clear();

    // shutdown resource dictionary
    if (this->resourceDictionary->IsValid())
    {
        this->resourceDictionary->Unload();
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
SharedResourceServer::HasSharedResource(const ResourceId& resId) const
{
    n_assert(resId.Value().IsValid());
    return this->resources.Contains(resId);
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<Resource>&
SharedResourceServer::LookupSharedResource(const ResourceId& resId) const
{
    return this->resources[resId];
}

//------------------------------------------------------------------------------
/**
    Register an existing resource object as shared resource. If the resource
    already has been registered, an assertion will be thrown. This will
    increment the use count of the resource by one.
*/
void
SharedResourceServer::RegisterSharedResource(const Ptr<Resource>& res)
{
    n_assert(res.isvalid());
    n_assert(res->GetResourceId().Value().IsValid());
    n_assert(!this->resources.Contains(res->GetResourceId()));
    res->IncrUseCount();
    this->resources.Add(res->GetResourceId(), res);
}

//------------------------------------------------------------------------------
/**
    Unregister a shared resource. This will decrement the use count of the
    resource. If the use count has reached zero, the resource will be
    discarded (unloaded and removed from the shared resource pool).
*/
void
SharedResourceServer::UnregisterSharedResource(const Ptr<Resource>& res)
{
    n_assert(res.isvalid());
    n_assert(res->GetResourceId().Value().IsValid());
    n_assert(this->resources.Contains(res->GetResourceId()));
    n_assert(res->GetUseCount() > 0);
    res->DecrUseCount();
    if (res->GetUseCount() == 0)
    {
        res->Unload();
        res->SetLoader(0);
        res->SetSaver(0);
        this->resources.Erase(res->GetResourceId());
    }
}

//------------------------------------------------------------------------------
/**
    Unregister a shared resource by resource id.
*/
void
SharedResourceServer::UnregisterSharedResource(const ResourceId& id)
{
    n_assert(this->HasSharedResource(id));

    // IMPORTANT: need to make a copy of the smart pointer, since 
    // internal array layout will change!
    Ptr<Resource> res = this->resources[id];
    this->UnregisterSharedResource(res);
}

//------------------------------------------------------------------------------
/**
    Create a shared resource object. If the resource already exists, its
    use count will be increased and the resource will be returned.
    If the resource doesn't exist yet, a new resource object will be created
    and registered as shared resource.
    Please note that you must call UnregisterSharedResource() when the
    resource is no longer needed in order to manage the use count properly.
*/
Ptr<Resource>
SharedResourceServer::CreateSharedResource(const ResourceId& resId, const Rtti& resClass)
{
    n_assert(resId.Value().IsValid());
    if (this->resources.Contains(resId))
    {
        // return existing resource
        const Ptr<Resource>& res = this->resources[resId];
        n_assert(res->IsInstanceOf(resClass));
        n_assert(res->GetResourceId() == resId);
        res->IncrUseCount();
        return res;
    }
    else
    {
        // resource doesn't exist yet, create new one
        Ptr<Resource> res = (Resource*) resClass.Create();
        res->SetResourceId(resId);
        this->RegisterSharedResource(res);
        return res;
    }
}

//------------------------------------------------------------------------------
/**
    Create and register a shared resource object with attached resource loader.
*/
Ptr<Resource>
SharedResourceServer::CreateSharedResource(const ResourceId& resId, const Rtti& resClass, const Ptr<ResourceLoader>& loader)
{
    n_assert(resId.Value().IsValid());
    if (this->resources.Contains(resId))
    {
        // resource already exists, make sure everything matches
        const Ptr<Resource>& res = this->resources[resId];
        n_assert(res->IsInstanceOf(resClass));
        //n_assert(res->GetLoader().isvalid() && res->GetLoader()->IsInstanceOf(*loader->GetRtti()));
        res->IncrUseCount();
        return res;
    }
    else
    {
        // resource doesn't exist yet, create new one
        Ptr<Resource> res = (Resource*) resClass.Create();
        res->SetResourceId(resId);
        res->SetLoader(loader);
        this->RegisterSharedResource(res);
        return res;
    }
}

//------------------------------------------------------------------------------
/**
    Create a shared resource object with attached resource loader and saver.
*/
Ptr<Resource>
SharedResourceServer::CreateSharedResource(const ResourceId& resId, const Rtti& resClass, const Ptr<ResourceLoader>& loader, const Ptr<ResourceSaver>& saver)
{
    n_assert(resId.Value().IsValid());

    if (this->resources.Contains(resId))
    {
        // resource already exists, make sure everything matches
        const Ptr<Resource>& res = this->resources[resId];
        n_assert(res->IsInstanceOf(resClass));
        n_assert(res->GetLoader().isvalid() && res->GetLoader()->IsInstanceOf(*loader->GetRtti()));
        n_assert(res->GetSaver().isvalid() && res->GetSaver()->IsInstanceOf(*saver->GetRtti()));
        res->IncrUseCount();
        return res;
    }
    else
    {
        // resource doesn't exist yet, create new one
        Ptr<Resource> res = (Resource*) resClass.Create();
        res->SetResourceId(resId);
        res->SetLoader(loader);
        res->SetSaver(saver);
        this->RegisterSharedResource(res);
        return res;
    }
}

//------------------------------------------------------------------------------
/**
    Returns an array of shared resources by type. This is a slow method.
*/
Array<Ptr<Resource> >
SharedResourceServer::GetSharedResourcesByType(const Core::Rtti& type) const
{
    Array<Ptr<Resource> > result;
    IndexT i;
    for (i = 0; i < this->resources.Size(); i++)
    {
        if (this->resources.ValueAtIndex(i)->IsA(type))
        {
            result.Append(resources.ValueAtIndex(i));
        }
    }
    return result;
}

} // namespace Resources