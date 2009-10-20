//------------------------------------------------------------------------------
//  spotlightentity.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/spotlightentity.h"
#include "graphics/stage.h"

namespace Graphics
{
__ImplementClass(Graphics::SpotLightEntity, 'SLEN', Graphics::AbstractLightEntity);

//------------------------------------------------------------------------------
/**
*/
SpotLightEntity::SpotLightEntity()
{
    this->SetLightType(Lighting::LightType::Spot);
}

//------------------------------------------------------------------------------
/**
*/
void
SpotLightEntity::Setup(const Ptr<Stage>& stage_)
{
    // call parent class
    AbstractLightEntity::Setup(stage_);

    // setup and send entity creation message
    Ptr<CreateSpotLightEntity> msg = CreateSpotLightEntity::Create();
    msg->SetStageName(this->stage->GetName());
    msg->SetTransform(this->transform);
    msg->SetVisible(this->isVisible);
    msg->SetLightColor(this->color);
    msg->SetProjMapUvOffsetAndScale(this->projMapUvOffsetAndScale);
    msg->SetCastShadows(this->castShadows);
    this->SendCreateMsg(msg.cast<CreateGraphicsEntity>());
}

} // namespace Graphics
