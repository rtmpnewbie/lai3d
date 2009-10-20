//------------------------------------------------------------------------------
//  pixelformat.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/pixelformat.h"

namespace CoreGraphics
{

//------------------------------------------------------------------------------
/**
    Convert a pixel format string into a pixel format code.
*/
PixelFormat::Code
PixelFormat::FromString(const Util::String& str)
{
    if (str == "X8R8G8B8") return X8R8G8B8;
    else if (str == "A8R8G8B8") return A8R8G8B8;
    else if (str == "R5G6B5") return R5G6B5;
    else if (str == "A1R5G5B5") return A1R5G5B5;
    else if (str == "A4R4G4B4") return A4R4G4B4;
    else if (str == "DXT1") return DXT1;
    else if (str == "DXT3") return DXT3;
    else if (str == "DXT5") return DXT5;
    else if (str == "R16F") return R16F;
    else if (str == "G16R16F") return G16R16F;
    else if (str == "A16B16G16R16F") return A16B16G16R16F;
    else if (str == "R32F") return R32F;         
    else if (str == "G32R32F") return G32R32F;      
    else if (str == "A32B32G32R32F") return A32B32G32R32F;
    else if (str == "A8") return A8;
    else if (str == "X2R10G10B10") return X2R10G10B10;
    else if (str == "A2R10G10B10") return A2R10G10B10;
    else if (str == "G16R16") return G16R16;
    else if (str == "D24S8") return D24S8;
    else if (str == "D24X8") return D24X8;

    // Xbox360 pixel-formats
    else if (str == "DXN") return DXN;
    else if (str == "LINDXN") return LINDXN;
    else if (str == "LINDXT1") return LINDXT1;
    else if (str == "LINDXT3") return LINDXT3;
    else if (str == "LINDXT5") return LINDXT5;
    else if (str == "LINA8R8G8B8") return LINA8R8G8B8;
    else if (str == "LINX8R8G8B8") return LINX8R8G8B8;
    else if (str == "EDG16R16") return EDG16R16;
    else if (str == "CTX1") return CTX1;
    else if (str == "LINCTX1") return LINCTX1;
    else if (str == "D24FS8") return D24FS8;

    // Wii pixel-formats
    else if (str == "RGBA8") return RGBA8;
    else if (str == "RGB5A3") return RGB5A3;
    else if (str == "CMPR") return CMPR;
    else
    {
        n_error("Invalid pixel format string '%s'!", str.AsCharPtr());
        return InvalidPixelFormat;
    }
}

//------------------------------------------------------------------------------
/**
    Convert pixel format code into a string.
*/
Util::String
PixelFormat::ToString(PixelFormat::Code code)
{
    switch (code)
    {
        case X8R8G8B8:      return "X8R8G8B8";
        case A8R8G8B8:      return "A8R8G8B8";
        case R5G6B5:        return "R5G6B5";
        case A1R5G5B5:      return "A1R5G5B5";
        case A4R4G4B4:      return "A4R4G4B4";
        case DXT1:          return "DXT1";
        case DXT3:          return "DXT3";
        case DXT5:          return "DXT5";
        case R16F:          return "R16F";
        case G16R16F:       return "G16R16F";
        case A16B16G16R16F: return "A16B16G16R16F";
        case R32F:          return "R32F";
        case G32R32F:       return "G32R32F";
        case A32B32G32R32F: return "A32B32G32R32F";
        case A8:            return "A8";
        case X2R10G10B10:   return "X2R10G10B10";
        case A2R10G10B10:   return "A2R10G10B10";
        case G16R16:        return "G16R16";
        case D24X8:         return "D24X8";
        case D24S8:         return "D24S8";

        // Xbox360 pixel-formats
        case DXN:           return "DXN";
        case LINDXN:        return "LINDXN";
        case LINDXT1:       return "LINDXT1";
        case LINDXT3:       return "LINDXT3";
        case LINDXT5:       return "LINDXT5";
        case LINA8R8G8B8:   return "LINA8R8G8B8";
        case LINX8R8G8B8:   return "LINX8R8G8B8";
        case EDG16R16:      return "EDG16R16";
        case CTX1:          return "CTX1";
        case LINCTX1:       return "LINCTX1";
        case D24FS8:        return "D24FS8";

        // Wii pixel-formats
        case RGBA8:         return "RGBA8";
        case RGB5A3:        return "RGB5A3";
        case CMPR:          return "CMPR";
    }
    n_error("Invalid pixel format code");
    return "";
}

} // namespace CoreGraphics
