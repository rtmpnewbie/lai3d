//------------------------------------------------------------------------------
//  vertexlayoutbase.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/base/vertexlayoutbase.h"

namespace Base
{
__ImplementClass(Base::VertexLayoutBase, 'VXLB', Core::RefCounted);

using namespace CoreGraphics;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
VertexLayoutBase::VertexLayoutBase() :
    vertexByteSize(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
VertexLayoutBase::~VertexLayoutBase()
{
    n_assert(!this->IsValid());
}

//------------------------------------------------------------------------------
/**
*/
String
VertexLayoutBase::BuildSignature(const Array<VertexComponent>& comps)
{
    n_assert(comps.Size() > 0);
    String sig;
    IndexT i;
    for (i = 0; i < comps.Size(); i++)
    {
        sig.Append(comps[i].GetSignature());
    }
    return sig;
}

//------------------------------------------------------------------------------
/**
*/
void
VertexLayoutBase::Setup(const Array<VertexComponent>& comps)
{
    n_assert(comps.Size() > 0);
    n_assert(!this->IsValid());
    n_assert(0 == this->vertexByteSize);
    this->components = comps;
    IndexT i;
    for (i = 0; i < comps.Size(); i++)
    {
        this->vertexByteSize += comps[i].GetByteSize();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
VertexLayoutBase::Discard()
{
    n_assert(this->IsValid());
    this->components.Clear();
    this->vertexByteSize = 0;
}

//------------------------------------------------------------------------------
/**
*/
IndexT
VertexLayoutBase::FindComponent(VertexComponent::SemanticName semName, IndexT semIndex) const
{
    IndexT i;
    for (i = 0; i < this->components.Size(); i++)
    {
        const VertexComponent& cur = this->components[i];
        if ((cur.GetSemanticName() == semName) && (cur.GetSemanticIndex() == semIndex))
        {
            return i;
        }
    }
    // fallthrough: not found
    return InvalidIndex;
}

//------------------------------------------------------------------------------
/**
    Returns the component offset in bytes from the beginning
    of a vertex to the start of the given vertex components. Returns InvalidIndex
    if the vertex component doesn't exist!
    @todo: precompute on creation
*/
IndexT
VertexLayoutBase::GetComponentByteOffset(VertexComponent::SemanticName semName, IndexT semIndex) const
{
    IndexT offset = 0;
    IndexT i;
    for (i = 0; i < this->components.Size(); i++)
    {
        const VertexComponent& cur = this->components[i];
        if ((cur.GetSemanticName() == semName) && (cur.GetSemanticIndex() == semIndex))
        {
            return offset;
        }
        offset += cur.GetByteSize();
    }
    // fallthrough: vertex component doesn't exist
    return InvalidIndex;
}

} // namespace Base
