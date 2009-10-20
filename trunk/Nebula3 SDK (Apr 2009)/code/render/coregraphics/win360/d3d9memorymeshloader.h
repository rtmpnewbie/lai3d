#pragma once
#ifndef WIN360_D3D9MEMORYMESHLOADER_H
#define WIN360_D3D9MEMORYMESHLOADER_H
//------------------------------------------------------------------------------
/**
    @class Win360::D3D9MemoryMeshLoader
    
    Setup a Mesh object from a given vertex, index buffer and primitive group.
    
    (C) 2008 Radon Labs GmbH
*/
#include "resources/resourceloader.h"
#include "coregraphics/base/resourcebase.h"
#include "coregraphics/vertexbuffer.h"
#include "coregraphics/indexbuffer.h"
#include "coregraphics/primitivegroup.h"

//------------------------------------------------------------------------------
namespace Win360
{
class D3D9MemoryMeshLoader : public Resources::ResourceLoader
{
    __DeclareClass(D3D9MemoryMeshLoader);
public:
    /// constructor
    D3D9MemoryMeshLoader();
    /// set the intended resource usage (default is UsageImmutable)
    void SetUsage(Base::ResourceBase::Usage usage);
    /// get resource usage
    Base::ResourceBase::Usage GetUsage() const;
    /// set the intended resource access (default is AccessNone)
    void SetAccess(Base::ResourceBase::Access access);
    /// get the resource access
    Base::ResourceBase::Access GetAccess() const;
    /// set vertex buffer
    void SetVertexBuffer(const Ptr<CoreGraphics::VertexBuffer>& vBuffer);
    /// set index buffer
    void SetIndexBuffer(const Ptr<CoreGraphics::IndexBuffer>& iBuffer);
    /// set primitive group
    void SetPrimitiveGroups(const Util::Array<CoreGraphics::PrimitiveGroup>& pGroup);

    /// called by resource when a load is requested
    virtual bool OnLoadRequested();

protected:
    /// setup mesh resource from given memory data
    bool D3D9MemoryMeshLoader::SetupMeshFromMemory();

private:    
    Base::ResourceBase::Usage usage;
    Base::ResourceBase::Access access;
    Ptr<CoreGraphics::VertexBuffer> vertexBuffer;
    Ptr<CoreGraphics::IndexBuffer> indexBuffer;
    Util::Array<CoreGraphics::PrimitiveGroup> primitiveGroups;
};

//------------------------------------------------------------------------------
/**
*/
inline void
D3D9MemoryMeshLoader::SetUsage(Base::ResourceBase::Usage usage_)
{
    this->usage = usage_;
}

//------------------------------------------------------------------------------
/**
*/
inline Base::ResourceBase::Usage
D3D9MemoryMeshLoader::GetUsage() const
{
    return this->usage;
}

//------------------------------------------------------------------------------
/**
*/
inline void
D3D9MemoryMeshLoader::SetAccess(Base::ResourceBase::Access access_)
{
    this->access = access_;
}

//------------------------------------------------------------------------------
/**
*/
inline Base::ResourceBase::Access
D3D9MemoryMeshLoader::GetAccess() const
{
    return this->access;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
D3D9MemoryMeshLoader::SetVertexBuffer(const Ptr<CoreGraphics::VertexBuffer>& vBuffer)
{
    this->vertexBuffer = vBuffer;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
D3D9MemoryMeshLoader::SetIndexBuffer(const Ptr<CoreGraphics::IndexBuffer>& iBuffer)
{
    this->indexBuffer = iBuffer;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
D3D9MemoryMeshLoader::SetPrimitiveGroups(const Util::Array<CoreGraphics::PrimitiveGroup>& pGroup)
{
    this->primitiveGroups = pGroup;
}
} // namespace Win360
//------------------------------------------------------------------------------
#endif
