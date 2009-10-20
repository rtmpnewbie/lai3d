#pragma once
#ifndef COREANIMATION_CURVETYPE_H
#define COREANIMATION_CURVETYPE_H
//------------------------------------------------------------------------------
/**
    @class CoreAnimation::AnimCurveType
  
    Describes the general data type of the keys stored in an animation curve.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/types.h"
#include "util/string.h"

//------------------------------------------------------------------------------
namespace CoreAnimation
{
class CurveType
{
public:
    /// animation curve types
    enum Code
    {
        Translation,    //> keys in curve describe a translation
        Scale,          //> keys in curve describe a scale
        Rotation,       //> keys in curve describe a rotation quaternion
        Color,          //> keys in curve describe a color
        Float4,         //> generic 4D key

        NumCurveTypes,
        InvalidCurveType,
    };

    /// convert from string
    static Code FromString(const Util::String& str);
    /// convert to string
    static Util::String ToString(Code c);
};

} // namespace CoreAnimation
//------------------------------------------------------------------------------
#endif

