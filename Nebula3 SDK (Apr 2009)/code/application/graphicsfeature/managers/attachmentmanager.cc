//------------------------------------------------------------------------------
//  attachmentmanager.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphicsfeature/managers//attachmentmanager.h"
#include "graphics/graphicsprotocol.h"
#include "graphicsfeature/graphicsfeatureprotocol.h"
#include "graphics/graphicsinterface.h"
#include "game/entity.h"
#include "basegamefeature/managers/entitymanager.h"
#include "basegamefeature/basegameattr/basegameattributes.h"
#include "basegamefeature/basegametiming/gametimesource.h"
#include "graphicsfeature/graphicsfeatureunit.h"

namespace GraphicsFeature
{
__ImplementClass(AttachmentManager, 'ATMA', Game::Manager);
__ImplementSingleton(AttachmentManager);

using namespace Util;
using namespace Math;

// FIXME: why does the AttachmentManager know about InternalGraphics???
using namespace InternalGraphics;

//------------------------------------------------------------------------------
/**
*/
AttachmentManager::AttachmentManager()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
AttachmentManager::~AttachmentManager()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
AttachmentManager::OnDeactivate()
{
    // clean up
    //pendingAttachments.Clear();
    //attachedAttachments.Clear();
}

//------------------------------------------------------------------------------
/**
*/
void
AttachmentManager::OnFrame()
{
    // at first check pending resources
    this->UpdatePendingAttachments();

    // check temporary attachments
    this->CheckTemporaryAttachments();
}

//------------------------------------------------------------------------------
/**
*/
void
AttachmentManager::AddGraphicsAttachment( const Util::StringAtom& jointName,
                                          const Ptr<Game::Entity>& baseEntityPtr,
                                          const Util::StringAtom& entityToAttachResId,
                                          const Math::matrix44& offset,
                                          bool keepLocal,
                                          InternalGraphics::AttachmentServer::AttachmentRotation rotation)
{
    this->AddGraphicsAttachmentTemporary( jointName,
                                          baseEntityPtr,
                                          entityToAttachResId,
                                          offset,
                                          keepLocal,
                                          rotation,
                                          -1);
}

//------------------------------------------------------------------------------
/**
*/
void
AttachmentManager::AddGraphicsAttachment( const Util::StringAtom& jointName, 
                                          const Util::StringAtom& baseEntityId, 
                                          const Util::StringAtom& entityToAttachResId, 
                                          const Math::matrix44& offset,
                                          bool keepLocal,
                                          InternalGraphics::AttachmentServer::AttachmentRotation rotation)
{        
    // find base entity
    this->AddGraphicsAttachmentTemporary( jointName,
        baseEntityId,
        entityToAttachResId,
        offset,
        keepLocal,
        rotation,
        -1);
}

//------------------------------------------------------------------------------
/**
*/
void
AttachmentManager::AddGraphicsAttachmentTemporary( const Util::StringAtom& jointName, 
                                                   const Util::StringAtom& baseEntityId, 
                                                   const Util::StringAtom& entityToAttachResId, 
                                                   const Math::matrix44& offset,
                                                   bool keepLocal,
                                                   InternalGraphics::AttachmentServer::AttachmentRotation rotation,
                                                   Timing::Time duration)
{        
    // find base entity
    const Ptr<Game::Entity> base = BaseGameFeature::EntityManager::Instance()->GetEntityByAttr(Attr::Attribute(Attr::Id, baseEntityId.Value()));
    n_assert(base.isvalid());

    this->AddGraphicsAttachmentTemporary( jointName, 
        base, 
        entityToAttachResId, 
        offset, 
        keepLocal, 
        rotation, 
        duration);
}

//------------------------------------------------------------------------------
/**
*/
void
AttachmentManager::AddGraphicsAttachmentTemporary( const Util::StringAtom& jointName, 
                                                   const Ptr<Game::Entity>& baseEntityPtr, 
                                                   const Util::StringAtom& entityToAttachResId, 
                                                   const Math::matrix44& offset, 
                                                   bool keepLocal, 
                                                   InternalGraphics::AttachmentServer::AttachmentRotation rotation,
                                                   Timing::Time duration)
{
    // first attach as pending
    PendingAttachment newOne;
    newOne.joint = jointName;
    newOne.offset = offset;
    newOne.keepLocal = keepLocal;
    newOne.rotation = rotation;
    newOne.duration = duration;

    Ptr<GetGraphicsEntities> getGfx1 = GetGraphicsEntities::Create();
    baseEntityPtr->SendSync(getGfx1.cast<Messaging::Message>());
    newOne.baseEntity = getGfx1->GetEntities()[0];

    // transform
    Math::matrix44 t = Math::matrix44::multiply(newOne.baseEntity->GetTransform(), offset);

    // create entity
    Ptr<Graphics::ModelEntity> entity = Graphics::ModelEntity::Create();
    entity->SetResourceId(entityToAttachResId);
    entity->SetTransform(t);
    entity->SetVisible(true);

    const Ptr<Graphics::Stage>& stage = GraphicsFeatureUnit::Instance()->GetDefaultStage();
    stage->AttachEntity(entity.cast<Graphics::GraphicsEntity>());

    newOne.attachedEntity = entity.cast<Graphics::GraphicsEntity>();

    this->pendingAttachments.Append(newOne);
}

//------------------------------------------------------------------------------
/**
*/
void
AttachmentManager::UpdatePendingAttachments()
{
    // check all pending
    IndexT index;
    for(index = 0; index < this->pendingAttachments.Size(); index++)
    {
        PendingAttachment& pending = this->pendingAttachments[index];
        if(pending.attachedEntity->IsEntityHandleValid() && pending.baseEntity->IsEntityHandleValid())
        {
            // send 
            this->SendFinalAttachment( pending );

            // start pending time
            pending.startTime = BaseGameFeature::GameTimeSource::Instance()->GetTime();

            // attach to other array
            this->attachedAttachments.Append(pending);

            // and delete from pending array
            this->pendingAttachments.EraseIndex(index);
            index--;
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
AttachmentManager::CheckTemporaryAttachments()
{
    // get time
    Timing::Time time = BaseGameFeature::GameTimeSource::Instance()->GetTime();

    // check all
    IndexT index;
    for (index = 0; index < this->attachedAttachments.Size(); index++)
    {
        const PendingAttachment& attachment = this->attachedAttachments[index];

        // skip if endless
        if (-1 == attachment.duration)
        {
            continue;
        }

        // if time ran out
        if (attachment.startTime + attachment.duration < time)
        {
            // remove that shit
            this->SendFinalDetachment(attachment);

            // check if remove from stage
            GraphicsFeatureUnit::Instance()->GetDefaultStage()->RemoveEntity(attachment.attachedEntity);
            
            // remove from array
            this->attachedAttachments.EraseIndex(index);
            index--;
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
AttachmentManager::SendFinalAttachment(const PendingAttachment& attachemnt)
{
    Ptr<Graphics::BindAttachment> msg = Graphics::BindAttachment::Create();
    msg->SetJoint(attachemnt.joint.Value());
    msg->SetOffset(attachemnt.offset);
    msg->SetEntityToAttachHandle(attachemnt.attachedEntity->GetEntityHandle());
    msg->SetBaseEntityHandle(attachemnt.baseEntity->GetEntityHandle());
    msg->SetClearType(InternalGraphics::AttachmentServer::ClearAllIdenticalOnJoint);
    msg->SetKeepLocal(attachemnt.keepLocal);
    msg->SetRotation(attachemnt.rotation);
    Graphics::GraphicsInterface::Instance()->SendBatched(msg.cast<Messaging::Message>());
}

//------------------------------------------------------------------------------
/**
*/
void
AttachmentManager::SendFinalDetachment(const PendingAttachment &detachment)
{
    Ptr<Graphics::UnbindAttachment> msg = Graphics::UnbindAttachment::Create();
    msg->SetBaseEntityHandle(detachment.baseEntity->GetEntityHandle());
    msg->SetEntityToAttachHandle(detachment.attachedEntity->GetEntityHandle());
    msg->SetJoint(detachment.joint.Value());
    msg->SetClearType(InternalGraphics::AttachmentServer::ClearAllIdenticalOnJoint);
    Graphics::GraphicsInterface::Instance()->SendBatched(msg.cast<Messaging::Message>());
}
} // namespace GraphicsFeature
