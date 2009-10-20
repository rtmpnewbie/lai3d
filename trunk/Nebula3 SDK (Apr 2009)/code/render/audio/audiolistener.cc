//------------------------------------------------------------------------------
//  audiolistener.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "audio/audiolistener.h"
#include "audio/audioprotocol.h"
#include "audio/audiointerface.h"

namespace Audio
{
__ImplementClass(Audio::AudioListener, 'AUDL', Core::RefCounted);
__ImplementSingleton(Audio::AudioListener);

using namespace Math;
using namespace Messaging;

//------------------------------------------------------------------------------
/**
*/
AudioListener::AudioListener() :
    transform(matrix44::identity()),
    velocity(vector::nullvec())
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
AudioListener::~AudioListener()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
AudioListener::OnFrame()
{
    // send an UpdateListener message to the audio thread
    Ptr<Audio::UpdateListener> msg = Audio::UpdateListener::Create();
    msg->SetTransform(this->transform);
    msg->SetVelocity(this->velocity);
    AudioInterface::Instance()->SendBatched(msg.cast<Message>());
}

} // namespace Audio