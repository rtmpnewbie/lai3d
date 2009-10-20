#pragma once
#ifndef RESOURCES_MANAGEDMESH_H
#define RESOURCES_MANAGEDMESH_H
//------------------------------------------------------------------------------
/**
    @class Resources::ManagedMesh
  
    Specialized managed resource for meshes.
    
    (C) 2007 Radon Labs GmbH
*/
#include "resources/managedresource.h"
#include "coregraphics/mesh.h"

//------------------------------------------------------------------------------
namespace Resources
{
class ManagedMesh : public ManagedResource
{
    __DeclareClass(ManagedMesh);
public:
    /// get contained mesh resource
    const Ptr<CoreGraphics::Mesh>& GetMesh() const;
};

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::Mesh>&
ManagedMesh::GetMesh() const
{
    return this->GetResource().downcast<CoreGraphics::Mesh>();
}

} // namespace Resources
//------------------------------------------------------------------------------
#endif
