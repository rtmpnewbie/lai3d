//------------------------------------------------------------------------------
//  modelinstance.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "models/modelinstance.h"
#include "models/modelnodeinstance.h"
#include "internalgraphics/internalmodelentity.h"

namespace Models
{
__ImplementClass(Models::ModelInstance, 'MODI', Core::RefCounted);

using namespace Util;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
ModelInstance::ModelInstance()
{
    this->transform = matrix44::identity();
}

//------------------------------------------------------------------------------
/**
*/
ModelInstance::~ModelInstance()
{
    n_assert(!this->IsAttachedToModel());
}

//------------------------------------------------------------------------------
/**
*/
void
ModelInstance::SetModelEntity(const Ptr<InternalGraphics::InternalModelEntity>& mdlEntity)
{
    this->modelEntity = mdlEntity;
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<InternalGraphics::InternalModelEntity>&
ModelInstance::GetModelEntity() const
{
    return this->modelEntity;
}

//------------------------------------------------------------------------------
/**
*/
void
ModelInstance::Discard()
{
    n_assert(this->IsAttachedToModel());
    this->model->RemoveInstance(this);
    n_assert(!this->model.isvalid());
}

//------------------------------------------------------------------------------
/**
    Get a pointer to the root node instance. This is always
    the first node instance.
*/
const Ptr<ModelNodeInstance>&
ModelInstance::GetRootNodeInstance() const
{
    n_assert(!this->nodeInstances.IsEmpty());
    n_assert(!this->nodeInstances[0]->HasParent());
    return this->nodeInstances[0];
}

//------------------------------------------------------------------------------
/**
    This method is called when this instance is attached to its Model.
    The most important thing happening at initialization is for each
    ModelNode, a corresponding ModelInstanceNode will be created,
    which holds ModelNode-per-instance data.
*/
void
ModelInstance::OnAttachToModel(const Ptr<Model>& m)
{
    n_assert(!this->IsAttachedToModel());
    this->model = m;

    // DEBUG
//    n_printf("ModelInstance created: %s\n", m->GetResourceId().Value().AsCharPtr());

    // create node instances
    this->model->GetRootNode()->CreateNodeInstanceHierarchy(this);
}

//------------------------------------------------------------------------------
/**
    This method is called when this instance is removed from its Model.
*/
void
ModelInstance::OnRemoveFromModel()
{
    n_assert(this->IsAttachedToModel());
    this->model = 0;

    // discard all node instances
    while (!this->nodeInstances.IsEmpty())
    {
        // need to keep a reference to the node when calling Discard()!
        Ptr<ModelNodeInstance> curNode = this->nodeInstances.Back();
        curNode->Discard();
    }
}

//------------------------------------------------------------------------------
/**
    Attach a node instance which has already been setup.
*/
void
ModelInstance::AttachNodeInstance(const Ptr<ModelNodeInstance>& nodeInst)
{
    n_assert(InvalidIndex == this->nodeInstances.FindIndex(nodeInst));
    this->nodeInstances.Append(nodeInst);
}

//------------------------------------------------------------------------------
/**
    Remove a node instance, do not discard it!
*/
void
ModelInstance::RemoveNodeInstance(const Ptr<ModelNodeInstance>& nodeInst)
{
    IndexT index = this->nodeInstances.FindIndex(nodeInst);
    n_assert(InvalidIndex != index);
    this->nodeInstances.EraseIndex(index);
}

//------------------------------------------------------------------------------
/**
    The Update() method is called once per frame on potentially visible
    ModelInstances. The method must be called AFTER transformation, visibility, 
    time or other parameters have been set. The Update() method will
    be propagated to all instance nodes.
*/
void
ModelInstance::Update(Timing::Time time)
{
    IndexT i;
    for (i = 0; i < this->nodeInstances.Size(); i++)
    {
#if NEBULA3_ENABLE_PROFILING
        this->nodeInstances[i]->ResetDebugTimer();
#endif
        this->nodeInstances[i]->Update(time);
    }
}

//------------------------------------------------------------------------------
/**
    This method is called by the Graphics subsystem when we are currently
    visible. Once all visible model instances are notified, the Graphics
    subsystem can get a render-order-optimized list of all visible
    model-node-instances through the ModelServer.
*/
void
ModelInstance::NotifyVisible(IndexT frameIndex)
{
    // all we need to do is to distribute the method to all our
    // model node instances
    IndexT i;
    SizeT num = this->nodeInstances.Size();
    for (i = 0; i < num; i++)
    {
        this->nodeInstances[i]->OnNotifyVisible(frameIndex);
    }
}

//------------------------------------------------------------------------------
/**
    This method is called from the RenderDebug of Graphics::ModelEntity.
*/
void 
ModelInstance::RenderDebug()
{
    IndexT i;
    SizeT num = this->nodeInstances.Size();
    for (i = 0; i < num; i++)
    {
        this->nodeInstances[i]->RenderDebug();
    }
}

//------------------------------------------------------------------------------
/**
    Careful, this method is SLOW!
*/
Ptr<ModelNodeInstance>
ModelInstance::LookupNodeInstance(const String& path) const
{
    Ptr<ModelNodeInstance> curPtr;
    if (this->nodeInstances.Size() > 0)
    {
        Array<String> tokens = path.Tokenize("/");
        n_assert(tokens.Size() > 0);
        curPtr = this->GetRootNodeInstance();
        if (tokens[0] == curPtr->GetName().Value())
        {
            IndexT i;
            for (i = 1; i < tokens.Size(); i++)
            {
                StringAtom curToken = tokens[i];
                if (curPtr->HasChild(curToken))
                {
                    curPtr = curPtr->LookupChild(curToken);
                }
                else
                {
                    // return an invalid ptr
                    return Ptr<ModelNodeInstance>();
                }
            }
        }
    }
    return curPtr;
}

//------------------------------------------------------------------------------
/**
*/
void 
ModelInstance::OnShow(Timing::Time time)
{
    IndexT i;
    SizeT num = this->nodeInstances.Size();
    for (i = 0; i < num; i++)
    {
        this->nodeInstances[i]->OnShow(time);
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
ModelInstance::OnHide(Timing::Time time)
{
    IndexT i;
    SizeT num = this->nodeInstances.Size();
    for (i = 0; i < num; i++)
    {
        this->nodeInstances[i]->OnHide(time);
    }
}

} // namespace Models
