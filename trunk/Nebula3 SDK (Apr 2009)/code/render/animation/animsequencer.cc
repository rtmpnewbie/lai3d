//------------------------------------------------------------------------------
//  animsequencer.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "animation/animsequencer.h"
#include "animation/playclipjob.h"
#include "coreanimation/animmixer.h"
#include "coreanimation/animdrivenmotionsampler.h"

namespace Animation
{
__ImplementClass(Animation::AnimSequencer, 'ASQR', Core::RefCounted);

using namespace Util;
using namespace CoreAnimation;

//------------------------------------------------------------------------------
/**
*/
AnimSequencer::AnimSequencer() :
    time(0),
    animDrivenMotion(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
AnimSequencer::~AnimSequencer()
{
    if (this->IsValid())
    {
        this->Discard();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
AnimSequencer::Setup(const Ptr<AnimResource>& animRsrc)
{
    n_assert(!this->IsValid());
    n_assert(animRsrc.isvalid());
    
    this->time = 0;
    this->animResource = animRsrc;
    IndexT i;
    for (i = 0; i < NumSampleBuffers; i++)
    {
        this->srcSampleBuffer[i] = AnimSampleBuffer::Create();
        this->srcSampleBuffer[i]->Setup(animRsrc);
    }
    this->dstSampleBuffer = AnimSampleBuffer::Create();
    this->dstSampleBuffer->Setup(animRsrc);
}

//------------------------------------------------------------------------------
/**
*/
void
AnimSequencer::Discard()
{
    n_assert(this->IsValid());
    this->animResource = 0;
    IndexT i;
    for (i = 0; i < NumSampleBuffers; i++)
    {
        this->srcSampleBuffer[i]->Discard();
        this->srcSampleBuffer[i] = 0;
    }
    this->dstSampleBuffer->Discard();
    this->dstSampleBuffer = 0;    
    for (i = 0; i < this->animJobs.Size(); i++)
    {
        this->animJobs[i]->OnRemoveFromSequencer();
        this->animJobs[i] = 0;
    }
    this->animJobs.Clear();
    this->startedAnimJobs.Clear();
    this->stoppedAnimJobs.Clear();
}

//------------------------------------------------------------------------------
/**
    Start an anim job. This will schedule the anim job for insertion in
    the next Evaluate(). This deferred handling is necessary because the
    actual base time of the anim job job is only known in Evaluate() (need
    to be careful to prevent those pesky one-frame problems).
*/
void
AnimSequencer::StartAnimJob(const Ptr<AnimJob>& animJob)
{
    this->startedAnimJobs.Append(animJob);
}

//------------------------------------------------------------------------------
/**
    Stop an anim job. This will schedule the anim job for stopping during
    the next Evaluate() (this is necessary to prevent that the anim job
    is one frame off. The fade-out time of the anim job will be taken into
    account, so the animation will actually stop at (now + fadeOutTime).
*/
void
AnimSequencer::StopAnimJob(const Ptr<AnimJob>& animJob)
{
    this->stoppedAnimJobs.Append(animJob);
}

//------------------------------------------------------------------------------
/**
    Stops all anim jobs matching the given name.
*/
void
AnimSequencer::StopAnimJobsByName(const StringAtom& name)
{
    IndexT i;
    for (i = 0; i < this->animJobs.Size(); i++)
    {
        const Ptr<AnimJob> animJob = this->animJobs[i];
        if (animJob->GetName().IsValid() && name.IsValid() && (animJob->GetName() == name))
        {
            this->StopAnimJob(animJob);
        }
    }
    for (i = 0; i < this->startedAnimJobs.Size(); i++)
    {
        const Ptr<AnimJob> animJob = this->startedAnimJobs[i];
        if (animJob->GetName().IsValid() && name.IsValid() && (animJob->GetName() == name))
        {
            this->StopAnimJob(animJob);
        }
    }
}

//------------------------------------------------------------------------------
/**
    Stops all anim jobs matching the given category.
*/
void
AnimSequencer::StopAnimJobsByCategory(const StringAtom& category)
{
    IndexT i;
    for (i = 0; i < this->animJobs.Size(); i++)
    {
        const Ptr<AnimJob>& animJob = this->animJobs[i];
        if (animJob->GetCategory().IsValid() && category.IsValid() && (animJob->GetCategory() == category))
        {
            this->StopAnimJob(animJob);
        }
    }
    for (i = 0; i < this->startedAnimJobs.Size(); i++)
    {
        const Ptr<AnimJob>& animJob = this->startedAnimJobs[i];
        if (animJob->GetCategory().IsValid() && category.IsValid() && (animJob->GetCategory() == category))
        {
            this->StopAnimJob(animJob);
        }
    }
}

//------------------------------------------------------------------------------
/**
    Stops all anim jobs in the sequencer.
*/
void
AnimSequencer::StopAllAnimJobs()
{
    IndexT i;
    for (i = 0; i < this->animJobs.Size(); i++)
    {
        this->StopAnimJob(this->animJobs[i]);
    }
    for (i = 0; i < this->startedAnimJobs.Size(); i++)
    {
        this->StopAnimJob(this->startedAnimJobs[i]);
    }
}

//------------------------------------------------------------------------------
/**
    This immediately removes an anim job from the sequencer, not taking
    any fade-out times into account.
*/
void
AnimSequencer::CancelAnimJob(const Ptr<AnimJob>& animJob)
{
    n_assert(animJob->IsAttachedToSequencer());
    IndexT index = this->animJobs.FindIndex(animJob);
    n_assert(InvalidIndex != index);
    animJob->OnRemoveFromSequencer();
    this->animJobs.EraseIndex(index);
}

//------------------------------------------------------------------------------
/**
    This method priority-inserts new anim jobs from the startedAnimJobs
    array into the animJobs array and sets the base time of the anim job so
    that it is properly synchronized with the current time. This method
    is called from Evaluate().
*/
void
AnimSequencer::InsertStartedAnimJobs(Timing::Tick time)
{
    // for each new anim job...
    IndexT startedAnimJobIndex;
    for (startedAnimJobIndex = 0; startedAnimJobIndex < this->startedAnimJobs.Size(); startedAnimJobIndex++)
    {
        const Ptr<AnimJob>& animJob = this->startedAnimJobs[startedAnimJobIndex];
        
        // set the anim job's base time to "now"
        animJob->SetBaseTime(time);

        // find the right insertion index
        IndexT i = 0;
        while ((i < this->animJobs.Size()) && 
               (this->animJobs[i]->GetBlendPriority() <= animJob->GetBlendPriority()))
        {
            if (this->animJobs[i]->GetBlendPriority() == animJob->GetBlendPriority())
            {
                if (this->animJobs[i]->GetStartTime() > animJob->GetStartTime())
                {
                    break;
                }
            }
            i++;
        }
        this->animJobs.Insert(i, animJob);

        // notify anim job that it has been attached to a sequencer    
        animJob->OnAttachedToSequencer(*this);
    }

    // clear the started anim jobs array for the next "frame"
    this->startedAnimJobs.Clear();
}

//------------------------------------------------------------------------------
/**
    This method updates the duration of each new stopped anim job
    so that it will stop at "now + fadeOutTime". This method is called
    from Evaluate() with the current evaluation time.
*/
void
AnimSequencer::UpdateStoppedAnimJobs(Timing::Tick time)
{
    // for each stopped anim job
    IndexT stoppedAnimJobIndex;
    for (stoppedAnimJobIndex = 0; stoppedAnimJobIndex < this->stoppedAnimJobs.Size(); stoppedAnimJobIndex++)
    {
        const Ptr<AnimJob>& animJob = this->stoppedAnimJobs[stoppedAnimJobIndex];

        // convert absolute current time to anim-job relative time, and add the fade-out time
        Timing::Tick newEndTime = (time - animJob->GetBaseTime()) + animJob->GetFadeOutTime();
        
        // compute the new duration of the anim job        
        Timing::Tick newDuration = newEndTime - animJob->GetStartTime();
        if (newDuration < 1)
        {
            newDuration = 1;
        }
        animJob->SetDuration(newDuration);
    }

    // clear the stopped anim jobs array for the next "frame"
    this->stoppedAnimJobs.Clear();
}

//------------------------------------------------------------------------------
/**
    Iterates through the currently attached anim jobs and
    removes all anim jobs which are expired. This method is called from
    Evaluate().
*/
void
AnimSequencer::RemoveExpiredAnimJobs(Timing::Tick time)
{
    IndexT i;
    for (i = this->animJobs.Size() - 1; i != InvalidIndex; i--)
    {
        if (this->animJobs[i]->IsExpired(time))
        {
            this->animJobs[i]->OnRemoveFromSequencer();
            this->animJobs.EraseIndex(i);
        }
    }
}   

//------------------------------------------------------------------------------
/**
    Update the current time of the sequencer. This should be called
    exactly once per frame, even if the animated object is currently
    invisible (near the camera but outside the view volume). This will
    update the anim jobs which have been started or stopped this frame,
    and it will remove expired anim jobs, but will not sample the animation.
*/
void
AnimSequencer::UpdateTime(Timing::Tick curTime)
{
    // update our current time
    this->time = curTime;

    // insert new anim jobs
    this->InsertStartedAnimJobs(time);
    
    // update any new stopped anim jobs
    this->UpdateStoppedAnimJobs(time);

    // discard any expired anim jobs
    this->RemoveExpiredAnimJobs(time);

    // update time of all active anim jobs
    this->UpdateTimeActiveAnimJobs(time);
}

//------------------------------------------------------------------------------
/**
    This method should be called once per-frame for each visible animated
    object AFTER UpdateTime() has been called. Actual animation sampling
    and mixing happens here.
*/
bool
AnimSequencer::UpdateAnimation()
{
    // get the number of active anim jobs beforehand, if only one active anim job
    // exists, sample directly into the destination buffer without mixing
    IndexT i;
    IndexT numActiveAnimJobs = 0;
    IndexT activeAnimJobIndex = InvalidIndex;
    for (i = 0; i < this->animJobs.Size(); i++)
    {
        if (this->animJobs[i]->IsActive(this->time))
        {
            numActiveAnimJobs++;
            activeAnimJobIndex = i;
        }
    }

    // if no active anim job exists, return false
    if (0 == numActiveAnimJobs)
    {
        return false;
    }
    else if (1 == numActiveAnimJobs)
    {
        // if only one anim job currently active, no mixing is needed, sample
        // directly into the destination buffer
        float curWeight = this->animJobs[activeAnimJobIndex]->Evaluate(this->time, this->dstSampleBuffer, this->animJobs[activeAnimJobIndex]->GetLastAnimDrivenJointPosition(), this->animDrivenMotionResult);
        return (curWeight > 0.0);
    }
    else
    {
        // more then one active anim job at the current time stamp, need to perform mixing
        activeAnimJobIndex = 0;
        bool allJobsZeroWeight = false;
        for (i = 0; i < this->animJobs.Size(); i++)
        {
            if (this->animJobs[i]->IsActive(this->time))
            {
                IndexT srcIndex = activeAnimJobIndex++ & 1;

                // sample current anim job
                float curWeight = this->animJobs[i]->Evaluate(this->time, this->srcSampleBuffer[srcIndex], this->animJobs[i]->GetLastAnimDrivenJointPosition(), this->srcAnimDrivenMotionBuffer[srcIndex]);

                allJobsZeroWeight |= curWeight > 0.0f;
                // blend with previous anim buffer into destination sample buffer
                if (i > 0)
                {
                    Ptr<AnimSampleBuffer> sampleBuffer = this->srcSampleBuffer[1 - srcIndex];
                    if (i > 1) sampleBuffer = this->dstSampleBuffer;
                    AnimMixer::Mix(this->animResource, 
                                   sampleBuffer, 
                                   this->srcSampleBuffer[srcIndex],
                                   curWeight,
                                   this->dstSampleBuffer);

                    if (this->animDrivenMotion)
                    {
                        Math::vector* sampleDeltaBuffer = &this->srcAnimDrivenMotionBuffer[1 - srcIndex];
                        if (i > 1) sampleDeltaBuffer = &this->animDrivenMotionResult;
                        // TODO: write Mix function only for translation
                        AnimDrivenMotionSampler::MixPositionDelta(this->animResource, 
                                       sampleDeltaBuffer, 
                                       &this->srcAnimDrivenMotionBuffer[srcIndex],
                                       curWeight,
                                       &this->animDrivenMotionResult,
                                       this->animJobs[i]->GetAnimDrivenMotionJointIndex());
                    }                    
                }
            }
        }
        return allJobsZeroWeight;
    }
}

//------------------------------------------------------------------------------
/**
*/
Util::Array<Ptr<AnimJob> > 
AnimSequencer::GetAnimJobsByName(const Util::StringAtom& name) const
{
    Util::Array<Ptr<AnimJob> > resultJobs;
    IndexT jobIdx;
    for (jobIdx = 0; jobIdx < this->animJobs.Size(); ++jobIdx)
    {
    	if (this->animJobs[jobIdx]->GetName() == name)
        {
            resultJobs.Append(this->animJobs[jobIdx]);
        }
    }
    return resultJobs;
}

//------------------------------------------------------------------------------
/**
    Collects all AnimEventInfos of all animjobs which are active in the given time range.
    If justDominatingJob flag is set, just use the clip with most blend factor.
*/
Util::Array<AnimEventInfo> 
AnimSequencer::EmitAnimEvents(Timing::Tick startTime, Timing::Tick endTime, bool justDominatingJob, const Util::String& optionalCatgeory) const
{
    Util::Array<AnimEventInfo> eventInfos;
    IndexT i = 0;
    SizeT jobCount = this->animJobs.Size();

    if(justDominatingJob)
    {
        // overwrite loop parameters
        i = this->FindDominatingJobIndex();
        jobCount = (InvalidIndex == i) ? -1 : 1 + i;
    }

    for (i; i < jobCount; i++)
    {
        if (this->animJobs[i]->IsActive(startTime) && this->animJobs[i]->IsActive(endTime))
        {
            eventInfos.AppendArray(this->animJobs[i]->EmitAnimEvents(startTime, endTime, optionalCatgeory));
        }
    }

    return eventInfos;
}

//------------------------------------------------------------------------------
/**
    Collects all AnimEventInfos of all animjobs which are active in the given time range.
    If justDominatingJob flag is set, just use the clip with most blend factor.
*/
Util::Array<AnimEventInfo> 
AnimSequencer::EmitAnimEventsForOneKeyDuration(Timing::Tick startTime,
                                               Timing::Tick endTime,
                                               bool justDominatingJob,                                                
                                               const Util::String& optionalCatgeory) const
{
    Util::Array<AnimEventInfo> eventInfos;
    IndexT i = 0;
    SizeT jobCount = this->animJobs.Size();

    if(justDominatingJob)
    {
        // overwrite loop parameters
        i = this->FindDominatingJobIndex();
        jobCount = (InvalidIndex == i) ? -1 : 1 + i;
    }

    for (i; i < jobCount; i++)
    {
        const Ptr<PlayClipJob> clipJob = this->animJobs[i].cast<PlayClipJob>();
        if (clipJob->IsActive(startTime))
        {            
            const int& clipIndex = clipJob->GetClipIndex();
            endTime = startTime + this->GetAnimResource()->GetClipByIndex(clipIndex).GetKeyDuration();

            eventInfos.AppendArray(clipJob->EmitAnimEvents(startTime, endTime, optionalCatgeory));
        }   
    }
    return eventInfos;
}

//------------------------------------------------------------------------------
/**
    find the dominating job
*/
IndexT
AnimSequencer::FindDominatingJobIndex() const
{
    // find dominating job
    float maxVal = 0.0f;
    IndexT maxIndex = InvalidIndex;
    IndexT i;
    for (i = 0; i < this->animJobs.Size(); i++)
    {
        float tempVal = this->animJobs[i]->GetBlendPriority() * this->animJobs[i]->GetBlendWeight();
        if(tempVal >= maxVal)
        {
            maxVal = tempVal;
            maxIndex = i;
        }
    }
    return maxIndex;
}

//------------------------------------------------------------------------------
/**
    Get the key duration
*/
Timing::Tick
AnimSequencer::GetMaximumKeyDuration() const
{
    Timing::Tick ticks = 0;
    IndexT i = 0;
    SizeT jobCount = this->animJobs.Size();
    for (i; i < jobCount; i++)
    {
        const Ptr<PlayClipJob> clipJob = this->animJobs[i].cast<PlayClipJob>();
        const int& clipIndex = clipJob->GetClipIndex();
        const Timing::Tick& dur = this->GetAnimResource()->GetClipByIndex(clipIndex).GetKeyDuration();
        if (dur > ticks)
        {
            ticks = dur;
        }
    } 
    return ticks;
}

//------------------------------------------------------------------------------
/**
*/
void  
AnimSequencer::SetAnimDrivenMotion(bool val)
{
    this->animDrivenMotion = val;
}

//------------------------------------------------------------------------------
/**
*/
void 
AnimSequencer::UpdateTimeActiveAnimJobs(Timing::Tick time)
{
    // update times of anim jobs started this frame
    IndexT i;
    for (i = 0; i < this->startedAnimJobs.Size(); i++)
    {
        this->startedAnimJobs[i]->UpdateTimes(time);        
    }
    // update already active jobs
    for (i = 0; i < this->animJobs.Size(); i++)
    {
        this->animJobs[i]->UpdateTimes(time);        
    }
}
} // namespace Animation