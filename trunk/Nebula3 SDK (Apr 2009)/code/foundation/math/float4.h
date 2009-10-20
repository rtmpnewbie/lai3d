#pragma once
#ifndef MATH_FLOAT4_H
#define MATH_FLOAT4_H
//------------------------------------------------------------------------------
/**
    @class Math::float4

    A 4-component float vector class. This is the basis class for points
    and vectors.
    
    (C) 2007 RadonLabs GmbH
*/
#if __WIN32__
#include "math/d3dx9/d3dx9_float4.h"
#elif __XBOX360__
#include "math/xbox360/xbox360_float4.h"
#elif __WII__
#include "math/wii/wii_float4.h"
#else
#error "float4 class not implemented!"
#endif
//------------------------------------------------------------------------------
#endif
