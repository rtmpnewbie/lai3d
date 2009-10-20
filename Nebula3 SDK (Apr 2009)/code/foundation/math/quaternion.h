#pragma once
#ifndef MATH_QUATERNION_H
#define MATH_QUATERNION_H
//------------------------------------------------------------------------------
/**
    @class Math::quaternion

    Nebula's quaternion class.

    (C) 2004 RadonLabs GmbH
*/
#if __WIN32__
#include "math/d3dx9/d3dx9_quaternion.h"
#elif __XBOX360__
#include "math/xbox360/xbox360_quaternion.h"
#elif __WII__
#include "math/wii/wii_quaternion.h"
#else
#error "quaternion class not implemented!"
#endif
//-------------------------------------------------------------------
#endif
