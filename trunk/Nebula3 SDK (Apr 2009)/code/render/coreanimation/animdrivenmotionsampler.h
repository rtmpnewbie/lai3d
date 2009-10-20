#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreAnimation::AnimDeltaSampler
    
    A class which contains methods to sample an animation clip at a 
    specific time.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/types.h"
#include "coreanimation/animclip.h"
#include "coreanimation/sampletype.h"
#include "coreanimation/animresource.h"
#include "coreanimation/animsamplebuffer.h"
#include "math/vector.h"
#include "math/point.h"

//------------------------------------------------------------------------------
namespace CoreAnimation
{
class AnimDrivenMotionSampler
{
public:
    /// sample an animation clip at some point in time into an AnimSampleBuffer
    static void Sample(const Ptr<AnimResource>& animResource, IndexT clipIndex, SampleType::Code sampleType, Timing::Tick time, const Ptr<AnimSampleBuffer>& result, IndexT jointCurveIndex, Math::point& lastResult, Math::vector& deltaResult, IndexT& lastKeyIndex);
    /// mix position delta
    static void MixPositionDelta(const Ptr<AnimResource>& animResource, Math::vector* src0, Math::vector* src1, float lerpValue, Math::vector* dst, IndexT jointCurveIndex);
private:
    /// clamp key index into valid range
    static IndexT ClampKeyIndexForDelta(IndexT keyIndex, const AnimClip& clip);
};

} // namespace CoreAnimation
//------------------------------------------------------------------------------
