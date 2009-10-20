#pragma once
//------------------------------------------------------------------------------
/**
    @class ToolkitUtil::TextureConverter
    
    Wraps texture conversion process for all supported target platforms.
    
    (C) 2008 Radon Labs GmbH
*/
#include "toolkitutil/platform.h"
#include "io/uri.h"
#include "util/string.h"
#include "toolkitutil/textureattrtable.h"

//------------------------------------------------------------------------------
namespace ToolkitUtil
{
class TextureConverter
{
public:
    /// constructor
    TextureConverter();
    /// destructor
    ~TextureConverter();

    /// set target platform
    void SetPlatform(Platform::Code platform);
    /// set texture attribute table path
    void SetTexAttrTablePath(const Util::String& path);
    /// set source directory
    void SetSrcDir(const Util::String& srcDir);
    /// set destination directory
    void SetDstDir(const Util::String& dstDir);
    /// set path to external converter tool (platform specific)
    void SetToolPath(const Util::String& toolPath);
    /// set force flag
    void SetForceFlag(bool b);
    /// set quiet flag
    void SetQuietFlag(bool b);

    /// setup the texture converter, read the texture attributes table
    bool Setup();
    /// discard the texture converter
    void Discard();
    /// return true if the object has been setup
    bool IsValid() const;
    /// convert all textures
    bool ConvertAll();
    /// convert all textures in a category
    bool ConvertCategory(const Util::String& categoryName);
    /// convert a single texture
    bool ConvertCategoryTexture(const Util::String& categoryName, const Util::String& texName);
    /// convert a texture from a given path
    bool ConvertTexture(const Util::String& srcPath, const Util::String& dstCategoryName, const Util::String& dstTexName);

private:
    /// convert a texture from a given path for Win32 platform
    bool ConvertTextureWin32(const Util::String& srcPath, const Util::String& dstCategoryName, const Util::String& dstTexName);
    /// convert a texture from a given path for Xbox360 platform
    bool ConvertTextureXbox360(const Util::String& srcPath, const Util::String& dstCategoryName, const Util::String& dstTexName);
    /// convert a texture from a given path for Wii platform
    bool ConvertTextureWii(const Util::String& srcPath, const Util::String& dstCategoryName, const Util::String& dstTexName);
    /// test if a conversion is needed (checks file time stamps and force flag)
    bool NeedsConversion(const Util::String& srcPath, const Util::String& dstPath);
    /// prepare texture conversion (platform independent stuff)
    bool PrepareConversion(const Util::String& srcPath, const Util::String& dstPath);

    Platform::Code platform;
    Util::String texAttrTablePath;
    Util::String srcDir;
    Util::String dstDir;
    Util::String toolPath;
    bool force;
    bool quiet;
    bool valid;
    TextureAttrTable textureAttrTable;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
TextureConverter::IsValid() const
{
    return this->valid;
}

//------------------------------------------------------------------------------
/**
*/
inline void
TextureConverter::SetPlatform(Platform::Code p)
{
    this->platform = p;
}

//------------------------------------------------------------------------------
/**
*/
inline void
TextureConverter::SetTexAttrTablePath(const Util::String& path)
{
    this->texAttrTablePath = path;
}

//------------------------------------------------------------------------------
/**
*/
inline void
TextureConverter::SetSrcDir(const Util::String& d)
{
    this->srcDir = d;
}

//------------------------------------------------------------------------------
/**
*/
inline void
TextureConverter::SetDstDir(const Util::String& d)
{
    this->dstDir = d;
}

//------------------------------------------------------------------------------
/**
*/
inline void
TextureConverter::SetToolPath(const Util::String& p)
{
    this->toolPath = p;
}

//------------------------------------------------------------------------------
/**
*/
inline void
TextureConverter::SetForceFlag(bool b)
{
    this->force = b;
}

//------------------------------------------------------------------------------
/**
*/
inline void
TextureConverter::SetQuietFlag(bool b)
{
    this->quiet = b;
}

} // namespace ToolkitUtil
//------------------------------------------------------------------------------

