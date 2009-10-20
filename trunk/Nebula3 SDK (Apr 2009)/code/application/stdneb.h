#pragma once
#ifndef STDNEB_H
#define STDNEB_H
//------------------------------------------------------------------------------
/**
    @file core/stdneb.h

    Precompiled header. Put platform-specific headers which rarely change
    in here (e.g. windows.h).

    (C) 2007 Radon Labs GmbH
*/
#include "core/config.h"
#if __WIN32__
#include "core/win32/precompiled.h"
#elif __XBOX360__
#include "core/xbox360/precompiled.h"
#elif __WII__
#include "core/wii/precompiled.h"
#else
#error "precompiled.h not implemented on this platform"
#endif
//------------------------------------------------------------------------------
#endif

    
    