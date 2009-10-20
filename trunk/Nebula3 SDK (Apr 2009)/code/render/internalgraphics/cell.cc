//------------------------------------------------------------------------------
//  cell.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalgraphics/cell.h"
#include "internalgraphics/internalstage.h"
#include "internalgraphics/stagebuilder.h"

namespace InternalGraphics
{
__ImplementClass(InternalGraphics::Cell, 'IGCL', Core::RefCounted);

using namespace Math;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
Cell::Cell() :
    numEntitiesInHierarchyAllTypes(0)
{
    Memory::Clear(this->numEntitiesInHierarchyByType, sizeof(this->numEntitiesInHierarchyByType));
}

//------------------------------------------------------------------------------
/**
*/
Cell::~Cell()
{
    // make sure we've been properly cleaned up
    n_assert(!this->IsAttachedToStage());
    n_assert(!this->parentCell.isvalid());
    n_assert(!this->stage.isvalid());
    n_assert(this->childCells.IsEmpty());
    n_assert(this->entities.IsEmpty());
    IndexT i;
    for (i = 0; i < InternalGraphicsEntityType::NumTypes; i++)
    {
        n_assert(this->entitiesByType[i].IsEmpty());
    }
}

//------------------------------------------------------------------------------
/**
*/
void
Cell::OnAttachToStage(const Ptr<InternalStage>& toStage)
{
    n_assert(toStage.isvalid());
    n_assert(!this->IsAttachedToStage());
    n_assert(this->entities.IsEmpty());

    this->stage = toStage;
    this->numEntitiesInHierarchyAllTypes = 0;
    Memory::Clear(this->numEntitiesInHierarchyByType, sizeof(this->numEntitiesInHierarchyByType));
        
    // recurse into child cells
    IndexT i;
    for (i = 0; i < this->childCells.Size(); i++)
    {
        this->childCells[i]->OnAttachToStage(toStage);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
Cell::OnRemoveFromStage()
{
    n_assert(this->IsAttachedToStage());

    // first recurse to child cells
    IndexT i;
    for (i = 0; i < this->childCells.Size(); i++)
    {
        this->childCells[i]->OnRemoveFromStage();
    }

    // detach entities from cell
    for (i = 0; i < this->entities.Size(); i++)
    {
        this->entities[i]->OnRemoveFromCell();
    }

    // cleanup
    this->stage = 0;
    this->parentCell = 0;
    this->numEntitiesInHierarchyAllTypes = 0;
    Memory::Clear(this->numEntitiesInHierarchyByType, sizeof(this->numEntitiesInHierarchyByType));
    this->childCells.Clear();
    this->entities.Clear();
    for (i = 0; i < InternalGraphicsEntityType::NumTypes; i++)
    {
        this->entitiesByType[i].Clear();
    }
}

//------------------------------------------------------------------------------
/**
    NOTE: the cell hierarchy may only be built during the setup phase while
    the cell hierarchy haven't been added to the stage yet.
*/
void
Cell::AttachChildCell(const Ptr<Cell>& cell)
{
    n_assert(!this->stage.isvalid());
    n_assert(cell.isvalid())
    n_assert(!cell->GetParentCell().isvalid()); 

    cell->parentCell = this;
    this->childCells.Append(cell);
}

//------------------------------------------------------------------------------
/**
    Attach an entity to this Cell. This will happen when a graphics entity
    moves through the world, leaving and entering cells as necessary.
*/
void
Cell::AttachEntity(const Ptr<InternalGraphicsEntity>& entity)
{
    n_assert(entity.isvalid());
    n_assert(this->stage.isvalid());
    n_assert(entity->GetStage() == this->stage);
    n_assert(!entity->GetCell().isvalid());
    n_assert(entity->GetType() < InternalGraphicsEntityType::NumTypes);

    entity->OnAttachToCell(this);
    
    this->entities.Append(entity);
    this->entitiesByType[entity->GetType()].Append(entity);

    this->UpdateNumEntitiesInHierarchy(entity->GetType(), +1);
}

//------------------------------------------------------------------------------
/**
*/
void
Cell::RemoveEntity(const Ptr<InternalGraphicsEntity>& entity)
{
    n_assert(entity.isvalid());
    n_assert(entity->GetCell().get() == this);
    n_assert(entity->GetType() < InternalGraphicsEntityType::NumTypes);

    entity->OnRemoveFromCell();
    
    IndexT entitiesIndex = this->entities.FindIndex(entity);
    n_assert(InvalidIndex != entitiesIndex);
    this->entities.EraseIndex(entitiesIndex);
    
    IndexT entitiesByTypeIndex = this->entitiesByType[entity->GetType()].FindIndex(entity);
    n_assert(InvalidIndex != entitiesByTypeIndex);
    this->entitiesByType[entity->GetType()].EraseIndex(entitiesByTypeIndex);
    
    this->UpdateNumEntitiesInHierarchy(entity->GetType(), -1);
}

//------------------------------------------------------------------------------
/**
    Starting from this cell, try to find the smallest cell which completely
    contains the given entity:

    - starting from initial cell:
        - if the entity does not fit into the cell, move up the
          tree until the first cell is found which the entity completely fits into
        - if the entity fits into a cell, check each child cell if the 
          entity fits completely into the cell

    The entity will not be attached! If the entity does not fit into the 
    root cell, the root cell will be returned, not 0.
    
    @param  entity      pointer of entity to find new cell for
    @return             cell which completely encloses the entity (the root cell is an exception)
*/
Ptr<Cell>
Cell::FindEntityContainmentCell(const Ptr<InternalGraphicsEntity>& entity)
{
    // get global bounding box of entity
    const bbox& entityBox = entity->GetGlobalBoundingBox();

    // find the first upward cell which completely contains the entity,
    // stop at tree root
    Ptr<Cell> curCell = this;
    while ((curCell->GetParentCell().isvalid()) && (!curCell->GetBoundingBox().contains(entityBox)))
    {
        curCell = curCell->GetParentCell();
    } 

    // find smallest downward cell which completely contains the entity
    IndexT cellIndex;
    SizeT numCells;
    do
    {
        const Array<Ptr<Cell> >& curChildren = curCell->GetChildCells();
        numCells = curChildren.Size();
        for (cellIndex = 0; cellIndex < numCells; cellIndex++)
        {
            const Ptr<Cell>& childCell = curChildren[cellIndex];
            if (childCell->GetBoundingBox().contains(entityBox))
            {
                curCell = childCell;
                break;
            }
        }
        // check for loop fallthrough: this means that the current cell either has
        // no children, or that none of the children completely contains the entity
    }
    while (cellIndex != numCells);
    return curCell;
}

//------------------------------------------------------------------------------
/**
    Insert a dynamic graphics entity into the cell tree. The entity
    will correctly be inserted into the smallest enclosing cell in the tree.
    The cell may not be currently attached to a cell, the refcount of the
    entity will be incremented.

    @param  entity      pointer to a graphics entity
*/
void
Cell::InsertEntity(const Ptr<InternalGraphicsEntity>& entity)
{
	Ptr<Cell> cell = this->FindEntityContainmentCell(entity);
    n_assert(cell.isvalid());
    cell->AttachEntity(entity);
}

//------------------------------------------------------------------------------
/**
    Recursively update visibility links. This method is called by the
    top level method UpdateLinks(). 
    
    NOTE: This is the core visibility detection method and must be FAST.
*/
void
Cell::RecurseUpdateLinks(const Ptr<InternalGraphicsEntity>& observerEntity,
                         uint observedEntityTypeMask,
                         InternalGraphicsEntity::LinkType linkType,
                         ClipStatus::Type clipStatus)
{
    n_assert(observerEntity.isvalid());

    // break immediately if no entity of wanted type in this cell or below
    if (this->GetNumEntitiesInHierarchyByTypeMask(observedEntityTypeMask) == 0)
    {
        return;
    }

    // if clip status unknown or clipped, get clip status of this cell against observer entity
    if ((ClipStatus::Invalid == clipStatus) || (ClipStatus::Clipped == clipStatus))
    {
        const bbox& cellBox = this->GetBoundingBox();
		clipStatus = observerEntity->ComputeClipStatus(cellBox);
    }

    // proceed depending on clip status of cell against observer entity
    if (ClipStatus::Outside == clipStatus)
    {
        // cell isn't visible by observer entity
        return;
    }
    else if (ClipStatus::Inside == clipStatus)
    {
        // cell is fully visible by observer entity, everything inside
        // the cell is fully visible as well
        IndexT observedType;
        for (observedType = 0; observedType < InternalGraphicsEntityType::NumTypes; observedType++)
        {
            if (0 != (observedEntityTypeMask & (1<<observedType)))
            {
                const Array<Ptr<InternalGraphicsEntity> >& observedEntities = this->entitiesByType[observedType];
				IndexT index;
                SizeT num = observedEntities.Size();
                for (index = 0; index < num; index++)
                {
                    const Ptr<InternalGraphicsEntity>& observedEntity = observedEntities[index];
                    
                    // short cut: if generating light links, check if the observed
                    // entity is actually visible by any camera
                    if ((InternalGraphicsEntity::LightLink == linkType) && 
                        (observedEntity->GetLinks(InternalGraphicsEntity::CameraLink).IsEmpty()))
                    {
                        continue;
                    }
                    else if (observedEntity->IsValid() && observedEntity->IsVisible())
                    {
                        // add bi-directional visibility link
						observerEntity->AddLink(linkType, observedEntity);
						observedEntity->AddLink(linkType, observerEntity);
                        observedEntity->OnNotifyVisible(observerEntity);
                    }
                }
			}
        }
    }
    else
    {
        // cell is partially visible, must check visibility of each contained entity
        IndexT observedType;
        for (observedType = 0; observedType < InternalGraphicsEntityType::NumTypes; observedType++)
        {
            if (0 != (observedEntityTypeMask & (1<<observedType)))
            {
                const Array<Ptr<InternalGraphicsEntity> >& observedEntities = this->entitiesByType[observedType];
                IndexT index;
                SizeT num = observedEntities.Size();
                for (index = 0; index < num; index++)
                {
                    const Ptr<InternalGraphicsEntity>& observedEntity = observedEntities[index];

                    // short cut: if generating light links, check if the observed
                    // entity is actually visible by any camera
                    if ((InternalGraphicsEntity::LightLink == linkType) && 
                        (observedEntity->GetLinks(InternalGraphicsEntity::CameraLink).IsEmpty()))
                    {
                        continue;
                    }
                    else if (observedEntity->IsValid() && observedEntity->IsVisible())
                    {
                        if (observerEntity->ComputeClipStatus(observedEntity->GetGlobalBoundingBox()) != ClipStatus::Outside)
                        {
                            // add bi-directional visibility link
                            observerEntity->AddLink(linkType, observedEntity);
                            observedEntity->AddLink(linkType, observerEntity);
                            observedEntity->OnNotifyVisible(observerEntity);
                        }
                    }
                }
            }
        }
    }

    // recurse into child cells (if this cell is fully or partially visible)
    IndexT childIndex;
    SizeT numChildren = this->childCells.Size();
    for (childIndex = 0; childIndex < numChildren; childIndex++)
    {
        this->childCells[childIndex]->RecurseUpdateLinks(observerEntity, observedEntityTypeMask, linkType, clipStatus);
    }
}

//------------------------------------------------------------------------------
/**
    Frontend method for updating visibility links. This method
    simply calls RecurseUpdateLinks() which recurses into child
    cells if necessary.
*/
void
Cell::UpdateLinks(const Ptr<InternalGraphicsEntity>& observerEntity, uint observedEntityTypeMask, InternalGraphicsEntity::LinkType linkType)
{
    this->RecurseUpdateLinks(observerEntity, observedEntityTypeMask, linkType, ClipStatus::Invalid);
}

//------------------------------------------------------------------------------
/**
    Update the number of entities in hierarchy. Must be called when
    entities are added or removed from this cell.
*/
void
Cell::UpdateNumEntitiesInHierarchy(InternalGraphicsEntityType::Code type, int num)
{
    n_assert((type >= 0) && (type < InternalGraphicsEntityType::NumTypes));

    this->numEntitiesInHierarchyAllTypes += num;
    this->numEntitiesInHierarchyByType[type] += num;
    n_assert(this->numEntitiesInHierarchyAllTypes >= 0);
    Cell* p = this->parentCell.get_unsafe();
    if (p) do
    {
        p->numEntitiesInHierarchyAllTypes += num;
        p->numEntitiesInHierarchyByType[type] += num;
        n_assert(p->numEntitiesInHierarchyAllTypes >= 0);
    }
    while (0 != (p = p->parentCell.get_unsafe()));
}

} // namespace InternalGraphics
