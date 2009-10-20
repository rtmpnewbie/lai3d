#pragma once
//------------------------------------------------------------------------------
/**
    @class ToolkitUtil::Platform
    
    Platform identifiers (Xbox360, Wii, etc...).
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/types.h"
#include "util/string.h"

//------------------------------------------------------------------------------
namespace ToolkitUtil
{
class Platform
{
public:
    /// platform enum
    enum Code
    {
        Win32,
        Xbox360,
        Wii,
        PS3,
        NDS,
        PSP,

        InvalidPlatform,
    };
    /// convert from string
    static Code FromString(const Util::String& str);
    /// convert to string
    static Util::String ToString(Code c);
};

} // namespace ToolkitUtil
//------------------------------------------------------------------------------
    