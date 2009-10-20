//------------------------------------------------------------------------------
//  model.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "models/model.h"
#include "models/modelinstance.h"
#include "models/modelserver.h"
#include "models/visresolver.h"

namespace Models
{
__ImplementClass(Models::Model, 'MODL', Resources::Resource);

using namespace Util;
using namespace Resources;

//------------------------------------------------------------------------------
/**
*/
Model::Model() :
    inLoadResources(false),
    resourcesLoaded(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Model::~Model()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
Model::Unload()
{    
    // discard model nodes
    IndexT i;
    for (i = 0; i < this->nodes.Size(); i++)
    {
        this->nodes[i]->OnRemoveFromModel();
    }
    this->nodes.Clear();
    this->visibleModelNodes.Reset();

    // discard instances
    for (i = 0; i < this->instances.Size(); i++)
    {
        this->instances[i]->OnRemoveFromModel();
    }
    this->instances.Clear();
        
    // call parent class
    Resource::Unload();
}

//------------------------------------------------------------------------------
/**
    This method asks all model nodes to load their resources. Note that 
    actual resource loading may be asynchronous and placeholder resources
    may be in place right after this method returns.
*/
void
Model::LoadResources()
{
    n_assert(!this->inLoadResources);
    this->inLoadResources = true;
    IndexT i;
    for (i = 0; i < this->nodes.Size(); i++)
    {
        this->nodes[i]->LoadResources();
    }
}

//------------------------------------------------------------------------------
/**
    This method asks all model nodes to unload their resources.
*/
void
Model::UnloadResources()
{
    n_assert(this->inLoadResources);
    IndexT i;
    for (i = 0; i < this->nodes.Size(); i++)
    {
        this->nodes[i]->UnloadResources();
    }
    this->inLoadResources = false;
}

//------------------------------------------------------------------------------
/**
    This checks whether all resource have been loaded, if yes the method
    OnResourcesLoaded() will be called once. If some resources are not
    loaded yet, the method will return false.
*/
bool
Model::CheckPendingResources()
{
    n_assert(this->inLoadResources);
    Resource::State result = Resource::Initial;
    IndexT i;
    for (i = 0; i < this->nodes.Size(); i++)
    {
        Resource::State state = this->nodes[i]->GetResourceState();
        if (state > result)
        {
            result = state;
        }
    }
    if (Resource::Loaded == result)
    {
        if (!this->resourcesLoaded)
        {
            // all resources have been loaded
            this->OnResourcesLoaded();
        }
        return true;
    }
    else
    {
        // not all resources have been loaded yet
        return false;
    }
}

//------------------------------------------------------------------------------
/**
    This method is called once when all pending asynchronous resources have 
    been loaded (the Model is ready for rendering), this is when
    Model::GetResourceState() returns Loaded for the first time.
*/
void
Model::OnResourcesLoaded()
{
    n_assert(!this->resourcesLoaded);
    this->resourcesLoaded = true;
    IndexT i;
    for (i = 0; i < this->nodes.Size(); i++)
    {
        this->nodes[i]->OnResourcesLoaded();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
Model::AttachNode(const Ptr<ModelNode>& node)
{
    n_assert(node->GetName().Value().IsValid());
    this->nodes.Append(node);
    node->OnAttachToModel(this);
}

//------------------------------------------------------------------------------
/**
*/
void
Model::RemoveNode(const Ptr<ModelNode>& node)
{
    IndexT nodeIndex = this->nodes.FindIndex(node);
    n_assert(InvalidIndex != nodeIndex);
    node->OnRemoveFromModel();
    this->nodes.EraseIndex(nodeIndex);
}

//------------------------------------------------------------------------------
/**
*/
void
Model::AddVisibleModelNode(IndexT frameIndex, ModelNodeType::Code t, const Ptr<ModelNode>& modelNode)
{
    this->visibleModelNodes.Add(frameIndex, t, modelNode);
    if (!this->visibleModelNodes.IsResolved(t))
    {
        this->visibleModelNodes.SetResolved(t, true);
        VisResolver::Instance()->AddVisibleModel(frameIndex, t, this);
    }
}

//------------------------------------------------------------------------------
/**
*/
Ptr<ModelInstance>
Model::CreateInstance()
{
    Ptr<ModelInstance> modelInstance = ModelInstance::Create();
    this->AttachInstance(modelInstance);
    return modelInstance;
}

//------------------------------------------------------------------------------
/**
*/
void
Model::AttachInstance(const Ptr<ModelInstance>& modelInstance)
{
    n_assert(!modelInstance->IsAttachedToModel());
    this->instances.Append(modelInstance);
    modelInstance->OnAttachToModel(this);
}

//------------------------------------------------------------------------------
/**
*/
void
Model::RemoveInstance(const Ptr<ModelInstance>& modelInstance)
{
    n_assert(modelInstance->IsAttachedToModel());
    n_assert(modelInstance->GetModel().get() == this);
    modelInstance->OnRemoveFromModel();
    IndexT index = this->instances.FindIndex(modelInstance);
    n_assert(InvalidIndex != index);
    this->instances.EraseIndex(index);
}

//------------------------------------------------------------------------------
/**
    This method will update the Model's bounding box to include the 
    bounding boxes of all ModelNodes owned by the Model object.
*/
void
Model::UpdateBoundingBox()
{
    n_assert(this->nodes.Size() > 0);
    this->boundingBox.begin_extend();
    IndexT i;
    for (i = 0; i < this->nodes.Size(); i++)
    {
        this->boundingBox.extend(this->nodes[i]->GetBoundingBox());
    }
}

//------------------------------------------------------------------------------
/**
    Careful, this method is SLOW!
*/
Ptr<ModelNode>
Model::LookupNode(const String& path) const
{
    Ptr<ModelNode> curPtr;
    if (this->nodes.Size() > 0)
    {
        Array<String> tokens = path.Tokenize("/");
        n_assert(tokens.Size() > 0);
        curPtr = this->GetRootNode();
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
                    return Ptr<ModelNode>();
                }
            }
        }
    }
    return curPtr;
}

} // namespace Models
