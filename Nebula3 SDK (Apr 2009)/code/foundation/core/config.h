#pragma once
//------------------------------------------------------------------------------
/**
    @file core/config.h

    Nebula3 compiler specific defines and configuration.
    
    (C) 2006 Radon Labs GmbH
*/

#ifdef __XBOX360__
#undef __XBOX360__
#endif
#ifdef _XBOX
#define __XBOX360__ (1)
#endif

#ifdef __WIN32__
#undef __WIN32__
#endif
#ifdef WIN32
#define __WIN32__ (1)
#endif

//------------------------------------------------------------------------------
/**
    Nebula3 configuration.
*/

// uitlize bink video handler
//#define USE_BINK_VIDEO

#ifdef _DEBUG
#define NEBULA3_DEBUG (1)
#endif

// enable/disable Nebula3 memory stats
#if NEBULA3_DEBUG
#define NEBULA3_MEMORY_STATS (1)
#define NEBULA3_MEMORY_ADVANCED_DEBUGGING (0)
#else
#define NEBULA3_MEMORY_STATS (0) // not useable on xbox360 in release mode cause of HeapWalk
#define NEBULA3_MEMORY_ADVANCED_DEBUGGING (0)
#endif

// override SQLite filesystem functions with Nebula functions?
// only useful on consoles
// win32 doesn't work without !!!
#if __WIN32__
#define NEBULA3_OVERRIDE_SQLITE_FILEFUNCTIONS (1)
#else
#define NEBULA3_OVERRIDE_SQLITE_FILEFUNCTIONS (1)
#endif

// enable/disable allocating RefCounted objects from MemoryPool
// NOTE: the object creation benchmark actually runs FASTER without
// memory pools doh... (even after removing asserts and taking the critical
// section) -> needs more investigation!
#define NEBULA3_REFCOUNTED_MEMORYPOOLS (0)

// average page size for RefCounted memory pools
#define NEBULA3_REFCOUNTED_MEMPOOLPAGESIZE (4096)

// enable/disable bounds checking in the container util classes
#if NEBULA3_DEBUG
#define NEBULA3_BOUNDSCHECKS (1)
#else
#define NEBULA3_BOUNDSCHECKS (0)
#endif

// enable/disable profiling (see Debug::DebugTimer, Debug::DebugCounter)
#if PUBLIC_BUILD
#define NEBULA3_ENABLE_PROFILING (0)
#else
#define NEBULA3_ENABLE_PROFILING (1)
#endif

// enable/disable support for Nebula2 file formats and concepts
#define NEBULA3_LEGACY_SUPPORT (1)

// enable/disable mini dumps
#define NEBULA3_ENABLE_MINIDUMPS (1)

// Nebula3's main window class
#define NEBULA3_WINDOW_CLASS "Nebula3::MainWindow"

// number of lines in the IO::HistoryConsoleHandler ring buffer
#if __WII__
#define NEBULA3_CONSOLE_HISTORY_SIZE (128)
#else
#define NEBULA3_CONSOLE_HISTORY_SIZE (4096)
#endif

//------------------------------------------------------------------------------
/**
    http define
*/
#if PUBLIC_BUILD
#define __NEBULA3_HTTP__ (0)
#else
#define __NEBULA3_HTTP__ (1)
#endif

//------------------------------------------------------------------------------
/**
    script define
*/
#if __WII__
#define __NEBULA3_SCRIPTING__ (0)
#else
#define __NEBULA3_SCRIPTING__ (1)
#endif

//------------------------------------------------------------------------------
/**
    Default values and pathes.
*/
#if __WII__
#define DEFAULT_IO_SCHEME "arc"
#else
#define DEFAULT_IO_SCHEME "file"
#endif

//------------------------------------------------------------------------------
/**
    Visual Studio specifics.
*/
#ifdef _MSC_VER
#define __VC__ (1)
#endif
#ifdef __VC__
#pragma warning( disable : 4251 )       // class XX needs DLL interface to be used...
#pragma warning( disable : 4355 )       // initialization list uses 'this' 
#pragma warning( disable : 4275 )       // base class has not dll interface...
#pragma warning( disable : 4786 )       // symbol truncated to 255 characters
#pragma warning( disable : 4530 )       // C++ exception handler used, but unwind semantics not enabled
#pragma warning( disable : 4995 )       // _OLD_IOSTREAMS_ARE_DEPRECATED
#pragma warning( disable : 4996 )       // _CRT_INSECURE_DEPRECATE, VS8: old string routines are deprecated
#pragma warning( disable : 4512 )       // 'class' : assignment operator could not be generated
#pragma warning( disable : 4610 )       // object 'class' can never be instantiated - user-defined constructor required
#pragma warning( disable : 4510 )       // 'class' : default constructor could not be generated
#endif

//------------------------------------------------------------------------------
/**
    Metrowerks Codewarrior specifics.
*/
#ifdef __MWERKS__
#pragma warning off (10216)     // illegal empty declaration used ( ; ; )
#pragma warning off (10317)     
#pragma warning off (10342)     // ...not inlined
#pragma warning off (10182)     // arg not used in function 
#endif

//------------------------------------------------------------------------------
/**
    GCC specifics.
*/
#ifndef __GNUC__
#  define  __attribute__(x)  /**/
#endif

//------------------------------------------------------------------------------

