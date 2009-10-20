//------------------------------------------------------------------------------
//  properties/lightproperty.cc
//  (C) 2005 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "properties/lightproperty.h"
#include "core/factory.h"
#include "game/entity.h"
#include "basegameattr/basegameattributes.h"
#include "graphics/graphicsserver.h"
#include "graphics/stage.h"
#include "basegameprotocol.h"
#include "graphicsfeature/graphicsfeatureunit.h"
#include "graphicsfeature/graphicsfeatureprotocol.h"
#include "graphics/spotlightentity.h"
#include "graphics/globallightentity.h"
#include "graphics/pointlightentity.h"

namespace Attr
{
    DefineIntWithDefault(LightType, 'LITY', ReadOnly, true);
    DefineFloat4WithDefault(LightColor, 'LCOL', ReadWrite, Math::float4(1.0f, 1.0f, 1.0f, 1.0f));
    DefineFloat4WithDefault(LightOppositeColor, 'LOCL', ReadWrite, Math::float4(0.1f, 0.1f, 0.2f, 1.0f));
    DefineFloatWithDefault(LightIntensity, 'LINT', ReadWrite, 1.0f);
    DefineFloatWithDefault(LightRange, 'LRAN', ReadWrite, 10.0f);    
    DefineFloat4WithDefault(LightAmbient, 'LAMB', ReadWrite, Math::float4(0.1, 0.1f, 0.1f, 1.0f));
    DefineBoolWithDefault(LightCastShadows, 'LCAS', ReadWrite, true);    
    DefineBoolWithDefault(LightFlickerEnable, 'LFLE', ReadWrite, false);
    DefineFloatWithDefault(LightFlickerFrequency, 'LFLF', ReadWrite, 5.0f);
    DefineFloatWithDefault(LightFlickerIntensity, 'LFLI', ReadWrite, 0.5f);
	DefineFloatWithDefault(LightFlickerPosition, 'LFLP', ReadWrite, 0.25f);
};

