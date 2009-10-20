#pragma once
//------------------------------------------------------------------------------
/**
    @class Characters::CharacterJointInstance
    
    Contains the per-instance data of a character joint.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/types.h"
#include "math/vector.h"
#include "math/matrix44.h"
#include "math/quaternion.h"

//------------------------------------------------------------------------------
namespace Characters
{
class CharacterJoint;

class CharacterJointInstance
{
public:
    /// constructor
    CharacterJointInstance();
    /// destructor
    ~CharacterJointInstance();    
    /// setup the joint instance
    void Setup(const CharacterJoint* joint, const CharacterJointInstance* parent);

    /// get pointer to CharacterJoint object
    const CharacterJoint* GetJoint() const;
    /// get pointer to parent joint instance
    const CharacterJointInstance* GetParentJointInstance() const;

    /// set current translation
    void SetTranslation(const Math::vector& t);
    /// get current translation
    const Math::vector& GetTranslation() const;
    /// set current rotation
    void SetRotation(const Math::quaternion& r);
    /// get current rotation
    const Math::quaternion& GetRotation() const;
    /// set current scale
    void SetScale(const Math::vector& s);
    /// get current scale
    const Math::vector& GetScale() const;
    /// set optional variation scale
    void SetVariationScale(const Math::vector& s);
    /// get optional variation scale
    const Math::vector& GetVariationScale() const;
    /// load translation, rotation, scale directly from 3 float4's in memory
    void Load(const Math::float4* ptr);
    
    /// evaluate the joint instance
    void Evaluate();

    /// get current evaluated model-space matrix
    const Math::matrix44& GetJointMatrix() const;
    /// get current skinning matrix 
    const Math::matrix44& GetSkinMatrix() const;

private:
    const CharacterJoint* charJoint;
    CharacterJointInstance* parentJointInst;

    Math::vector translation;
    Math::quaternion rotation;
    Math::vector scale;
    Math::vector variationScale;
    Math::float4 finalScale;        // w-component always 1.0f!

    Math::matrix44 unscaledMatrix;
    Math::matrix44 scaledMatrix;
    Math::matrix44 skinMatrix;

    bool dirty;
};

//------------------------------------------------------------------------------
/**
*/
inline const CharacterJoint*
CharacterJointInstance::GetJoint() const
{
    return this->charJoint;
}

//------------------------------------------------------------------------------
/**
*/
inline const CharacterJointInstance*
CharacterJointInstance::GetParentJointInstance() const
{
    return this->parentJointInst;
}

//------------------------------------------------------------------------------
/**
*/
inline void
CharacterJointInstance::Load(const Math::float4* ptr)
{
    this->dirty = true;
    this->translation.load((Math::scalar*) &ptr[0]);
    this->rotation.load((Math::scalar*) &ptr[1]);
    this->scale.load((Math::scalar*) &ptr[2]);

    // need to merge variation scale as well
    this->finalScale = Math::float4::multiply(this->scale, this->variationScale);
    this->finalScale.w() = 1.0f;
}

//------------------------------------------------------------------------------
/**
*/
inline void
CharacterJointInstance::SetTranslation(const Math::vector& t)
{
    this->translation = t;
    this->dirty = true;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::vector&
CharacterJointInstance::GetTranslation() const
{
    return this->translation;
}

//------------------------------------------------------------------------------
/**
*/
inline void
CharacterJointInstance::SetRotation(const Math::quaternion& q)
{
    this->rotation = q;
    this->dirty = true;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::quaternion&
CharacterJointInstance::GetRotation() const
{
    return this->rotation;
}

//------------------------------------------------------------------------------
/**
*/
inline void
CharacterJointInstance::SetScale(const Math::vector& s)
{
    this->scale = s;
    this->finalScale = Math::float4::multiply(this->scale, this->variationScale);
    this->finalScale.w() = 1.0f;
    this->dirty = true;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::vector&
CharacterJointInstance::GetScale() const
{
    return this->scale;
}

//------------------------------------------------------------------------------
/**
*/
inline void
CharacterJointInstance::SetVariationScale(const Math::vector& s)
{
    this->variationScale = s;
    this->finalScale = Math::float4::multiply(this->scale, this->variationScale);
    this->finalScale.w() = 1.0f;
    this->dirty = true;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::vector&
CharacterJointInstance::GetVariationScale() const
{
    return this->variationScale;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
CharacterJointInstance::GetJointMatrix() const
{
    return this->scaledMatrix;
}

//------------------------------------------------------------------------------
/**
*/  
inline const Math::matrix44&
CharacterJointInstance::GetSkinMatrix() const
{
    return this->skinMatrix;
}

} // namespace Characters
//------------------------------------------------------------------------------

    