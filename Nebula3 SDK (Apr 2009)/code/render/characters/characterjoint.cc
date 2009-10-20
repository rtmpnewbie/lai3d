//------------------------------------------------------------------------------
//  characterjoint.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "characters/characterjoint.h"

namespace Characters
{
using namespace Util;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
CharacterJoint::CharacterJoint() :
    poseTranslation(vector::nullvec()),
    poseRotation(quaternion::identity()),
    poseScale(1.0f, 1.0f, 1.0f),
    poseMatrix(matrix44::identity()),
    invPoseMatrix(matrix44::identity()),
    parentJointIndex(InvalidIndex),
    parentJoint(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
CharacterJoint::~CharacterJoint()
{
    this->parentJointIndex = InvalidIndex;
    this->parentJoint = 0;
    this->name.Clear();
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterJoint::Setup(IndexT parentIndex, const CharacterJoint* parent, const Math::vector& t, const Math::quaternion& r, const Math::vector& s, const Util::StringAtom& n)
{
    this->poseTranslation = t;
    this->poseRotation = r;
    this->poseScale = s;
    this->parentJointIndex = parentIndex;
    this->parentJoint = parent;
    this->name = n;

    this->poseMatrix = matrix44::identity();
    this->poseMatrix.scale(this->poseScale);
    this->poseMatrix = matrix44::multiply(this->poseMatrix, matrix44::rotationquaternion(this->poseRotation));
    this->poseMatrix.translate(this->poseTranslation);
    if (0 != parent)
    {
        this->poseMatrix = matrix44::multiply(this->poseMatrix, parent->poseMatrix);
    }
    this->invPoseMatrix = matrix44::inverse(this->poseMatrix);
}

} // namespace Characters
