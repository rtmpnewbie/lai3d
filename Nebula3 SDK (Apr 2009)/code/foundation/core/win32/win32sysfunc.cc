//------------------------------------------------------------------------------
//  sysfunc.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "core/win32/win32sysfunc.h"
#include "core/refcounted.h"
#include "debug/minidump.h"
#include "util/blob.h"
#include "util/guid.h"
#include "net/socket/socket.h"
#include "debug/minidump.h"
#include "threading/thread.h"

namespace Win32
{
using namespace Util;

bool volatile SysFunc::SetupCalled = false;
const Core::ExitHandler* SysFunc::ExitHandlers = 0;

//------------------------------------------------------------------------------
/**
    This method must be called at application start before any threads
    are spawned. It is used to setup static objects beforehand (i.e.
    private heaps of various utility classes). Doing this eliminates
    the need for fine-grained locking in the affected classes.
*/
void
SysFunc::Setup()
{
    if (!SetupCalled)
    {
        SetupCalled = true;
        #if !__MAYA__
        Threading::Thread::SetMyThreadName("MainThread");
        #endif
        Memory::SetupHeaps();
        Memory::Heap::Setup();
        Blob::Setup();
        Guid::Setup();
        #if !__MAYA__
        Net::Socket::InitNetwork();
        Debug::MiniDump::Setup();
        #endif   
    }
}

//------------------------------------------------------------------------------
/**
    This method is called by Application::Exit(), or otherwise must be
    called right before the end of the programs main() function. The method
    will properly shutdown the Nebula3 runtime environment, and report 
    refcounting and memory leaks (debug builds only). This method will not
    return.
*/
void
SysFunc::Exit(int exitCode)
{
    // first produce a RefCount leak report
    #if NEBULA3_DEBUG
    Core::RefCounted::DumpRefCountingLeaks();
    #endif

    // call exit handlers
    const Core::ExitHandler* exitHandler = SysFunc::ExitHandlers;
    while (0 != exitHandler)
    {
        exitHandler->OnExit();
        exitHandler = exitHandler->Next();
    }

    // shutdown the C runtime, this cleans up static objects but doesn't shut 
    // down the process
    _cexit();

    // call static shutdown methods
    Blob::Shutdown();
    Guid::Shutdown();

    // shutdown global factory object
    Core::Factory::Destroy();

    // report mem leaks
    #if NEBULA3_MEMORY_ADVANCED_DEBUGGING
    Memory::DumpMemoryLeaks();
    #endif   

    // finally terminate the process
    ExitProcess(exitCode);
}

//------------------------------------------------------------------------------
/**
    This displays a Win32 error message box and quits the program
*/
void
SysFunc::Error(const char* error)
{
    #ifdef _DEBUG
    OutputDebugString(error);
    #endif
    /*
    HWND hwnd = FindWindow(NEBULA3_WINDOW_CLASS, NULL);
    if (hwnd)
    {
        ShowWindow(hwnd, SW_MINIMIZE);
    }
    */
    ::MessageBox(NULL, error, "NEBULA3 SYSTEM ERROR", MB_OK|MB_APPLMODAL|MB_SETFOREGROUND|MB_TOPMOST|MB_ICONERROR);
    #if !__MAYA__
    Debug::MiniDump::WriteMiniDump();
    #endif
    abort();
}

//------------------------------------------------------------------------------
/**
    This displays a Win32 message box
*/
void
SysFunc::MessageBox(const char* msg)
{
    #ifdef _DEBUG
    OutputDebugString(msg);
    #endif
    HWND hwnd = FindWindow(NEBULA3_WINDOW_CLASS, NULL);
    if (hwnd)
    {
        ShowWindow(hwnd, SW_MINIMIZE);
    }
    ::MessageBox(NULL, msg, "NEBULA3 MESSAGE", MB_OK|MB_APPLMODAL|MB_SETFOREGROUND|MB_TOPMOST|MB_ICONERROR);
}

//------------------------------------------------------------------------------
/**
    Sleep for a specified amount of seconds, give up time slice.
*/
void
SysFunc::Sleep(double sec)
{
    int milliSecs = int(sec * 1000.0);
    ::Sleep(milliSecs);
}

//------------------------------------------------------------------------------
/**
    Put a message on the debug console.
*/
void
SysFunc::DebugOut(const char* msg)
{
    OutputDebugString(msg);
}

//------------------------------------------------------------------------------
/**
    Register a new exit handler. This method is called at startup time
    from the constructor of static exit handler objects. This is the only
    supported way to register exit handlers. The method will return
    a pointer to the next exit handler in the forward linked list 
    (or 0 if this is the first exit handler).
*/
const Core::ExitHandler*
SysFunc::RegisterExitHandler(const Core::ExitHandler* exitHandler)
{
    const Core::ExitHandler* firstHandler = ExitHandlers;
    ExitHandlers = exitHandler;
    return firstHandler;
}

} // namespace Win32