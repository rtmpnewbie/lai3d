//------------------------------------------------------------------------------
//  texturebatcherapp.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "texturebatcherapp.h"

namespace Toolkit
{

//------------------------------------------------------------------------------
/**
*/
bool
TextureBatcherApp::ParseCmdLineArgs()
{
    if (ToolkitApp::ParseCmdLineArgs())
    {
        this->textureConverter.SetForceFlag(this->args.GetBoolFlag("-force"));
        this->category = this->args.GetString("-cat", "");
        this->texture = this->args.GetString("-tex", "");
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
TextureBatcherApp::SetupProjectInfo()
{
    if (ToolkitApp::SetupProjectInfo())
    {
        this->textureConverter.SetPlatform(this->platform);
        this->textureConverter.SetToolPath(this->projectInfo.GetAttr("TextureTool"));
        this->textureConverter.SetTexAttrTablePath(this->projectInfo.GetAttr("TextureAttrTable"));
        this->textureConverter.SetSrcDir(this->projectInfo.GetAttr("TextureSrcDir"));
        this->textureConverter.SetDstDir(this->projectInfo.GetAttr("TextureDstDir"));        
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
TextureBatcherApp::ShowHelp()
{
    n_printf("Nebula3 texture batch exporter.\n"
             "(C) Radon Labs GmbH 2008.\n"
             "-help       -- display this help\n"
             "-platform   -- select platform (win32, xbox360, wii, ps3)\n"
             "-waitforkey -- wait for key when complete\n"
             "-force      -- force export (don't check time stamps)\n"
             "-cat        -- select specific category\n"
             "-tex        -- select specific texture (also needs -cat)\n");
}

//------------------------------------------------------------------------------
/**
*/
void
TextureBatcherApp::Run()
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

    // setup the texture converter
    if (!this->textureConverter.Setup())
    {
        n_printf("ERROR: failed to setup texture converter!\n");
        this->SetReturnCode(10);
        return;
    }

    // perform texture conversion
    bool result = true;
    if (this->category.IsValid())
    {
        if (this->texture.IsValid())
        {
            result = this->textureConverter.ConvertCategoryTexture(this->category, this->texture);
        }
        else
        {
            result = this->textureConverter.ConvertCategory(this->category);
        }
    }
    else
    {
        result = this->textureConverter.ConvertAll();
    }
}

} // namespace ToolkitUtil
