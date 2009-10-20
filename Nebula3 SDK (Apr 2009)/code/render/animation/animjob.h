#pragma once
#ifndef ANIMATION_ANIMJOB_H
#define ANIMATION_ANIMJOB_H
//------------------------------------------------------------------------------
/**
    @class Animation::AnimJob
  
    Descibes a single animation sampling job in the AnimController. AnimJob
    objects have a start time and a duration and are arranged in parallel 
    tracks. The sampling results of parallel AnimJobs at a given point in
    time are mixed into a single resulting animation by the AnimController.
    Subclasses of AnimJob are used to implement specific tasks like
    a lookat-controller, IK, and so forth...

    FIXME: the current implementation of setting an absolute evaluation
    time doesn't allow to manipulate the playback speed (for this,
    advancing the time by a relative amount would be better).

    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "coreanimation/animresource.h"
#include "coreanimation/animsamplebuffer.h"
#include "math/vector.h"
#include "math/point.h"

//------------------------------------------------------------------------------
namespace Animation
{
class AnimSequencer;
class AnimEventInfo;

class AnimJob : public Core::RefCounted
{
    __DeclareClass(AnimJob);
public:
    /// constructor
    AnimJob();
    /// destructor
    virtual ~AnimJob();

    /// called when attached to anim sequencer
    virtual void OnAttachedToSequencer(const AnimSequencer& animSequencer);
    /// called when removed from sequencer
    virtual void OnRemoveFromSequencer();
    /// return true if the job is currently attached to a sequencer
    bool IsAttachedToSequencer() const;

    /// evaluate the animation into the provided result buffer, returns current blend weight
    virtual float Evaluate(Timing::Tick time, const Ptr<CoreAnimation::AnimSampleBuffer>& result, Math::point& lastResult, Math::vector& deltaResult);
    /// return true if the job has currently playing (EvalTime within start/end time)
    bool IsActive(Timing::Tick time) const;
    /// return true if the job has been queued for playback but has not started yet
    bool IsPending(Timing::Tick time) const;
    /// return true if the job has finished playback
    bool IsFinished(Timing::Tick time) const;
    /// return true when the job has expired
    bool IsExpired(Timing::Tick time) const;

    /// set a name for this anim job
    void SetName(const Util::StringAtom& name);
    /// get name of this anim job
    const Util::StringAtom& GetName() const;
    /// set a category name for this anim job
    void SetCategory(const Util::StringAtom& category);
    /// get category name of this anim job
    const Util::StringAtom& GetCategory() const;
    /// set the base time of the anim job (set by sequencer when job is attached)
    void SetBaseTime(Timing::Tick time);
    /// get the base time of the anim job
    Timing::Tick GetBaseTime() const;
    /// set the start time of the anim job (always relative to base time, so 0 means "now")
    void SetStartTime(Timing::Tick time);
    /// get the start time of the anim job (always relative to base time)
    Timing::Tick GetStartTime() const;
    /// set the duration of the anim job (0 == infinite)
    void SetDuration(Timing::Tick time);
    /// get the duration of the anim job
    Timing::Tick GetDuration() const;
    /// return true if the anim job is infinite
    bool IsInfinite() const;
    /// set the fade-in time of the anim job
    void SetFadeInTime(Timing::Tick fadeInTime);
    /// get the fade-in time of the anim job
    Timing::Tick GetFadeInTime() const;
    /// set the fade-out time of the anim job
    void SetFadeOutTime(Timing::Tick fadeOutTime);
    /// get the fade-out time of the anim job
    Timing::Tick GetFadeOutTime() const;
    /// set expire time, job will be removed from sequencer when expired
    void SetExpireTime(Timing::Tick expTime);
    /// get expire time
    Timing::Tick GetExpireTime() const;
    /// set blend weight of the anim job (default is 1.0)
    void SetBlendWeight(float w);
    /// get blend weight of the anim job
    float GetBlendWeight() const;
    /// set the blend priority (default priority is 0)
    void SetBlendPriority(int blendPriority);
    /// get the track index of the anim job
    int GetBlendPriority() const;
    /// set time factor
    void SetTimeFactor(Timing::Time timeFactor);
    /// get time factor
    Timing::Time GetTimeFactor() const;
    /// set sample time offset (if sampling should not start at the beginning)
    void SetTimeOffset(Timing::Tick timeOffset);
    /// get sample time offset
    Timing::Tick GetTimeOffset() const;

    /// get last position of animation driven joint	
    Math::point& GetLastAnimDrivenJointPosition();
    /// set anim-drive-motion joint index
    void SetAnimDrivenMotionJointIndex(IndexT val);
    /// get anim-driven-motion joint index
    IndexT GetAnimDrivenMotionJointIndex() const;

    /// emit anim events inside given time range
    virtual Util::Array<AnimEventInfo> EmitAnimEvents(Timing::Tick startTime, Timing::Tick endTime, const Util::String& optionalCatgeory);

    /// compute sample time for next evaluation, always done, also if character isn't visible and no evaluation takes place
    virtual void UpdateTimes(Timing::Tick time);
    
protected:
    /// compute current relative evaluation time, this is a helper method for subclasses
    Timing::Tick ComputeRelEvalTime(Timing::Tick time) const;
    /// compute current blend weight, this should take fade-in into account
    float ComputeBlendWeight(Timing::Tick relEvalTime) const;
    /// fix fade-in/fade-out times if the sum is bigger then the play duration
    void FixFadeTimes();

    const AnimSequencer* animSequencer;
    Util::StringAtom name;
    Util::StringAtom category;
    int blendPriority;
    float blendWeight;
    Timing::Tick baseTime;
    Timing::Tick startTime;
    Timing::Tick duration;
    Timing::Tick fadeInTime;
    Timing::Tick fadeOutTime;
    Timing::Tick expireTime;
    Timing::Tick curRelEvalTime;
    Timing::Tick lastRelEvalTime;
    Timing::Tick curSampleTime;
    Timing::Tick lastSampleTime;
    Timing::Time timeFactor;
    Timing::Tick timeOffset;
    Math::point lastJointPosition;
    IndexT animDrivenMotionJointIndex;

#if NEBULA3_DEBUG
    bool timesUpToDate;
#endif
};

//------------------------------------------------------------------------------
/**
*/
inline void
AnimJob::SetName(const Util::StringAtom& n)
{
    this->name = n;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom&
AnimJob::GetName() const
{
    return this->name;
}

//------------------------------------------------------------------------------
/**
*/
inline void
AnimJob::SetCategory(const Util::StringAtom& cat)
{
    this->category = cat;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom&
AnimJob::GetCategory() const
{
    return this->category;
}

//------------------------------------------------------------------------------
/**
*/
inline void
AnimJob::SetBaseTime(Timing::Tick t)
{
    this->baseTime = t;
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Tick
AnimJob::GetBaseTime() const
{
    return this->baseTime;
}

//------------------------------------------------------------------------------
/**
*/
inline void
AnimJob::SetStartTime(Timing::Tick t)
{
    this->startTime = t;
    this->lastSampleTime = t;
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Tick
AnimJob::GetStartTime() const
{
    return this->startTime;
}

//------------------------------------------------------------------------------
/**
*/
inline void
AnimJob::SetDuration(Timing::Tick t)
{
    this->duration = t;
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Tick
AnimJob::GetDuration() const
{
    return this->duration;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
AnimJob::IsInfinite() const
{
    return (0 == this->duration);
}

//------------------------------------------------------------------------------
/**
*/
inline void
AnimJob::SetFadeInTime(Timing::Tick t)
{
    this->fadeInTime = t;
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Tick
AnimJob::GetFadeInTime() const
{
    return this->fadeInTime;
}

//------------------------------------------------------------------------------
/**
*/
inline void
AnimJob::SetFadeOutTime(Timing::Tick t)
{
    this->fadeOutTime = t;
}
//------------------------------------------------------------------------------
/**
*/
inline Timing::Tick
AnimJob::GetFadeOutTime() const
{
    return this->fadeOutTime;
}

//------------------------------------------------------------------------------
/**
*/
inline void
AnimJob::SetExpireTime(Timing::Tick t)
{
    this->expireTime = t;
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Tick
AnimJob::GetExpireTime() const
{
    return this->expireTime;
}

//------------------------------------------------------------------------------
/**
*/
inline void
AnimJob::SetBlendWeight(float w)
{
    this->blendWeight = w;
}

//------------------------------------------------------------------------------
/**
*/
inline float
AnimJob::GetBlendWeight() const
{
    return this->blendWeight;
}

//------------------------------------------------------------------------------
/**
*/
inline void
AnimJob::SetBlendPriority(int pri)
{
    this->blendPriority = pri;
}

//------------------------------------------------------------------------------
/**
*/
inline int
AnimJob::GetBlendPriority() const
{
    return this->blendPriority;
}

//------------------------------------------------------------------------------
/**
*/
inline Math::point& 
AnimJob::GetLastAnimDrivenJointPosition()
{
    return this->lastJointPosition;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
AnimJob::SetTimeFactor(Timing::Time timeFactor)
{
    this->timeFactor = timeFactor;
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Time 
AnimJob::GetTimeFactor() const
{
    return this->timeFactor;
}

//------------------------------------------------------------------------------
/**
*/
inline void
AnimJob::SetAnimDrivenMotionJointIndex(IndexT val)
{
    this->animDrivenMotionJointIndex = val;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT
AnimJob::GetAnimDrivenMotionJointIndex() const
{
    return this->animDrivenMotionJointIndex;
}

//------------------------------------------------------------------------------
/**
*/
inline void
AnimJob::SetTimeOffset(Timing::Tick t)
{
    this->timeOffset = t;
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Tick
AnimJob::GetTimeOffset() const
{
    return this->timeOffset;
}
} // namespace Animation
//------------------------------------------------------------------------------
#endif
