#pragma once
//------------------------------------------------------------------------------
/**
    @class Timing::TimingType

    Time types used for synchronization.

    (C) 2009 Radon Labs GmbH
*/
#include "core/types.h"
#include "util/string.h"

//------------------------------------------------------------------------------
namespace Timing
{
class TimingType
{
public:
    /// enums
    enum Code
    {
        MasterTime = 0,
        ScaledTime = 1,

        NumTimingTypes,
        InvalidTimingType,
    };

    /// convert from string
    static Code FromString(const Util::String& str);
    /// convert to string
    static Util::String ToString(Code code);
};

} // namespace Timing
//------------------------------------------------------------------------------

