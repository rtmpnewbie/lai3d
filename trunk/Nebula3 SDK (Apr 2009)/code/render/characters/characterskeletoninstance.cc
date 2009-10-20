//------------------------------------------------------------------------------
//  characterskeletoninstance.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "characters/characterskeletoninstance.h"

// for debug visualization
#include "coregraphics/shaperenderer.h"
#include "threading/thread.h"

namespace Characters
{
using namespace Math;
using namespace Util;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
CharacterSkeletonInstance::CharacterSkeletonInstance() :
    isValid(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
CharacterSkeletonInstance::~CharacterSkeletonInstance()
{
    n_assert(!this->IsValid());
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterSkeletonInstance::Setup(const CharacterSkeleton& skeleton)
{
    n_assert(!this->IsValid());
    n_assert(skeleton.IsValid());

    this->isValid = true;

    // setup joint instance array
    this->jointInstArray.SetSize(skeleton.GetNumJoints());
    IndexT i;
    for (i = 0; i < skeleton.GetNumJoints(); i++)
    {
        const CharacterJoint& joint = skeleton.GetJoint(i);
        const CharacterJointInstance* parentJointInst = 0;
        if (joint.HasParentJoint())
        {
            parentJointInst = &(this->jointInstArray[joint.GetParentJointIndex()]);
        }
        this->jointInstArray[i].Setup(&joint, parentJointInst);
    }

    // evaluate the skeleton once to setup initial joint matrices
    this->Evaluate();
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterSkeletonInstance::Discard()
{
    n_assert(this->IsValid());
    this->jointInstArray.Clear();
    this->isValid = false;
}

//------------------------------------------------------------------------------
/**
    Directly load joint positions, rotations and scales from a float4 sample
    buffer (usually containing animation samples). The number of samples
    must be NumJoint * 3, and the order is: position, rotation, scale.
*/
void
CharacterSkeletonInstance::Load(const float4* samples, SizeT numSamples)
{
    n_assert(0 != samples);
    n_assert(numSamples == this->GetNumJoints() * 3);

    IndexT i = 0;
    SizeT num = this->jointInstArray.Size();
    for (i = 0; i < num; i++)
    {
        this->jointInstArray[i].Load(samples);
        samples += 3;
    }
}

//------------------------------------------------------------------------------
/**
    Evaluate the joints in the skeleton. Must be called after joints
    have been updated with animation data (usually called by 
    the CharacterNodeInstance which own this skeleton instance).
*/
void
CharacterSkeletonInstance::Evaluate()
{
    IndexT i;
    SizeT num = this->jointInstArray.Size();
    for (i = 0; i < num; i++)
    {
        this->jointInstArray[i].Evaluate();
    }
}

//------------------------------------------------------------------------------
/**
    Render a debug visualization of the character.
*/
void
CharacterSkeletonInstance::RenderDebug(const matrix44& modelTransform)
{
    Array<float4> lineList;
    IndexT i;
    SizeT numJoints = this->GetNumJoints();
    for (i = 0; i < numJoints; i++)
    {
        const CharacterJointInstance& joint = this->GetJoint(i);
        if (0 != joint.GetParentJointInstance())
        {
            lineList.Append(joint.GetParentJointInstance()->GetJointMatrix().get_position());
            lineList.Append(joint.GetJointMatrix().get_position());
        }
    }
    if (!lineList.IsEmpty())
    {
        RenderShape shape;
        shape.SetupPrimitives(Threading::Thread::GetMyThreadId(),
                              modelTransform,
                              PrimitiveTopology::LineList,
                              lineList.Size() / 2,
                              &(lineList.Front()),
                              4,
                              float4(1.0f, 1.0f, 0.0f, 1.0f));
        ShapeRenderer::Instance()->AddShape(shape);
    }
}

} // namespace Characters