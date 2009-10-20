#pragma once
//------------------------------------------------------------------------------
/**
    @class Audio::AudioInterface
  
    Implements the interface to the CoreAudio threaded subsystem. Apps
    don't call directly into AudioInterface, instead use the provided 
    front-end classes in the Audio subsystem.
    
    (C) 2008 Radon Labs GmbH
*/    
#include "interface/interfacebase.h"
#include "core/singleton.h"
#include "messaging/batchmessage.h"

//------------------------------------------------------------------------------
namespace Audio
{
class AudioInterface : public Interface::InterfaceBase
{
    __DeclareClass(AudioInterface);
    __DeclareInterfaceSingleton(AudioInterface);
public:
    /// constructor
    AudioInterface();
    /// destructor
    virtual ~AudioInterface();

    /// open the interface object
    virtual void Open();
    /// close the interface object
    virtual void Close();

    /// send a batched messages (call FlushBatchedMessages() to send them to audio thread)
    void SendBatched(const Ptr<Messaging::Message>& msg);
    /// flush batched messages, call once per frame
    void FlushBatchedMessages();

protected:
    /// derive in subclass, create and attach handlers from here
    virtual void OnCreateHandlers();

    Threading::ThreadId mainThreadId;
    Ptr<Messaging::BatchMessage> batchMessage;
};

} // namespace Audio
//------------------------------------------------------------------------------
