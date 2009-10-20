//------------------------------------------------------------------------------
//  audiointerface.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "audio/audiointerface.h"
#include "audio/audiohandler.h"

namespace Audio
{
__ImplementClass(Audio::AudioInterface, 'AUDI', Interface::InterfaceBase);
__ImplementInterfaceSingleton(Audio::AudioInterface);

using namespace Interface;
using namespace Messaging;
using namespace Threading;

//------------------------------------------------------------------------------
/**
*/
AudioInterface::AudioInterface()
{
    __ConstructSingleton;
    this->SetThreadCpuCoreId(System::Cpu::AudioThreadCore);
    this->mainThreadId = Thread::GetMyThreadId();
}    

//------------------------------------------------------------------------------
/**
*/
AudioInterface::~AudioInterface()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
AudioInterface::Open()
{
    // the audio device needs to be triggered from time to time, but we
    // also need to react immediately to messages
    this->SetBehaviour(WaitForMessageOrTimeOut);
    this->SetWaitTimeout(int(AudioHandler::AudioTriggerInterval * 1000.0));
    this->SetName("AudioInterface Thread");
    this->batchMessage = BatchMessage::Create();
    InterfaceBase::Open();
}

//------------------------------------------------------------------------------
/**
*/
void
AudioInterface::Close()
{
    InterfaceBase::Close();
    this->batchMessage = 0;    
}

//------------------------------------------------------------------------------
/**
*/
void
AudioInterface::OnCreateHandlers()
{
    Ptr<AudioHandler> audioHandler = AudioHandler::Create();
    this->AttachHandler(audioHandler.cast<Messaging::Handler>());
}

//------------------------------------------------------------------------------
/**
    Send a batched message. The message will not be sent to the audio thread
    immediately, but instead will be collected in a batch message for later
    sending. Use this method to reduce communication overhead
    between the main and audio thread by sending only one batch message
    instead of many messages. This method may only be called from the 
    main thread!
*/
void
AudioInterface::SendBatched(const Ptr<Message>& msg)
{
    #if NEBULA3_DEBUG
    n_assert(Thread::GetMyThreadId() == this->mainThreadId);
    #endif
    this->batchMessage->AddMessage(msg);
}

//------------------------------------------------------------------------------
/**
    Send off the batched messages to the audio thread. This reduces 
    thread synchronization overhead dramatically if many messages must
    be sent per-frame. Only the main thread may call this method.
*/
void
AudioInterface::FlushBatchedMessages()
{
    #if NEBULA3_DEBUG
    n_assert(Thread::GetMyThreadId() == this->mainThreadId);
    #endif

    // send current batch message
    this->Send(this->batchMessage.cast<Message>());

    // create a new, empty batch message
    this->batchMessage = BatchMessage::Create();
}

} // namespace Audio


