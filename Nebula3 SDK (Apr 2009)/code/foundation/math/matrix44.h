#pragma once
#ifndef MATH_MATRIX44_H
#define MATH_MATRIX44_H
//------------------------------------------------------------------------------
/**
    @file math/matrix44.h

    Frontend header for matrix classes.

    (C) 2006 Radon Labs GmbH
*/
#if __WIN32__
#include "math/d3dx9/d3dx9_matrix44.h"
#elif __XBOX360__
#include "math/xbox360/xbox360_matrix44.h"
#elif __WII__
#include "math/wii/wii_matrix44.h"
#else
#error "matrix44 class not implemented!"
#endif
//-------------------------------------------------------------------
#endif
