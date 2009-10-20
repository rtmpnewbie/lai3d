//------------------------------------------------------------------------------
//  internalmodelentity.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalgraphics/internalmodelentity.h"
#include "models/modelserver.h"
#include "coregraphics/shaperenderer.h"
#include "threading/thread.h"
#include "internalgraphics/internalgraphicsserver.h"
#include "characters/characternode.h"
#include "characters/characternodeinstance.h"
#include "internalgraphics/internalgraphicsprotocol.h"
#include "animation/animeventserver.h"

namespace InternalGraphics
{
__ImplementClass(InternalGraphics::InternalModelEntity, 'IMDE', InternalGraphics::InternalGraphicsEntity);

using namespace Threading;
using namespace CoreGraphics;
using namespace Math;
using namespace Models;
using namespace Resources;
using namespace Characters;
using namespace Animation;

//------------------------------------------------------------------------------
/**
*/
InternalModelEntity::InternalModelEntity() :
    updateFrameId(InvalidIndex),
    renderFrameId(InvalidIndex),
    nebula2CharacterRotationHack(false),
    lastAnimEventCheckEndTime(-1)
{
    this->SetType(InternalGraphicsEntityType::Model);
}

//------------------------------------------------------------------------------
/**
*/
InternalModelEntity::~InternalModelEntity()
{
    n_assert(!this->modelInstance.isvalid());
}

//------------------------------------------------------------------------------
/**
    This will initialize the managed model inside the ModelEntity.
*/
void
InternalModelEntity::OnActivate()
{
    n_assert(!this->IsActive());
    n_assert(this->resId.IsValid());
    n_assert(!this->charInst.isvalid());
    n_assert(!this->character.isvalid());
    InternalGraphicsEntity::OnActivate();
    
    // note: we will remain invalid until at least our model has loaded
    this->SetValid(false);
    if (!this->managedModel.isvalid())
    {
        this->managedModel = ModelServer::Instance()->LoadManagedModel(this->resId);
    }  
#if NEBULA3_DEBUG
    //n_printf("InternalModelEntity::OnActivate(): %s\n", this->resId.Value().AsCharPtr());
#endif
    this->ValidateModelInstance();
}

//------------------------------------------------------------------------------
/**
    Cleanup our managed model, and ModelInstance if it is already 
    initialized.
*/
void
InternalModelEntity::OnDeactivate()
{
    n_assert(this->IsActive());
    n_assert(this->managedModel.isvalid());

    // clear character instance reference (if valid)
    this->charInst = 0;
    this->character = 0;

    // discard our model instance (if exists)
    if (this->modelInstance.isvalid())
    {
        this->modelInstance->Discard();
        this->modelInstance = 0;
    }

    // discard our managed model
    if (ModelServer::Instance()->HasManagedModel(this->managedModel->GetResourceId()))
    {
        ModelServer::Instance()->DiscardManagedModel(this->managedModel);
    }    
    this->managedModel = 0;

    // up to parent class
    InternalGraphicsEntity::OnDeactivate();
}

//------------------------------------------------------------------------------
/**
    In OnTransformChanged() we need to update the transformation
    of our ModelInstance (if it has already been initialised).
*/
void
InternalModelEntity::OnTransformChanged()
{
    if (this->modelInstance.isvalid())
    {
        if (this->nebula2CharacterRotationHack)
        {
            // AAARGH implement the age old 180 degree hack for Nebula characters
            matrix44 m = matrix44::multiply(matrix44::rotationy(n_deg2rad(180.0f)), this->GetTransform());
            this->modelInstance->SetTransform(m);
        }
        else
        {
            this->modelInstance->SetTransform(this->GetTransform());
        }
    }
}

//------------------------------------------------------------------------------
/**
    In OnUpdate() the ModelEntity first needs to check whether the
    ManagedModel has already been loaded, and if yes, a ModelInstance
    will be created, finally, the ModelInstance will be prepared for
    rendering.
*/
void
InternalModelEntity::OnUpdate(Timing::Time time)
{
    // only update once-per-frame
    IndexT curFrameId = InternalGraphicsServer::Instance()->GetFrameCount();
    if (this->updateFrameId != curFrameId)
    {
        this->updateFrameId = curFrameId;
        // set current time, in case of some methods here need the 
        // actual time, and the graphics entity sets its time at InternalGraphicsEntity::OnUpdate()
        // here at last method call        
        this->entityTime = time;

        // first check if our model has been loaded already, if yes
        // our model instance must be initialized 
        if (!this->IsValid())
        {
            this->ValidateModelInstance();
        }

        // if we're a character, update it's time stamp and check anim events
        if (this->charInst.isvalid())
        {
            this->charInst->UpdateTime(Timing::SecondsToTicks(time));
            
            // check if its time to emit anim events
            Timing::Tick timeInTicks = Timing::SecondsToTicks(time);
            if(timeInTicks >= this->lastAnimEventCheckEndTime)
            {                
                // first frame?!
                if(-1 == this->lastAnimEventCheckEndTime)
                {
                    this->lastAnimEventCheckEndTime = timeInTicks;
                }

                // emit anim events
                Timing::Tick startTime = this->lastAnimEventCheckEndTime;
                this->lastAnimEventCheckEndTime = timeInTicks + this->charInst->AnimController().AnimSequencer().GetMaximumKeyDuration();
                const Util::Array<AnimEventInfo>& eventInfos = this->charInst->AnimController().AnimSequencer().EmitAnimEvents(startTime, this->lastAnimEventCheckEndTime, true, "");

                // handle events if there are some
                if (eventInfos.Size() > 0)
                { 
                    // first fill up the id's
                    IndexT indeX;
                    for (indeX = 0; indeX < eventInfos.Size(); indeX++)
                    {
                        eventInfos[indeX].SetEntityId(this->GetId());
                    }
                    // now handle that shit
                    Animation::AnimEventServer::Instance()->HandleEventz(eventInfos);
                }

                // fallthrough: set last check end time, to current time
                if (startTime == this->lastAnimEventCheckEndTime)
                {
                    this->lastAnimEventCheckEndTime = timeInTicks;
                }
            }
        }

        // if our model instance is valid, let it update itself
        if (this->modelInstance.isvalid() && this->IsVisible())
        {
            this->modelInstance->Update(time);
        }

        if (this->charInst.isvalid())
        {
            // if AnimSequencer is calculating translation delta from animation
            // we have to set the position change in our transform matrix
            if (charInst->AnimController().AnimSequencer().GetComputeDeltaTranslation())
            {
                vector posDelta = this->charInst->AnimController().GetAnimDrivenMotionVector();                
                this->animationDrivenMovement = float4::transform(posDelta, this->GetTransform());
                /*Util::String debugOut;
                debugOut.Format("DeltaPos: %s \n", Util::String::FromFloat4(this->animationDrivenMovement).AsCharPtr());
                n_printf(debugOut.AsCharPtr());*/
            }            
        }
    }

    // important: call parent class!
    InternalGraphicsEntity::OnUpdate(time);
}

//------------------------------------------------------------------------------
/**
    This method is called once (per-view) before the entity is rendered,
    and only for visible entities.
*/
void
InternalModelEntity::OnNotifyVisible(const Ptr<InternalGraphicsEntity>& observer)
{
    // only perform once-per-frame
    IndexT curFrameId = InternalGraphicsServer::Instance()->GetFrameCount();
    if (this->renderFrameId != curFrameId)
    {
        this->renderFrameId = curFrameId;

        // need to prepare character for rendering (if exists)
        if (this->charInst.isvalid())
        {
            this->charInst->PrepareForRendering();
        }
    }

    // call parent-class
    InternalGraphicsEntity::OnNotifyVisible(observer);
}

//------------------------------------------------------------------------------
/**
    This creates and initializes our ModelInstance if needed. Since Model 
    loading happens asynchronously this may happen at any time after
    the ModelEntity is activated.
*/
void
InternalModelEntity::ValidateModelInstance()
{
    n_assert(!this->IsValid());
    if (!this->modelInstance.isvalid())
    {
        if (this->managedModel->GetState() == Resource::Loaded)
        {
            const Ptr<Model> model = this->managedModel->GetModel();
            n_assert(model->IsLoaded());
            
            // only set the entity to valid if the complete 
            // model node is completely loaded (no placeholders)
            // FIXME: this should be better adjustable...
            // e.g. meshes must be loaded, but textures may
            // still be loading?
            if (model->CheckPendingResources())
            {
                this->SetValid(true);
                
                // setup model and create a new model instance
                this->SetLocalBoundingBox(model->GetBoundingBox());
                this->modelInstance = model->CreateInstance();
                this->modelInstance->SetTransform(this->GetTransform());
                this->modelInstance->SetModelEntity(this);
                if (this->IsVisible())
                {
                    this->OnShow();
                }
                else
                {
                    this->OnHide();
                }

                // check if the model instance contains a character
                const Ptr<ModelNodeInstance>& rootNodeInst = this->modelInstance->GetRootNodeInstance();
                if (rootNodeInst->IsA(CharacterNodeInstance::RTTI))
                {
                    // FIXME FIXME FIXME!!! HACK HACK HACK!!!!
                    this->nebula2CharacterRotationHack = true;
                    this->OnTransformChanged();
                        
                    this->charInst = rootNodeInst.downcast<CharacterNodeInstance>()->GetCharacterInstance();
                    this->character = rootNodeInst->GetModelNode().downcast<CharacterNode>()->GetCharacter();

                    // check internal model entity if any jointname for animtion driven movement is set
                    const Util::String& jointName = this->GetAnimationJointName();
                    if (jointName.IsValid())
                    {                        
                        this->GetCharacterInstance()->AnimController().AnimSequencer().SetAnimDrivenMotion(jointName.IsValid());
                        IndexT jointIndex = this->character->Skeleton().GetJointIndexByName(jointName);
                        this->GetCharacterInstance()->AnimController().SetAnimDrivenMotionJointIndex(jointIndex);                        
                    }
                }

                // notify any entity event handler
                Ptr<ModelInstanceCreated> event = ModelInstanceCreated::Create();
                event->SetGraphicsEntity(this);
                InternalGraphicsServer::Instance()->PutGraphicsEntityEvent(this->GetType(), event.cast<InternalGraphicsEntityEvent>());
            }
        }
        else if (this->managedModel->GetState() == Resource::Failed)
        {
            n_error("Failed to load model entity '%s'!", this->resId.Value().AsCharPtr());
        }
    }
}

//------------------------------------------------------------------------------
/**
    Render our debug visualization (the bounding box).
*/
void
InternalModelEntity::OnRenderDebug()
{
    float4 color(1.0f, 0.0f, 0.0f, 0.5f);
    ShapeRenderer::Instance()->AddShape(RenderShape(Thread::GetMyThreadId(), RenderShape::Box, this->globalBox.to_matrix44(), color));
    this->GetModelInstance()->RenderDebug();
}

//------------------------------------------------------------------------------
/**
    This method is called from the SetVisible() method when the entity 
    changes from invisible to visible state.
*/
void
InternalModelEntity::OnShow()
{
    if (this->modelInstance.isvalid())
    {
        this->modelInstance->OnShow(this->entityTime);
    }
    InternalGraphicsEntity::OnShow();
}

//------------------------------------------------------------------------------
/**
    This method is called from the SetVisible() method when the entity
    changes from visible to invisible state.
*/
void
InternalModelEntity::OnHide()
{
    if (this->modelInstance.isvalid())
    {
        this->modelInstance->OnHide(this->entityTime);
    }
    InternalGraphicsEntity::OnHide();
}
} // namespace InternalGraphics
