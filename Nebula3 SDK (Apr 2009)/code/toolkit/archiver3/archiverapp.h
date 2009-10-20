#pragma once
//------------------------------------------------------------------------------
/**
    @class Toolkit::ArchiverApp

    Application class for archiver3 tool.

    (C) 2009 Radon Labs GmbH
*/
#include "toolkitutil/toolkitapp.h"

//------------------------------------------------------------------------------
namespace Toolkit
{
class ArchiverApp : public ToolkitUtil::ToolkitApp
{
public:
    /// run the application
    virtual void Run();

private:
    /// setup project info object
    virtual bool SetupProjectInfo();
    /// print help text
    virtual void ShowHelp();
    /// pack directory using ZIP for the Win32 and Xbox360 platforms
    void PackDirectoryWin360(const Util::String& dir);
    /// pack all asset directories for the Wii platform, and copy to Wii SDK's DVDROOT
    void PackAndCopyWiiArchive();

    Util::String toolPath;
    Util::String wiiDvdRoot;
};

} // namespace Toolkit