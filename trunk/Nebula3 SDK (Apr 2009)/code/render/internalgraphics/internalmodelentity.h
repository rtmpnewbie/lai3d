#pragma once
#ifndef INTERNALGRAPHICS_INTERNALMODELENTITY_H
#define INTERNALGRAPHICS_INTERNALMODELENTITY_H
//------------------------------------------------------------------------------
/**
    @class InternalGraphics::InternalModelEntity
    
    Represents a visible graphics object.
    
    (C) 2007 Radon Labs GmbH
*/
#include "internalgraphics/internalgraphicsentity.h"
#include "resources/resourceid.h"
#include "models/managedmodel.h"
#include "models/modelinstance.h"
#include "characters/characterinstance.h"

//------------------------------------------------------------------------------
namespace InternalGraphics
{
class InternalModelEntity : public InternalGraphicsEntity
{
    __DeclareClass(InternalModelEntity);
public:
    /// constructor
    InternalModelEntity();
    /// destructor
    virtual ~InternalModelEntity();

    /// set the model's resource id
    void SetResourceId(const Resources::ResourceId& resId);
    /// get the model's resource id
    const Resources::ResourceId& GetResourceId() const;
    /// get the state of the contained managed model resource
    Resources::Resource::State GetModelResourceState() const;
    /// get pointer to model instance (only valid if already loaded)
    const Ptr<Models::ModelInstance>& GetModelInstance() const;
    /// return true if this is a character
    bool HasCharacter() const;
    /// get pointer to character instance
    const Ptr<Characters::CharacterInstance>& GetCharacterInstance() const;
    /// get pointer to character
    const Ptr<Characters::Character>& GetCharacter() const;

    /// enable/disable the Nebula2 character rotation hack (180 degree hack)
    void SetNebula2CharacterRotationHackEnabled(bool b);
    /// get Nebula2 character rotation hack flag
    bool GetNebula2CharacterRotationHackEnabled() const;

    /// set managed model
    void SetManagedModel(const Ptr<Models::ManagedModel>&  mModel);

    /// get AnimationDrivenMovement	
    const Math::vector& GetAnimationDrivenMovement() const;
    /// set AnimationDrivenMovement
    void SetAnimationDrivenMovement(const Math::vector& val);
    /// get AnimationJointName	
    const Util::String& GetAnimationJointName() const;
    /// set AnimationJointName
    void SetAnimationJointName(const Util::String& val);

protected:
    /// called when entity is created
    virtual void OnActivate();
    /// called before entity is destroyed
    virtual void OnDeactivate();
    /// called when transform matrix changed
    virtual void OnTransformChanged();
    /// called to update the entity before rendering
    virtual void OnUpdate(Timing::Time time);
    /// called when the entity has been found visible, may be called several times per frame!
    virtual void OnNotifyVisible(const Ptr<InternalGraphicsEntity>& observer);
    /// called to render a debug visualization of the entity
    virtual void OnRenderDebug();
    /// validate the ModelInstance
    void ValidateModelInstance();    
    /// called when the entity becomes visible
    virtual void OnShow();
    /// called when the entity becomes invisible
    virtual void OnHide();

    IndexT updateFrameId;
    IndexT renderFrameId;
    Resources::ResourceId resId;
    Ptr<Models::ManagedModel> managedModel;
    Ptr<Models::ModelInstance> modelInstance;
    Ptr<Characters::CharacterInstance> charInst;
    Ptr<Characters::Character> character;
    bool nebula2CharacterRotationHack;
    Math::vector animationDrivenMovement;

    Timing::Tick lastAnimEventCheckEndTime;
    Util::String animationJointName;
};

//------------------------------------------------------------------------------
/**
*/
inline void
InternalModelEntity::SetNebula2CharacterRotationHackEnabled(bool b)
{
    this->nebula2CharacterRotationHack = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
InternalModelEntity::GetNebula2CharacterRotationHackEnabled() const
{
    return this->nebula2CharacterRotationHack;
}

//------------------------------------------------------------------------------
/**
*/
inline void
InternalModelEntity::SetResourceId(const Resources::ResourceId& id)
{
    n_assert(!this->IsActive());
    this->resId = id;
}

//------------------------------------------------------------------------------
/**
*/
inline const Resources::ResourceId&
InternalModelEntity::GetResourceId() const
{
    return this->resId;
}

//------------------------------------------------------------------------------
/**
*/
inline Resources::Resource::State
InternalModelEntity::GetModelResourceState() const
{
    return this->managedModel->GetState();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Models::ModelInstance>&
InternalModelEntity::GetModelInstance() const
{
    return this->modelInstance;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
InternalModelEntity::HasCharacter() const
{
    return this->charInst.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Characters::CharacterInstance>&
InternalModelEntity::GetCharacterInstance() const
{
    return this->charInst;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Characters::Character>&
InternalModelEntity::GetCharacter() const
{
    return this->character;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
InternalModelEntity::SetManagedModel(const Ptr<Models::ManagedModel>& mModel)
{
    this->managedModel = mModel;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::vector& 
InternalModelEntity::GetAnimationDrivenMovement() const
{
    return this->animationDrivenMovement;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
InternalModelEntity::SetAnimationDrivenMovement(const Math::vector& val)
{
    this->animationDrivenMovement = val;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
InternalModelEntity::SetAnimationJointName(const Util::String& val)
{
    this->animationJointName = val;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::String& 
InternalModelEntity::GetAnimationJointName() const
{
    return this->animationJointName;
}

} // namespace InternalGraphics
//------------------------------------------------------------------------------
#endif
    