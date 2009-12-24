#pragma once
//------------------------------------------------------------------------------
/**
    @class TestConsoleApp
  
    It's a test console application.

    (C) 2009 CgPanda
*/
#include "app/consoleapplication.h"

//------------------------------------------------------------------------------
namespace Test
{
class TestConsoleApp : public App::ConsoleApplication
{
public:
	/// constructor
	TestConsoleApp();
	/// destructor
	virtual ~TestConsoleApp();
	/// open the application
	virtual bool Open();
	/// close the application
	virtual void Close();
	/// run the application, return when user wants to exit
	virtual void Run();

private:
};

} // namespace Test
//------------------------------------------------------------------------------
