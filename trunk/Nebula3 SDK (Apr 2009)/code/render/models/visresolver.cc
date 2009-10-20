//------------------------------------------------------------------------------
//  visresolver.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "models/visresolver.h"
#include "models/model.h"
#include "models/modelnode.h"
#include "models/modelnodeinstance.h"
#include "models/modelinstance.h"

namespace Models
{
__ImplementClass(Models::VisResolver, 'VSRV', Core::RefCounted);
__ImplementSingleton(Models::VisResolver);

using namespace Util;

//------------------------------------------------------------------------------
/**
*/
VisResolver::VisResolver() :
    isOpen(false),
    inResolve(false),
    frameIndex(InvalidIndex)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
VisResolver::~VisResolver()
{
    n_assert(!this->isOpen);
    n_assert(!this->inResolve);
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
VisResolver::Open()
{
    n_assert(!this->isOpen);
    n_assert(!this->inResolve);
    this->isOpen = true;
    this->frameIndex = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
VisResolver::Close()
{
    n_assert(this->isOpen);
    n_assert(!this->inResolve);
    this->visibleModels.Reset();
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
void
VisResolver::BeginResolve()
{
    n_assert(this->isOpen);
    n_assert(!this->inResolve);
    this->inResolve = true;
    this->frameIndex++;
    this->visibleModels.Reset();
}

//------------------------------------------------------------------------------
/**
*/
void
VisResolver::AttachVisibleModelInstance(const Ptr<ModelInstance>& inst)
{
    inst->NotifyVisible(this->frameIndex);
}

//------------------------------------------------------------------------------
/**
*/
void
VisResolver::EndResolve()
{
    n_assert(this->inResolve);
    this->inResolve = false;
}

//------------------------------------------------------------------------------
/**
*/
const Array<Ptr<Model> >&
VisResolver::GetVisibleModels(ModelNodeType::Code nodeType) const
{
    n_assert(!this->inResolve);
    return this->visibleModels.Get(nodeType);
}

//------------------------------------------------------------------------------
/**
*/
const Array<Ptr<ModelNode> >&
VisResolver::GetVisibleModelNodes(ModelNodeType::Code nodeType, const Ptr<Model>& model) const
{
    return model->GetVisibleModelNodes(nodeType);
}

//------------------------------------------------------------------------------
/**
*/
const Array<Ptr<ModelNodeInstance> >&
VisResolver::GetVisibleModelNodeInstances(ModelNodeType::Code nodeType, const Ptr<ModelNode>& modelNode) const
{
    return modelNode->GetVisibleModelNodeInstances(nodeType);
}

} // namespace Models