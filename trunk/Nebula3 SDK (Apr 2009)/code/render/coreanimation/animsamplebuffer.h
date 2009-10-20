#pragma once
#ifndef COREANIMATION_ANIMSAMPLEBUFFER_H
#define COREANIMATION_ANIMSAMPLEBUFFER_H
//------------------------------------------------------------------------------
/**
    @class CoreAnimation::AnimSampleBuffer
  
    Stores the result of an animation sampling operation, stores 
    samples key values and sample-counts which keep track of the number 
    of samples which contributed to a mixing result (this is necessary
    for correct mixing of partial animations).
     
    (C) 2008 Radon Labs GmbH
*/    
#include "core/refcounted.h"
#include "coreanimation/animresource.h"

//------------------------------------------------------------------------------
namespace CoreAnimation
{
class AnimSampleBuffer : public Core::RefCounted
{
    __DeclareClass(AnimSampleBuffer);
public:
    /// constructor
    AnimSampleBuffer();
    /// destructor
    virtual ~AnimSampleBuffer();
    
    /// setup the object from an animation resource
    void Setup(const Ptr<AnimResource>& animResource);
    /// discard the object
    void Discard();
    /// return true if the object has been setup
    bool IsValid() const;
    
    /// get the number of samples in the buffer
    SizeT GetNumSamples() const;
    /// gain read/write access to sample buffer
    Math::float4* MapSamples();
    /// give up access to sample buffer
    void UnmapSamples();
    /// gain read/write access to sample counts
    uchar* MapSampleCounts();
    /// give up access to sample counts
    void UnmapSampleCounts();

    /// get LastKeyIndex0	
    IndexT GetLastKeyIndex0() const;
    /// set LastKeyIndex0
    void SetLastKeyIndex0(IndexT val);

    /// get LastKeyIndex1	
    IndexT GetLastKeyIndex1() const;
    /// set LastKeyIndex1
    void SetLastKeyIndex1(IndexT val);

private:
    Ptr<AnimResource> animResource;
    SizeT numSamples;
    Math::float4* samples;
    uchar* sampleCounts;
    bool samplesMapped;
    bool sampleCountsMapped;
    IndexT lastKeyIndex0;
    IndexT lastKeyIndex1;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
AnimSampleBuffer::IsValid() const
{
    return this->animResource.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
AnimSampleBuffer::GetNumSamples() const
{
    return this->numSamples;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT 
AnimSampleBuffer::GetLastKeyIndex0() const
{
    return this->lastKeyIndex0;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
AnimSampleBuffer::SetLastKeyIndex0(IndexT val)
{
    this->lastKeyIndex0 = val;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT 
AnimSampleBuffer::GetLastKeyIndex1() const
{
    return this->lastKeyIndex1;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
AnimSampleBuffer::SetLastKeyIndex1(IndexT val)
{
    this->lastKeyIndex1 = val;
}
} // namespace CoreAnimation
//------------------------------------------------------------------------------
#endif
