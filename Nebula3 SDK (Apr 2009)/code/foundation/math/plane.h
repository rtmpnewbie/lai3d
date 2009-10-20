#pragma once
#ifndef MATH_PLANE_H
#define MATH_PLANE_H
//------------------------------------------------------------------------------
/**
    @class Math::plane

    Nebula's plane class.

    (C) 2007 RadonLabs GmbH
*/
#if __WIN32__
#include "math/d3dx9/d3dx9_plane.h"
#elif __XBOX360__
#include "math/xbox360/xbox360_plane.h"
#elif __WII__
#include "math/wii/wii_plane.h"
#else
#error "plane class not implemented!"
#endif
//------------------------------------------------------------------------------
#endif
