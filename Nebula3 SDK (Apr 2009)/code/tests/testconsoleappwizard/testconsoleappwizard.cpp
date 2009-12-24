//------------------------------------------------------------------------------
//  testconsoleappwizard.cpp
//  (C) 2009 CgPanda
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "TestConsoleApp.h"

using namespace Test;
using namespace Util;


//------------------------------------------------------------------------------
/**
*/
void
main(int argc, const char** argv)
{
	CommandLineArgs args(argc, argv);
	TestConsoleApp app;
	app.SetCompanyName("Giant Interactive Group Inc");
	app.SetAppName("testconsoleappwizard");
	app.SetCmdLineArgs(args);
	if (app.Open())
	{
		app.Run();
		app.Close();
	}
	app.Exit();
}