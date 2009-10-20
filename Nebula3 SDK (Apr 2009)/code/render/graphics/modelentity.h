#pragma once
//------------------------------------------------------------------------------
/**
    @class Graphics::ModelEntity
    
    Client-side proxy of a InternalGraphics::InternalModelEntity.
    
    (C) 2008 Radon Labs GmbH
*/
#include "graphics/graphicsentity.h"
#include "graphics/graphicsprotocol.h"
#include "math/quaternion.h"

//------------------------------------------------------------------------------
namespace Graphics
{
class ModelEntity : public GraphicsEntity
{
    __DeclareClass(ModelEntity);
public:
    /// constructor
    ModelEntity();
    /// destructor
    virtual ~ModelEntity();

    /// set the model's resource id
    void SetResourceId(const Resources::ResourceId& resId);
    /// get the model's resource id
    const Resources::ResourceId& GetResourceId() const;
    /// set instance shader variable
    void SetShaderVariable(const Util::String& nodeName, const Util::String& variableSemantic, const Util::Variant& value);
    /// set animation driven
    void SetAnimationJointName(const Util::String& jointName);
    /// get animation driven flag
    bool IsCharacterAnimationDriven();
    /// update animation driven transform
    void UpdateAnimationDrivenTransform();
    /// get AnimationDrivenMovement	
    const Math::vector& GetAnimationDrivenMovement() const;
    /// get ClipStatus	
    Math::ClipStatus::Type GetClipStatus() const;

private:
    /// called by stage when entity should setup itself
    virtual void Setup(const Ptr<Stage>& stage);

    Resources::ResourceId resId;
    Util::String animationJointName;
    Math::vector animationDrivenMovement;
    Ptr<GetAnimationDrivenVector> updateMsg;
    Math::ClipStatus::Type clipStatus; // currently only valid with aniamtion driven movement use    
};

//------------------------------------------------------------------------------
/**
*/
inline void
ModelEntity::SetResourceId(const Resources::ResourceId& id)
{
    this->resId = id;
}

//------------------------------------------------------------------------------
/**
*/
inline const Resources::ResourceId&
ModelEntity::GetResourceId() const
{
    return this->resId;
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
ModelEntity::IsCharacterAnimationDriven()
{
    return this->animationJointName.IsValid();
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::vector& 
ModelEntity::GetAnimationDrivenMovement() const
{
    return this->animationDrivenMovement;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
ModelEntity::SetAnimationJointName(const Util::String& jointName)
{
    this->animationJointName = jointName;
}

//------------------------------------------------------------------------------
/**
*/
inline Math::ClipStatus::Type 
ModelEntity::GetClipStatus() const
{
    return this->clipStatus;
}
} // namespace Graphics
//------------------------------------------------------------------------------
