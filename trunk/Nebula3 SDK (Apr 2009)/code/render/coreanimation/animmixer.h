#pragma once
#ifndef COREANIMATION_ANIMMIXER_H
#define COREANIMATION_ANIMMIXER_H
//------------------------------------------------------------------------------
/**
    @class CoreAnimation::AnimMixer
    
    A class which contains methods to mix 2 samples animation clips
    into a result.

    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "coreanimation/animresource.h"
#include "coreanimation/animsampler.h"

//------------------------------------------------------------------------------
namespace CoreAnimation
{
class AnimMixer
{
public:
    /// mix 2 source anim samplers into a destination sampler: dst = lerp(src0, src1, lerpValue)
    static void Mix(const Ptr<AnimResource>& animResource, const Ptr<AnimSampleBuffer>& src0, const Ptr<AnimSampleBuffer>& src1, float lerpValue, const Ptr<AnimSampleBuffer>& dst);
};

} // namespace AnimMixer
//------------------------------------------------------------------------------
#endif
