#pragma once
//------------------------------------------------------------------------------
/**
    @class ToolkitUtil::ResourceDictionaryBuilder
    
    Build resource dictionary files.
    
    (C) 2008 Radon Labs GmbH
*/
#include "toolkitutil/platform.h"
#include "io/uri.h"
#include "util/stringatom.h"

//------------------------------------------------------------------------------
namespace ToolkitUtil
{
class ResourceDictionaryBuilder
{
public:
    /// constructor
    ResourceDictionaryBuilder();
    
    /// set target platform
    void SetPlatform(Platform::Code platform);
    /// set texture directory
    void SetTextureDirectory(const Util::String& texDir);

    /// build the dictionary
    bool BuildDictionary();

private:
    /// add textures to the dictionary
    bool AddTexturesToDictionary();
    /// save dictionary
    bool SaveDictionary();

    static const SizeT MaxResIdLength = 124;
    Platform::Code platform;
    Util::String texDir;
    Util::Dictionary<Util::StringAtom, SizeT> dict;
};

//------------------------------------------------------------------------------
/**
*/
inline void
ResourceDictionaryBuilder::SetPlatform(Platform::Code p)
{
    this->platform = p;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ResourceDictionaryBuilder::SetTextureDirectory(const Util::String& dir)
{
    this->texDir = dir;
}

} // namespace ToolkitUtil
//------------------------------------------------------------------------------
