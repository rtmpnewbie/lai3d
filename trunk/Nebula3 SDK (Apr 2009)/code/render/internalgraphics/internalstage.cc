//------------------------------------------------------------------------------
//  internalstage.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalgraphics/internalstage.h"
#include "internalgraphics/stagebuilder.h"
#include "internalgraphics/cell.h"
#include "internalgraphics/internalcameraentity.h"
#include "timing/timer.h"
#include "rendermodules/rt/rtpluginregistry.h"

namespace InternalGraphics
{
__ImplementClass(InternalGraphics::InternalStage, 'IGSG', Core::RefCounted);

using namespace Util;

//------------------------------------------------------------------------------
/**
*/
InternalStage::InternalStage() :
    isAttachedToServer(false),
    updateEntitiesFrameCount(InvalidIndex)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
InternalStage::~InternalStage()
{
    // make sure we've been properly cleaned up
    n_assert(!this->IsAttachedToServer());
    n_assert(!this->rootCell.isvalid());
    n_assert(!this->stageBuilder.isvalid());
    n_assert(this->entities.IsEmpty());
    IndexT i;
    for (i = 0; i < InternalGraphicsEntityType::NumTypes; i++)
    {
        n_assert(this->entitiesByType[i].IsEmpty());
    }
}

//------------------------------------------------------------------------------
/**
    Setting a root cell will also initialize it.
*/
void
InternalStage::SetRootCell(const Ptr<Cell>& cell)
{
    n_assert(!this->rootCell.isvalid());
    this->rootCell = cell;
    this->rootCell->OnAttachToStage(this);
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<Cell>&
InternalStage::GetRootCell() const
{
    return this->rootCell;
}

//------------------------------------------------------------------------------
/**
*/
void
InternalStage::SetStageBuilder(const Ptr<StageBuilder>& b)
{
    this->stageBuilder = b;
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<StageBuilder>&
InternalStage::GetStageBuilder() const
{
    return this->stageBuilder;
}

//------------------------------------------------------------------------------
/**
*/
void
InternalStage::OnAttachToServer()
{
    n_assert(!this->IsAttachedToServer());
    this->isAttachedToServer = true;

    // if a stage builder has been set, ask it to initialize us,
    // otherwise require a valid root cell
    if (this->stageBuilder.isvalid())
    {
        this->stageBuilder->BuildStage(this);
    }
    n_assert(this->rootCell.isvalid());
    n_assert(this->rootCell->IsAttachedToStage());
}

//------------------------------------------------------------------------------
/**
*/
void
InternalStage::OnRemoveFromServer()
{
    n_assert(this->IsAttachedToServer());
    n_assert(this->rootCell.isvalid());

    // properly shutdown root cell
    n_assert(this->rootCell->IsAttachedToStage());
    this->rootCell->OnRemoveFromStage();
    this->rootCell = 0;

    // properly cleanup entities
    IndexT entityIndex;
    for (entityIndex = 0; entityIndex < this->entities.Size(); entityIndex++)
    {
        const Ptr<InternalGraphicsEntity>& curEntity = this->entities[entityIndex];
        curEntity->OnRemoveFromStage();
        curEntity->OnDeactivate();
    }
    this->entities.Clear();
    IndexT typeIndex;
    for (typeIndex = 0; typeIndex < InternalGraphicsEntityType::NumTypes; typeIndex++)
    {
        this->entitiesByType[typeIndex].Clear();
    }

    // release stage builder
    this->stageBuilder = 0;

    this->isAttachedToServer = false;
}

//------------------------------------------------------------------------------
/**
    Add an entity to the stage. The method OnAttachToStage() will be
    invoked on the entity, and the entity will be inserted into 
    the cell hierarchy of the stage (which in turn call OnAttachToCell()
    on the entity).
*/
void
InternalStage::AttachEntity(const Ptr<InternalGraphicsEntity>& entity)
{
    n_assert(!entity->IsActive());
    n_assert(!entity->IsAttachedToStage());
    n_assert(entity->GetType() < InternalGraphicsEntityType::NumTypes);

    this->entities.Append(entity);
    this->entitiesByType[entity->GetType()].Append(entity);
    entity->OnActivate();
    entity->OnAttachToStage(this);
    this->rootCell->InsertEntity(entity);

    // notify render-thread plugins
    RenderModules::RTPluginRegistry::Instance()->OnAttachEntity(entity);
}

//------------------------------------------------------------------------------
/**
    Remove an entity from the stage. This will remove the entity from
    the cell hierarchy of the stage (which invoked OnRemoveFromCell()
    on the entity), and then the method OnRemoveFromStage() will
    be called on the entity.
*/
void
InternalStage::RemoveEntity(const Ptr<InternalGraphicsEntity>& entity)
{
    n_assert(entity->IsActive());
    n_assert(entity->IsAttachedToStage());
    n_assert(entity->GetStage().get() == this);
    n_assert(entity->IsAttachedToCell());
    n_assert(entity->GetType() < InternalGraphicsEntityType::NumTypes);

    // notify render-thread plugins
    RenderModules::RTPluginRegistry::Instance()->OnRemoveEntity(entity);

    // first remove entity from its cell, stage and deactivate it
    entity->GetCell()->RemoveEntity(entity);
    entity->OnRemoveFromStage();
    entity->OnDeactivate();
    
    IndexT entitiesByTypeIndex = this->entitiesByType[entity->GetType()].FindIndex(entity);
    n_assert(InvalidIndex != entitiesByTypeIndex);
    this->entitiesByType[entity->GetType()].EraseIndex(entitiesByTypeIndex);

    IndexT entitiesIndex = this->entities.FindIndex(entity);
    n_assert(InvalidIndex != entitiesIndex);
    this->entities.EraseIndex(entitiesIndex);
}

//------------------------------------------------------------------------------
/**
*/
void
InternalStage::RemoveAllEntities()
{
    while (this->entities.Size() > 0)
    {
        const Ptr<InternalGraphicsEntity>& curEntity = this->entities[0];
        this->RemoveEntity(curEntity);
    }
    this->entities.Clear();
    IndexT typeIndex;
    for (typeIndex = 0; typeIndex < InternalGraphicsEntityType::NumTypes; typeIndex++)
    {
        this->entitiesByType[typeIndex].Clear();
    }
}

//------------------------------------------------------------------------------
/**
    Update the entities in the stage. This usually implements graphics
    animations, etc... This method must be called first after
    BeginUpdate() and before any of the visibility link update methods to 
    make sure entity transforms and bounding boxes are uptodate before resolving 
    visibility.

    FIXME: this is no good for worlds with many static entities, entities should
    know whether they need to have their Update method called, and
    register themselves somewhere!
*/
void
InternalStage::UpdateEntities(Timing::Time curTime, IndexT curFrameCount)
{
    // check if entities have already been updated for this frame
    if (curFrameCount != this->updateEntitiesFrameCount)
    {
        this->updateEntitiesFrameCount = curFrameCount;
        IndexT entityIndex;
        SizeT numEntities = this->entities.Size();
        for (entityIndex = 0; entityIndex < numEntities; entityIndex++)
        {
            this->entities[entityIndex]->OnUpdate(curTime);            
        }
    }
}

//------------------------------------------------------------------------------
/**
    Update visibility links for a given camera. This will create bidirectional
    visibility links between the camera and all other entities (most importantly
    light and model entities) which are visible through this camera. This
    method must be called once for each active camera after UpdateEntities()
    and before UpdateVisibleLightLinks().
*/
void
InternalStage::UpdateCameraLinks(const Ptr<InternalCameraEntity>& cameraEntity)
{
    n_assert(cameraEntity.isvalid());

    // clear camera links in all entities
    IndexT entityIndex;
    SizeT numEntities = this->entities.Size();
    for (entityIndex = 0; entityIndex < numEntities; entityIndex++)
    {
        this->entities[entityIndex]->ClearLinks(InternalGraphicsEntity::CameraLink);
    }

    if (cameraEntity.isvalid())
    {
        // resolve visible model and light entities from this camera
        uint entityTypeMask = (1 << InternalGraphicsEntityType::Model) | (1 << InternalGraphicsEntityType::Light);
        this->rootCell->UpdateLinks(cameraEntity.cast<InternalGraphicsEntity>(), entityTypeMask, InternalGraphicsEntity::CameraLink);
    }
}

//------------------------------------------------------------------------------
/**
    For each visible light entity, this method will create light links
    between the light entities, and model entities influenced by 
    this light. This method must be called after UpdateCameraLinks() (this
    makes sure that no invisible lights and models will be checked).
*/
void
InternalStage::UpdateLightLinks()
{
    // clear light links in all entities
    IndexT entityIndex;
    SizeT numEntities = this->entities.Size();
    for (entityIndex = 0; entityIndex < numEntities; entityIndex++)
    {
        this->entities[entityIndex]->ClearLinks(InternalGraphicsEntity::LightLink);
    }

    // for each visible light...
    const Array<Ptr<InternalGraphicsEntity> >& lightEntities = this->entitiesByType[InternalGraphicsEntityType::Light];
    IndexT lightIndex;
    for (lightIndex = 0; lightIndex < lightEntities.Size(); lightIndex++)
    {
        const Ptr<InternalGraphicsEntity>& lightEntity = lightEntities[lightIndex];
        if (lightEntity->GetLinks(InternalGraphicsEntity::CameraLink).Size() > 0)
        {
            // find model entities influenced by this light
            this->rootCell->UpdateLinks(lightEntity, (1<<InternalGraphicsEntityType::Model), InternalGraphicsEntity::LightLink);
        }
    }
}
} // namespace InternalGraphics