//------------------------------------------------------------------------------
//  characterinstance.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "characters/characterinstance.h"
#include "characters/character.h"
#include "coreanimation/animsamplebuffer.h"
#include "math/point.h"
#include "math/vector.h"
#include "math/quaternion.h"
#include "models/modelinstance.h"
#include "internalgraphics/internalmodelentity.h"

// for debug visualization
#include "coregraphics/shaperenderer.h"
#include "threading/thread.h"

namespace Characters
{
__ImplementClass(Characters::CharacterInstance, 'CRIN', Core::RefCounted);

using namespace Util;
using namespace Math;
using namespace CoreAnimation;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
CharacterInstance::CharacterInstance()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
CharacterInstance::~CharacterInstance()
{
    n_assert(!this->IsValid());
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterInstance::Setup(const Ptr<Character>& origCharacter, const Ptr<Models::ModelInstance>& modelInst)
{
    n_assert(!this->IsValid());
    n_assert(origCharacter.isvalid());
    n_assert(origCharacter->IsValid());
    
    this->character = origCharacter;
    this->modelInstance = modelInst;
    this->skeletonInst.Setup(origCharacter->Skeleton());
    this->skinSet.Setup(origCharacter->SkinLibrary(), modelInst);
    this->animController.Setup(origCharacter->AnimationLibrary());
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterInstance::Discard()
{
    n_assert(this->IsValid());    

    this->skeletonInst.Discard();
    this->skinSet.Discard();
    this->animController.Discard();
    this->modelInstance = 0;
    this->character = 0;
}

//------------------------------------------------------------------------------
/**
    Update the character's animation time. This method should be called
    per-frame from within the CharacterModelNodeInstance's Update method
*/
void
CharacterInstance::UpdateTime(Timing::Tick time)
{
    n_assert(this->IsValid());
    this->animController.UpdateTime(time);
}

//------------------------------------------------------------------------------
/**
    Prepare the character for rendering. Call this method once per frame 
    after the character has been manipulated (like starting or stopping
    animations), and only if the character is currently visible. This method
    will for instance evaluate the character's animation, transfer the animation
    to the joint skeleton, and do everything else necessary before rendering,
    so it's relatively expensive and should only be called when the character
    is actually on screen.
*/
void
CharacterInstance::PrepareForRendering()
{
    n_assert(this->IsValid());

    // update the character's animation state
    if (this->animController.UpdateAnimation())
    {
        // transfer sampled animation to joint skeleton
        this->TransferAnimationSamplesToSkeleton();

        // evaluate the skeleton (updates skinning matrices)
        this->skeletonInst.Evaluate();
    }
}

//------------------------------------------------------------------------------
/**
    This method will transfer the current animation samples to the joint 
    skeleton.
*/
void
CharacterInstance::TransferAnimationSamplesToSkeleton()
{
    // gain access to animation samples
    const Ptr<AnimSampleBuffer> sampleBuffer = this->animController.AnimSequencer().GetResult();
    SizeT numSamples = sampleBuffer->GetNumSamples();
    const float4* samplesPtr = sampleBuffer->MapSamples();
    
    // transfer sample data
    this->skeletonInst.Load(samplesPtr, numSamples);

    // release access sample buffer
    sampleBuffer->UnmapSamples();
}

//------------------------------------------------------------------------------
/**
    Render a debug visualization of the character.
*/
void
CharacterInstance::RenderDebug(const matrix44& modelTransform)
{
    this->skeletonInst.RenderDebug(modelTransform);
       
    float4 rootPositionDelta = this->animController.AnimSequencer().GetAnimDrivenMotionResult();    
    Array<float4> lineList;    
    lineList.Append(modelTransform.get_position());
    lineList.Append(rootPositionDelta * 10.0f);
    if (!lineList.IsEmpty())
    {
        RenderShape shape;
        shape.SetupPrimitives(Threading::Thread::GetMyThreadId(),
            modelTransform,
            PrimitiveTopology::LineList,
            lineList.Size() / 2,
            &(lineList.Front()),
            4,
            float4(1.0f, 0.0f, 1.0f, 1.0f));
        ShapeRenderer::Instance()->AddShape(shape);
    }
}

} // namespace Characters
