//------------------------------------------------------------------------------
//  emittermesh.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "particles/emittermesh.h"

namespace Particles
{
using namespace CoreGraphics;
using namespace Util;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
EmitterMesh::EmitterMesh() :
    numVertices(0),
    vertices(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
EmitterMesh::~EmitterMesh()
{
    if (this->IsValid())
    {
        this->Discard();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
EmitterMesh::Setup(const Ptr<Mesh>& mesh, IndexT primGroupIndex)
{
    n_assert(!this->IsValid());

    // we need to extract mesh vertices from the primitive group
    // without duplicate vertices
    const PrimitiveGroup& primGroup = mesh->GetPrimitiveGroupAtIndex(primGroupIndex);
    const Ptr<IndexBuffer>& indexBuffer = mesh->GetIndexBuffer();    
    n_assert(indexBuffer->GetIndexType() == IndexType::Index16);
    const Ptr<VertexBuffer>& vertexBuffer = mesh->GetVertexBuffer();
    SizeT numVertices = vertexBuffer->GetNumVertices();

    IndexT baseIndex = primGroup.GetBaseIndex();
    SizeT numIndices = primGroup.GetNumIndices();
    ushort* indices = (ushort*) indexBuffer->Map(IndexBuffer::MapRead);

    // allocate a "flag array" which holds a 0 at a
    // vertex index which hasn't been encountered yet, and
    // a 1 if the index has been encountered before
    SizeT size = numVertices * sizeof(uchar);
    uchar* flagArray = (uchar*) Memory::Alloc(Memory::ScratchHeap, size);
    Memory::Clear(flagArray, size);

    Array<ushort> emitterIndices;
    emitterIndices.Reserve(numVertices);
    IndexT i;
    for (i = baseIndex; i < (baseIndex + numIndices); i++)
    {
        ushort vertexIndex = indices[i];
        n_assert(vertexIndex < numVertices);
        if (flagArray[vertexIndex] == 0)
        {
            // this index hasn't been encountered yet
            emitterIndices.Append(vertexIndex);
            flagArray[vertexIndex] = 1;
        }
    }
    Memory::Free(Memory::ScratchHeap, flagArray);
    flagArray = 0;
    indexBuffer->Unmap();

    // the emitterIndices array now contains the indices of all vertices
    // we need to copy
    this->numVertices = emitterIndices.Size();
    this->vertices = (float4*) Memory::Alloc(Memory::ResourceHeap, this->numVertices * VertexWidth * sizeof(float4));

    // make sure the emitter mesh actually has the components we need
    const Ptr<VertexLayout>& vertexLayout = vertexBuffer->GetVertexLayout();
    IndexT posCompIndex = vertexLayout->FindComponent(VertexComponent::Position, 0);
    n_assert(InvalidIndex != posCompIndex);
    IndexT normCompIndex = vertexLayout->FindComponent(VertexComponent::Normal, 0);
    n_assert(InvalidIndex != normCompIndex);
    IndexT tanCompIndex = vertexLayout->FindComponent(VertexComponent::Tangent, 0);
    n_assert(InvalidIndex != tanCompIndex);
    n_assert(vertexLayout->GetComponentAt(posCompIndex).GetFormat() == VertexComponent::Float3);
    n_assert(vertexLayout->GetComponentAt(normCompIndex).GetFormat() == VertexComponent::UByte4N);
    n_assert(vertexLayout->GetComponentAt(tanCompIndex).GetFormat() == VertexComponent::UByte4N);

    // get the byte offset from the start of the vertex
    IndexT posByteOffset = vertexLayout->GetComponentByteOffset(VertexComponent::Position, 0);
    IndexT normByteOffset = vertexLayout->GetComponentByteOffset(VertexComponent::Normal, 0);
    IndexT tanByteOffset = vertexLayout->GetComponentByteOffset(VertexComponent::Tangent, 0);

    // gain access to vertices and transfer vertex info
    uchar* verts = (uchar*) vertexBuffer->Map(VertexBuffer::MapRead);
    SizeT vertexByteSize = vertexLayout->GetVertexByteSize();
    for (i = 0; i < this->numVertices; i++)
    {
        uchar* src = verts + vertexByteSize * emitterIndices[i];
        float4* dst = this->vertices + i * VertexWidth;
        dst[0].load_float3(src + posByteOffset, 1.0f);
        dst[1].load_ubyte4n_signed(src + normByteOffset, 0.0f);
        dst[2].load_ubyte4n_signed(src + tanByteOffset, 0.0f);
    }
    vertexBuffer->Unmap();
}

//------------------------------------------------------------------------------
/**
*/
void
EmitterMesh::Discard()
{
    n_assert(this->IsValid());
    Memory::Free(Memory::ResourceHeap, this->vertices);
    this->vertices = 0;
    this->numVertices = 0;
}

//------------------------------------------------------------------------------
/**
*/
const float4*
EmitterMesh::GetEmitterVertex(IndexT key) const
{
    IndexT vertexIndex = key % this->numVertices;
    return this->vertices + vertexIndex * VertexWidth;
}

} // namespace Particles
