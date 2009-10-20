#pragma once
//------------------------------------------------------------------------------
/**
    @class core/types.h

    Basic type definitions for Nebula3.
    
    (C) 2006 Radon Labs GmbH
*/
#include "memory/memory.h"

// fixing Windows defines...
#ifdef DeleteFile
#undef DeleteFile
#endif
#ifdef CopyFile
#undef CopyFile
#endif
#ifdef GetObject
#undef GetObject
#endif

typedef unsigned long  ulong;
typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;
typedef unsigned char  ubyte;

typedef int IndexT;     // the index type
typedef int SizeT;      // the size type
static const int InvalidIndex = -1;

#define N_ARGB(a,r,g,b) ((uint)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#define N_RGBA(r,g,b,a) N_ARGB(a,r,g,b)
#define N_XRGB(r,g,b)   N_ARGB(0xff,r,g,b)
#define N_COLORVALUE(r,g,b,a) N_RGBA((uint)((r)*255.f),(uint)((g)*255.f),(uint)((b)*255.f),(uint)((a)*255.f))

#if __WIN32__
#define n_stricmp stricmp
#define n_snprintf StringCchPrintf
#elif __XBOX360__
#define n_stricmp lstrcmpi
#define n_snprintf StringCchPrintf
#elif __WII__
#define n_stricmp stricmp
#define n_snprintf sprintf
#else
#error "Unsupported platform!"
#endif

#if __MAYA__
#define ThreadLocal 
#elif __WIN32__
#define ThreadLocal __declspec(thread)
#elif __XBOX360__
#define ThreadLocal __declspec(thread)
#elif __WII__
#define ThreadLocal
#else
#error "Unsupported platform!"
#endif
//------------------------------------------------------------------------------
