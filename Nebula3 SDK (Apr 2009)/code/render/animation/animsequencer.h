#pragma once
#ifndef ANIMATION_ANIMSEQEUNCER_H
#define ANIMATION_ANIMSEQUENCER_H
//------------------------------------------------------------------------------
/**
    @class Animation::AnimSequencer
  
    An AnimSequencer object arranges AnimJobs along the time line to
    produce a single, priority-blended result. AnimJobs which are overlapping
    on the time-line will be blended by the following rules:

    - AnimJobs with a higher blend priority dominate lower-priority anim jobs
    - if AnimJobs have the same blend priority, the start time of the anim
      job is used to determine blend priority (jobs which start later dominate
      jobs which start earlier)

    (C) 2008 Radon Labs GmbH
*/    
#include "core/refcounted.h"
#include "animation/animjob.h"
#include "animation/animeventinfo.h"

//------------------------------------------------------------------------------
namespace Animation
{
class AnimSequencer : public Core::RefCounted
{
    __DeclareClass(AnimSequencer);
public:
    /// constructor
    AnimSequencer();
    /// destructor
    virtual ~AnimSequencer();

    /// setup the animation controller
    void Setup(const Ptr<CoreAnimation::AnimResource>& animResource);
    /// discard the anim sequencer
    void Discard();
    /// return true if between Setup/Discard
    bool IsValid() const;

    /// start an anim job
    void StartAnimJob(const Ptr<AnimJob>& animJob);
    /// stop an anim job (taking fade-out time into account)
    void StopAnimJob(const Ptr<AnimJob>& animJob);
    /// stop animation jobs by name
    void StopAnimJobsByName(const Util::StringAtom& name);
    /// stop animation jobs by category
    void StopAnimJobsByCategory(const Util::StringAtom& category);
    /// stops all animation jobs
    void StopAllAnimJobs();
    /// cancel a pending anim job, immediately removing it from the sequencer
    void CancelAnimJob(const Ptr<AnimJob>& animJob);
    /// get anim jobs by name
    Util::Array<Ptr<AnimJob> > GetAnimJobsByName(const Util::StringAtom& name) const;
    /// get anim jobs by category name
    Util::Array<Ptr<AnimJob> > GetAnimJobsByCategory(const Util::StringAtom& category) const;

    /// update the animation sequencer time
    void UpdateTime(Timing::Tick time);
    /// evaluate the sequencer at the currently set time
    bool UpdateAnimation();
    /// get the currently set time
    Timing::Tick GetTime() const;
    /// get the final sampled result of the last evaluation
    const Ptr<CoreAnimation::AnimSampleBuffer>& GetResult() const;
    /// get pointer to animation resource object
    const Ptr<CoreAnimation::AnimResource>& GetAnimResource() const;

    /// set ComputeDeltaTranslation
    void SetAnimDrivenMotion(bool val);
    /// get ComputeDeltaTranslation	
    bool GetComputeDeltaTranslation() const;
    /// get the final sampled result of the last delta evaluation
    const Math::vector& GetAnimDrivenMotionResult() const;

    /// emit anim event infos
    Util::Array<AnimEventInfo> EmitAnimEvents(  Timing::Tick startTime, 
                                                Timing::Tick endTime, 
                                                bool justDominatingJob,
                                                const Util::String& optionalCatgeory = "") const;
    /// emit anim events, but just use one key duration as time
    Util::Array<AnimEventInfo> EmitAnimEventsForOneKeyDuration( Timing::Tick startTime,
                                                                Timing::Tick endTime,
                                                                bool justDominatingJob,                                                
                                                                const Util::String& optionalCatgeory = "") const;
    /// get maximum key duration 
    Timing::Tick GetMaximumKeyDuration() const;

private:
    /// insert new anim jobs (called from Evaluate)
    void InsertStartedAnimJobs(Timing::Tick time);
    /// update stopped anim jobs (called from Evaluate)
    void UpdateStoppedAnimJobs(Timing::Tick time);
    /// delete expired anim jobs
    void RemoveExpiredAnimJobs(Timing::Tick time);
    /// update active animjobs times
    void UpdateTimeActiveAnimJobs(Timing::Tick time);

    /// helper method, for determining dominating job
    IndexT FindDominatingJobIndex() const;

    static const int NumSampleBuffers = 2;

    Timing::Tick time;
    bool animDrivenMotion;
    Ptr<CoreAnimation::AnimResource> animResource;
    Ptr<CoreAnimation::AnimSampleBuffer> srcSampleBuffer[NumSampleBuffers];
    Ptr<CoreAnimation::AnimSampleBuffer> dstSampleBuffer;
    Math::vector srcAnimDrivenMotionBuffer[NumSampleBuffers];
    Math::vector animDrivenMotionResult;

    Util::Array<Ptr<AnimJob> > startedAnimJobs;      // anim jobs started this "frame"
    Util::Array<Ptr<AnimJob> > stoppedAnimJobs;      // anim jobs stopped this "frame"
    Util::Array<Ptr<AnimJob> > animJobs;             // currently sequenced anim jobs
};

//------------------------------------------------------------------------------
/**
*/
inline Timing::Tick
AnimSequencer::GetTime() const
{
    return this->time;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
AnimSequencer::IsValid() const
{
    return this->animResource.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreAnimation::AnimSampleBuffer>&
AnimSequencer::GetResult() const
{
    return this->dstSampleBuffer;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::vector&
AnimSequencer::GetAnimDrivenMotionResult() const
{
    return this->animDrivenMotionResult;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreAnimation::AnimResource>&
AnimSequencer::GetAnimResource() const
{
    return this->animResource;
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
AnimSequencer::GetComputeDeltaTranslation() const
{
    return this->animDrivenMotion;
}

} // namespace Animation
//------------------------------------------------------------------------------
#endif
