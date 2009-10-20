//------------------------------------------------------------------------------
//  shadercompiler.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "toolkitutil/shadercompiler.h"
#include "toolkitutil/applauncher.h"
#include "io/ioserver.h"

namespace ToolkitUtil
{
using namespace Util;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
ShaderCompiler::ShaderCompiler() :
    platform(Platform::Win32),
    force(false),
    debug(false),
    quiet(false),
    nebula2Mode(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ShaderCompiler::~ShaderCompiler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
ShaderCompiler::CompileShader(const String& srcFile, const String& dstFile, const String& extraParams)
{
    n_assert(srcFile.IsValid());
    IoServer* ioServer = IoServer::Instance();

    // build an absolute filename to the src file
    String srcPath;
    srcPath.Format("%s/%s", this->srcShaderDir.AsCharPtr(), srcFile.AsCharPtr());
    String resolvedSrcPath = AssignRegistry::Instance()->ResolveAssignsInString(srcPath);

    // make sure the source file exists
    if (!ioServer->FileExists(srcPath))
    {
        n_printf("WARNING: shader file '%s' does not exist!\n", resolvedSrcPath.AsCharPtr());
        return false;
    }

    // make sure target directory exists
    ioServer->CreateDirectory(this->dstShaderDir);

    // construct target file name and debug pdb file name
    String dstPath;
    dstPath.Format("%s/%s", this->dstShaderDir.AsCharPtr(), dstFile.AsCharPtr());
    dstPath = AssignRegistry::Instance()->ResolveAssignsInString(dstPath);

    // check if file needs recompilation
    bool compile = true;
    if (!this->force)
    {
        // check file stamps
        if (ioServer->FileExists(dstPath))
        {
            FileTime srcTime = ioServer->GetFileWriteTime(srcPath);
            FileTime dstTime = ioServer->GetFileWriteTime(dstPath);
            if (dstTime > srcTime)
            {
                compile = false;
            }
        }
    }
    if (!compile)
    {
        return true;
    }

    // platform-specific handling...
    if (Platform::Wii == this->platform)
    {
        // on the Wii, the shader files are simply copied
        bool result = ioServer->CopyFile(srcPath, dstPath);
        n_printf("copy shader %s\n", srcPath.AsCharPtr());
        return result;
    }
    else
    {
        if ((Platform::Xbox360 == this->platform) || (Platform::Win32 == this->platform))
        {
            if (!this->toolPath.IsValid())
            {
                n_printf("WARNING: no compile tool set!\n");
                return false;
            }

            // Xbox360/Win32: build shader args
            String args = "/nologo ";
            if (Platform::Xbox360 == this->platform)
            {
                // Xbox360 platform
                args.Append("/XOautoz /T fxl_3_0 /D__XBOX360__=1 ");
                if (this->debug)
                {
                    // build a filename for the debug info pdb file
                    String pdbPath = this->dstShaderDir;
                    String resolvedPdbPath = AssignRegistry::Instance()->ResolveAssignsInString(pdbPath);

                    resolvedPdbPath.Append("/");
                    resolvedPdbPath.Append(srcFile);
                    resolvedPdbPath.StripFileExtension();
                    resolvedPdbPath.Append(".updb");
                    
                    args.Append("/XZi ");
                    args.Append("/XFd ");
                    args.Append(resolvedPdbPath);
                    args.Append(" ");
                }
            }
            else
            {
                // Win32 platform
                args.Append("/T fx_2_0 /D__WIN32__=1 ");
                if (this->debug)
                {
                    args.Append("/Od /Zi ");
                }
            }
            if (this->additionalParams.IsValid())
            {
                args.Append(" ");
                args.Append(this->additionalParams);
                args.Append(" ");
            }
            if (extraParams.IsValid())
            {
                args.Append(" ");
                args.Append(extraParams);
                args.Append(" ");
            }
            args.Append("/Fo \"");
            args.Append(dstPath);
            args.Append("\" \"");
            args.Append(resolvedSrcPath);
            args.Append("\" ");

            // launch external compiler tool
            AppLauncher appLauncher;
            appLauncher.SetExecutable(this->toolPath);
            appLauncher.SetWorkingDirectory(this->srcShaderDir);
            appLauncher.SetArguments(args);
            appLauncher.SetNoConsoleWindow(this->quiet);
            if (!appLauncher.LaunchWait())
            {
                n_printf("WARNING: failed to launch compiler tool '%s'!\n", this->toolPath.AsCharPtr());
                return false;
            }
        }
        else
        {
            n_printf("WARNING: unknown platform '%s'!\n", Platform::ToString(this->platform).AsCharPtr());
            return false;
        }
    }
    return true;
}

//------------------------------------------------------------------------------
/**
    Compile a shader in Nebula2 legacy mode.
*/
bool
ShaderCompiler::CompileShaderNebula2(const Util::String& srcFile, const Util::String& dstFile)
{
    n_assert(Platform::Win32 == this->platform);
    bool success = true;

    Array<KeyValuePair<String, String>> shaderModels;
    shaderModels.Append(KeyValuePair<String,String>("vs_2_0", "ps_2_0"));
    shaderModels.Append(KeyValuePair<String,String>("vs_2_a", "ps_2_a"));
    shaderModels.Append(KeyValuePair<String,String>("vs_2_0", "ps_2_b"));
    shaderModels.Append(KeyValuePair<String,String>("vs_3_0", "ps_3_0"));

    Array<String> variations;
    variations.Append("");
    variations.Append("dir");
    variations.Append("point");
    variations.Append("dirshadow");
    variations.Append("pointshadow");

    String extraParams;
    IndexT smIndex;
    for (smIndex = 0; smIndex < shaderModels.Size(); smIndex++)
    {
        const String& vsModel = shaderModels[smIndex].Key();
        const String& psModel = shaderModels[smIndex].Value();
        IndexT varIndex;
        for (varIndex = 0; varIndex < variations.Size(); varIndex++)
        {
            const String& var = variations[varIndex];

            // build extra params
            extraParams.Append(" /D VS_PROFILE=");
            extraParams.Append(vsModel);
            extraParams.Append(" /D PS_PROFILE=");
            extraParams.Append(psModel);
            if (psModel == "ps_2_0")
            {
                extraParams.Append(" /D LOWDETAIL=1");
            }
            if (var == "point")
            {
                extraParams.Append(" /D POINTLIGHT");
            }
            else if (var == "dir")
            {
                extraParams.Append(" /D DIRLIGHT");
            }
            else if (var == "pointshadow")
            {
                extraParams.Append(" /D POINTLIGHT /D SHADOW");
            }
            else if (var == "dirshadow")
            {
                extraParams.Append(" /D DIRLIGHT /D SHADOW");
            }
            else
            {
                extraParams.Append(" /D NOLIGHT");
            }

            // build dst shader name
            String n2DstFile = dstFile;
            if (var.IsValid())
            {
                n2DstFile.Append("_");
                n2DstFile.Append(var);
            }
            n2DstFile.Append("_");
            n2DstFile.Append(vsModel);
            n2DstFile.Append("_");
            n2DstFile.Append(psModel);

            // call actual shader compilation method
            success &= this->CompileShader(srcFile, n2DstFile, extraParams);
        }
    }
    return success;
}

//------------------------------------------------------------------------------
/**
*/
bool
ShaderCompiler::CompileAllShaders()
{
    n_assert(this->srcShaderDir.IsValid());
    n_assert(this->dstShaderDir.IsValid());
    IoServer* ioServer = IoServer::Instance();

    // check if source dir exists
    if (!ioServer->DirectoryExists(this->srcShaderDir))
    {
        n_printf("WARNING: shader source directory '%s' not found!\n", this->srcShaderDir.AsCharPtr());
        return false;
    }

    // for each file in source directory
    String pattern;
    if (Platform::Wii == this->platform)
    {
        pattern = "*.xml";
    }
    else
    {
        pattern = "*.fx";
    }

    // for each shader file...
    bool success = true;
    Array<String> srcFiles = ioServer->ListFiles(this->srcShaderDir, pattern);
    IndexT i;
    for (i = 0; i < srcFiles.Size(); i++)
    {
        String dstFile = srcFiles[i];
        dstFile.StripFileExtension();
        if (this->nebula2Mode)
        {
            success &= this->CompileShaderNebula2(srcFiles[i], dstFile);
        }
        else
        {
            success &= this->CompileShader(srcFiles[i], dstFile, "");
        }
    }
    return success;
}

//------------------------------------------------------------------------------
/**
    NOTE: Frame shaders are currently simply copied, there is no 
    binary format for them.
*/
bool
ShaderCompiler::CompileFrameShaders()
{
    n_assert(this->srcFrameShaderDir.IsValid());
    n_assert(this->dstFrameShaderDir.IsValid());
    IoServer* ioServer = IoServer::Instance();

    // check if source dir exists
    if (!ioServer->DirectoryExists(this->srcFrameShaderDir))
    {
        n_printf("WARNING: frame shader source directory '%s' not found!\n", this->srcFrameShaderDir.AsCharPtr());
        return false;
    }

    // make sure target dir exists
    ioServer->CreateDirectory(this->dstFrameShaderDir);

    // for each frame shader...
    bool success = true;
    Array<String> srcFiles = ioServer->ListFiles(this->srcFrameShaderDir, "*.xml");
    IndexT i;
    for (i = 0; i < srcFiles.Size(); i++)
    {
        // build absolute source and target filenames
        String srcPath;
        srcPath.Format("%s/%s", this->srcFrameShaderDir.AsCharPtr(), srcFiles[i].AsCharPtr());
        String dstPath;
        dstPath.Format("%s/%s", this->dstFrameShaderDir.AsCharPtr(), srcFiles[i].AsCharPtr());
        success &= ioServer->CopyFile(srcPath, dstPath);
        n_printf("copy frame shader: %s\n", srcPath.AsCharPtr());
    }
    return success;
}

} // namespace ToolkitUtil