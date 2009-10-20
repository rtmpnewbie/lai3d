#pragma once
//------------------------------------------------------------------------------
/**
    @class Models::ModelInstance
    
    A ModelInstance contains the per-instance data for rendering a Model. 
    Usually there is one ModelInstance created per game object.
    
    A ModelInstance is roughly comparable to a Nebula2 nRenderContext.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "timing/time.h"
#include "models/model.h"
#include "models/modelnodeinstance.h"

namespace InternalGraphics
{
    class InternalModelEntity;
}

//------------------------------------------------------------------------------
namespace Models
{
class ModelInstance : public Core::RefCounted
{
    __DeclareClass(ModelInstance);
public:
    /// constructor
    ModelInstance();
    /// destructor
    virtual ~ModelInstance();

    /// cleanup and unlink from model
    void Discard();
    /// return true if currently attached to Model
    bool IsAttachedToModel() const;
    /// get the Model this instance was created from
    const Ptr<Model>& GetModel() const;
    /// get the ModelEntity which owns this instance
    const Ptr<InternalGraphics::InternalModelEntity>& GetModelEntity() const;

    /// set world space transform of the instance
    void SetTransform(const Math::matrix44& m);
    /// get world space transform
    const Math::matrix44& GetTransform() const;

    /// lookup a node instance, return invalid ptr if not exists, this method is SLOW
    Ptr<ModelNodeInstance> LookupNodeInstance(const Util::String& path) const;
    /// get all node instances
    const Util::Array<Ptr<ModelNodeInstance> >& GetNodeInstances() const;
    /// get pointer to top-level node instance
    const Ptr<ModelNodeInstance>& GetRootNodeInstance() const;
    /// attach a node instance which has already been setup
    void AttachNodeInstance(const Ptr<ModelNodeInstance>& nodeInst);
    /// remove a node instance, does not discard the node instance!
    void RemoveNodeInstance(const Ptr<ModelNodeInstance>& nodeInst);

    /// perform per-frame update (after setting transform, visibility, time, etc)
    void Update(Timing::Time time);

    /// render node specific debug shape
    void RenderDebug();

protected:
    friend class Model;
    friend class VisResolver;
    friend class InternalGraphics::InternalModelEntity;

    /// set pointer to ModelEntity which owns this instance
    void SetModelEntity(const Ptr<InternalGraphics::InternalModelEntity>& mdlEntity);
    /// notify the model instance that it is currently visible
    void NotifyVisible(IndexT frameIndex);
    /// called when attached to Model
    virtual void OnAttachToModel(const Ptr<Model>& model);
    /// called when removed from Model
    virtual void OnRemoveFromModel();
    /// called when the entity becomes visible with current time
    void OnShow(Timing::Time time);
    /// called when the entity becomes invisible
    void OnHide(Timing::Time time);

    Ptr<Model> model;
    Ptr<ModelNode> modelNode;
    Ptr<InternalGraphics::InternalModelEntity> modelEntity;
    Math::matrix44 transform;
    Timing::Time time;
    Util::Array<Ptr<ModelNodeInstance> > nodeInstances;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
ModelInstance::IsAttachedToModel() const
{
    return this->model.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Model>&
ModelInstance::GetModel() const
{
    return this->model;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ModelInstance::SetTransform(const Math::matrix44& m)
{
    this->transform = m;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
ModelInstance::GetTransform() const
{
    return this->transform;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Ptr<ModelNodeInstance> >&
ModelInstance::GetNodeInstances() const
{
    return this->nodeInstances;
}

} // namespace Models
//------------------------------------------------------------------------------

