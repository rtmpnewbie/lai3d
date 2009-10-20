//------------------------------------------------------------------------------
//  internalspotlightentity.h
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "lighting/internalspotlightentity.h"

namespace Lighting
{
__ImplementClass(Lighting::InternalSpotLightEntity, 'SPLE', Lighting::InternalAbstractLightEntity);

using namespace Math;

//------------------------------------------------------------------------------
/**
*/
InternalSpotLightEntity::InternalSpotLightEntity()
{
    this->SetLightType(LightType::Spot);
}

//------------------------------------------------------------------------------
/**
*/
Math::ClipStatus::Type
InternalSpotLightEntity::ComputeClipStatus(const Math::bbox& box)
{
    // compute spot light clip status
    ClipStatus::Type clipStatus = box.clipstatus(this->invLightProjTransform);
    return clipStatus;
}

//------------------------------------------------------------------------------
/**
*/
void
InternalSpotLightEntity::OnTransformChanged()
{
    InternalAbstractLightEntity::OnTransformChanged();

    // compute the spot light's perspective projection matrix from
    // its transform matrix (spot direction is along -z, and goes
    // throught the rectangle formed by the x and y components
    // at the end of -z
    float widthAtFarZ  = this->transform.getrow0().length() * 2.0f;
    float heightAtFarZ = this->transform.getrow1().length() * 2.0f;
    float nearZ = 0.005f; // put the near plane at 0.05cm
    float farZ = this->transform.getrow2().length();    
    n_assert(farZ > 0.0f);
    if (nearZ >= farZ)
    {
        nearZ = farZ / 2.0f;
    }
    float widthAtNearZ  = (widthAtFarZ / farZ) * nearZ;
    float heightAtNearZ = (heightAtFarZ / farZ) * nearZ;
    this->projTransform = matrix44::persprh(widthAtNearZ, heightAtNearZ, nearZ, farZ);

    // compute the invLightProj matrix which transforms from world space to light space
    this->invLightProjTransform = matrix44::multiply(this->invTransform, this->projTransform);
}

} // namespace Lighting
