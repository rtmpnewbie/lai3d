#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::PixelFormat
    
    Pixel format enumeration.

    FIXME: use DX10 notations (more flexible but less readable...)

    (C) 2006 Radon Labs GmbH
*/
#include "core/types.h"
#include "util/string.h"

//------------------------------------------------------------------------------
namespace CoreGraphics
{
class PixelFormat
{
public:
    /// enums
    enum Code
    {
        X8R8G8B8 = 0,
        A8R8G8B8,
        R5G6B5,
        A1R5G5B5,
        A4R4G4B4,
        DXT1,
        DXT3,
        DXT5,
        R16F,                       // 16 bit float, red only
        G16R16F,                    // 32 bit float, 16 bit red, 16 bit green
        A16B16G16R16F,              // 64 bit float, 16 bit rgba each
        R32F,                       // 32 bit float, red only
        G32R32F,                    // 64 bit float, 32 bit red, 32 bit green
        A32B32G32R32F,              // 128 bit float, 32 bit rgba each
        A8,
        X2R10G10B10,
        A2R10G10B10,
        G16R16,
        D24X8,
        D24S8,
        
        // Xbox360 specific pixel formats
        DXN,
        LINDXN,
        LINDXT1,
        LINDXT3,
        LINDXT5,
        LINA8R8G8B8,
        LINX8R8G8B8,
        EDG16R16,
        CTX1,
        LINCTX1,
        D24FS8,

        // Wii specific pixel formats
        RGBA8,
        RGB5A3,
        CMPR,

        NumPixelFormats,
        InvalidPixelFormat,
    };

    /// convert from string
    static Code FromString(const Util::String& str);
    /// convert to string
    static Util::String ToString(Code code);
};

} // namespace CoreGraphics
//------------------------------------------------------------------------------

    