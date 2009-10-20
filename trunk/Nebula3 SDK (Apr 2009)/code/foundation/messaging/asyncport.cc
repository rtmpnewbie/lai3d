//------------------------------------------------------------------------------
//  asyncport.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "messaging/asyncport.h"
#include "messaging/batchmessage.h"

namespace Messaging
{
__ImplementClass(Messaging::AsyncPort, 'ASPT', Core::RefCounted);
__ImplementClass(Messaging::AsyncPort::HandlerThread, 'ASHT', Threading::Thread);

using namespace Util;
using namespace Threading;

//------------------------------------------------------------------------------
/**
*/
AsyncPort::AsyncPort() :
    isOpen(false),
    behaviour(WaitForMessage),
    waitTimeOut(0)
{
    // create a thread object (but don't start it!)
    this->thread = HandlerThread::Create();
}

//------------------------------------------------------------------------------
/**
*/
AsyncPort::~AsyncPort()
{
    if (this->IsOpen())
    {
        this->Close();
    }
    this->thread = 0;
}

//------------------------------------------------------------------------------
/**
    Open the async port. The async port needs a valid name before it
    is opened. Messages can only be sent to an open port. This method
    will first call OnCreateHandlers() and then create the handler
    thread.
*/
void
AsyncPort::Open()
{
    n_assert(!this->IsOpen());

    // let subclass create and attach message handlers
    this->OnCreateHandlers();
    
    // start the handler thread, and wait until handlers are open
    this->thread->SetBehaviour(this->behaviour);
    this->thread->SetWaitTimeout(this->waitTimeOut);
    this->thread->Start();
    this->thread->WaitForHandlersOpened();

    this->isOpen = true;
}

//------------------------------------------------------------------------------
/**
    Closes the async port.
*/
void
AsyncPort::Close()
{
    n_assert(this->IsOpen());

    // wait until all messages have been handled before stopping the thread
    this->Flush();
    this->thread->Stop();
    this->thread->ClearHandlers();
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
    Called by OnCreateHandlers() method of subclass to attach a handler
    to the port.
*/
void
AsyncPort::AttachHandler(const Ptr<Handler>& h)
{
    this->thread->AttachHandler(h);
}

//------------------------------------------------------------------------------
/**
    This message is called by Open() when message handlers should be
    created and attached via AttachHandler(). Override this method
    in a subclass.
*/
void
AsyncPort::OnCreateHandlers()
{
    // empty, override in subclass!
}

//------------------------------------------------------------------------------
/**
    Handle an asynchronous message and return immediately. If the caller
    expects any results from the message he can poll with the AsyncPort::Peek()
    method, or he may wait for the message to be handled with the 
    AsyncPort::Wait() method.
*/
void
AsyncPort::Send(const Ptr<Message>& msg)
{
    n_assert(msg.isvalid());
    n_assert(this->thread.isvalid());
    n_assert(!msg->Handled());
    this->thread->AddMessage(msg);
}

//------------------------------------------------------------------------------
/**
    Send an asynchronous message and wait until the message has been
    handled.
*/
void
AsyncPort::SendWait(const Ptr<Message>& msg)
{
    this->Send(msg);
    this->Wait(msg);
}

//------------------------------------------------------------------------------
/**
    This method peeks whether a message has already been handled. If the
    caller expects any return arguments from the message handling it 
    can use this message to check whether the results are ready using
    this non-blocking method. The caller can also wait for the results
    to become ready using the Wait() method.
*/
bool
AsyncPort::Peek(const Ptr<Message>& msg)
{
    n_assert(msg.isvalid());
    return msg->Handled();
}

//------------------------------------------------------------------------------
/**
    This method will cancel a pending message.
*/
void
AsyncPort::Cancel(const Ptr<Message>& msg)
{
    n_assert(msg.isvalid());
    n_assert(this->thread.isvalid());
    this->thread->CancelMessage(msg);
}

//------------------------------------------------------------------------------
/**
    This method will wait until a message has been handled. If the caller
    expects any return arguments from the message handling it can use
    this method to wait for the results.
*/
void
AsyncPort::Wait(const Ptr<Message>& msg)
{
    n_assert(msg.isvalid());
    n_assert(this->thread.isvalid());

    // only wait if the message hasn't been handled yet
    if (!msg->Handled())
    {
        this->thread->WaitForMessage(msg);
        n_assert(msg->Handled());
    }
}

//------------------------------------------------------------------------------
/**
    This method will wait until ALL pending messages have been handled.
    Note that this method will be called automatically before the
    AsyncPort shutsdown.
*/
void
AsyncPort::Flush()
{
    n_assert(this->thread.isvalid());
    this->thread->Flush();
}

//------------------------------------------------------------------------------
/**
    HandlerThread constructor.
*/
AsyncPort::HandlerThread::HandlerThread() :
    behaviour(AsyncPort::WaitForMessage),
    waitTimeout(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
AsyncPort::HandlerThread::SetBehaviour(Behaviour b)
{
    this->behaviour = b;
    if (DoNotWait == this->behaviour)
    {
        this->msgQueue.SetSignalOnEnqueueEnabled(false);
    }
    else
    {
        this->msgQueue.SetSignalOnEnqueueEnabled(true);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
AsyncPort::HandlerThread::SetWaitTimeout(int ms)
{
    this->waitTimeout = ms;
}

//------------------------------------------------------------------------------
/**
    Waits until the thread proc signals that handlers have been opened.
*/
void
AsyncPort::HandlerThread::WaitForHandlersOpened()
{
    n_assert(this->IsRunning());
    this->handlersOpenedEvent.Wait();
}

//------------------------------------------------------------------------------
/**
    This method is called by Thread::Stop() after setting the 
    stopRequest event and before waiting for the thread to stop.
*/
void
AsyncPort::HandlerThread::EmitWakeupSignal()
{
    // wake up our thread if it's waiting for messages
    this->msgQueue.Signal();
}

//------------------------------------------------------------------------------
/**
    Handle a single message in the handler thread. The method returns 
    true if the message has been handled / accepted by one of the
    attached message handlers.
*/
bool
AsyncPort::HandlerThread::HandleMessage(const Ptr<Message>& msg)
{
    bool msgHandled = false;

    // let each handler look at the message
    this->handlersCritSect.Enter();
    IndexT i;
    for (i = 0; i < this->handlers.Size(); i++)
    {    
        const Ptr<Handler>& curHandler = this->handlers[i];
        
        // open handler on demand (a handler may have
        // been added as the result of a message call,
        // so let's make sure it will be able to handle 
        // message as soon as possible
        if (!curHandler->IsOpen())
        {
            curHandler->Open();
        }
    
        // let handler handle the current message
        if (curHandler->HandleMessage(msg))
        {
            // message has been accepted by this handler
            if (msg->IsDeferred())
            {
                // message has been defered by the handler
                // and will be handled at some later time,
                // put the message into the deferred queue
                // and handle at some later DoWork() call
                this->deferredMessages.Append(msg);
            }
            else
            {
                // message has been handled normally
                msg->SetHandled(true);
                msgHandled = true;
            }
            
            // break after the first handler accepts the message
            break;
        }
    }
    this->handlersCritSect.Leave();
    return msgHandled;
}

//------------------------------------------------------------------------------
/**
    This implements the handler thread's do work method. It will
    just sit on the message queue waiting for incoming messages. When
    a message arrives, it will be routed through all attached handlers.
    Whenever a message has been handled, its handled flag will be set,
    and the msgHandledEvent will be signalled.
*/
void
AsyncPort::HandlerThread::DoWork()
{
    // open attached handlers and signal event when all handlers are open
    this->handlersCritSect.Enter();
    Array<Ptr<Handler> >::Iterator iter;
    for (iter = this->handlers.Begin(); iter < this->handlers.End(); iter++)
    {
        (*iter)->Open();
    }
    this->handlersCritSect.Leave();
    this->handlersOpenedEvent.Signal();

    // handle messages
    while (!this->ThreadStopRequested())
    {
        // flag will be set to true if at least one message has been handled
        bool msgHandled = false;
        
        // wait for the next message to arrive, note that 
        // this may also be signalled by the Stop() method to 
        // wake us up
        if (AsyncPort::WaitForMessage == this->behaviour)
        {
            this->msgQueue.Wait();
        }
        else if (AsyncPort::WaitForMessageOrTimeOut == this->behaviour)
        {
            this->msgQueue.WaitTimeout(this->waitTimeout);
        }
        else
        {
            Thread::YieldThread();
        }

        // handle deferred message from previous frames
        IndexT deferredIndex;
        for (deferredIndex = this->deferredMessages.Size() - 1; deferredIndex != InvalidIndex; deferredIndex--)
        {
            Ptr<Message> msg = this->deferredMessages[deferredIndex];
            if (msg->DeferredHandled())
            {
                this->deferredMessages.EraseIndex(deferredIndex);
                msg->SetHandled(true);
                msgHandled = true;
            }
        }

        // handle pending messages
        while (!this->msgQueue.IsEmpty())
        {
            Ptr<Message> msg = this->msgQueue.Dequeue();
            
            // special case: batch message?
            if (msg->CheckId(BatchMessage::Id))
            {
                // process batched messages
                const Array<Ptr<Message> >& msgBatch = msg.cast<BatchMessage>()->GetMessages();
                IndexT msgBatchIndex;
                for (msgBatchIndex = 0; msgBatchIndex < msgBatch.Size(); msgBatchIndex++)
                {
                    msgHandled |= this->HandleMessage(msgBatch[msgBatchIndex]);
                }
                msg->SetHandled(true);
                msgHandled = true;
            }
            else
            {
                // a normal message
                msgHandled |= this->HandleMessage(msg);
            }
        }

        // signal msgHandledEvent if at least one message has been handled
        if (msgHandled)
        {
            this->msgHandledEvent.Signal();
        }

        // call the DoWork-method on attached handlers:
        // NOTE: handlers may have been added during message handling!
        // we don't call DoWork on those since they are not open yet!
        this->handlersCritSect.Enter();
        for (iter = this->handlers.Begin(); iter < this->handlers.End(); iter++)
        {
            // make sure the handler is open, it may happen that handlers
            // are added as the result of a message, and such a handler
            // may not have been opened yet
            if (!(*iter)->IsOpen())
            {
                (*iter)->Open();
            }
            (*iter)->DoWork();
        }
        this->handlersCritSect.Leave();
    }

    // clear leftover deferred messages
    this->deferredMessages.Clear();

    // shutdown handlers
    this->handlersCritSect.Enter();
    for (iter = this->handlers.Begin(); iter < this->handlers.End(); iter++)
    {
        (*iter)->Close();
    }
    this->handlersCritSect.Leave();
}

//------------------------------------------------------------------------------
/**
    Attach a message handler to the port. This method may now be called
    from anywhere at any time.
*/
void
AsyncPort::HandlerThread::AttachHandler(const Ptr<Handler>& h)
{
    n_assert(h.isvalid());
    
    this->handlersCritSect.Enter();    
    n_assert(InvalidIndex == this->handlers.FindIndex(h));
    this->handlers.Append(h);
    this->handlersCritSect.Leave();
}

//------------------------------------------------------------------------------
/**
    This clears all attached message handlers.
*/
void
AsyncPort::HandlerThread::ClearHandlers()
{
    this->handlersCritSect.Enter();
    this->handlers.Clear();
    this->handlersCritSect.Leave();
}

//------------------------------------------------------------------------------
/**
    This adds a new message to the thread's message queue.
*/
void
AsyncPort::HandlerThread::AddMessage(const Ptr<Message>& msg)
{
    n_assert(msg.isvalid());
    this->msgQueue.Enqueue(msg);
}

//------------------------------------------------------------------------------
/**
    This removes a message from the thread's message queue, regardless
    of its state.
*/
void
AsyncPort::HandlerThread::CancelMessage(const Ptr<Message>& msg)
{
    n_assert(msg.isvalid());
    this->msgQueue.EraseMatchingElements(msg);
}

//------------------------------------------------------------------------------
/**
    This waits until the message given as argument has been handled.
*/
void
AsyncPort::HandlerThread::WaitForMessage(const Ptr<Message>& msg)
{
    while (!msg->Handled())
    {
        this->msgHandledEvent.Wait();
    }
}

//------------------------------------------------------------------------------
/**
    This waits until all pending messages have been handled.
*/
void
AsyncPort::HandlerThread::Flush()
{
    while (!this->msgQueue.IsEmpty())
    {
        this->msgHandledEvent.Wait();
    }
}

} // namespace Messaging
