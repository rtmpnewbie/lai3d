//------------------------------------------------------------------------------
//  internalgraphicsentity.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalgraphics/internalgraphicsentity.h"
#include "internalgraphics/internalstage.h"
#include "internalgraphics/cell.h"
#include "internalgraphics/internalgraphicsserver.h"

namespace InternalGraphics
{
__ImplementClass(InternalGraphics::InternalGraphicsEntity, 'GREN', Core::RefCounted);

using namespace Util;
using namespace Math;

InternalGraphicsEntity::Id InternalGraphicsEntity::UniqueIdCounter = 0;

//------------------------------------------------------------------------------
/**
*/
InternalGraphicsEntity::InternalGraphicsEntity() :
    type(InternalGraphicsEntityType::InvalidType),
    isActive(false),
    isValid(false),
    isVisible(true),
    globalBoxDirty(true),
    transformChanged(false),
    links(NumLinkTypes),
    entityTime(0.0),
    clipStatus(ClipStatus::Invalid),
    culledLastFrame(false)
{
    this->id = ++UniqueIdCounter;
    this->transform = matrix44::identity();
}

//------------------------------------------------------------------------------
/**
*/
InternalGraphicsEntity::~InternalGraphicsEntity()
{
    n_assert(!this->IsActive());
}

//------------------------------------------------------------------------------
/**
    Activate the entity. This method is called when the entity
    is created an attached to the graphics server. During OnActivate()
    the entity should perform any one-time initializations.
*/
void
InternalGraphicsEntity::OnActivate()
{
    n_assert(!this->IsActive());
    this->isActive = true;
    this->isValid = true;
    InternalGraphicsServer::Instance()->RegisterEntity(this);
}

//------------------------------------------------------------------------------
/**
    Deactivate the entity, this method is called when the entity
    is removed from the graphics server. Any initialization done
    in OnActivate() should be undone here.
*/
void
InternalGraphicsEntity::OnDeactivate()
{
    n_assert(this->IsActive());
    if (this->IsAttachedToStage())
    {
        this->stage->RemoveEntity(this);
    }
    n_assert(!this->IsAttachedToStage());
    n_assert(!this->IsAttachedToCell());
    IndexT i;
    for (i = 0; i < NumLinkTypes; i++)
    {
        n_assert(this->links[i].IsEmpty());
    }
    InternalGraphicsServer::Instance()->UnregisterEntity(this);
    this->isActive = false;
}

//------------------------------------------------------------------------------
/**
    This method is called when the graphics entity is attached to a stage. An
    entity may only be attached to one stage at any time, but can be
    attached to different stages during its lifetime. Attachging an entity
    to a stage may be relatively slow because the entity must be inserted
    into the cell hierarchy.
*/
void
InternalGraphicsEntity::OnAttachToStage(const Ptr<InternalStage>& s)
{
    n_assert(this->IsActive());
    n_assert(!this->IsAttachedToStage());
    n_assert(!this->IsAttachedToCell());
    IndexT i;
    for (i = 0; i < NumLinkTypes; i++)
    {
        n_assert(this->links[i].IsEmpty());
    }
    this->stage = s;
}

//------------------------------------------------------------------------------
/**
    This method is called when the graphics entity is removed from a stage.
*/
void
InternalGraphicsEntity::OnRemoveFromStage()
{
    n_assert(this->IsActive());
    n_assert(this->IsAttachedToStage());
    n_assert(!this->IsAttachedToCell());
    this->ClearLinks(CameraLink);
    this->ClearLinks(LightLink);
    this->stage = 0;
}

//------------------------------------------------------------------------------
/**
    This method is called when the graphics entity is attached to a cell inside
    a stage. When entity travel through the graphics world, they will be remove
    and attached themselves from and to Cells as they cross Cell borders.
*/
void
InternalGraphicsEntity::OnAttachToCell(const Ptr<Cell>& c)
{
    n_assert(this->IsActive());
    n_assert(this->IsAttachedToStage());
    n_assert(!this->IsAttachedToCell());
    this->cell = c;
}

//------------------------------------------------------------------------------
/**
    Called when the graphics entity is removed from a cell inside a
    stage.
*/
void
InternalGraphicsEntity::OnRemoveFromCell()
{
    n_assert(this->IsActive());
    n_assert(this->IsAttachedToStage());
    n_assert(this->IsAttachedToCell());
    this->cell = 0;
}

//------------------------------------------------------------------------------
/**
    This method is called from the SetVisible() method when the entity 
    changes from invisible to visible state.
*/
void
InternalGraphicsEntity::OnShow()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This method is called from the SetVisible() method when the entity
    changes from visible to invisible state.
*/
void
InternalGraphicsEntity::OnHide()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This method is called on the graphics entity to update itself,
    for instance to implement hierarchy animation or particle 
    updates.
*/
void
InternalGraphicsEntity::OnUpdate(Timing::Time time)
{
    // if our transformation or bounding box has changed
    // we need to update our position in the cell tree
    if (this->transformChanged)
    {
        this->transformChanged = false;
        this->UpdatePositionInCellTree();
    }

    // remember if we were culled last frame or not
    this->culledLastFrame = (this->clipStatus == ClipStatus::Outside);    

    // reset clipstatus to invalid, will be set in onnotifyvisible
    this->SetClipStatus(ClipStatus::Outside);

    // save the given time
    this->entityTime = time;
}

//------------------------------------------------------------------------------
/**
    This method is called during visibility linking when an observed entity
    is found to be visible by an observer (a camera or a light entity).
    NOTE that this method will be called several times per frame, so it may
    be a good idea to use the graphics server's frame counter to protect
    expensive code from multiple execution.
*/
void
InternalGraphicsEntity::OnNotifyVisible(const Ptr<InternalGraphicsEntity>& observer)
{
    // reset clipstatus to inside
    this->SetClipStatus(ClipStatus::Inside);
}

//------------------------------------------------------------------------------
/**
    This method is called whenever the transformation matrix has changed
    by a call to SetTransform(). Override this method in a subclass if
    it wants to be informed about changes to the transformation matrix.
*/
void
InternalGraphicsEntity::OnTransformChanged()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This method is called on the graphics entity to render a debug 
    visualization of itself.
*/
void
InternalGraphicsEntity::OnRenderDebug()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This method is called to create an exact clone of this graphics entity.
    The new entity will not be attached to a stage.
*/
Ptr<InternalGraphicsEntity>
InternalGraphicsEntity::CreateClone() const
{
    Ptr<InternalGraphicsEntity> clone = (InternalGraphicsEntity*) this->GetRtti()->Create();
    clone->SetTransform(this->transform);
    clone->SetLocalBoundingBox(this->localBox);
    return clone;
}

//------------------------------------------------------------------------------
/**
*/
void
InternalGraphicsEntity::ClearLinks(LinkType linkType)
{
    this->links[linkType].Clear();
}

//------------------------------------------------------------------------------
/**
*/
void
InternalGraphicsEntity::UpdateGlobalBoundingBox()
{
    n_assert(this->globalBoxDirty);
    this->globalBox = this->localBox;
    //vector t = this->transform.get_position();
    //matrix44 m = matrix44::identity();
    //m.translate(t);
    this->globalBox.transform(this->transform);
    this->globalBoxDirty = false;
}

//------------------------------------------------------------------------------
/**
    This method is called from OnUpdate() when either the 
    entity's transformation or bounding box has changed. It checks if the
    entity still fits into its current cell, and if not, moves the entity
    in a new cell.
*/
void
InternalGraphicsEntity::UpdatePositionInCellTree()
{
    // see if we need to move from one cell to another
    if (this->cell.isvalid())
    {
        Ptr<InternalGraphicsEntity> thisPtr(this);
        const Ptr<Cell>& newCell = this->cell->FindEntityContainmentCell(thisPtr);
        if (this->cell != newCell)
        {
            this->cell->RemoveEntity(thisPtr);
            newCell->AttachEntity(thisPtr);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<InternalStage>&
InternalGraphicsEntity::GetStage() const
{
    return this->stage;
}

//------------------------------------------------------------------------------
/**
*/
bool
InternalGraphicsEntity::IsAttachedToStage() const
{
    return this->stage.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<Cell>&
InternalGraphicsEntity::GetCell() const
{
    return this->cell;
}

//------------------------------------------------------------------------------
/**
*/
bool
InternalGraphicsEntity::IsAttachedToCell() const
{
    return this->cell.isvalid();
}

//------------------------------------------------------------------------------
/**
    Compute the clip status between this entity and a bounding box in
    global space. This method must be overwritten in a derived class.
*/
ClipStatus::Type
InternalGraphicsEntity::ComputeClipStatus(const bbox& box)
{
    return ClipStatus::Outside;
}
} // namespace InternalGraphics