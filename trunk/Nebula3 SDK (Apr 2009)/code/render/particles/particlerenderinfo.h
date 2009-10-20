#pragma once
//------------------------------------------------------------------------------
/**
    @class Particles::ParticleRenderInfo
    
    ParticleRenderInfo objects are returned by the ParticleRenderer singleton
    when a visible particle system is attached. The caller needs to store
    this object and needs to hand it back to the ParticleRenderer when
    actually rendering of the particle system needs to happen.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/types.h"

//------------------------------------------------------------------------------
namespace Particles
{
class ParticleRenderInfo
{
public:
    /// default constructor
    ParticleRenderInfo();
    /// constructor
    ParticleRenderInfo(IndexT baseVertexIndex, SizeT numVertices);
    /// clear the object
    void Clear();
    /// get base vertex index
    IndexT GetBaseVertexIndex() const;
    /// get number of vertices
    SizeT GetNumVertices() const;
private:
    IndexT baseVertexIndex;
    SizeT numVertices;
};

//------------------------------------------------------------------------------
/**
*/
inline
ParticleRenderInfo::ParticleRenderInfo() :
    baseVertexIndex(0),
    numVertices(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
ParticleRenderInfo::ParticleRenderInfo(IndexT base, SizeT num) :
    baseVertexIndex(base),
    numVertices(num)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline void
ParticleRenderInfo::Clear()
{
    this->baseVertexIndex = 0;
    this->numVertices = 0;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT
ParticleRenderInfo::GetBaseVertexIndex() const
{
    return this->baseVertexIndex;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
ParticleRenderInfo::GetNumVertices() const
{
    return this->numVertices;
}

} // namespace Particles
//------------------------------------------------------------------------------
    