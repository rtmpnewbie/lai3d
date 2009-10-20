#pragma once
//------------------------------------------------------------------------------
/**
    @class Characters::CharacterInstance
    
    Contains the per-instance data of a character.
        
    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "characters/characterskeletoninstance.h"
#include "characters/characterskinset.h"
#include "characters/characteranimationcontroller.h"

//------------------------------------------------------------------------------
namespace Characters
{
class Character;
    
class CharacterInstance : public Core::RefCounted
{
    __DeclareClass(CharacterInstance);
public:
    /// constructor
    CharacterInstance();
    /// destructor
    virtual ~CharacterInstance();

    /// setup the character instance from a character
    void Setup(const Ptr<Character>& origCharacter, const Ptr<Models::ModelInstance>& modelInst);
    /// discard the character instance
    void Discard();
    /// return true if the object has been setup
    bool IsValid() const;

    /// update time, call once per frame BEFORE manipulating character
    void UpdateTime(Timing::Tick time);
    /// prepare the character for rendering, call once per frame right before rendering
    void PrepareForRendering();
    /// render a debug visualization of the character
    void RenderDebug(const Math::matrix44& modelTransform);

    /// access to get the character's skeleton
    CharacterSkeletonInstance& Skeleton();
    /// access to character's skin set
    CharacterSkinSet& SkinSet();
    /// access to character's animation controller
    CharacterAnimationController& AnimController();
    /// access to the character's texture set
    //CharacterTextureSet& TextureSet();
    /// access to the character's variation
    //CharacterVariationSet& VariationSet();

private:
    /// transfer current animation samples to joint skeleton
    void TransferAnimationSamplesToSkeleton();

    Ptr<Character> character;
    Ptr<Models::ModelInstance> modelInstance;
    CharacterSkeletonInstance skeletonInst;
    CharacterSkinSet skinSet;
    CharacterAnimationController animController;
    //CharacterTextureSet textureSet;
    //CharacterVariationSet variationSet;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
CharacterInstance::IsValid() const
{
    return this->character.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
inline CharacterSkeletonInstance&
CharacterInstance::Skeleton()
{
    return this->skeletonInst;
}

//------------------------------------------------------------------------------
/**
*/
inline CharacterSkinSet&
CharacterInstance::SkinSet()
{
    return this->skinSet;
}

//------------------------------------------------------------------------------
/**
*/
inline CharacterAnimationController&
CharacterInstance::AnimController()
{
    return this->animController;
}

} // namespace Characters
//------------------------------------------------------------------------------

    