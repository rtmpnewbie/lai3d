//------------------------------------------------------------------------------
//  archiveapp.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "archiverapp.h"
#include "toolkitutil/applauncher.h"

namespace Toolkit
{
using namespace ToolkitUtil;
using namespace Util;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
void
ArchiverApp::ShowHelp()
{
    n_printf("Nebula3 archiver.\n"
             "(C) Radon Labs GmbH 2008.\n"
             "Creates platform-specific asset archives (e.g. export.zip, export_win32.zip)\n");
}

//------------------------------------------------------------------------------
/**
*/
bool
ArchiverApp::SetupProjectInfo()
{
    if (ToolkitApp::SetupProjectInfo())
    {
        if (this->projectInfo.HasAttr("ArchiverTool"))
        {
            this->toolPath = this->projectInfo.GetAttr("ArchiverTool");
        }
        else
        {
            n_printf("ERROR: no ArchiveTool attribute set in projectinfo.xml!");
            return false;
        }
        if (this->projectInfo.HasAttr("WiiDvdRoot"))
        {
            this->wiiDvdRoot = this->projectInfo.GetAttr("WiiDvdRoot");
        }
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
ArchiverApp::Run()
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
    
    // invoke platformspecific packers
    switch (this->platform)
    {
        case Platform::Win32:
        case Platform::Xbox360:
            this->PackDirectoryWin360("export");
            this->PackDirectoryWin360("export_" + Platform::ToString(this->platform));
            break;

        case Platform::Wii:
            this->PackAndCopyWiiArchive();
            break;
    }
}

//------------------------------------------------------------------------------
/**
    Packs a single directory using zip.exe.
*/
void
ArchiverApp::PackDirectoryWin360(const String& dirName)
{
    IoServer* ioServer = IoServer::Instance();

    // make sure the directory exists
    String dirPath = "proj:" + dirName;
    if (!ioServer->DirectoryExists(dirPath))
    {
        n_printf("ERROR: dir '%s' does not exist!", dirPath.AsCharPtr());
        return;
    }

    // delete the target file, if exists
    String fileName = dirName + ".zip";
    String filePath = "proj:" + fileName;
    if (ioServer->FileExists(filePath))
    {
        ioServer->DeleteFile(filePath);
    }

    // invoke the zip tool
    String args;
    args.Format("-r %s %s\\* -x *.db4", fileName.AsCharPtr(), dirName.AsCharPtr());
    
    n_printf("Archiving: %s %s", this->toolPath.AsCharPtr(), args.AsCharPtr());
    AppLauncher appLauncher;
    appLauncher.SetExecutable(this->toolPath);
    appLauncher.SetWorkingDirectory("proj:");
    appLauncher.SetArguments(args);
    if (!appLauncher.LaunchWait())
    {
        n_printf("WARNING: failed to launch converter tool '%s'!\n", this->toolPath.AsCharPtr());
    }
}

//------------------------------------------------------------------------------
/**
    Invokes the Wii packer to pack the export and export_wii directories.
*/
void
ArchiverApp::PackAndCopyWiiArchive()
{       
    // make sure the expected directories exists
    if (!ioServer->DirectoryExists("proj:export"))
    {
        n_printf("ERROR: dir 'proj:export' does not exist!\n");
        return;
    }
    if (!ioServer->DirectoryExists("proj:export_wii"))
    {
        n_printf("ERROR: dir 'proj:export_wii' does not exist!\n");
        return;
    }

    // delete old archive file if exists
    if (ioServer->FileExists("proj:export.arc"))
    {
        ioServer->DeleteFile("proj:export.arc");
    }

    // invoke the darch tool
    String args = "-c export export_wii export.arc";
    n_printf("Archiving: %s %s", this->toolPath.AsCharPtr(), args.AsCharPtr());
    AppLauncher appLauncher;
    appLauncher.SetExecutable(this->toolPath);
    appLauncher.SetWorkingDirectory("proj:");
    appLauncher.SetArguments(args);
    if (!appLauncher.LaunchWait())
    {
        n_printf("ERROR: failed to launch converter tool '%s'!\n", this->toolPath.AsCharPtr());
        return;
    }

    // copy the result to the Wii DvdRoot directory
    if (!this->wiiDvdRoot.IsValid())
    {
        n_printf("ERROR: no WiiDvdRoot attribute defined in projectinfo.xml!\n");
        return;
    }
    String dstPath = this->wiiDvdRoot + "/export.arc";
    if (ioServer->FileExists(dstPath))
    {
        ioServer->DeleteFile(dstPath);
    }
    if (!ioServer->CopyFile("proj:export.arc", dstPath))
    {
        n_printf("WARNING: failed to copy Wii archive to '%s'!\n", dstPath.AsCharPtr());
    }
}

} // namespace Toolkit
