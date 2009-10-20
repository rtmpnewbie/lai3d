//------------------------------------------------------------------------------
//  mouserayutil.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "renderutil/mouserayutil.h"

namespace RenderUtil
{
using namespace Math;

//------------------------------------------------------------------------------
/**
    Utility function which computes a ray in world space between the eye
    and the current mouse position on the near plane.
*/
line
MouseRayUtil::ComputeWorldMouseRay(const float2& mousePos, float length, const matrix44& viewMatrix, const matrix44& invProjMatrix, float nearPlane)
{
    // Compute mouse position in world coordinates.
    point screenCoord3D((mousePos.x() - 0.5f) * 2.0f, (mousePos.y() - 0.5f) * 2.0f, 1.0f);
    vector viewCoord = vector::transform(screenCoord3D, invProjMatrix);
    point localMousePos = viewCoord * nearPlane * 1.1f;
    localMousePos.y() = -1 * localMousePos.y();
    point worldMousePos = vector::transform(localMousePos, viewMatrix);
    vector worldMouseDir = worldMousePos - point(viewMatrix.get_position());
    worldMouseDir = vector::normalize(worldMouseDir);
    worldMouseDir *= length;

    return line(worldMousePos, worldMousePos + worldMouseDir);
}

} // namespace RenderUtil
