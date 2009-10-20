#pragma once
#ifndef COREANIMATION_ANIMSAMPLER_H
#define COREANIMATION_ANIMSAMPLER_H
//------------------------------------------------------------------------------
/**
    @class CoreAnimation::AnimSampler
    
    A class which contains methods to sample an animation clip at a 
    specific time.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/types.h"
#include "coreanimation/animclip.h"
#include "coreanimation/sampletype.h"
#include "coreanimation/animresource.h"
#include "coreanimation/animsamplebuffer.h"

//------------------------------------------------------------------------------
namespace CoreAnimation
{
class AnimSampler
{
public:
    /// sample an animation clip at some point in time into an AnimSampleBuffer
    static void Sample(const Ptr<AnimResource>& animResource, IndexT clipIndex, SampleType::Code sampleType, Timing::Tick time, const Ptr<AnimSampleBuffer>& result);
    /// clamp key index into valid range
    static IndexT ClampKeyIndex(IndexT keyIndex, const AnimClip& clip);
    /// compute inbetween ticks for a given sample time
    static Timing::Tick InbetweenTicks(Timing::Tick sampleTime, const AnimClip& clip);
};

} // namespace CoreAnimation
//------------------------------------------------------------------------------
#endif
