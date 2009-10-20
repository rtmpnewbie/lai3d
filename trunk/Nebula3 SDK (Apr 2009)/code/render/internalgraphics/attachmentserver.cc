//------------------------------------------------------------------------------
//  attachmentserver.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalgraphics/attachmentserver.h"
#include "graphics/graphicsprotocol.h"
#include "characters/characterjointinstance.h"
#include "internalgraphics/internalgraphicsentity.h"
#include "internalgraphics/internalmodelentity.h"
#include "internalgraphics/internalgraphicsserver.h"
#include "internalgraphics/internalview.h"
#include "characters/character.h"
#include "io/excelxmlreader.h"
#include "io/ioserver.h"
#include "internalgraphics/internalstage.h"
#include "animation/animeventinfo.h"

namespace InternalGraphics
{
__ImplementClass(InternalGraphics::AttachmentServer, 'ATSV', Core::RefCounted);
__ImplementSingleton(InternalGraphics::AttachmentServer);

using namespace Util;
using namespace Math;
using namespace Characters;
using namespace Animation;

//------------------------------------------------------------------------------
/**
*/
AttachmentServer::AttachmentServer() : isOpen(false)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
AttachmentServer::~AttachmentServer()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
bool
AttachmentServer::Open()
{
    n_assert(!this->IsOpen());    
    this->isOpen = true;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
AttachmentServer::Close()
{
    n_assert(this->IsOpen());
    // clean up
    this->attachments.Clear();

    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
void
AttachmentServer::AttachEntityTemporary(const Math::matrix44& offset, 
                                        const ClearType& clearType, 
                                        const Util::String& joint, 
                                        const Ptr<InternalGraphicsEntity>& entityToAttach,
                                        const Ptr<InternalModelEntity>& baseEntity, 
                                        bool keepLocal,
                                        Timing::Time duration,
                                        AttachmentServer::AttachmentRotation rotation)
{
    // first detach 
    this->DetachEntity(clearType, joint, entityToAttach, baseEntity);

    // create new
    InternalAttachment newOne;
    newOne.attachedEntity = entityToAttach;
    n_assert(baseEntity->GetType() == InternalGraphicsEntityType::Model);
    newOne.baseEntity = baseEntity;
    newOne.jointIndex = newOne.baseEntity->GetCharacter()->Skeleton().GetJointIndexByName(joint);
    newOne.offset = offset;
    newOne.startTime = this->time;
    newOne.duration = duration;
    newOne.keepLocal = keepLocal;
    newOne.initPosSet = false;
    newOne.rotation = rotation;

    // and attach
    this->attachments.Append(newOne);
}

//------------------------------------------------------------------------------
/**
*/
void
AttachmentServer::AttachEntity(const Math::matrix44& offset,
                               const ClearType& clearType,
                               const Util::String& joint,
                               const Ptr<InternalGraphicsEntity>& entityToAttach,
                               const Ptr<InternalModelEntity>& baseEntity,
                               bool keepLocal,
                               AttachmentServer::AttachmentRotation rotation)
{
    this->AttachEntityTemporary(offset,
                                clearType,
                                joint,
                                entityToAttach,
                                baseEntity,
                                keepLocal,
                                -1,
                                rotation);
}

//------------------------------------------------------------------------------
/**
*/
void
AttachmentServer::DetachEntity(const ClearType& clearType,
                               const Util::String& joint,
                               const Ptr<InternalGraphicsEntity>& entityToAttach,
                               const Ptr<InternalModelEntity>& baseEntity)
{
    // assert, that stage from entity to attach is the same as that from base entity
    // -> so we can say, that entity to attach is the same stage like the already attached one,
    //    cause we always attach the entity to the stage of the base entities
    Ptr<InternalStage> stage; 
    if (entityToAttach.isvalid())
    {
        stage = entityToAttach->GetStage();
    }
    else
    {
        stage = InternalGraphicsServer::Instance()->GetDefaultView()->GetStage();        
    }

    // TODO.. maybe optimize this method, there are 4 loops lookin' very redundant...
    //        thought about pulling' the switch case into one big loop... but i think
    //        this way its a little bit faster.... what do you think?!
    IndexT index;
    switch (clearType)
    {
    case ClearAll:
        // remove from stage...
        for (index = 0; index < this->attachments.Size(); index++)
        {
            const Ptr<InternalStage>& stage = this->attachments[index].attachedEntity->GetStage();
            if (stage.isvalid())
            {
                stage->RemoveEntity(this->attachments[index].attachedEntity);
            }
        }
        this->attachments.Clear();
        break;

    case ClearAllIdenticalOnEntity:
        for(index = 0; index < this->attachments.Size(); index++)
        {
            if(this->attachments[index].baseEntity == baseEntity)
            {
                if(this->attachments[index].attachedEntity == entityToAttach)
                {
                    // remove from stage...
                    stage->RemoveEntity(this->attachments[index].attachedEntity);

                    // ... and from this server
                    this->attachments.EraseIndex(index);
                    index--;                    
                }
            }
        }
        break;

    case ClearAllOnEntity:
        for(index = 0; index < this->attachments.Size(); index++)
        {
            if(this->attachments[index].baseEntity == baseEntity)
            {
                if (stage.isvalid())
                {
                    // remove from stage...
                    stage->RemoveEntity(this->attachments[index].attachedEntity);
                }

                // ... and from this server
                this->attachments.EraseIndex(index);
                index--;
            }
        }
        break;

    case ClearAllIdenticalOnJoint:
        for(index = 0; index < this->attachments.Size(); index++)
        {
            if(this->attachments[index].baseEntity == baseEntity)
            {
                int jointIndex = this->attachments[index].baseEntity->GetCharacter()->Skeleton().GetJointIndexByName(joint);
                if(this->attachments[index].jointIndex == jointIndex && 
                   this->attachments[index].attachedEntity == entityToAttach)
                {
                    if (stage.isvalid())
                    {
                        // remove from stage...
                        stage->RemoveEntity(this->attachments[index].attachedEntity);
                    }

                    // ... and from this server
                    this->attachments.EraseIndex(index);
                    index--;
                }
            }
        }
        break;

    case ClearAllOnJoint:
        for(index = 0; index < this->attachments.Size(); index++)
        {
            if(this->attachments[index].baseEntity == baseEntity)
            {
                int jointIndex = this->attachments[index].baseEntity->GetCharacter()->Skeleton().GetJointIndexByName(joint);
                if(this->attachments[index].jointIndex == jointIndex)
                {
                    if (stage.isvalid())
                    {
                        // remove from stage...
                        stage->RemoveEntity(this->attachments[index].attachedEntity);
                    }

                    // ... and from this server
                    this->attachments.EraseIndex(index);
                    index--;
                }
            }
        }
        break;

    case ClearNone:
    default:
        break;
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
AttachmentServer::DetachEntity(const Ptr<InternalGraphics::InternalGraphicsEntity>& entity)
{
    // assert, that stage from entity to attach is the same as that from base entity
    // -> so we can say, that entity to attach is the same stage like the allready attached one,
    //    cause we always attach the entity to the stage of the base enties    
    Ptr<InternalStage> stage; 
    if (entity.isvalid())
    {
        stage = entity->GetStage();
    }
    else
    {
        stage = InternalGraphicsServer::Instance()->GetDefaultView()->GetStage();        
    }

    // search
    IndexT index;
    for (index = 0; index < this->attachments.Size(); index++)
    {
        if (entity == this->attachments[index].attachedEntity)
        {
            // stage could be invalid on shutdown
            if (stage.isvalid())
            {
                // remove from stage...                    
                stage->RemoveEntity(this->attachments[index].attachedEntity);
            }          

            // ... and from this server
            this->attachments.EraseIndex(index);
            return;
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
AttachmentServer::OnFrame(Timing::Time time)
{
    // first set time
    this->time = time;

    // a correctional matrix which rotates the joint matrix by 180
    // degree around global Y to account for the fact the Nebula2
    // characters look along +Z, not -Z
    matrix44 rot180 = matrix44::rotationy(n_deg2rad(180.0f));

    // do for all
    IndexT index;
    for(index = 0; index < this->attachments.Size(); index++)
    {
        // get current attachment
        InternalAttachment& attachment = this->attachments[index];

        // check if any resource isn't valid anymore
        bool removeAttachment = !attachment.baseEntity->HasCharacter();

        // check duration
        if (-1 != attachment.duration && attachment.startTime + attachment.duration < this->time)
        {
            // effect has been ended, remove from server
            removeAttachment = true;
        }

        if (!removeAttachment)
        {
            if(attachment.keepLocal || !attachment.initPosSet)
            {            
                // get world matrix of char entity
                const matrix44& worldMatrix = attachment.baseEntity->GetTransform();

                // get joint
                const CharacterJointInstance& joint = attachment.baseEntity->GetCharacterInstance()->Skeleton().GetJoint(attachment.jointIndex);
                        
                // calc and rotation and transformation depending on settings
                matrix44 transformation = attachment.offset;
                switch (attachment.rotation)
                {
                case Local:
                    transformation = matrix44::multiply(transformation, joint.GetJointMatrix());
                    transformation = matrix44::multiply(transformation, rot180);
                    transformation = matrix44::multiply(transformation, worldMatrix);
                    break;                
                case Entity:
                    transformation = matrix44::multiply(transformation, rot180);
                    transformation = matrix44::multiply(transformation, worldMatrix);
                    transformation.translate(vector(joint.GetJointMatrix().get_position()));
                    break;
                case World:
                    transformation.translate(vector(worldMatrix.get_position()));
                    break;
                default:
                    n_error("AttachmentServer::OnFrame: unsupported attachment rotation used!\n");
                    break;
                }                
                attachment.attachedEntity->SetTransform(transformation);

                if (!attachment.initPosSet)
                {
                    attachment.initPosSet = true;
                    attachment.attachedEntity->SetVisible(true);
                }                
            }

            //CoreGraphics::Shape shape;hotSpotAttachments
            //shape.SetupSimpleShape(Threading::Thread::GetMyThreadId(),
            //    CoreGraphics::Shape::Sphere,
            //    baseTransform,
            //    Math::float4(1,0,0,0.4f));
            //CoreGraphics::ShapeRenderer::Instance()->AddShape(shape);

            //CoreGraphics::Shape shape2;
            //shape2.SetupSimpleShape(Threading::Thread::GetMyThreadId(),
            //    CoreGraphics::Shape::Sphere,
            //    transform,
            //    Math::float4(0,1,0,0.4f));
            //CoreGraphics::ShapeRenderer::Instance()->AddShape(shape2);
        }
        else
        {
            // character not valid remove attachment
            this->DetachEntity(attachment.attachedEntity);
            index--;
        }
    }
}
} // namespace InternalGraphics
