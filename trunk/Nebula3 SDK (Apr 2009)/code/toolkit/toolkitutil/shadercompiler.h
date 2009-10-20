#pragma once
//------------------------------------------------------------------------------
/**
    @class ToolkitUtil::ShaderCompiler
    
    Wraps the shader compilation process for all supported target platforms.
        
    (C) 2008 Radon Labs GmbH
*/
#include "toolkitutil/platform.h"
#include "io/uri.h"
#include "util/string.h"

//------------------------------------------------------------------------------
namespace ToolkitUtil
{
class ShaderCompiler
{
public:
    /// constructor
    ShaderCompiler();
    /// destructor
    ~ShaderCompiler();

    /// set target platform
    void SetPlatform(Platform::Code platform);
    /// set source directory
    void SetShaderSrcDir(const Util::String& srcDir);
    /// set destination directory
    void SetShaderDstDir(const Util::String& dstDir);
    /// set frame shader source dir
    void SetFrameShaderSrcDir(const Util::String& srcDir);
    /// set frame shader destination dir
    void SetFrameShaderDstDir(const Util::String& dstDir);
    /// set path to compile tool
    void SetToolPath(const Util::String& toolPath);
    /// set shader file pattern (i.e. *.fx)
    void SetShaderFilePattern(const Util::String& pattern);
    /// set force conversion flag (otherwise check timestamps)
    void SetForceFlag(bool b);
    /// set debugging flag
    void SetDebugFlag(bool b);
    /// set additional command line params
    void SetAdditionalParams(const Util::String& params);
    /// set quiet flag
    void SetQuietFlag(bool b);
    /// enable Nebula2 legacy mode
    void SetNebula2Mode(bool b);

    /// compile a single shader file
    bool CompileShader(const Util::String& srcFile, const Util::String& dstFile, const Util::String& extraParams);
    /// compile all shaders 
    bool CompileAllShaders();
    /// compile frame shader files
    bool CompileFrameShaders();
    /// compile legacy Nebula2 shader
    bool CompileShaderNebula2(const Util::String& srcFile, const Util::String& dstFile);

private:
    Platform::Code platform;
    Util::String srcShaderDir;
    Util::String dstShaderDir;
    Util::String srcFrameShaderDir;
    Util::String dstFrameShaderDir;
    Util::String toolPath;
    bool force;
    bool quiet;
    bool debug;
    bool nebula2Mode;
    Util::String additionalParams;
};

//------------------------------------------------------------------------------
/**
*/
inline void
ShaderCompiler::SetPlatform(Platform::Code p)
{
    this->platform = p;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ShaderCompiler::SetNebula2Mode(bool b)
{
    this->nebula2Mode = b;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ShaderCompiler::SetShaderSrcDir(const Util::String& d)
{
    this->srcShaderDir = d;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ShaderCompiler::SetShaderDstDir(const Util::String& d)
{
    this->dstShaderDir = d;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ShaderCompiler::SetFrameShaderSrcDir(const Util::String& d)
{
    this->srcFrameShaderDir = d;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ShaderCompiler::SetFrameShaderDstDir(const Util::String& d)
{
    this->dstFrameShaderDir = d;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ShaderCompiler::SetToolPath(const Util::String& p)
{
    this->toolPath = p;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ShaderCompiler::SetForceFlag(bool b)
{
    this->force = b;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ShaderCompiler::SetDebugFlag(bool b)
{
    this->debug = b;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ShaderCompiler::SetAdditionalParams(const Util::String& p)
{
    this->additionalParams = p;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ShaderCompiler::SetQuietFlag(bool b)
{
    this->quiet = b;
}

} // namespace ToolkitUtil
//------------------------------------------------------------------------------
 