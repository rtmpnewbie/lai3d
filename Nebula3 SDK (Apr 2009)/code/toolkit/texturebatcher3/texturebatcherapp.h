#pragma once
//------------------------------------------------------------------------------
/**
    @class Toolkit::TextureBatcherApp.

    Application class for the texturebatcher3 tool.
    
    @todo WaitForKey not implemented!
    
    (C) 2008 Radon Labs GmbH
*/
#include "toolkitutil/toolkitapp.h"
#include "toolkitutil/textureconverter.h"

//------------------------------------------------------------------------------
namespace Toolkit
{
class TextureBatcherApp : public ToolkitUtil::ToolkitApp
{
public:
    /// run the application
    virtual void Run();

private:
    /// parse command line arguments
    virtual bool ParseCmdLineArgs();
    /// setup project info object
    virtual bool SetupProjectInfo();
    /// print help text
    virtual void ShowHelp();

    ToolkitUtil::TextureConverter textureConverter;
    Util::String category;
    Util::String texture;
};

} // namespace Toolkit
//------------------------------------------------------------------------------
