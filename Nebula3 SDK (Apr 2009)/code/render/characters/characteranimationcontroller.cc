//------------------------------------------------------------------------------
//  characteranimationcontroller.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "characters/characteranimationcontroller.h"
#include "animation/playclipjob.h"

namespace Characters
{
using namespace Math;
using namespace Util;
using namespace Timing;
using namespace Animation;
using namespace CoreAnimation;

//------------------------------------------------------------------------------
/**
*/
CharacterAnimationController::CharacterAnimationController() :
    isValid(false),
    animDrivenMotionJointIndex(InvalidIndex),
    animDrivenMotionVector(vector::nullvec())
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
CharacterAnimationController::~CharacterAnimationController()
{
    n_assert(!this->IsValid());
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterAnimationController::Setup(const CharacterAnimationLibrary& animLib)
{
    n_assert(!this->IsValid());
    this->isValid = true;

    // setup embedded animation controller
    this->animSequencer.Setup(animLib.GetAnimResource());
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterAnimationController::Discard()
{
    n_assert(this->IsValid());
    this->animSequencer.Discard();
    this->isValid = false;
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterAnimationController::PlayOnce(const StringAtom& clipName, const StringAtom& category, Tick startTime, Tick fadeInTime, Tick fadeOutTime, Tick sampleTimeOffset, int blendPriority, float blendWeight, Timing::Time timeFactor)
{
    const Ptr<AnimResource>& animRes = this->animSequencer.GetAnimResource();
    IndexT clipIndex = animRes->GetClipIndexByName(clipName);
    if (InvalidIndex == clipIndex)
    {
        n_error("CharacterAnimationController: Invalid clip name '%s'!", clipName.Value().AsCharPtr());
    }

    // create a new animation job
    Ptr<PlayClipJob> animJob = PlayClipJob::Create();
    animJob->SetName(clipName);
    animJob->SetCategory(category);
    animJob->SetClipIndex(clipIndex);
    animJob->SetStartTime(startTime);
    animJob->SetDuration(animRes->GetClipByIndex(clipIndex).GetClipDuration());
    animJob->SetFadeInTime(fadeInTime);
    animJob->SetFadeOutTime(fadeOutTime);
    animJob->SetTimeOffset(sampleTimeOffset);
    animJob->SetTimeFactor(timeFactor);
    animJob->SetBlendPriority(blendPriority);
    animJob->SetBlendWeight(blendWeight);
    animJob->SetAnimDrivenMotionJointIndex(this->animDrivenMotionJointIndex);
    this->animSequencer.StartAnimJob(animJob.cast<AnimJob>());
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterAnimationController::PlayLoop(const StringAtom& clipName, const StringAtom& category, uint loopCount, Tick startTime, Tick fadeInTime, Tick fadeOutTime, Tick sampleTimeOffset, int blendPriority, float blendWeight, Timing::Time timeFactor)
{
    const Ptr<AnimResource>& animRes = this->animSequencer.GetAnimResource();
    IndexT clipIndex = animRes->GetClipIndexByName(clipName);
    if (InvalidIndex == clipIndex)
    {
        n_error("CharacterAnimationController: Invalid clip name '%s'!", clipName.Value().AsCharPtr());
    }

    // create a new animation job
    Timing::Tick playDuration = animRes->GetClipByIndex(clipIndex).GetClipDuration() * loopCount;
    Ptr<PlayClipJob> animJob = PlayClipJob::Create();
    animJob->SetName(clipName);
    animJob->SetCategory(category);
    animJob->SetClipIndex(clipIndex);
    animJob->SetStartTime(startTime);
    animJob->SetDuration(playDuration);
    animJob->SetFadeInTime(fadeInTime);
    animJob->SetFadeOutTime(fadeOutTime);
    animJob->SetTimeOffset(sampleTimeOffset);
    animJob->SetTimeFactor(timeFactor);
    animJob->SetBlendPriority(blendPriority);
    animJob->SetBlendWeight(blendWeight);
    animJob->SetAnimDrivenMotionJointIndex(this->animDrivenMotionJointIndex);
    this->animSequencer.StartAnimJob(animJob.cast<AnimJob>());
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterAnimationController::PlayDuration(const StringAtom& clipName, const StringAtom& category, Tick duration, Tick startTime, Tick fadeInTime, Tick fadeOutTime, Tick sampleTimeOffset, int blendPriority, float blendWeight, Timing::Time timeFactor)
{
    const Ptr<AnimResource>& animRes = this->animSequencer.GetAnimResource();
    IndexT clipIndex = animRes->GetClipIndexByName(clipName);
    if (InvalidIndex == clipIndex)
    {
        n_error("CharacterAnimationController: Invalid clip name '%s'!", clipName.Value().AsCharPtr());
    }

    // create a new animation job
    Ptr<PlayClipJob> animJob = PlayClipJob::Create();
    animJob->SetName(clipName);
    animJob->SetCategory(category);
    animJob->SetClipIndex(clipIndex);
    animJob->SetStartTime(startTime);
    animJob->SetDuration(duration);
    animJob->SetFadeInTime(fadeInTime);
    animJob->SetFadeOutTime(fadeOutTime);
    animJob->SetTimeOffset(sampleTimeOffset);
    animJob->SetTimeFactor(timeFactor);
    animJob->SetBlendPriority(blendPriority);
    animJob->SetBlendWeight(blendWeight);
    animJob->SetAnimDrivenMotionJointIndex(this->animDrivenMotionJointIndex);
    this->animSequencer.StartAnimJob(animJob.cast<AnimJob>());
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterAnimationController::StopClips(const StringAtom& clipName)
{
    this->animSequencer.StopAnimJobsByName(clipName);
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterAnimationController::StopCategory(const StringAtom& category)
{
    this->animSequencer.StopAnimJobsByCategory(category);
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterAnimationController::StopAll()
{
    this->animSequencer.StopAllAnimJobs();
}

//------------------------------------------------------------------------------
/**
    Update the animation controller's current time. This method must be
    called once per frame after animations have been defined.
*/
void
CharacterAnimationController::UpdateTime(Timing::Tick t)
{
    this->animSequencer.UpdateTime(t);
}   

//------------------------------------------------------------------------------
/**
    Update the character's animation. This method must be called once
    per frame *after* starting or stopping animations! The method will
    return false if no animation is currently active.
*/
bool
CharacterAnimationController::UpdateAnimation()
{
    this->UpdateAnimDrivenMotionVector();
    return this->animSequencer.UpdateAnimation();
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterAnimationController::UpdateAnimDrivenMotionVector()
{
    if (InvalidIndex != this->animDrivenMotionJointIndex)
    {        
        this->animDrivenMotionVector = this->animSequencer.GetAnimDrivenMotionResult() * -1;     
    }
    else
    {
        this->animDrivenMotionVector = vector::nullvec();
    }
}

} // namespace Characters
