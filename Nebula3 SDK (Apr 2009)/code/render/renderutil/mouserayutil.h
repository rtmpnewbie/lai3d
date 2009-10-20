#pragma once
//------------------------------------------------------------------------------
/**
    @class RenderUtil::MouseRayUtil

    Helper class to compute a world-space ray from mouse coords.

    (C) 2009 Radon Labs GmbH
*/
#include "math/float2.h"
#include "math/matrix44.h"
#include "math/line.h"

//------------------------------------------------------------------------------
namespace RenderUtil
{
class MouseRayUtil
{
public:
    /// compute world-space ray from mouse position
    static Math::line ComputeWorldMouseRay(const Math::float2& mousePos, float length, const Math::matrix44& viewMatrix, const Math::matrix44& invProjMatrix, float nearPlane);
};

} // namespace RenderUtil
//------------------------------------------------------------------------------

