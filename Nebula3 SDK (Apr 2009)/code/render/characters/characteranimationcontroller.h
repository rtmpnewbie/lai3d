#pragma once
//------------------------------------------------------------------------------
/**
    @class Characters::CharacterAnimationController
    
    Allows to control the animation of a character instance, this is
    basically a convenience wrapper around Animation::AnimSequencer.
    
    (C) 2008 Radon Labs GmbH
*/
#include "animation/animsequencer.h"
#include "characters/characteranimationlibrary.h"

//------------------------------------------------------------------------------
namespace Characters
{
class CharacterAnimationController
{
public:
    /// constructor
    CharacterAnimationController();
    /// destructor
    ~CharacterAnimationController();

    /// setup the anim controller object
    void Setup(const CharacterAnimationLibrary& animLib);
    /// discard the anim controller object
    void Discard();
    /// return true if object has been setup
    bool IsValid() const;

    /// direct access to embedded anim sequencer object
    Animation::AnimSequencer& AnimSequencer();

    /// set animation driven movement joint index (InvalidIndex if anim driven movement is off)
    void SetAnimDrivenMotionJointIndex(IndexT i);
    /// get animatiom driven movement joint index
    IndexT GetAnimDrivenMotionJointIndex() const;
    /// get computed animation driven motion vector
    const Math::vector& GetAnimDrivenMotionVector() const;
    /// check if character is animation driven, looks for valid joint index
    bool IsAnimationDriven() const;

    /// play a one-shot animation
    void PlayOnce(const Util::StringAtom& clipName, const Util::StringAtom& category, Timing::Tick startTime, Timing::Tick fadeInTime, Timing::Tick fadeOutTime, Timing::Tick sampleTimeOffset, int blendPriority, float blendWeight, Timing::Time timeFactor);
    /// play a looped animation (loop=0: loop infinitely)
    void PlayLoop(const Util::StringAtom& clipName, const Util::StringAtom& category, uint loopCount, Timing::Tick startTime, Timing::Tick fadeInTime, Timing::Tick fadeOutTime, Timing::Tick sampleTimeOffset, int blendPriority, float blendWeight, Timing::Time timeFactor);
    /// play an animation for a given duration
    void PlayDuration(const Util::StringAtom& clipName, const Util::StringAtom& category, Timing::Tick duration, Timing::Tick startTime, Timing::Tick fadeInTime, Timing::Tick fadeOutTime, Timing::Tick sampleTimeOffset, int blendPriority, float blendWeight, Timing::Time timeFactor);
    /// stop animations matching clip name
    void StopClips(const Util::StringAtom& clipName);
    /// stop animations matching category name
    void StopCategory(const Util::StringAtom& categoryName);
    /// stop all animations
    void StopAll();

    /// update the animation time, call for every character, even if not currently visible
    void UpdateTime(Timing::Tick time);
    /// update animation, call only if character object is on-screen
    bool UpdateAnimation();
    /// get the time which is currently set in the anim controller
    Timing::Tick GetTime() const;

private:
    /// update anim-driven-motion vector
    void UpdateAnimDrivenMotionVector();

    Animation::AnimSequencer animSequencer;
    bool isValid;
    IndexT animDrivenMotionJointIndex;
    Math::vector animDrivenMotionVector;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
CharacterAnimationController::IsValid() const
{
    return this->isValid;
}

//------------------------------------------------------------------------------
/**
*/
inline Animation::AnimSequencer&
CharacterAnimationController::AnimSequencer()
{
    return this->animSequencer;
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Tick
CharacterAnimationController::GetTime() const
{
    return this->animSequencer.GetTime();
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::vector& 
CharacterAnimationController::GetAnimDrivenMotionVector() const
{
    return this->animDrivenMotionVector;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
CharacterAnimationController::SetAnimDrivenMotionJointIndex(IndexT i)
{
    this->animDrivenMotionJointIndex = i;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT
CharacterAnimationController::GetAnimDrivenMotionJointIndex() const
{
    return this->animDrivenMotionJointIndex;
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
CharacterAnimationController::IsAnimationDriven() const
{
    return (this->animDrivenMotionJointIndex != InvalidIndex);
}
} // namespace Characters
//------------------------------------------------------------------------------
    