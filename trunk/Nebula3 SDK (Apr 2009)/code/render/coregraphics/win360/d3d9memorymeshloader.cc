//------------------------------------------------------------------------------
//  D3d9MemoryMeshLoader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/win360/d3d9memorymeshloader.h"
#include "coregraphics/mesh.h"
#include "coregraphics/legacy/nvx2streamreader.h"

namespace Win360
{
__ImplementClass(Win360::D3D9MemoryMeshLoader, 'DMML', Resources::ResourceLoader);

using namespace Resources;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
D3D9MemoryMeshLoader::D3D9MemoryMeshLoader() :
    usage(Base::ResourceBase::UsageImmutable),
    access(Base::ResourceBase::AccessNone)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool 
D3D9MemoryMeshLoader::OnLoadRequested()
{
    n_assert(this->GetState() == Resource::Initial);
    n_assert(this->resource.isvalid());
    
    // perform direct load    
    if (this->SetupMeshFromMemory())
    {
        this->SetState(Resource::Loaded);
        return true;
    }
    // fallthrough: synchronous loading failed
    this->SetState(Resource::Failed);
    return false;    
}

//------------------------------------------------------------------------------
/**
*/
bool 
D3D9MemoryMeshLoader::SetupMeshFromMemory()
{   
    n_assert(this->vertexBuffer.isvalid());

    const Ptr<Mesh>& res = this->resource.downcast<Mesh>();
    res->SetVertexBuffer(this->vertexBuffer);
    if (this->indexBuffer.isvalid())
    {
        res->SetIndexBuffer(this->indexBuffer);    
    }
    if (this->primitiveGroups.Size() > 0)
    {
        res->SetPrimitiveGroups(this->primitiveGroups);
    }   

    return true;
}

} // namespace Win360
