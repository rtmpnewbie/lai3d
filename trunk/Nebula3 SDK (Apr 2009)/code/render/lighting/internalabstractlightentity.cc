//------------------------------------------------------------------------------
//  internalabstractlightentity.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "lighting/internalabstractlightentity.h"

namespace Lighting
{
__ImplementClass(Lighting::InternalAbstractLightEntity, 'ALIE', InternalGraphics::InternalGraphicsEntity);

using namespace Math;

//------------------------------------------------------------------------------
/**
*/
InternalAbstractLightEntity::InternalAbstractLightEntity() :
    lightType(LightType::InvalidLightType),
    color(1.0f, 1.0f, 1.0f, 1.0f),
    projMapUvOffsetAndScale(0.5f, 0.5f, 0.5f, -0.5f),
    shadowBufferUvOffsetAndScale(0.5f, 0.5f, 0.5f, -0.5f),
    castShadows(false),
    invTransform(matrix44::identity()),
    projTransform(matrix44::identity()),
    invLightProjTransform(matrix44::identity())
{
    this->SetType(InternalGraphics::InternalGraphicsEntityType::Light);
}

//------------------------------------------------------------------------------
/**
*/
InternalAbstractLightEntity::~InternalAbstractLightEntity()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
InternalAbstractLightEntity::OnTransformChanged()
{
    InternalGraphicsEntity::OnTransformChanged();

    // update inverse transform
    this->invTransform = matrix44::inverse(this->transform);
}

} // namespace Lighting
