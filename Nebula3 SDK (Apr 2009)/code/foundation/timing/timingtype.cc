//------------------------------------------------------------------------------
//  type.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "timing/timingtype.h"

namespace Timing
{
//------------------------------------------------------------------------------
/**
    Convert a timing type string into a timing type string.
*/
TimingType::Code
TimingType::FromString(const Util::String& str)
{
    if (str == "MasterTime") return MasterTime;
    else if (str == "ScaledTime") return ScaledTime;
    else
    {
        n_error("Invalid timing type string '%s'!", str.AsCharPtr());
        return InvalidTimingType;
    }
}

//------------------------------------------------------------------------------
/**
    Convert timing type code into a string.
*/
Util::String
TimingType::ToString(TimingType::Code code)
{
    switch (code)
    {
        case MasterTime:      return "MasterTime";
        case ScaledTime:       return "ScaledTime";      
    }
    n_error("Invalid timing type code");
    return "";
}

} // namespace Timing
