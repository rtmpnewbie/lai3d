#pragma once
//------------------------------------------------------------------------------
/**
    @class App::Application
  
    Provides a simple application model for Nebula3 apps.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/types.h"
#include "core/singleton.h"
#include "util/string.h"
#include "util/commandlineargs.h"
#include "system/appentry.h"

//------------------------------------------------------------------------------
namespace App
{
class Application
{
    __DeclareSingleton(Application);
public:
    /// constructor
    Application();
    /// destructor
    virtual ~Application();
    /// set company name
    void SetCompanyName(const Util::String& n);
    /// get company name
    const Util::String& GetCompanyName() const;
    /// set application name
    void SetAppName(const Util::String& n);
    /// get application name
    const Util::String& GetAppName() const;
    /// set command line args
    void SetCmdLineArgs(const Util::CommandLineArgs& a);
    /// get command line args
    const Util::CommandLineArgs& GetCmdLineArgs() const;
    /// open the application
    virtual bool Open();
    /// close the application
    virtual void Close();
    /// exit the application, call right before leaving main()
    virtual void Exit();
    /// run the application, return when user wants to exit
    virtual void Run();
    /// return true if app is open
    bool IsOpen() const;
    /// get the return code
    int GetReturnCode() const;

protected:
    /// set return code
    void SetReturnCode(int c);

    Util::String companyName;
    Util::String appName;
    Util::CommandLineArgs args;
    bool isOpen;
    int returnCode;
};

//------------------------------------------------------------------------------
/**
*/
inline
bool
Application::IsOpen() const
{
    return this->isOpen;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
Application::SetCompanyName(const Util::String& n)
{
    this->companyName = n;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::String&
Application::GetCompanyName() const
{
    return this->companyName;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
Application::SetAppName(const Util::String& n)
{
    this->appName = n;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::String&
Application::GetAppName() const
{
    return this->appName;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
Application::SetCmdLineArgs(const Util::CommandLineArgs& a)
{
    this->args = a;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::CommandLineArgs&
Application::GetCmdLineArgs() const
{
    return this->args;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Application::SetReturnCode(int c)
{
    this->returnCode = c;
}

//------------------------------------------------------------------------------
/**
*/
inline int
Application::GetReturnCode() const
{
    return this->returnCode;
}

} // namespace App
//------------------------------------------------------------------------------
