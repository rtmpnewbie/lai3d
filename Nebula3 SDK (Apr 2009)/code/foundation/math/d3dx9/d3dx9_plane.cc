//------------------------------------------------------------------------------
//  plane.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "math/plane.h"
#include "math/matrix44.h"

namespace Math
{

//------------------------------------------------------------------------------
/**
*/
plane
plane::transform(const plane& p, const matrix44& m)
{
    plane res;
    D3DXPlaneTransform((D3DXPLANE*)&res, (CONST D3DXPLANE*)&p, (CONST D3DXMATRIX*)&m);
    return res;
}

} // namespace Math