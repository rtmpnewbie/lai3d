//------------------------------------------------------------------------------
//  applauncher.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "applauncher.h"

namespace ToolkitUtil
{
using namespace Util;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
AppLauncher::AppLauncher() :
    noConsoleWindow(false)
{ 
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
AppLauncher::LaunchWait() const
{
    n_assert(this->exePath.IsValid());

    STARTUPINFO startupInfo = { sizeof(STARTUPINFO), 0 };
    PROCESS_INFORMATION processInfo = { 0 };

    // build a command line
    String cmdLine = this->exePath.LocalPath();
    cmdLine.Append(" ");
    cmdLine.Append(this->args);

    DWORD creationFlags = 0;
    if (this->noConsoleWindow)
    {
        creationFlags |= CREATE_NO_WINDOW;
    }
    if (!CreateProcess(NULL,                            // lpApplicationName
                       (LPSTR) cmdLine.AsCharPtr(),     // lpCommandLine
                       NULL,                            // lpProcessAttributes
                       NULL,                            // lpThreadAttributes
                       FALSE,                           // bInheritsHandle
                       creationFlags,                   // dwCreationFlags
                       NULL,                            // lpEnvironment
                       this->workingDir.LocalPath().AsCharPtr(),    // lpCurrentDirectory
                       &startupInfo,                    // lpStartupInfo
                       &processInfo))                   // lpProcessInformation
    {
        return false;
    }

    // wait until process exits
    WaitForSingleObject(processInfo.hProcess, INFINITE);
    
    // cleanup
    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);

    return true;
}

} // namespace ToolkitUtil