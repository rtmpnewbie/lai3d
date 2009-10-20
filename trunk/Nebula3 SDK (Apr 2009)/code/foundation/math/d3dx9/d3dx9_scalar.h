#pragma once
//------------------------------------------------------------------------------
/**
    @file math/d3dx9/d3dx9_scalar.h
    
    Scalar typedef and math functions for D3DX9 math functions.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/types.h"

namespace Math
{
typedef float scalar;

const scalar LN_2 = 0.693147180559945f;

//------------------------------------------------------------------------------
/**
    Return a pseudo random number between 0 and 1.
*/
__forceinline scalar 
n_rand()
{
    return scalar(rand()) / scalar(RAND_MAX);
}

//------------------------------------------------------------------------------
/**
    Return a pseudo random number between min and max.
*/
__forceinline scalar 
n_rand(scalar min, scalar max)
{
	scalar unit = scalar(rand()) / RAND_MAX;
	scalar diff = max - min;
	return min + unit * diff;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_sin(scalar x)
{
    return sinf(x);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_cos(scalar x)
{
    return cosf(x);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_tan(scalar x)
{
    return tanf(x);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_asin(scalar x)
{
    return asinf(x);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_acos(scalar x)
{
    return acosf(x);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_atan(scalar x)
{
    return atanf(x);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar 
n_sqrt(scalar x)
{
    return sqrtf(x);
}

//------------------------------------------------------------------------------
/**
    Chop float to int.
*/
__forceinline int 
n_fchop(scalar f)
{
    /// @todo type cast to int is slow!
    return int(f);
}

//------------------------------------------------------------------------------
/**
    Normalize an angular value into the range rad(0) to rad(360).
*/
__forceinline scalar 
n_modangle(scalar a) 
{
    // FIXME: hmm...
    const float pi2 = 2.0f * 3.14159265358f;    // 360 degree
    while(a < 0.0f)
    {
        a += pi2;
    }
    if (a >= pi2)
    {
        a = fmodf(a, pi2);
    }
    return a;
}

//------------------------------------------------------------------------------
/**
    log2() function.
*/
__forceinline scalar 
n_log2(scalar f) 
{ 
    return logf(f) / LN_2; 
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_exp(scalar x)
{
    return expf(x);
}

//------------------------------------------------------------------------------
/**
    Round float to integer.
*/
__forceinline int 
n_frnd(scalar f)
{
    return n_fchop(floorf(f + 0.5f));
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_fmod(scalar x, scalar y)
{
    return fmodf(x, y);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_pow(scalar x, scalar y)
{
    return powf(x, y);
}

} // namespace Math
//------------------------------------------------------------------------------



    