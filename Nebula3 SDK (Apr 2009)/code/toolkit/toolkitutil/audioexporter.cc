//------------------------------------------------------------------------------
//  audioexporter.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "audioexporter.h"
#include "io/ioserver.h"
#include "io/uri.h"
#include "toolkitutil/applauncher.h"

namespace ToolkitUtil
{
using namespace Util;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
AudioExporter::AudioExporter() :
    platform(Platform::Win32),
    force(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
AudioExporter::~AudioExporter()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
AudioExporter::Export()
{
    if ((this->platform == Platform::Win32) || (this->platform == Platform::Xbox360))
    {
        return this->ExportWin360();
    }
    else
    {
        n_printf("WARNING: unsupported platform '%s' in AudioExporter!\n", Platform::ToString(this->platform).AsCharPtr());
        return true;
    }
}

//------------------------------------------------------------------------------
/**
    Audio export function for Windows and Xbox360 platform.
*/
bool
AudioExporter::ExportWin360()
{
    n_assert(this->toolPath.IsValid());
    n_assert(this->projectFile.IsValid());      
    n_assert(this->dstDir.IsValid());
    IoServer* ioServer = IoServer::Instance();

    // resolve assigns in project file path
    String resolvedProjFilePath = AssignRegistry::Instance()->ResolveAssignsInString(this->projectFile);
    if (!ioServer->FileExists(resolvedProjFilePath))
    {
        n_printf("WARNING: XACT project file '%s' does not exist!\n", resolvedProjFilePath.AsCharPtr());
        return false;
    }
    
    // resolve assigns in destination dir and make sure that path exists
    String resolvedDstPath = AssignRegistry::Instance()->ResolveAssignsInString(this->dstDir);
    ioServer->CreateDirectory(this->dstDir);

    // setup command line args and launch xactbld3.exe
    String args = "/L /X:HEADER /X:CUELIST /X:REPORT ";
    if (this->force)
    {
        args.Append("/F ");
    }
    if (Platform::Win32 == this->platform)
    {
        args.Append("/WIN32 ");
    }
    else
    {
        args.Append("/XBOX360 ");
    }
    args.Append("\"");
    args.Append(resolvedProjFilePath);
    args.Append("\" \"");
    args.Append(resolvedDstPath);
    args.Append("\"");

    AppLauncher appLauncher;
    appLauncher.SetExecutable(this->toolPath);
    appLauncher.SetWorkingDirectory(resolvedDstPath);
    appLauncher.SetArguments(args);
    if (!appLauncher.LaunchWait())
    {
        n_printf("WARNING: failed to launch audio tool '%s'!\n", this->toolPath.AsCharPtr());
        return false;
    }
    return true;
}

} // namespace ToolkitUtil