#pragma once
#ifndef COREANIMATION_SAMPLETYPE_H
#define COREANIMATION_SAMPLETYPE_H
//------------------------------------------------------------------------------
/**
    @class CoreAnimation::SampleType
  
    Describes how an animation curve should be sampled.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/types.h"
#include "util/string.h"

//------------------------------------------------------------------------------
namespace CoreAnimation
{
class SampleType
{
public:
    /// animation sample types
    enum Code
    {
        Step,           //> do not interpolate, just step
        Linear,         //> simple linear interpolation
        Hermite,        //> herminte spline interpolation
        CatmullRom,     //> catmull-rom interpolation

        NumSampleTypes,
        InvalidSampleType,
    };

    /// convert from string
    static Code FromString(const Util::String& str);
    /// convert to string
    static Util::String ToString(Code c);
};

} // namespace CoreAnimation
//------------------------------------------------------------------------------
#endif

