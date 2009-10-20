//------------------------------------------------------------------------------
//  nviewer3.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tools/nviewer3/nviewer3application.h"

using namespace Tools;
using namespace Util;

ImplementNebulaApplication();

//------------------------------------------------------------------------------
/**
*/
void
NebulaMain(const CommandLineArgs& args)
{
    NViewer3Application app;
    app.SetCompanyName("Radon Labs GmbH");
    app.SetAppName("nViewer3");
    app.SetCmdLineArgs(args);
    if (app.Open())
    {
        app.Run();
        app.Close();
    }
    app.Exit();
}
