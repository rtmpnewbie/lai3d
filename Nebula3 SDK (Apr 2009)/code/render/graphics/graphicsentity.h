#pragma once
//------------------------------------------------------------------------------
/**
    @class Graphics::GraphicsEntity
    
    Client-side proxy of a InternalGraphics::InternalGraphicsEntity.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "internalgraphics/internalgraphicsentity.h"
#include "util/queue.h"
#include "messaging/message.h"
#include "graphics/graphicsprotocol.h"
#include "graphics/graphicsentitytype.h"

//------------------------------------------------------------------------------
namespace Graphics
{
class Stage;

class GraphicsEntity : public Core::RefCounted
{
    __DeclareClass(GraphicsEntity);
public:
    /// constructor
    GraphicsEntity();
    /// destructor
    virtual ~GraphicsEntity();
        
    /// return true if entity is valid (is attached to a stage)
    bool IsValid() const;
    /// get the entity type
    GraphicsEntityType::Code GetType() const;
    /// set the entity's world space transform
    void SetTransform(const Math::matrix44& m);
    /// get the entity's world space transform
    const Math::matrix44& GetTransform() const;
    /// set the entity's visibility
    void SetVisible(bool b);
    /// return true if entity is set to visible
    bool IsVisible() const;
    /// get the stage proxy this entity is attached to
    const Ptr<Stage>& GetStage() const;
    /// send a message to the server-side graphics entity
    void SendMsg(const Ptr<GraphicsEntityMessage>& msg);

    /// test if the entity handle is valid
    bool IsEntityHandleValid() const;
    /// get the entity handle
    const Handle& GetEntityHandle() const;

protected:
    friend class Stage;

    /// set graphics entity type, called from constructor of subclass
    void SetType(GraphicsEntityType::Code t);
    /// called by stage when entity should setup itself
    virtual void Setup(const Ptr<Stage>& stage);
    /// called by stage when entity should discard itself
    virtual void Discard();
    /// validate entity handle
    void ValidateEntityHandle(bool wait=false); 
    /// send off a specific create message from the subclass
    void SendCreateMsg(const Ptr<CreateGraphicsEntity>& msg);
    /// called when transform matrix changed
    virtual void OnTransformChanged();
    Math::matrix44 transform;
    Ptr<Stage> stage;
    GraphicsEntityType::Code type;
    bool isVisible;
    Handle graphicsEntityHandle;    
    Ptr<CreateGraphicsEntity> pendingCreateMsg;
    Util::Array<Ptr<GraphicsEntityMessage> > pendingMessages; 
}; 

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
GraphicsEntity::GetTransform() const
{
    return this->transform;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
GraphicsEntity::IsVisible() const
{
    return this->isVisible;
}

//------------------------------------------------------------------------------
/**
*/
inline const Handle&
GraphicsEntity::GetEntityHandle() const
{
    return this->graphicsEntityHandle;
}

//------------------------------------------------------------------------------
/**
*/
inline void
GraphicsEntity::SetType(GraphicsEntityType::Code t)
{
    this->type = t;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
GraphicsEntity::IsEntityHandleValid() const
{
    return this->graphicsEntityHandle.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
inline GraphicsEntityType::Code
GraphicsEntity::GetType() const
{
    return this->type;
}

} // namespace Graphics
//------------------------------------------------------------------------------

    
