//------------------------------------------------------------------------------
//  float4.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "math/float4.h"
#include "math/matrix44.h"

namespace Math
{

//------------------------------------------------------------------------------
/**
*/
float4
float4::transform(const float4& v, const matrix44& m)
{
    float4 res;
    D3DXVec4Transform((D3DXVECTOR4*)&res, (CONST D3DXVECTOR4*)&v, (CONST D3DXMATRIX*)&m);
    return res;
}

//------------------------------------------------------------------------------
/**
*/
float4
float4::clamp(const float4& vClamp, const float4& vMin, const float4& vMax)
{
    return minimize(maximize(vClamp, vMin), vMax);
}

//------------------------------------------------------------------------------
/**
*/
scalar 
float4::angle(const float4& v0, const float4& v1)
{
    float4 v0n = float4::normalize(v0);
    float4 v1n = float4::normalize(v1);
    float a = n_acos(float4::dot3(v0n, v1n));
    return a;
}
} // namespace Math