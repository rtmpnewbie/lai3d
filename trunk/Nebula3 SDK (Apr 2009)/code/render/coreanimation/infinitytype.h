#pragma once
#ifndef COREANIMATION_INFINITYTYPE_H
#define COREANIMATION_INFINITYTYPE_H
//------------------------------------------------------------------------------
/**
    @class CoreAnimation::InfinityType
  
    Describes how time position outsides of an animation curve's scope 
    are handled.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/types.h"
#include "util/string.h"

//------------------------------------------------------------------------------
namespace CoreAnimation
{
class InfinityType
{
public:
    /// anim infinity types
    enum Code
    {
        Constant,       //> return first or last key
        Cycle,          //> loop the animation
    
        NumInfinityTypes,
        InvalidInfinityType,
    };
    
    /// convert from string
    static Code FromString(const Util::String& str);
    /// convert to string
    static Util::String ToString(Code c);
};

} // namespace CoreAnimation
//------------------------------------------------------------------------------
#endif
