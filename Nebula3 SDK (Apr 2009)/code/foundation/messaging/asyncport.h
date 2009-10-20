#pragma once
//------------------------------------------------------------------------------
/**
    @class Messaging::AsyncPort
  
    The AsyncPort class runs its handlers in a separate thread, so that
    message processing happens in a separate thread and doesn't block
    the main thread. Instead, subclasses of AsyncPort are responsible for 
    creating the handler thread and creating and attaching the handlers 
    to the port.
      
    (C) 2006 Radon Labs GmbH
*/    
#include "core/refcounted.h"
#include "core/ptr.h"
#include "messaging/message.h"
#include "messaging/handler.h"
#include "threading/thread.h"
#include "threading/safequeue.h"
#include "threading/criticalsection.h"

//------------------------------------------------------------------------------
namespace Messaging
{
class AsyncPort : public Core::RefCounted
{
    __DeclareClass(AsyncPort);
public:
    /// behaviour enum
    enum Behaviour
    {
        WaitForMessage,                 //> wait until message arrives
        WaitForMessageOrTimeOut,        //> wait until message arrives, or time-out is triggered
        DoNotWait,                      //> never wait for messages
    };

    /// constructor
    AsyncPort();
    /// destructor
    virtual ~AsyncPort();
    
    /// set the name of the async port (required)
    void SetName(const Util::String& n);
    /// get the name of the async port
    const Util::String& GetName() const;
    /// set optional thread priority
    void SetThreadPriority(Threading::Thread::Priority pri);
    /// get optional thread priority
    Threading::Thread::Priority GetThreadPriority() const;
    /// set optional thread stack size
    void SetThreadStackSize(unsigned int s);
    /// get optional thread stack size
    unsigned int GetThreadStackSize() const;
    /// set the thread cpu core id
    void SetThreadCpuCoreId(System::Cpu::CoreId coreId);
    /// get the thread cpu core id
    System::Cpu::CoreId GetThreadCpuCoreId() const;
    /// set the port's wait-behaviour (default is Wait)
    void SetBehaviour(Behaviour b);
    /// get the port's behaviour
    Behaviour GetBehaviour() const;
    /// set the time-out in milliseconds
    void SetWaitTimeout(int ms);
    /// get the time-out value
    int GetWaitTimeout() const;
        
    /// attach a handler to the port, may be called before or after Open()
    virtual void AttachHandler(const Ptr<Handler>& h);
    /// open the async port
    virtual void Open();
    /// close the async port
    virtual void Close();
    /// return true if port is open
    bool IsOpen() const;

    /// send an asynchronous message to the port
    void Send(const Ptr<Message>& msg);
    /// send a message and wait for completion
    void SendWait(const Ptr<Message>& msg);
    /// wait for a message to be handled
    void Wait(const Ptr<Message>& msg);
    /// peek a message whether it has been handled
    bool Peek(const Ptr<Message>& msg);
    /// cancel a pending message
    void Cancel(const Ptr<Message>& msg);
    /// wait until all pending messages have been handled
    void Flush();

    /// handler thread class (must be public because of some CodeWarrior fuckup)
    class HandlerThread : public Threading::Thread
    {
        __DeclareClass(HandlerThread);
    public:
        /// constructor
        HandlerThread();
        /// wait until handlers have been opened
        void WaitForHandlersOpened();
        /// set wait-for-message-behaviour
        void SetBehaviour(Behaviour b);
        /// set wait-timeout
        void SetWaitTimeout(int ms);
        /// called if thread needs a wakeup call before stopping
        virtual void EmitWakeupSignal();
        /// this method runs in the thread context
        virtual void DoWork();
        /// attach a message handler (called by OnCreateHandlers())
        void AttachHandler(const Ptr<Handler>& h);
        /// clear all attached message handlers
        void ClearHandlers();
        /// add a message to be handled
        void AddMessage(const Ptr<Message>& msg);
        /// cancel a pending message
        void CancelMessage(const Ptr<Message>& msg);
        /// wait for message to be handled
        void WaitForMessage(const Ptr<Message>& msg);
        /// wait for all pending messages to be handled
        void Flush();
        /// handle a single message (called by DoWork())
        bool HandleMessage(const Ptr<Message>& msg);
    
        friend class AsyncPort;
    
        Behaviour behaviour;
        int waitTimeout;
        Threading::Event handlersOpenedEvent;
        Threading::Event msgHandledEvent;
        Threading::SafeQueue<Ptr<Message> > msgQueue;
        Util::Array<Ptr<Message> > deferredMessages;
        Threading::CriticalSection handlersCritSect;
        Util::Array<Ptr<Handler> > handlers;
    };

protected:
    /// derive in subclass, create and attach handlers from here
    virtual void OnCreateHandlers();

private:
    /// clear all attached message handlers
    void ClearHandlers();

    Ptr<HandlerThread> thread;
    bool isOpen;
    Behaviour behaviour;
    int waitTimeOut;
};

//------------------------------------------------------------------------------
/**
*/
inline void
AsyncPort::SetName(const Util::String& n)
{
    this->thread->SetName(n);
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::String&
AsyncPort::GetName() const
{
    return this->thread->GetName();
}

//------------------------------------------------------------------------------
/**
*/
inline bool
AsyncPort::IsOpen() const
{
    return this->isOpen;
}

//------------------------------------------------------------------------------
/**
*/
inline void
AsyncPort::SetThreadPriority(Threading::Thread::Priority p)
{
    this->thread->SetPriority(p);
}

//------------------------------------------------------------------------------
/**
*/
inline Threading::Thread::Priority
AsyncPort::GetThreadPriority() const
{
    return this->thread->GetPriority();
}

//------------------------------------------------------------------------------
/**
*/
inline void
AsyncPort::SetThreadStackSize(unsigned int s)
{
    this->thread->SetStackSize(s);
}

//------------------------------------------------------------------------------
/**
*/
inline unsigned int
AsyncPort::GetThreadStackSize() const
{
    return this->thread->GetStackSize();
}

//------------------------------------------------------------------------------
/**
*/
inline void
AsyncPort::SetThreadCpuCoreId(System::Cpu::CoreId coreId)
{
    this->thread->SetCoreId(coreId);
}

//------------------------------------------------------------------------------
/**
*/
inline System::Cpu::CoreId
AsyncPort::GetThreadCpuCoreId() const
{
    return this->thread->GetCoreId();
}

//------------------------------------------------------------------------------
/**
*/
inline void
AsyncPort::SetBehaviour(Behaviour b)
{
    this->behaviour = b;
}

//------------------------------------------------------------------------------
/**
*/
inline AsyncPort::Behaviour
AsyncPort::GetBehaviour() const
{
    return this->behaviour;
}

//------------------------------------------------------------------------------
/**
*/
inline void
AsyncPort::SetWaitTimeout(int ms)
{
    this->waitTimeOut = ms;
}

//------------------------------------------------------------------------------
/**
*/
inline int
AsyncPort::GetWaitTimeout() const
{
    return this->waitTimeOut;
}

} // namespace Messaging
//------------------------------------------------------------------------------




