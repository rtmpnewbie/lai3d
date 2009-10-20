#pragma once
//------------------------------------------------------------------------------
/**
    class Graphics::Stage
    
    A client-side proxy of a InternalGraphics::InternalStage.
    The Stage offers a friendly frontend to the
    client thread, and communicates with its server-side 
    Stage object through the GraphicsInterface method. There
    is a 1:1 relationship between the server-side Stage and the
    client-side Stage, thus the Stage may safely store
    read-only information on the client side without synchronizing
    with its InternalStage.

    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/rtti.h"
#include "util/stringatom.h"
#include "attr/attributecontainer.h"
#include "graphics/handle.h"
#include "graphics/graphicsentity.h"
#include "graphics/modelentity.h"

//------------------------------------------------------------------------------
namespace Graphics
{
class Stage : public Core::RefCounted
{
    __DeclareClass(Stage);
public:
    /// constructor
    Stage();
    /// destructor
    virtual ~Stage();

    /// return true if the Stage is valid
    bool IsValid() const;
    /// get name of stage
    const Util::StringAtom& GetName() const;
    /// get StageBuilder type
    const Core::Rtti& GetStageBuilderClass() const;
    /// get StageBuilder attributes
    const Attr::AttributeContainer& GetStageBuilderAttributes() const;

    /// attach a graphics entity
    void AttachEntity(const Ptr<GraphicsEntity>& entity);
    /// remove a graphics entity 
    void RemoveEntity(const Ptr<GraphicsEntity>& entity);
    /// get an array of all entities attached to the stage
    const Util::Array<Ptr<GraphicsEntity> >& GetEntities() const;
    /// get an array of entities filtered by type
    const Util::Array<Ptr<GraphicsEntity> >& GetEntitiesByType(GraphicsEntityType::Code type) const;
    
private:
    friend class GraphicsServer;

    /// set name of stage
    void SetName(const Util::StringAtom& name);
    /// set StageBuilder type
    void SetStageBuilderClass(const Core::Rtti& stageBuilderClass);
    /// set StageBuilder attributes
    void SetStageBuilderAttributes(const Attr::AttributeContainer& stageBuilderAttrs);
    /// setup the stage (waits for completion)
    void Setup();
    /// discard the stage (waits for completion)
    void Discard();
    /// call this method per-frame
    void OnFrame();

    Util::StringAtom name;
    const Core::Rtti* stageBuilderClass;
    Attr::AttributeContainer stageBuilderAttrs;
    Graphics::Handle stageHandle;
    Util::Array<Ptr<GraphicsEntity> > entities;
    Util::Array<Ptr<GraphicsEntity> > entitiesByType[GraphicsEntityType::NumTypes];
    Util::Array<Ptr<GraphicsEntity> > pendingEntities;
    Util::Array<Ptr<ModelEntity> > animationDrivenEntities;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
Stage::IsValid() const
{
    return this->stageHandle.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
inline void
Stage::SetName(const Util::StringAtom& n)
{
    this->name = n;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom&
Stage::GetName() const
{
    return this->name;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Stage::SetStageBuilderClass(const Core::Rtti& cl)
{
    this->stageBuilderClass = &cl;
}

//------------------------------------------------------------------------------
/**
*/
inline const Core::Rtti&
Stage::GetStageBuilderClass() const
{
    n_assert(0 != this->stageBuilderClass);
    return *(this->stageBuilderClass);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Stage::SetStageBuilderAttributes(const Attr::AttributeContainer& attrs)
{
    this->stageBuilderAttrs = attrs;
}

//------------------------------------------------------------------------------
/**
*/
inline const Attr::AttributeContainer&
Stage::GetStageBuilderAttributes() const
{
    return this->stageBuilderAttrs;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Ptr<GraphicsEntity> >& 
Stage::GetEntities() const
{
    return this->entities;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Ptr<GraphicsEntity> >& 
Stage::GetEntitiesByType(GraphicsEntityType::Code entityType) const
{
    n_assert((entityType >= 0) && (entityType < GraphicsEntityType::NumTypes));
    return this->entitiesByType[entityType];
}

} // namespace Graphics
//------------------------------------------------------------------------------
