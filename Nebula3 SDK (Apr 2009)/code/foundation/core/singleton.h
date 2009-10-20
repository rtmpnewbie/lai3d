#pragma once
//------------------------------------------------------------------------------
/**
    @class Core::Singleton
  
    Implements a system specific Singleton
    
    (C) 2007 Radon Labs GmbH
*/
#if __WIN32__
#include "core/win32/win32singleton.h"
#elif __XBOX360__
#include "core/xbox360/xbox360singleton.h"
#elif __WII__
#include "core/wii/wiisingleton.h"
#else
#error "IMPLEMENT ME!"
#endif
