#pragma once
//------------------------------------------------------------------------------
/**
    @file math/scalar.h
    
    Nebula's scalar datatype.
    
    (C) 2007 Radon Labs GmbH
*/
#if __WIN32__
#include "math/d3dx9/d3dx9_scalar.h"
#elif __XBOX360__
#include "math/xbox360/xbox360_scalar.h"
#elif __WII__
#include "math/wii/wii_scalar.h"
#else
#error "scalar class not implemented!"
#endif

// common platform-agnostic definitions
namespace Math
{
#ifndef PI
#define PI (3.1415926535897932384626433832795028841971693993751f)
#endif
#define N_PI PI

#ifndef TINY
#define TINY (0.0000001f)
#endif
#define N_TINY TINY

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_max(scalar a, scalar b)
{
    return (a > b) ? a : b;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline double
n_max(double a, double b)
{
    return (a > b) ? a : b;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline int
n_max(int a, int b)
{
    return (a > b) ? a : b;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_min(scalar a, scalar b)
{
    return (a < b) ? a : b;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline double
n_min(double a, double b)
{
    return (a < b) ? a : b;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline int
n_min(int a, int b)
{
    return (a < b) ? a : b;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_abs(scalar a)
{
    return (a < 0.0f) ? -a : a;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline double
n_abs(double a)
{
    return (a < 0.0f) ? -a : a;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline int
n_abs(int a)
{
    return (a < 0.0f) ? -a : a;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_sgn(scalar a)
{
    return (a < 0.0f) ? -1.0f : 1.0f;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_deg2rad(scalar d)
{
    return (d * PI) / 180.0f;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_rad2deg(scalar r)
{
    return (r * 180.0f) / PI;
}

//------------------------------------------------------------------------------
/**
    Integer clamping.
*/
__forceinline int 
n_iclamp(int val, int minVal, int maxVal)
{
    if (val < minVal)      return minVal;
    else if (val > maxVal) return maxVal;
    else return val;
}

//------------------------------------------------------------------------------
/**
    A fuzzy floating point equality check
*/
__forceinline bool 
n_fequal(scalar f0, scalar f1, scalar tol) 
{
    scalar f = f0 - f1;
    return ((f > (-tol)) && (f < tol));
}

//------------------------------------------------------------------------------
/**
    A fuzzy floating point less-then check.
*/
__forceinline bool 
n_fless(scalar f0, scalar f1, scalar tol) 
{
    return ((f0 - f1) < tol);
}

//------------------------------------------------------------------------------
/**
    A fuzzy floating point greater-then check.
*/
__forceinline bool 
n_fgreater(scalar f0, scalar f1, scalar tol) 
{
    return ((f0 - f1) > tol);
}

//------------------------------------------------------------------------------
/**
    Clamp a value against lower und upper boundary.
*/
__forceinline scalar
n_clamp(scalar val, scalar lower, scalar upper)
{
    if (val < lower)      return lower;
    else if (val > upper) return upper;
    else                  return val;
}

//------------------------------------------------------------------------------
/**
    Saturate a value (clamps between 0.0f and 1.0f)
*/
__forceinline scalar 
n_saturate(scalar val)
{
    if (val < 0.0f)      return 0.0f;
    else if (val > 1.0f) return 1.0f;
    else return val;
}

//------------------------------------------------------------------------------
/**
Saturate a value (clamps between 0.0f and 1.0f)
*/
__forceinline double 
n_saturate(double val)
{
    if (val < 0.0)      return 0.0;
    else if (val > 1.0) return 1.0;
    else return val;
}

//------------------------------------------------------------------------------
/**
    Linearly interpolate between 2 values: ret = x + l * (y - x)
*/
__forceinline scalar 
n_lerp(scalar x, scalar y, scalar l)
{
    return x + l * (y - x);
}

//------------------------------------------------------------------------------
/**
    Linearly interpolate between 2 values: ret = x + l * (y - x)
*/
__forceinline double 
n_lerp(double x, double y, double l)
{
    return x + l * (y - x);
}

//------------------------------------------------------------------------------
/**
    Get angular distance.
*/
__forceinline scalar 
n_angulardistance(scalar from, scalar to)
{
	scalar normFrom = n_modangle(from);
    scalar normTo   = n_modangle(to);
    scalar dist = normTo - normFrom;
    if (dist < n_deg2rad(-180.0f))
    {
        dist += n_deg2rad(360.0f);
    }
    else if (dist > n_deg2rad(180.0f))
    {
        dist -= n_deg2rad(360.0f);
    }
    return dist;
}

//------------------------------------------------------------------------------
/**
    Returns true if the input scalar is denormalized (#DEN)
*/
__forceinline bool
n_isdenormal(scalar s)
{
    return (((*(uint*)&s)&0x7f800000)==0);
}

//------------------------------------------------------------------------------
/**
    Returns 0 if scalar is denormal.
*/
__forceinline float
n_undenormalize(scalar s)
{
    if (n_isdenormal(s))
    {
        return 0.0f;
    }
    else
    {
        return s;
    }
}

} // namespace Math
//------------------------------------------------------------------------------
    