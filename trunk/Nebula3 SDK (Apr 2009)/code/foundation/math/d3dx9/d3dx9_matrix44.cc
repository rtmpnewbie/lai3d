//------------------------------------------------------------------------------
//  matrix44.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "math/matrix44.h"
#include "math/plane.h"
#include "math/quaternion.h"

namespace Math
{

//------------------------------------------------------------------------------
/**
*/
matrix44
matrix44::reflect(const plane& p)
{
    matrix44 res;
    D3DXMatrixReflect((D3DXMATRIX*)&res, (CONST D3DXPLANE*)&p);
    return res;
}

//------------------------------------------------------------------------------
/**
*/
void
matrix44::decompose(float4& outScale, quaternion& outRotation, float4& outTranslation) const
{
    outScale.set(0.0f, 0.0f, 0.0f, 0.0f);
    outTranslation.set(0.0f, 0.0f, 0.0f, 0.0f);
    D3DXMatrixDecompose((D3DXVECTOR3*)&outScale, (D3DXQUATERNION*)&outRotation, (D3DXVECTOR3*)&outTranslation, (CONST D3DXMATRIX*)this);
}

//------------------------------------------------------------------------------
/**
*/
matrix44
matrix44::affinetransformation(scalar scaling, const float4& rotationCenter, const quaternion& rotation, const float4& translation)
{
    matrix44 res;
    D3DXMatrixAffineTransformation((D3DXMATRIX*)&res, scaling, (CONST D3DXVECTOR3*)&rotationCenter, (CONST D3DXQUATERNION*)&rotation, (CONST D3DXVECTOR3*)&translation);
    return res;
}

//------------------------------------------------------------------------------
/**
*/
matrix44
matrix44::rotationquaternion(const quaternion& q)
{
    matrix44 res;
    D3DXMatrixRotationQuaternion((D3DXMATRIX*)&res, (CONST D3DXQUATERNION*)&q);
    return res;
}

//------------------------------------------------------------------------------
/**
*/
matrix44
matrix44::transformation(const float4& scalingCenter, const quaternion& scalingRotation, const float4& scaling, const float4& rotationCenter, const quaternion& rotation, const float4& translation)
{
    matrix44 res;
    D3DXMatrixTransformation((D3DXMATRIX*)&res, 
                             (CONST D3DXVECTOR3*)&scalingCenter, 
                             (CONST D3DXQUATERNION*)&scalingRotation, 
                             (CONST D3DXVECTOR3*)&scaling, 
                             (CONST D3DXVECTOR3*)&rotationCenter, 
                             (CONST D3DXQUATERNION*)&rotation, 
                             (CONST D3DXVECTOR3*)&translation);
    return res;
}

} // namespace Math
