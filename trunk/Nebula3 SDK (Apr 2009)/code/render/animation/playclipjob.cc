//------------------------------------------------------------------------------
//  playclipjob.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "animation/playclipjob.h"
#include "animation/animsequencer.h"
#include "coreanimation/animsampler.h"
#include "coreanimation/animeventemitter.h"
#include "coreanimation/animdrivenmotionsampler.h"

namespace Animation
{
__ImplementClass(Animation::PlayClipJob, 'PCLJ', Animation::AnimJob);

using namespace CoreAnimation;

//------------------------------------------------------------------------------
/**
*/
PlayClipJob::PlayClipJob() :
    clipIndex(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
PlayClipJob::~PlayClipJob()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
float
PlayClipJob::Evaluate(Timing::Tick time, const Ptr<AnimSampleBuffer>& resultBuffer, Math::point& lastResult, Math::vector& deltaResult)
{
    n_assert(this->IsActive(time));
    n_assert(this->IsAttachedToSequencer());

    // computation of relative evaluation time must be done before in UpdateTimes
#if NEBULA3_DEBUG
    //n_assert(this->timesUpToDate);
    this->timesUpToDate = false;
#endif

    // perform sampling
    if (InvalidIndex == this->animDrivenMotionJointIndex)
    {
        // no anim driven motion stuff
        AnimSampler::Sample(this->animSequencer->GetAnimResource(), 
                            this->clipIndex, 
                            SampleType::Linear,
                            this->curSampleTime, 
                            resultBuffer);
    }
    else
    {
        AnimDrivenMotionSampler::Sample(this->animSequencer->GetAnimResource(), 
                            this->clipIndex, SampleType::Linear, 
                            this->curSampleTime, resultBuffer, 
                            this->animDrivenMotionJointIndex * 3, 
                            lastResult, 
                            deltaResult,
                            this->lastKeyIndex);
    }

    // return the current blend weight back to the AnimSequencer
    float curBlendWeight = this->ComputeBlendWeight(this->curRelEvalTime);
    return curBlendWeight;
}

//------------------------------------------------------------------------------
/**
    collects animevents active in time range and fills animevent info array
*/
Util::Array<AnimEventInfo> 
PlayClipJob::EmitAnimEvents(Timing::Tick startTime, Timing::Tick endTime, const Util::String& optionalCatgeory)
{
    Util::Array<AnimEventInfo> eventInfos;
    n_assert(this->animSequencer != 0);
    const AnimClip& clip = this->animSequencer->GetAnimResource()->GetClipByIndex(this->clipIndex);

    // map absolute time to clip relative time
    int nettoClipDuration;
    if(InvalidIndex == this->animDrivenMotionJointIndex)
    {
        nettoClipDuration = clip.GetNumKeys() * clip.GetKeyDuration();
        startTime = ((int)((startTime - this->baseTime) * (float)this->timeFactor)) % nettoClipDuration;
        endTime = ((int)((endTime - this->baseTime) * (float)this->timeFactor)) % nettoClipDuration;
    }
    else
    {
        nettoClipDuration = (clip.GetNumKeys()-1) * clip.GetKeyDuration();
        startTime = ((int)((startTime - this->baseTime) * (float)this->timeFactor)) % nettoClipDuration;
        endTime = ((int)((endTime - this->baseTime) * (float)this->timeFactor)) % nettoClipDuration;
    }

    Util::Array<AnimEvent> events = AnimEventEmitter::EmitAnimEvents(clip, startTime, endTime, this->IsInfinite());
    IndexT i;
    for (i = 0; i < events.Size(); ++i)
    {
        if (optionalCatgeory.IsEmpty() || 
            events[i].GetCategory().IsValid() && 
            optionalCatgeory == events[i].GetCategory().Value())
        {
            AnimEventInfo newInfo;
            newInfo.SetAnimEvent(events[i]);
            newInfo.SetAnimJobName(this->GetName());
            newInfo.SetWeight(this->blendWeight);
    	    eventInfos.Append(newInfo);
        }
    }
    return eventInfos;
}
} // namespace Animation
