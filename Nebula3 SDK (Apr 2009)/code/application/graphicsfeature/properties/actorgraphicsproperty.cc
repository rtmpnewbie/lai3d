//------------------------------------------------------------------------------
//  actorgraphicsproperty.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "properties/actorgraphicsproperty.h"
#include "graphics/graphicsprotocol.h"
#include "graphicsfeatureunit.h"

namespace GraphicsFeature
{
__ImplementClass(GraphicsFeature::ActorGraphicsProperty, 'AGPR', GraphicsFeature::GraphicsProperty);

using namespace Util;
using namespace Graphics;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
void
ActorGraphicsProperty::SetupDefaultAttributes()
{
    GraphicsProperty::SetupDefaultAttributes();
    this->entity->AddString(Attr::AnimSet);
    this->entity->AddString(Attr::CharacterSet);
}

//------------------------------------------------------------------------------
/**
*/
void
ActorGraphicsProperty::SetupAcceptedMessages()
{
    GraphicsProperty::SetupAcceptedMessages();
    this->RegisterMessage(Graphics::AnimPlayOnce::Id);
    this->RegisterMessage(Graphics::AnimPlayLoop::Id);
    this->RegisterMessage(Graphics::AnimPlayDuration::Id);
    this->RegisterMessage(Graphics::AnimStopClip::Id);
    this->RegisterMessage(Graphics::AnimStopCategory::Id);
    this->RegisterMessage(Graphics::AnimStopAll::Id);
    this->RegisterMessage(Graphics::AnimModifyBlendWeight::Id);
    this->RegisterMessage(Graphics::AnimModifyTimeFactor::Id);
    this->RegisterMessage(Graphics::AnimGetClipDuration::Id);
    this->RegisterMessage(Graphics::ApplySkinList::Id);
    this->RegisterMessage(Graphics::ShowSkin::Id);
    this->RegisterMessage(Graphics::HideSkin::Id);
    this->RegisterMessage(Graphics::GetActiveAnimEvents::Id);
    this->RegisterMessage(Graphics::GetMaximumKeyDuration::Id);
}

//------------------------------------------------------------------------------
/**
*/
void
ActorGraphicsProperty::HandleMessage(const Ptr<Messaging::Message>& msg)
{
    n_assert(msg);
    if (msg->IsA(GraphicsEntityMessage::RTTI))
    {
        this->graphicsEntities[0]->SendMsg(msg.cast<GraphicsEntityMessage>());
    }
    else
    {
        GraphicsProperty::HandleMessage(msg);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
ActorGraphicsProperty::SetupGraphicsEntities()
{
    const String& resName = this->GetGraphicsResource();
    const Ptr<Stage>& stage = GraphicsFeatureUnit::Instance()->GetDefaultStage();
    const matrix44& worldMatrix = this->entity->GetMatrix44(Attr::Transform);
    const String& skinList = this->entity->GetString(Attr::CharacterSet);

    String resId;
    resId.Format("mdl:%s.n3", resName.AsCharPtr());

    Ptr<ModelEntity> modelEntity = ModelEntity::Create();
    modelEntity->SetResourceId(resId);
    modelEntity->SetTransform(worldMatrix);
    stage->AttachEntity(modelEntity.upcast<GraphicsEntity>());
    this->graphicsEntities.Append(modelEntity);

    Ptr<Graphics::ApplySkinList> applySkinList = Graphics::ApplySkinList::Create();
    applySkinList->SetSkinList(skinList);
    modelEntity->SendMsg(applySkinList.cast<GraphicsEntityMessage>());
}

} // namespace GraphicsFeature