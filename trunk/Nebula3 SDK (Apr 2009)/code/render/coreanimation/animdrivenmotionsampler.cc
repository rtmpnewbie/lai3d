//------------------------------------------------------------------------------
//  AnimDeltaSampler.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coreanimation/animdrivenmotionsampler.h"
#include "coreanimation/animsampler.h"
#include "math/quaternion.h"

namespace CoreAnimation
{
using namespace Math;
using namespace Timing;
using namespace Util;

//------------------------------------------------------------------------------
/**
Clamp key indices into the valid range, take pre-infinity and 
post-infinity type into account.
*/
IndexT
AnimDrivenMotionSampler::ClampKeyIndexForDelta(IndexT keyIndex, const AnimClip& clip)
{
    SizeT clipNumKeys = clip.GetNumKeys() - 1;
    if (keyIndex < 0)
    {
        if (clip.GetPreInfinityType() == InfinityType::Cycle)
        {
            keyIndex %= clipNumKeys;
            if (keyIndex < 0)
            {
                keyIndex += clipNumKeys;
            }
        }
        else
        {
            keyIndex = 0;
        }
    }
    else if (keyIndex >= clipNumKeys)
    {
        if (clip.GetPostInfinityType() == InfinityType::Cycle)
        {
            keyIndex %= clipNumKeys;
        }
        else
        {
            keyIndex = clipNumKeys - 1;
        }
    }
    return keyIndex;
}

//------------------------------------------------------------------------------
/**
NOTE: The sampler will *NOT* the start time of the clip into account!
TODO: seperate delta computation from default sampling!!!
*/
void
AnimDrivenMotionSampler::Sample(const Ptr<AnimResource>& animResource, 
                    IndexT clipIndex, 
                    SampleType::Code sampleType, 
                    Tick time, 
                    const Ptr<AnimSampleBuffer>& result, 
                    IndexT jointCurveIndex, 
                    Math::point& lastResult, 
                    Math::vector& deltaResult, 
                    IndexT& lastKeyIndex)
{
    n_assert(animResource.isvalid());
    n_assert(result.isvalid());
    n_assert((sampleType == SampleType::Step) || (sampleType == SampleType::Linear));

    const AnimClip& clip = animResource->GetClipByIndex(clipIndex);
    SizeT keyStride  = clip.GetKeyStride();
    Tick keyDuration = clip.GetKeyDuration();
    n_assert(clip.GetNumCurves() == result->GetNumSamples());

    // compute the relative first key index, second key index and lerp value
    IndexT keyIndex0 = AnimDrivenMotionSampler::ClampKeyIndexForDelta((time / keyDuration), clip);    
    IndexT keyIndex1 = AnimSampler::ClampKeyIndex(keyIndex0 + 1, clip);
    Tick inbetweenTicks = AnimSampler::InbetweenTicks(time, clip);
    float lerpValue = float(inbetweenTicks) / float(keyDuration);  

    // sample curves...
    float4 f4Key0, f4Key1, f4SampleKey;
    quaternion qKey0, qKey1, qSampleKey;

    float4* srcKeyStart = (float4*) animResource->GetKeyBuffer()->Map();
    float4* dstKeyBuffer = result->MapSamples();
    uchar* dstSampleCounts = result->MapSampleCounts();
    IndexT curveIndex;
    SizeT numCurves = clip.GetNumCurves();
    for (curveIndex = 0; curveIndex < numCurves; curveIndex++)
    {
        const AnimCurve& curve = clip.CurveByIndex(curveIndex);
        if (!curve.IsActive())
        {
            // curve is not active, set sample count to 0
            dstSampleCounts[curveIndex] = 0;            
        }
        else
        {
            // curve is active, set sample count to 1
            dstSampleCounts[curveIndex] = 1;
            scalar* resultPtr = (scalar*) &(dstKeyBuffer[curveIndex]);
            if (curve.IsStatic())
            {
                // the curve is constant, just copy the constant value into the result buffer
                curve.GetStaticKey().store(resultPtr);
            }
            else
            {
                // non-static curve, need to actually sample the result
                IndexT startKeyIndex = curve.GetFirstKeyIndex();

                // compute position of first key in source key buffer
                scalar* keyPtr0 = (scalar*) (srcKeyStart + startKeyIndex + (keyIndex0 * keyStride));
                if (SampleType::Step == sampleType)
                {
                    // if no interpolation needed, just copy the key to the sample buffer
                    f4SampleKey.load(keyPtr0);
                    f4SampleKey.store(resultPtr);
                }
                else
                {
                    // need to interpolate between 2 keys
                    scalar* keyPtr1 = (scalar*) (srcKeyStart + startKeyIndex + (keyIndex1 * keyStride));
                    if (curve.GetCurveType() == CurveType::Rotation)
                    {
                        // perform spherical interpolation
                        qKey0.load(keyPtr0);
                        qKey1.load(keyPtr1);
                        qSampleKey = quaternion::slerp(qKey0, qKey1, lerpValue);
                        qSampleKey.store(resultPtr);
                    }
                    else
                    {
                        // perform linear interpolation
                        f4Key0.load(keyPtr0);
                        f4Key1.load(keyPtr1);
                        f4SampleKey = float4::lerp(f4Key0, f4Key1, lerpValue);
                        f4SampleKey.store(resultPtr);
                    }
                }
            }

            // compute delta result if needed
            if (curveIndex == jointCurveIndex)
            {
                // skip rotation or scaling
                if (curve.GetCurveType() == CurveType::Translation)
                {
                    if (curve.IsStatic()) 
                    {
                        deltaResult = curve.GetStaticKey();
                    }
                    else
                    {
                        bool inCycleRestartSegment = (lastKeyIndex  > keyIndex0);
                        if (inCycleRestartSegment)
                        {       
                            // on a restart we need the way from last pos to end 
                            // and from start to current pos, cause there can be no valid blending
                            // therefore the last anim key have to be the same as the first one                            
                            IndexT startKeyIndex = curve.GetFirstKeyIndex();                                                   
                            float4* startKeyPtr = (float4*) (srcKeyStart + startKeyIndex);                                                                            
                            float4* endKeyPtr = (float4*) (srcKeyStart + startKeyIndex + ((clip.GetNumKeys()-1) * keyStride));                                                    
                            float4 lastToEnd = *endKeyPtr - lastResult;
                            float4 startToCur = *(float4*)resultPtr - *startKeyPtr;
                            deltaResult = lastToEnd + startToCur;
                        }
                        else
                        {
                            // just compute difference
                            deltaResult = *(float4*)resultPtr - lastResult;
                        }
                    }

                    // save current key as last key for next frame
                    lastResult = *(float4*)resultPtr;
                    lastKeyIndex = keyIndex0;                    

                    // clear translation, to avoid gpu skinning on this joint
                    (*(float4*)resultPtr).x() = 0;
                    (*(float4*)resultPtr).y() = 0;
                    (*(float4*)resultPtr).z() = 0;
                }
            }
        }
    }
    // done
    result->UnmapSampleCounts();
    result->UnmapSamples();
    animResource->GetKeyBuffer()->Unmap();
}

//------------------------------------------------------------------------------
/**
    Mixes 2 float4 delta positions.
*/
void 
AnimDrivenMotionSampler::MixPositionDelta(const Ptr<AnimResource>& animResource, Math::vector* src0, Math::vector* src1, float lerpValue, Math::vector* dst, IndexT jointCurveIndex)
{  
    n_assert(animResource.isvalid());

    // get a template clip from the anim resource, this defines the anim curve
    // types which we need for correct lerping
    const AnimClip& templClip = animResource->GetClipByIndex(0);
    vector* dstSamplePtr  = 0;
    if (src0 != dst)
    {
        dstSamplePtr  = dst;
    }
    else
    {
        dstSamplePtr = src0;
    }
    
    IndexT positionJointIndex = jointCurveIndex * 3;
    // perform the mixing...
    quaternion q0, q1, qDst;
    vector f0, f1, fDst;
    const AnimCurve& curve = templClip.CurveByIndex(positionJointIndex);        
    // both source samples are valid, need to mix into result
    n_assert(curve.GetCurveType() == CurveType::Translation);
    // linear interpolation
    f0.load((scalar*)src0);
    f1.load((scalar*)src1);
    fDst = vector::lerp(f0, f1, lerpValue);
    fDst.store((scalar*)dstSamplePtr);
}
} // namespace CoreAnimation