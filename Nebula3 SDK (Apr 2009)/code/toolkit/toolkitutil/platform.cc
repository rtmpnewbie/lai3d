//------------------------------------------------------------------------------
//  platform.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "platform.h"

namespace ToolkitUtil
{

//------------------------------------------------------------------------------
/**
*/
Platform::Code
Platform::FromString(const Util::String& str)
{
    if (str == "win32")        return Win32;
    else if (str == "xbox360") return Xbox360;
    else if (str == "wii")     return Wii;
    else if (str == "ps3")     return PS3;
    else if (str == "nds")     return NDS;
    else if (str == "psp")     return PSP;
    else
    {
        n_error("Platform::FromString(): invalid platform string '%s'!", str.AsCharPtr());
        return InvalidPlatform;
    }
}

//------------------------------------------------------------------------------
/**
*/
Util::String
Platform::ToString(Platform::Code code)
{
    switch (code)
    {
        case Win32:     return "win32";
        case Xbox360:   return "xbox360";
        case Wii:       return "wii";
        case PS3:       return "ps3";
        case NDS:       return "nds";
        case PSP:       return "psp";
        default:
            n_error("Platform::ToString(): invalid platform code!");
            return "";
    }
}

} // namespace ToolkitUtil
