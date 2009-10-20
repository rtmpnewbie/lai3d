#pragma once
//------------------------------------------------------------------------------
/**
    @class Particles::EmitterMesh
    
    An emitter mesh holds positions and normals for particle emission.
    The actual format of a vertex in the emitter mesh is:

    float4 position;
    float4 normal;
    float4 tangent;
    
    (C) 2008 Radon Labs GmbH
*/
#include "coregraphics/mesh.h"
#include "util/fixedarray.h"

//------------------------------------------------------------------------------
namespace Particles
{
class EmitterMesh
{
public:
    /// constructor
    EmitterMesh();
    /// destructor
    ~EmitterMesh();
    
    /// setup the emitter mesh
    void Setup(const Ptr<CoreGraphics::Mesh>& mesh, IndexT primGroupIndex);
    /// discard the emitter mesh
    void Discard();
    /// return true if object has been setup
    bool IsValid() const;
    
    /// get pointer to emitter vertex
    const Math::float4* GetEmitterVertex(IndexT key) const;

private:
    static const SizeT VertexWidth = 3;    // width in number of float4's...
    SizeT numVertices;
    Math::float4* vertices;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
EmitterMesh::IsValid() const
{
    return (0 != this->vertices);
}

} // namespace Particles
//------------------------------------------------------------------------------
