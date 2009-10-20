//------------------------------------------------------------------------------
//  globallightentity.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "lighting/internalgloballightentity.h"

namespace Lighting
{
__ImplementClass(Lighting::InternalGlobalLightEntity, 'GLBE', Lighting::InternalAbstractLightEntity);

using namespace Math;

//------------------------------------------------------------------------------
/**
*/
InternalGlobalLightEntity::InternalGlobalLightEntity() :
    backLightColor(0.0f, 0.0f, 0.0f, 0.0f),
    ambientLightColor(0.0f, 0.0f, 0.0f, 0.0f),
    lightDir(0.0f, 0.0f, -1.0f)
{
    this->SetLightType(LightType::Global);
}

//------------------------------------------------------------------------------
/**
*/
ClipStatus::Type
InternalGlobalLightEntity::ComputeClipStatus(const Math::bbox& box)
{
    // since we are essentially a directional light, everything is visible,
    // we depend on the visibility detection code in the Cell class to
    // only create light links for objects that are actually visible
    return ClipStatus::Inside;
}

//------------------------------------------------------------------------------
/**
*/
void
InternalGlobalLightEntity::OnTransformChanged()
{
    InternalAbstractLightEntity::OnTransformChanged();

    // extract the light's direction from the transformation matrix
    this->lightDir = float4::normalize(this->transform.getrow2());

    // extend transformation to a very big size, since we need to be visible
    // from everywhere in the stage
    const float size = 100000.0f;
    this->transform.set_xaxis(float4::normalize(this->transform.get_xaxis()) * size);
    this->transform.set_yaxis(float4::normalize(this->transform.get_yaxis()) * size);
    this->transform.set_zaxis(float4::normalize(this->transform.get_zaxis()) * size);
}

} // namespace Lighting
