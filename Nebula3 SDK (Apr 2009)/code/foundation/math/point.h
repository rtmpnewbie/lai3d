#pragma once
#ifndef MATH_POINT_H
#define MATH_POINT_H
//------------------------------------------------------------------------------
/**
    @class Math::point
    
    A point in homogenous space. A point describes a position in space,
    and has its W component set to 1.0.
    
    (C) 2007 Radon Labs GmbH
*/
#if __WIN32__
#include "math/d3dx9/d3dx9_point.h"
#elif __XBOX360__
#include "math/xbox360/xbox360_point.h"
#elif __WII__
#include "math/wii/wii_point.h"
#else
#error "point class not implemented!"
#endif
//------------------------------------------------------------------------------
#endif
    