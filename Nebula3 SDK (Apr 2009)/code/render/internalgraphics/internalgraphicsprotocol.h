#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "messaging/message.h"
#include "internalgraphics/internalstage.h"

//------------------------------------------------------------------------------
namespace InternalGraphics
{
//------------------------------------------------------------------------------
class InternalGraphicsEntityEvent : public Messaging::Message
{
    __DeclareClass(InternalGraphicsEntityEvent);
    __DeclareMsgId;
public:
    InternalGraphicsEntityEvent() 
    { };
public:
    void SetGraphicsEntity(Ptr<InternalGraphicsEntity> val)
    {
        n_assert(!this->handled);
        this->graphicsentity = val;
    };
    Ptr<InternalGraphicsEntity> GetGraphicsEntity() const
    {
        return this->graphicsentity;
    };
private:
    Ptr<InternalGraphicsEntity> graphicsentity;
};
//------------------------------------------------------------------------------
class ModelInstanceCreated : public InternalGraphicsEntityEvent
{
    __DeclareClass(ModelInstanceCreated);
    __DeclareMsgId;
public:
    ModelInstanceCreated() 
    { };
};
} // namespace InternalGraphics
//------------------------------------------------------------------------------
