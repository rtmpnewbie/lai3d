#pragma once
//------------------------------------------------------------------------------
/**
    @class System::AppEntry
    
    Implements a platform-indepent app-entry point. In your main file,
    put the line
    
    ImplementNebulaApplication();
    
    And then replace your main() function with:
    
    void NebulaMain(const CommandLineArgs& args)

    (C) 2007 Radon Labs GmbH
*/
#include "core/types.h"

//------------------------------------------------------------------------------
#if __WIN32__
#define ImplementNebulaApplication() \
void NebulaMain(const Util::CommandLineArgs& args); \
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR lpCmdLine, int nShowCmd) \
{ \
    Util::CommandLineArgs args(lpCmdLine); \
    NebulaMain(args); \
    return 0; \
}
#elif __XBOX360__
#define ImplementNebulaApplication() \
void NebulaMain(const Util::CommandLineArgs& args); \
void __cdecl main() \
{ \
    Util::CommandLineArgs args; \
    NebulaMain(args); \
}
#elif __WII__
#define ImplementNebulaApplication() \
void NebulaMain(const Util::CommandLineArgs& args); \
void main() \
{ \
    Util::CommandLineArgs args; \
    NebulaMain(args); \
}
#else
#define ImplementNebulaApplication() \
void NebulaMain(const Util::CommandLineArgs& args); \
int __cdecl main(int argc, const char** argv) \
{ \
    Util::CommandLineArgs args(argc, argv); \
    NebulaMain(args); \
    return 0; \
}
#endif
//------------------------------------------------------------------------------
