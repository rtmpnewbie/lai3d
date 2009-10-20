//------------------------------------------------------------------------------
//  audioserver.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "audio/audioserver.h"
#include "audio/audioprotocol.h"
#include "audio/audiointerface.h"

namespace Audio
{
__ImplementClass(Audio::AudioServer, 'AUSV', Core::RefCounted);
__ImplementSingleton(Audio::AudioServer);

using namespace Util;
using namespace Messaging;
using namespace Resources;

//------------------------------------------------------------------------------
/**
*/
AudioServer::AudioServer() :
    isOpen(false),
    adapterIndex(DefaultAdapter),
    globalSettingsURI("audio:globals.xgs")
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
AudioServer::~AudioServer()
{
    n_assert(!this->IsOpen());
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
Array<CoreAudio::AudioAdapterInfo>
AudioServer::GetAdapterInfos() const
{
    Ptr<Audio::GetAudioAdapterInfos> msg = Audio::GetAudioAdapterInfos::Create();
    AudioInterface::Instance()->SendWait(msg.cast<Message>());
    n_assert(msg->Handled());
    return msg->GetResult();
}

//------------------------------------------------------------------------------
/**
*/
void
AudioServer::Open()
{
    n_assert(!this->IsOpen());

    // setup the CoreAudio device
    Ptr<Audio::SetupAudio> msg = Audio::SetupAudio::Create();
    msg->SetAdapterIndex(this->adapterIndex);
    msg->SetGlobalSettingsURI(this->globalSettingsURI);
    AudioInterface::Instance()->SendWait(msg.cast<Message>());
    n_assert(msg->Handled());

    // create an 3d listener singleton
    this->audioListener = AudioListener::Create();

    this->isOpen = true;
}

//------------------------------------------------------------------------------
/**
*/
void
AudioServer::Close()
{
    n_assert(this->IsOpen());
    this->audioListener = 0;
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
void
AudioServer::OnFrame()
{
    n_assert(this->IsOpen());

    // update the audio listener
    this->audioListener->OnFrame();

    // need to update audio emitters
    // FIXME: find a way to not iterate through audio emitters, 
    // instead only update them if necessary?
    IndexT i;
    for (i = 0; i < this->audioEmitters.Size(); i++)
    {
        this->audioEmitters[i]->OnFrame();
    }

    // send off batched audio messages to the audio thread
    AudioInterface::Instance()->FlushBatchedMessages();
}

//------------------------------------------------------------------------------
/**
*/
bool
AudioServer::LoadSoundBank(const ResourceId& resId, bool streaming)
{
    n_assert(this->IsOpen());
    Ptr<Audio::LoadSoundBank> msg = Audio::LoadSoundBank::Create();
    msg->SetResourceId(resId);
    msg->SetStreaming(streaming);
    AudioInterface::Instance()->SendWait(msg.cast<Message>());
    n_assert(msg->Handled());
    return msg->GetResult();
}

//------------------------------------------------------------------------------
/**
*/
void
AudioServer::UnloadSoundBank(const ResourceId& resId)
{
    n_assert(this->IsOpen());
    Ptr<Audio::UnloadSoundBank> msg = Audio::UnloadSoundBank::Create();
    msg->SetResourceId(resId);
    AudioInterface::Instance()->SendWait(msg.cast<Message>());
    n_assert(msg->Handled());
}

//------------------------------------------------------------------------------
/**
*/
void
AudioServer::RegisterAudioEmitter(const Ptr<AudioEmitter>& audioEmitter)
{
    n_assert(InvalidIndex == this->audioEmitters.BinarySearchIndex(audioEmitter));
    this->audioEmitters.InsertSorted(audioEmitter);
}

//------------------------------------------------------------------------------
/**
*/
void
AudioServer::UnregisterAudioEmitter(const Ptr<AudioEmitter>& audioEmitter)
{
    IndexT index = this->audioEmitters.BinarySearchIndex(audioEmitter);
    n_assert(InvalidIndex != index);
    this->audioEmitters.EraseIndex(index);
}

} // namespace Audio
