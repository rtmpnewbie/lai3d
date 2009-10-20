#pragma once
#ifndef ANIMATION_PLAYCLIPJOB_H
#define ANIMATION_PLAYCLIPJOB_H
//------------------------------------------------------------------------------
/** 
    @class Animation::PlayClipJob
    
    An AnimJob which simply plays an animation clip.
    
    (C) 2008 Radon Labs GmbH
*/
#include "animation/animjob.h"

//------------------------------------------------------------------------------
namespace Animation
{
class PlayClipJob : public AnimJob
{
    __DeclareClass(PlayClipJob);
public:
    /// constructor
    PlayClipJob();
    /// destructor
    virtual ~PlayClipJob();
    /// evaluate the animation into the provided result buffer
    virtual float Evaluate(Timing::Tick time, const Ptr<CoreAnimation::AnimSampleBuffer>& result, Math::point& lastResult, Math::vector& deltaResult);

    /// set the clip index to play
    void SetClipIndex(IndexT clipIndex);
    /// get the animation clip index
    IndexT GetClipIndex() const;

    /// emit anim events inside given time range
    virtual Util::Array<AnimEventInfo> EmitAnimEvents(Timing::Tick startTime, Timing::Tick endTime, const Util::String& optionalCatgeory);

private:
    IndexT clipIndex;
    IndexT lastKeyIndex;
};

//------------------------------------------------------------------------------
/**
*/
inline void
PlayClipJob::SetClipIndex(IndexT i)
{
    this->clipIndex = i;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT
PlayClipJob::GetClipIndex() const
{
    return this->clipIndex;
}

} // namespace Animation
//------------------------------------------------------------------------------
#endif

