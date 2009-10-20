//------------------------------------------------------------------------------
//  characterjointinstance.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "characters/characterjointinstance.h"
#include "characters/characterjoint.h"

namespace Characters
{
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
CharacterJointInstance::CharacterJointInstance() :
    charJoint(0),
    parentJointInst(0),
    translation(vector::nullvec()),
    rotation(quaternion::identity()),
    scale(1.0f, 1.0f, 1.0f),
    variationScale(1.0f, 1.0f, 1.0f),
    finalScale(1.0f, 1.0f, 1.0f, 1.0f),
    unscaledMatrix(matrix44::identity()),
    scaledMatrix(matrix44::identity()),
    skinMatrix(matrix44::identity()),
    dirty(true)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
CharacterJointInstance::~CharacterJointInstance()
{
    this->charJoint = 0;
    this->parentJointInst = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterJointInstance::Setup(const CharacterJoint* joint, const CharacterJointInstance* parent)
{
    n_assert(0 != joint);
    this->charJoint = joint;
    this->parentJointInst = const_cast<CharacterJointInstance*>(parent);
    this->SetTranslation(joint->GetPoseTranslation());
    this->SetRotation(joint->GetPoseRotation());
    this->SetScale(joint->GetPoseScale());
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterJointInstance::Evaluate()
{
    if (this->dirty)
    {
        this->dirty = false;

        this->unscaledMatrix = matrix44::rotationquaternion(this->rotation);
        this->unscaledMatrix.translate(this->translation);
        this->scaledMatrix.set_xaxis(this->unscaledMatrix.get_xaxis() * this->finalScale.x());
        this->scaledMatrix.set_yaxis(this->unscaledMatrix.get_yaxis() * this->finalScale.y());
        this->scaledMatrix.set_zaxis(this->unscaledMatrix.get_zaxis() * this->finalScale.z());
        
        if (0 == this->parentJointInst)
        {
            this->scaledMatrix.set_position(this->unscaledMatrix.get_position());
        }
        else
        {
            if (this->parentJointInst->dirty)
            {
                this->parentJointInst->Evaluate();
            }
            this->unscaledMatrix.set_position(float4::multiply(this->unscaledMatrix.get_position(), this->parentJointInst->finalScale));
            this->scaledMatrix.set_position(float4::multiply(this->unscaledMatrix.get_position(), this->parentJointInst->finalScale));
            this->unscaledMatrix = matrix44::multiply(this->unscaledMatrix, this->parentJointInst->unscaledMatrix);
            this->scaledMatrix = matrix44::multiply(this->scaledMatrix, this->parentJointInst->unscaledMatrix);
        }
        this->skinMatrix = matrix44::multiply(this->charJoint->GetInvPoseMatrix(), this->scaledMatrix);
    }
}

} // namespace Characters
