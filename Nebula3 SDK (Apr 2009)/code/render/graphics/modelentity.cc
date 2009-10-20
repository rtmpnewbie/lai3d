//------------------------------------------------------------------------------
//  modelentity.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/modelentity.h"
#include "graphics/stage.h"

namespace Graphics
{
__ImplementClass(Graphics::ModelEntity, 'MDLE', Graphics::GraphicsEntity);

//------------------------------------------------------------------------------
/**
*/
ModelEntity::ModelEntity():
    clipStatus(Math::ClipStatus::Invalid)
{
    this->SetType(GraphicsEntityType::Model);
}

//------------------------------------------------------------------------------
/**
*/
ModelEntity::~ModelEntity()
{
    this->updateMsg = 0;
}

//------------------------------------------------------------------------------
/**
    Setup the server-side ModelEntity.
*/
void
ModelEntity::Setup(const Ptr<Stage>& stage_)
{
    n_assert(this->resId.Value().IsValid());

    // call parent class
    GraphicsEntity::Setup(stage_);

    // setup and send an entity creation message
    Ptr<CreateModelEntity> msg = CreateModelEntity::Create();
    msg->SetStageName(this->stage->GetName());
    msg->SetTransform(this->transform);
    msg->SetVisible(this->isVisible);
    msg->SetResourceId(this->resId);
    msg->SetAnimationDrivenJointName(this->animationJointName);
    this->SendCreateMsg(msg.cast<CreateGraphicsEntity>());
}

//------------------------------------------------------------------------------
/**
*/
void
ModelEntity::SetShaderVariable(const Util::String& nodeName, const Util::String& variableSemantic, const Util::Variant& value)
{
    Ptr<UpdateInstanceShaderVariable> msg = UpdateInstanceShaderVariable::Create();
    msg->SetNodeInstanceName(nodeName);
    msg->SetSemantic(variableSemantic);
    msg->SetValue(value);
    this->SendMsg(msg.cast<GraphicsEntityMessage>());
}

//------------------------------------------------------------------------------
/**
*/
void 
ModelEntity::UpdateAnimationDrivenTransform()
{
    n_assert(this->animationJointName.IsValid());
    if (this->updateMsg.isvalid())
    {
        if (this->updateMsg->Handled())
        {
            this->animationDrivenMovement = this->updateMsg->GetMovement();
            this->clipStatus = this->updateMsg->GetClipStatus();
            this->updateMsg->SetHandled(false);

            // send our msg to graphics thread to get transform in the next frame
            this->SendMsg(this->updateMsg.cast<GraphicsEntityMessage>());
        }
        
    }
    else
    {
        this->updateMsg = GetAnimationDrivenVector::Create();
        // send our msg to graphics thread to get transform in the next frame
        this->SendMsg(this->updateMsg.cast<GraphicsEntityMessage>());
    }
}

} // namespace Graphics