namespace GraphicsFeature
{
__ImplementClass(GraphicsFeature::LightProperty, 'LIPR', Game::Property);

using namespace Game;
using namespace Messaging;
using namespace Graphics;
using namespace Math;
using namespace BaseGameFeature;
using namespace Lighting;

//------------------------------------------------------------------------------
/**
*/
void
LightProperty::SetupDefaultAttributes()
{
    Property::SetupDefaultAttributes();
    this->entity->AddMatrix44(Attr::Transform);
    this->entity->AddInt(Attr::LightType);
    this->entity->AddFloat4(Attr::LightColor);
    this->entity->AddFloat4(Attr::LightOppositeColor);
    this->entity->AddFloat(Attr::LightRange);
    this->entity->AddFloat4(Attr::LightAmbient);    
    this->entity->AddBool(Attr::LightCastShadows);
    this->entity->AddBool(Attr::LightFlickerEnable);
    this->entity->AddFloat(Attr::LightFlickerFrequency);
    this->entity->AddFloat(Attr::LightFlickerIntensity);
    this->entity->AddFloat(Attr::LightFlickerPosition);
}

//------------------------------------------------------------------------------
/**
*/
void
LightProperty::SetupCallbacks()
{
    this->entity->RegisterPropertyCallback(this, Render);
}

//------------------------------------------------------------------------------
/**
*/
void
LightProperty::OnActivate()
{
    Property::OnActivate();

    // create and setup graphics light entity
    int lightType = this->entity->GetInt(Attr::LightType);    
#if (1)//__NEBULA2_EXPORT__
    // map lightcode from n2 to n3
    // nebula2:          in nebula3:
    // Point = 0,        Global = 0,
    // Directional,      Spot,
    // Spot,             Point,

    LightType::Code LightCodesSubstitute[LightType::NumLightTypes]; 
    LightCodesSubstitute[0] = LightType::Point;
	LightCodesSubstitute[1] = LightType::Global;
    LightCodesSubstitute[2] = LightType::Spot;	
    lightType = LightCodesSubstitute[lightType];
#endif
    
    switch (lightType)
    {
        case LightType::Global:
            this->lightEntity = GlobalLightEntity::Create();
            // broadcast global light entity to posteffectmanager
            if(PostEffect::PostEffectManager::HasInstance())
            {
                PostEffect::PostEffectManager::Instance()->SetGlobalLightEntity(this->lightEntity.cast<Graphics::GlobalLightEntity>());
            }
            break;
        case LightType::Spot:
            {
                this->lightEntity = SpotLightEntity::Create();
                // spot light is rotated 
                matrix44 trans = this->GetEntity()->GetMatrix44(Attr::Transform);
                trans.set_zaxis(trans.get_zaxis() * -1);
                this->GetEntity()->SetMatrix44(Attr::Transform, trans);
                break;
            }
        case LightType::Point:
            this->lightEntity = PointLightEntity::Create();
            break;
    }	
    this->UpdateLightFromAttributes();    

    // register with graphics server
    Graphics::Stage* stage = GraphicsFeatureUnit::Instance()->GetDefaultStage();
    n_assert(stage);
    stage->AttachEntity(this->lightEntity.upcast<Graphics::GraphicsEntity>());

    // setup the light flicker utility
    this->lightFlickerUtil.SetLightEntity(this->lightEntity);
    this->UpdateLightFlickerUtilFromAttributes();
}

//------------------------------------------------------------------------------
/**
*/
void
LightProperty::OnDeactivate()
{
    // cleanup graphics light property
    Graphics::Stage* stage = GraphicsFeatureUnit::Instance()->GetDefaultStage();
    stage->RemoveEntity(this->lightEntity.upcast<Graphics::GraphicsEntity>());
    this->lightEntity = 0;
    
    Property::OnDeactivate();
}

//------------------------------------------------------------------------------
/**
*/
void
LightProperty::SetupAcceptedMessages()
{
    this->RegisterMessage(BaseGameFeature::UpdateTransform::Id);
    this->RegisterMessage(SetVisibleMsg::Id);
    this->RegisterMessage(GraphicsFeature::GetLightEntity::Id);
    //this->RegisterMessage(AttributesUpdated::Id);
    Property::SetupAcceptedMessages();
}

//------------------------------------------------------------------------------
/**
*/
void
LightProperty::HandleMessage(const Ptr<Message>& msg)
{
    n_assert(msg);
    if (msg->CheckId(BaseGameFeature::UpdateTransform::Id))
    {
        this->UpdateLightTransform(msg.cast<BaseGameFeature::UpdateTransform>()->GetMatrix());
    }
    else if (msg->CheckId(SetVisibleMsg::Id))
    {
        this->lightEntity->SetVisible(msg.cast<SetVisibleMsg>()->GetVisible());
    }
    else if (msg->CheckId(GraphicsFeature::GetLightEntity::Id))
    {
        msg.cast<GraphicsFeature::GetLightEntity>()->SetEntity(this->lightEntity);
    }
    /*else if (msg->CheckId(AttributesUpdated::Id))
    {
        // this is usually coming only from the level editor
        this->UpdateLightFlickerUtilFromAttributes();
        this->UpdateLightFromAttributes();
    }
    */
    else
    {
        Property::HandleMessage(msg);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
LightProperty::OnRender()
{
    // need to update the light flickering utility
    this->lightFlickerUtil.Update();
}

//------------------------------------------------------------------------------
/**
    Update the light flicker util object from attributes.
*/
void
LightProperty::UpdateLightFlickerUtilFromAttributes()
{
    this->lightFlickerUtil.SetEnabled(this->entity->GetBool(Attr::LightFlickerEnable));
    this->lightFlickerUtil.SetFrequency(this->entity->GetFloat(Attr::LightFlickerFrequency));
    this->lightFlickerUtil.SetIntensityAmplitude(this->entity->GetFloat(Attr::LightFlickerIntensity));
    this->lightFlickerUtil.SetPositionAmplitude(this->entity->GetFloat(Attr::LightFlickerPosition));
}

//------------------------------------------------------------------------------
/**
    Update embedded light entity from attributes.
*/
void
LightProperty::UpdateLightFromAttributes()
{    
    // set generic light attributes
    this->lightEntity->SetColor(this->entity->GetFloat4(Attr::LightColor) * this->entity->GetFloat(Attr::LightIntensity));    
    this->lightEntity->SetCastShadows(this->entity->GetBool(Attr::LightCastShadows));  

    if (this->lightEntity->IsInstanceOf(GlobalLightEntity::RTTI))
    {
        // set global light attributes
        const Ptr<GlobalLightEntity>& globalLight = this->lightEntity.cast<GlobalLightEntity>();
        globalLight->SetBackLightColor(this->entity->GetFloat4(Attr::LightOppositeColor));
    }
    this->UpdateLightTransform(this->entity->GetMatrix44(Attr::Transform));
}

//------------------------------------------------------------------------------
/**
    Update light entity transform from provided transform matrix.
*/
void
LightProperty::UpdateLightTransform(const matrix44& transform)
{
    matrix44 m = transform;
    if (this->lightEntity->IsInstanceOf(GlobalLightEntity::RTTI))
    {
        // set global light transform
        m.set_xaxis(-m.get_xaxis());
        m.set_zaxis(-m.get_zaxis());
        this->lightEntity.cast<GlobalLightEntity>()->SetTransform(m);
    }
    else
    {
        // set spot and point light transform
        float lightRange = this->entity->GetFloat(Attr::LightRange);
        m.set_xaxis(-m.get_xaxis() * lightRange);
        m.set_yaxis(m.get_yaxis() * lightRange);
        m.set_zaxis(-m.get_zaxis() * lightRange);
        this->lightEntity->SetTransform(m);
    }
}

} // namespace GraphicsFeature
