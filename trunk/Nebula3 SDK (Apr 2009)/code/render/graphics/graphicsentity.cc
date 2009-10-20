//------------------------------------------------------------------------------
//  graphicsentity.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/graphicsentity.h"
#include "graphics/graphicsinterface.h"
#include "graphics/stage.h"

namespace Graphics
{
__ImplementClass(Graphics::GraphicsEntity, 'GREP', Core::RefCounted);

using namespace Math;
using namespace Messaging;

//------------------------------------------------------------------------------
/**
*/
GraphicsEntity::GraphicsEntity() :
    transform(matrix44::identity()),
    type(GraphicsEntityType::InvalidType),
    isVisible(true),
    graphicsEntityHandle(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
GraphicsEntity::~GraphicsEntity()
{
    n_assert(!this->IsValid());
}

//------------------------------------------------------------------------------
/**
*/
bool
GraphicsEntity::IsValid() const
{
    return this->stage.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<Stage>&
GraphicsEntity::GetStage() const
{
    return this->stage;
}

//------------------------------------------------------------------------------
/**
    Setup the graphics entity. Subclasses must send a specific
    creation message in this method. This method is called from
    StageProxy::AttachEntityProxy().
*/
void
GraphicsEntity::Setup(const Ptr<Stage>& stage_)
{
    n_assert(!this->IsValid());
    n_assert(stage_.isvalid());
    n_assert(!this->IsEntityHandleValid());
    this->stage = stage_;
}

//------------------------------------------------------------------------------
/**
    Discard the server-side graphics entity. This method is called from
    StageProxy::RemoveEntityProxy(). If the server-side entity hasn't been
    created yet, this method needs to wait for completion, since the
    entity handle won't be available yet!
*/
void
GraphicsEntity::Discard()
{
    n_assert(this->IsValid());
    this->stage = 0;

    // first check if we need to wait for completion of the create message
    if (!this->IsEntityHandleValid())
    {
        n_assert(this->pendingCreateMsg.isvalid());
        
        // first cancel all pending messages, they are no longer needed
        this->pendingMessages.Clear();

        // now wait for the render thread to create the entity
        this->ValidateEntityHandle(true);
        n_assert(this->IsEntityHandleValid());
    }

    // alright, create and send off the Discard message
    Ptr<DiscardGraphicsEntity> msg = DiscardGraphicsEntity::Create();
    msg->SetEntityHandle(this->graphicsEntityHandle);
    GraphicsInterface::Instance()->Send(msg.cast<Message>());

	// clear the entity handle, since it is invalid now
	this->graphicsEntityHandle = 0;
}

//------------------------------------------------------------------------------
/**
    Tries to validate the entity handle by checking whether the 
    pending creation message has already been handled by the server side.
    If the wait flag is true, the method will wait until the entity
    has been created on the server side. The default is to NOT wait.
    If the graphics entity handle becomes valid, all pending messages
    will be sent off to the server-side entity.
*/
void
GraphicsEntity::ValidateEntityHandle(bool wait)
{
    if (!this->IsEntityHandleValid() && this->pendingCreateMsg.isvalid())
    {
        // need to wait for the server-side creation? this should
        // only be the case for the final discard message
        if (wait && (!this->pendingCreateMsg->Handled()))
        {
            GraphicsInterface::Instance()->Wait(this->pendingCreateMsg.cast<Message>());
            n_assert(this->pendingCreateMsg->Handled());
        }
        
        // if render thread has created our entity, read handle from
        // message, delete message and send off any pending messages
        if (this->pendingCreateMsg->Handled())
        {
            this->graphicsEntityHandle = this->pendingCreateMsg->GetEntityHandle();
            n_assert(this->IsEntityHandleValid());
            this->pendingCreateMsg = 0;

            // send off any pending messages which have been created before
            // the server-side entity was created
            IndexT i;
            for (i = 0; i < this->pendingMessages.Size(); i++)
            {
                this->pendingMessages[i]->SetEntityHandle(this->graphicsEntityHandle);
                GraphicsInterface::Instance()->SendBatched(this->pendingMessages[i].cast<Message>());
            }
            this->pendingMessages.Clear();
        }
    }
}

//------------------------------------------------------------------------------
/**
    Send a generic GraphicsEntityMessage to the server-side entity. Do not
    initialize the entity handle of the message before calling this method,
    since the server-side entity doesn't have to exist yet. In this case,
    the message will be queued up and sent off as soon as the server-side
    entity becomes valid.
*/
void
GraphicsEntity::SendMsg(const Ptr<GraphicsEntityMessage>& msg)
{
    // try to validate the entity handle, do not wait for completion!
    this->ValidateEntityHandle(false);

    // if the entity handle is valid, we can directly send off the message,
    // otherwise we need to queue up the message for later execution
    if (this->IsEntityHandleValid())
    {
        msg->SetEntityHandle(this->graphicsEntityHandle);
        GraphicsInterface::Instance()->SendBatched(msg.cast<Message>());
    }
    else
    {
        // need to queue the message for later execution
        this->pendingMessages.Append(msg);
    }
}

//------------------------------------------------------------------------------
/**
    This method must be called from the Setup() method of a subclass to
    send off a specific creation message. The message will be stored
    in the proxy to get the entity handle back later when the server-side
    graphics entity has been created.
*/
void
GraphicsEntity::SendCreateMsg(const Ptr<CreateGraphicsEntity>& msg)
{
    n_assert(!this->pendingCreateMsg.isvalid());
    n_assert(!this->IsEntityHandleValid());
    this->pendingCreateMsg = msg;
    GraphicsInterface::Instance()->Send(msg.cast<Message>());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsEntity::SetTransform(const matrix44& m)
{
    this->transform = m;
    this->OnTransformChanged();
}

//------------------------------------------------------------------------------
/**
    Called by SetTransform(). This gives subclasses a chance to react
    to changes on the transformation matrix.
*/
void
GraphicsEntity::OnTransformChanged()
{
    // may need to notify server-side entity about transform change
    Ptr<Graphics::UpdateTransform> msg = Graphics::UpdateTransform::Create();
    msg->SetTransform(this->transform);
    this->SendMsg(msg.cast<GraphicsEntityMessage>());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsEntity::SetVisible(bool b)
{
    this->isVisible = b;
    Ptr<Graphics::SetVisibility> msg = Graphics::SetVisibility::Create();
    msg->SetVisible(b);
    this->SendMsg(msg.cast<GraphicsEntityMessage>());
}

} // namespace Graphics
