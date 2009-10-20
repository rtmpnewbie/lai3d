#pragma once
//------------------------------------------------------------------------------
/**
    @class ToolkitUtil::AppLauncher
    
    Launch an external Win32 application.
    
    (C) 2008 Radon Labs GmbH
*/
#include "util/string.h"
#include "io/uri.h"

//------------------------------------------------------------------------------
namespace ToolkitUtil
{
class AppLauncher
{
public:
    /// constructor
    AppLauncher();
    
    /// set the executable path
    void SetExecutable(const IO::URI& uri);
    /// set working directory
    void SetWorkingDirectory(const IO::URI& uri);
    /// set command line arguments
    void SetArguments(const Util::String& args);
    /// do not open a console window for the new process
    void SetNoConsoleWindow(bool b);
    
    /// launch application and wait for its termination
    bool LaunchWait() const;
    
private:
    bool noConsoleWindow;
    IO::URI exePath;
    IO::URI workingDir;
    Util::String args;    
};

//------------------------------------------------------------------------------
/**
*/
inline void
AppLauncher::SetExecutable(const IO::URI& uri)
{
    this->exePath = uri;
}

//------------------------------------------------------------------------------
/**
*/
inline void
AppLauncher::SetWorkingDirectory(const IO::URI& uri)
{
    this->workingDir = uri;
}

//------------------------------------------------------------------------------
/**
*/
inline void
AppLauncher::SetArguments(const Util::String& a)
{
    this->args = a;
}

//------------------------------------------------------------------------------
/**
*/
inline void
AppLauncher::SetNoConsoleWindow(bool b)
{
    this->noConsoleWindow = b;
}

} // namespace ToolkitUtil
//------------------------------------------------------------------------------
    