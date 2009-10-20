//------------------------------------------------------------------------------
//  animmixer.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coreanimation/animmixer.h"
#include "math/quaternion.h"

namespace CoreAnimation
{
using namespace Math;

//------------------------------------------------------------------------------
/**
    Mixes 2 source sample buffers into a destination sample buffer using a
    single lerp-value between 0.0 and 1.0. Mixing takes sample counts
    into consideration. A source sample count of 0 indicates, the this
    sample is not valid and the result is made of 100% of the other sample.
    If both source samples are valid, the result is blended from both
    source samples. This gives the expected results when an animation clip
    only manipulates parts of a character skeleton.
*/
void
AnimMixer::Mix(const Ptr<AnimResource>& animResource, 
               const Ptr<AnimSampleBuffer>& src0, 
               const Ptr<AnimSampleBuffer>& src1, 
               float lerpValue,
               const Ptr<AnimSampleBuffer>& dst)
{
    n_assert(animResource.isvalid());

    // get a template clip from the anim resource, this defines the anim curve
    // types which we need for correct lerping
    const AnimClip& templClip = animResource->GetClipByIndex(0);
    n_assert(templClip.GetNumCurves() == src0->GetNumSamples());
    n_assert(templClip.GetNumCurves() == src1->GetNumSamples());
    n_assert(templClip.GetNumCurves() == dst->GetNumSamples());
    float4* src0SamplePtr = src0->MapSamples();
    float4* src1SamplePtr = src1->MapSamples();
    float4* dstSamplePtr  = 0;
    if (src0 != dst)
    {
        dstSamplePtr  = dst->MapSamples();
    }
    else
    {
        dstSamplePtr = src0SamplePtr;
    }
    uchar* src0SampleCounts = src0->MapSampleCounts();
    uchar* src1SampleCounts = src1->MapSampleCounts();
    uchar* dstSampleCounts  = 0;
    if (src0 != dst)
    {
        dstSampleCounts = dst->MapSampleCounts();
    }
    else
    {
        dstSampleCounts = src0SampleCounts;
    }

    // perform the mixing...
    IndexT i;
    SizeT num = src0->GetNumSamples();
    quaternion q0, q1, qDst;
    float4 f0, f1, fDst;
    for (i = 0; i < num; i++)
    {
        const AnimCurve& curve = templClip.CurveByIndex(i);
        uchar src0Count = *src0SampleCounts++;
        uchar src1Count = *src1SampleCounts++;
        *dstSampleCounts++ = src0Count + src1Count;

        if ((src0Count > 0) && (src1Count > 0))
        {
            // both source samples are valid, need to mix into result
            if (curve.GetCurveType() == CurveType::Rotation)
            {
                // spherical interpolation
                q0.load((scalar*)src0SamplePtr);
                q1.load((scalar*)src1SamplePtr);
                qDst = quaternion::slerp(q0, q1, lerpValue);
                qDst.store((scalar*)dstSamplePtr);
            }
            else
            {
                // linear interpolation
                f0.load((scalar*)src0SamplePtr);
                f1.load((scalar*)src1SamplePtr);
                fDst = float4::lerp(f0, f1, lerpValue);
                fDst.store((scalar*)dstSamplePtr);
            }
        }
        else if (src0Count > 0)
        {
            // only src0 key is valid, copy to destination
            f0.load((scalar*)src0SamplePtr);
            f0.store((scalar*)dstSamplePtr);
        }
        else if (src1Count > 0)
        {
            // only src1 key is valid, copy to destination
            f1.load((scalar*)src1SamplePtr);
            f1.store((scalar*)dstSamplePtr);
        }
        else
        {
            // neither the src0 nor the src1 key contribute to the result,
            // the resulting destination sample count will be 0 and the
            // sample key is undefined
        }

        // update sample pointers
        src0SamplePtr++;
        src1SamplePtr++;
        dstSamplePtr++;
    }
    if (dst != src0) dst->UnmapSampleCounts();
    src1->UnmapSampleCounts();
    src0->UnmapSampleCounts();
    if (dst != src0) dst->UnmapSamples();
    src1->UnmapSamples();
    src0->UnmapSamples();
}

} // namespace CoreAnimation
