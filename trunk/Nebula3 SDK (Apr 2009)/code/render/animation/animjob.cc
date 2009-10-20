//------------------------------------------------------------------------------
//  animjob.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "animation/animjob.h"
#include "animation/animsequencer.h"

namespace Animation
{
__ImplementClass(Animation::AnimJob, 'AJOB', Core::RefCounted);

using namespace Util;
using namespace CoreAnimation;

//------------------------------------------------------------------------------
/**
*/
AnimJob::AnimJob() :
    animSequencer(0),
    blendPriority(0),
    blendWeight(1.0f),
    baseTime(0),
    startTime(0),
    duration(0),
    fadeInTime(0),
    fadeOutTime(0),
    expireTime(0),
    timeFactor(1.0),
    lastRelEvalTime(0),
    curRelEvalTime(0),
    curSampleTime(0),
    timeOffset(0),
    animDrivenMotionJointIndex(InvalidIndex)
#if NEBULA3_DEBUG
    ,timesUpToDate(false)
#endif
{
    // empty
}    

//------------------------------------------------------------------------------
/**
*/
AnimJob::~AnimJob()
{
    n_assert(!this->IsAttachedToSequencer());
}

//------------------------------------------------------------------------------
/**
*/
void
AnimJob::OnAttachedToSequencer(const AnimSequencer& animSeq)
{
    n_assert(!this->IsAttachedToSequencer());
    this->animSequencer = &animSeq;

    // if the fade-in and fade-out times add up to be greater then the
    // play duration, we need to fix them in order to prevent blending problems
    this->FixFadeTimes();
}

//------------------------------------------------------------------------------
/**
*/
void
AnimJob::OnRemoveFromSequencer()
{
    n_assert(this->IsAttachedToSequencer());
    this->animSequencer = 0;
}

//------------------------------------------------------------------------------
/**
    This method checks if the fade-in plus the fade-out times would 
    be bigger then the play-duration, if yes it will fix the fade times
    in order to prevent "blend-popping".
*/
void
AnimJob::FixFadeTimes()
{
    if (!this->IsInfinite())
    {
        Timing::Tick fadeTime = this->fadeInTime + this->fadeOutTime;
        if (fadeTime > 0)
        {
            if (fadeTime > this->duration)
            {
                float mul = float(duration) / float(fadeTime);
                this->fadeInTime = Timing::Tick(this->fadeInTime * mul);
                this->fadeOutTime = Timing::Tick(this->fadeOutTime * mul);
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
AnimJob::IsAttachedToSequencer() const
{
    return (0 != this->animSequencer);
}

//------------------------------------------------------------------------------
/**
    This method will return true if the current eval time is between
    the start time and end time of the anim job.
*/
bool
AnimJob::IsActive(Timing::Tick time) const
{
    Timing::Tick absStartTime = this->baseTime + this->startTime;
    Timing::Tick absEndTime = absStartTime + this->duration;
    if (this->IsInfinite())
    {
        return (time >= absStartTime);
    }
    else
    {
        return (time >= absStartTime) && (time < absEndTime);
    }
}

//------------------------------------------------------------------------------
/**
    This method will return true as long as the current eval time is
    before the start time (the job hasn't started yet).
*/
bool
AnimJob::IsPending(Timing::Tick time) const
{
    return (time < (this->baseTime + this->startTime));
}

//------------------------------------------------------------------------------
/**
    This method will return true if the current eval time is greater
    then the end time of the job.
*/
bool
AnimJob::IsFinished(Timing::Tick time) const
{
    if (this->IsInfinite())
    {
        return false;
    }
    else
    {
        return (time >= (this->baseTime + this->startTime + this->duration));
    }
}

//------------------------------------------------------------------------------
/**
    This method will return true if the current eval time is greater
    then the end time plus the expiration time. This means the job
    can be removed from the anim sequencer.
*/
bool
AnimJob::IsExpired(Timing::Tick time) const
{
    if (this->IsInfinite())
    {
        return false;
    } 
    else
    {
        return (time >= (this->baseTime + this->startTime + this->duration + this->expireTime));
    }
}

//------------------------------------------------------------------------------
/**
    Compute the relative evaluation time, this method is called by subclasses
    to get the clip-relative sample time. This method is allowed to return
    a negative time value, but since usually only active anim jobs are called
    back by the anim sequencer this will normally not happen.

    FIXME: this method must be more advanced if playback speed should be
    be implemented.
*/
Timing::Tick
AnimJob::ComputeRelEvalTime(Timing::Tick time) const
{
    return (time - (this->baseTime + this->startTime));
}

//------------------------------------------------------------------------------
/**
    This is a helper method for subclasses and returns the current blend
    weight for the current relative evaluation time, taking the fade-in
    and fade-out phases into account.
*/
float
AnimJob::ComputeBlendWeight(Timing::Tick relEvalTime) const
{
    n_assert(relEvalTime >= 0);

    // check if we're past the fade-out time
    Timing::Tick fadeOutStartTime = this->duration - this->fadeOutTime;
    if (!this->IsInfinite() && (this->fadeOutTime > 0) && (relEvalTime > fadeOutStartTime))
    {
        // we're during the fade-out phase
        float fadeOutWeight = this->blendWeight * (1.0f - (float(relEvalTime - fadeOutStartTime) / float(fadeOutTime)));
        return fadeOutWeight;
    }
    else if ((fadeInTime > 0) && (relEvalTime < this->fadeInTime))
    {
        // we're during the fade-in phase
        float fadeInWeight = this->blendWeight * (float(relEvalTime) / float(fadeInTime));
        return fadeInWeight;
    }
    else
    {
        // we're between the fade-in and fade-out phase
        return this->blendWeight;
    }
}

//------------------------------------------------------------------------------
/**
    This method is called by the AnimSequencer when this job is active
    (the current eval time is between the start and end time of the job).
    The AnimJob object is expected to fill the provided AnimSampleBuffer 
    with a result (sampled keys and sample counts, the sample counts indicate 
    whether a given sample contributes to the final blended result. The method
    is expected to return the blend weight for mixing by the AnimSequencer
    (usually the job's blend weight is returned, unless in the fade-in-phase
    where an interpolated blend weight from 0 to max should be returned,
    however, derived anim jobs are basically to return whatever blend
    weight they want).

    This method is usually implemented by subclasses.
*/
float
AnimJob::Evaluate(Timing::Tick time, const Ptr<AnimSampleBuffer>& result, Math::point& lastResult, Math::vector& deltaResult)
{
    // implement in subclass!
    return 0.0f;
}

//------------------------------------------------------------------------------
/**
*/
Util::Array<AnimEventInfo> 
AnimJob::EmitAnimEvents(Timing::Tick startTime, Timing::Tick endTime, const Util::String& optionalCatgeory)
{
    // implement in subclass
    Util::Array<AnimEventInfo> result;
    return result;
}

//------------------------------------------------------------------------------
/**
    Updates evaluation times. 
    Must be done every frame, even if character is not visible and so animjob is not evaluated.
*/
void 
AnimJob::UpdateTimes(Timing::Tick time)
{
    this->curRelEvalTime = this->ComputeRelEvalTime(time);        
    Timing::Tick frameTicks = this->curRelEvalTime - this->lastRelEvalTime;    
    this->lastRelEvalTime = this->curRelEvalTime;    
    Timing::Tick timeDiff = (Timing::Tick)(frameTicks * this->timeFactor);
    this->curSampleTime  = this->lastSampleTime + timeDiff;
    this->lastSampleTime = this->curSampleTime;
    this->curSampleTime += this->timeOffset;

#if NEBULA3_DEBUG   
    this->timesUpToDate = true;
#endif
}
} // namespace Animation
