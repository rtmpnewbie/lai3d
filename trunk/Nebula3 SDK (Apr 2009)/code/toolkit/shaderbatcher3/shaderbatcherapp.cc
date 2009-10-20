//------------------------------------------------------------------------------
//  shaderbatcherapp.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "shaderbatcherapp.h"

namespace Toolkit
{

//------------------------------------------------------------------------------
/**
*/
bool
ShaderBatcherApp::ParseCmdLineArgs()
{
    if (ToolkitApp::ParseCmdLineArgs())
    {
        this->shaderCompiler.SetForceFlag(this->args.GetBoolFlag("-force"));
        this->shaderCompiler.SetDebugFlag(this->args.GetBoolFlag("-debug"));
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
ShaderBatcherApp::SetupProjectInfo()
{
    if (ToolkitApp::SetupProjectInfo())
    {
        this->shaderCompiler.SetPlatform(this->platform);
        if (this->projectInfo.HasAttr("ShaderTool"))
        {
            this->shaderCompiler.SetToolPath(this->projectInfo.GetAttr("ShaderTool"));
        }
        if (this->projectInfo.HasAttr("ShaderToolParams"))
        {
            this->shaderCompiler.SetAdditionalParams(this->projectInfo.GetAttr("ShaderToolParams"));
        }
        this->shaderCompiler.SetShaderSrcDir(this->projectInfo.GetAttr("ShaderSrcDir"));
        this->shaderCompiler.SetShaderDstDir(this->projectInfo.GetAttr("ShaderDstDir"));
        this->shaderCompiler.SetFrameShaderSrcDir(this->projectInfo.GetAttr("FrameShaderSrcDir"));
        this->shaderCompiler.SetFrameShaderDstDir(this->projectInfo.GetAttr("FrameShaderDstDir"));
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
ShaderBatcherApp::ShowHelp()
{
    n_printf("Nebula3 shader batch compiler.\n"
             "(C) Radon Labs GmbH 2008.\n"
             "-help       -- display this help\n"
             "-platform   -- select platform (win32, xbox360, wii, ps3)\n"
             "-waitforkey -- wait for key when complete\n"
             "-force      -- force recompile\n"
             "-debug      -- compile with debugging information\n");
}

//------------------------------------------------------------------------------
/**
*/
void
ShaderBatcherApp::Run()
{
    // parse command line args
    if (!this->ParseCmdLineArgs())
    {
        return;
    }

    // setup the project info object
    if (!this->SetupProjectInfo())
    {
        return;
    }

    // call the shader compiler tool
    if (!this->shaderCompiler.CompileAllShaders())
    {
        this->SetReturnCode(10);
        return;
    }
    if (!this->shaderCompiler.CompileFrameShaders())
    {
        this->SetReturnCode(10);
        return;
    }
}

} // namespace Toolkit
