#pragma once
#ifndef MATH_VECTOR_H
#define MATH_VECTOR_H
//------------------------------------------------------------------------------
/**
    @class Math::vector
    
    A vector in homogenous space. A point describes a direction and length
    in 3d space and always has a w component of 0.0.
    
    (C) 2007 Radon Labs GmbH
*/
#if __WIN32__
#include "math/d3dx9/d3dx9_vector.h"
#elif __XBOX360__
#include "math/xbox360/xbox360_vector.h"
#elif __WII__
#include "math/wii/wii_vector.h"
#else
#error "vector class not implemented!"
#endif
//------------------------------------------------------------------------------
#endif
