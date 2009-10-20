#pragma once
//------------------------------------------------------------------------------
/**
    @class Characters::CharacterSkeletonInstance
    
    Contains the per-instance skeleton data of a character.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/types.h"
#include "characters/characterskeleton.h"
#include "characters/characterjointinstance.h"

//------------------------------------------------------------------------------
namespace Characters
{
class CharacterSkeletonInstance
{
public:
    /// constructor
    CharacterSkeletonInstance();
    /// destructor
    ~CharacterSkeletonInstance();
    
    /// setup from CharacterSkeleton
    void Setup(const CharacterSkeleton& skeleton);
    /// discard the object
    void Discard();
    /// return true if the object has been setup
    bool IsValid() const;
    /// render a debug visualization of the character
    void RenderDebug(const Math::matrix44& modelTransform);

    /// load joint positions, rotations, scales from float4 sample buffer
    void Load(const Math::float4* samples, SizeT numSamples);
    /// evaluate the joints (computes new skin matrices)
    void Evaluate();

    /// get number of joint instances in the skeleton
    SizeT GetNumJoints() const;
    /// get a joint instance by index
    const CharacterJointInstance& GetJoint(IndexT i) const;
    /// access to joints
    const Util::FixedArray<CharacterJointInstance>& GetJointArray() const;

private:
    Util::FixedArray<CharacterJointInstance> jointInstArray;
    bool isValid;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
CharacterSkeletonInstance::IsValid() const
{
    return this->isValid;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
CharacterSkeletonInstance::GetNumJoints() const
{
    return this->jointInstArray.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline const CharacterJointInstance&
CharacterSkeletonInstance::GetJoint(IndexT i) const
{
    return this->jointInstArray[i];
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::FixedArray<CharacterJointInstance>&
CharacterSkeletonInstance::GetJointArray() const
{
    return this->jointInstArray;
}

} // namespace Characters
//------------------------------------------------------------------------------

    