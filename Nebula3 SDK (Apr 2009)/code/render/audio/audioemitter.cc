//------------------------------------------------------------------------------
//  audioemitter.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "audio/audioemitter.h"
#include "audio/audioserver.h"
#include "audio/audioprotocol.h"
#include "audio/audiointerface.h"

namespace Audio
{
__ImplementClass(Audio::AudioEmitter, 'AUEM', Core::RefCounted);

using namespace Util;
using namespace Math;
using namespace CoreAudio;
using namespace Messaging;

//------------------------------------------------------------------------------
/**
*/
AudioEmitter::AudioEmitter() :
    isValid(false),
    transform(matrix44::identity()),
    velocity(vector::nullvec()),
    stateMask(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
AudioEmitter::~AudioEmitter()
{
    n_assert(!this->IsValid());
}

//------------------------------------------------------------------------------
/**
*/
void
AudioEmitter::Setup()
{
    n_assert(!this->IsValid());
    n_assert(!this->cueHandle.isvalid());
    n_assert(!this->pendingPlayMsg.isvalid());
    n_assert(!this->pendingUpdateMsg.isvalid());
    n_assert(!this->cueId.IsValid());
    n_assert(0 == this->stateMask);

    AudioServer::Instance()->RegisterAudioEmitter(this);

    this->isValid = true;
}

//------------------------------------------------------------------------------
/**
*/
void
AudioEmitter::Discard()
{
    n_assert(this->IsValid());
    
    if (this->IsPlaying())
    {
        this->Stop();
    }
    n_assert(!this->pendingPlayMsg.isvalid());
    n_assert(!this->pendingUpdateMsg.isvalid());
    n_assert(!this->cueId.IsValid());
    this->stateMask = 0;

    AudioServer::Instance()->UnregisterAudioEmitter(this);
    this->isValid = false;
}

//------------------------------------------------------------------------------
/**
*/
void
AudioEmitter::Play(const CueId& cid)
{
    n_assert(this->IsValid());
    n_assert(!this->IsPlaying());
    n_assert(cid.IsValid());
    n_assert(!this->cueHandle.isvalid());

    this->cueId = cid;
    this->stateMask = SoundState::Playing;

    // send off a new PlayCue message, store the message because
    // we need to read back the cue handle later
    this->pendingPlayMsg = Audio::PlayCue::Create();
    this->pendingPlayMsg->SetCueId(this->cueId);
    this->pendingPlayMsg->SetStartPlaying(true);
    this->pendingPlayMsg->SetEnable3D(true);
    this->pendingPlayMsg->SetTransform(this->transform);
    this->pendingPlayMsg->SetVelocity(this->velocity);
    AudioInterface::Instance()->SendBatched(this->pendingPlayMsg.cast<Message>());
}

//------------------------------------------------------------------------------
/**
*/
void
AudioEmitter::Stop()
{
    n_assert(this->IsValid());
    n_assert(this->IsPlaying());
    
    // if the cue hasn't even been created yet on the server side,
    // we need to wait until its ready
    this->ValidateCueHandle(true);
    n_assert(!this->pendingPlayMsg.isvalid());

    // the cue handle doesn't have to be valid if the cue is already exhausted,
    // but if it is we need to send a stop message
    if (this->cueHandle.isvalid())
    {
        Ptr<Audio::StopCue> msg = Audio::StopCue::Create();
        msg->SetCueHandle(this->cueHandle);
        AudioInterface::Instance()->SendBatched(msg.cast<Message>());
        this->cueHandle = 0;
    }

    // clean up everything
    this->pendingUpdateMsg = 0;
    this->stateMask = SoundState::Stopped;
    this->cueId.Clear();
}

//------------------------------------------------------------------------------
/**
*/
void
AudioEmitter::Pause()
{
    n_assert(this->IsValid());

    // if the cue hasn't even been created yet on the server side,
    // we need to wait until its ready
    this->ValidateCueHandle(true);
    n_assert(!this->pendingPlayMsg.isvalid());

    // the cue handle doesn't have to be valid if the cue is already exhausted,
    // but if it is we need to send a stop message
    if (this->cueHandle.isvalid())
    {
        Ptr<Audio::PauseCue> msg = Audio::PauseCue::Create();
        msg->SetCueHandle(this->cueHandle);
        AudioInterface::Instance()->SendBatched(msg.cast<Message>());

        this->stateMask = SoundState::Paused;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
AudioEmitter::Resume()
{
    n_assert(this->IsValid());

    Ptr<Audio::ResumeCue> msg = Audio::ResumeCue::Create();
    msg->SetCueHandle(this->cueHandle);
    AudioInterface::Instance()->SendBatched(msg.cast<Message>());

    this->stateMask = SoundState::Playing;
}

//------------------------------------------------------------------------------
/**
    This does the per-frame-stuff for this audio emitter, reading results
    back from the audio thread, and sending UpdateCue messages with
    updated transform and velocity (the Cue state is piggybacked as a
    return value in the UpdateCue message).
*/
void
AudioEmitter::OnFrame()
{
    n_assert(this->IsValid());

    // only do something if we're between Play()/Stop()
    if (this->IsPlaying())
    {
        // first check if we've been freshly created
        this->ValidateCueHandle(false);

        if (this->pendingPlayMsg.isvalid() && this->pendingPlayMsg->Handled())
        {
            this->cueHandle = this->pendingPlayMsg->GetCueHandle();
            this->pendingPlayMsg = 0;
        }

        if (this->cueHandle.isvalid())
        {
            // check if our audio-thread-cue has expired
            bool expired = false;
            if (this->pendingUpdateMsg.isvalid() && this->pendingUpdateMsg->Handled())
            {
                expired = 0 != (this->pendingUpdateMsg->GetStateMask() & SoundState::Stopped);
                this->pendingUpdateMsg = 0;
            }

            // if we're expired (the audio-thread-cue has stopped playing our cue)
            // we need to "simulate" a stop
            if (expired)
            {
                this->cueHandle = 0;
                this->Stop();
            }
            else if (!this->pendingUpdateMsg.isvalid())
            {
                // check if a new update message should be sent, don't even bother
                // if the previous hasn't been processed yet
                n_assert(this->cueHandle.isvalid());
                this->pendingUpdateMsg = Audio::UpdateCue::Create();
                this->pendingUpdateMsg->SetCueHandle(this->cueHandle);
                this->pendingUpdateMsg->SetTransform(this->transform);
                this->pendingUpdateMsg->SetVelocity(this->velocity);
                AudioInterface::Instance()->SendBatched(this->pendingUpdateMsg.cast<Message>());
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
    This tries to validate our cue handle by checking whether the
    PlayCue message has already been handled. Optionally the method
    waits for completion of the message, this is required by the Stop method
    only!
*/
void
AudioEmitter::ValidateCueHandle(bool wait)
{
    if ((!this->cueHandle.isvalid()) && this->pendingPlayMsg.isvalid())
    {
        // need to wait for completion?
        if (wait && this->pendingPlayMsg->Handled())
        {
            AudioInterface::Instance()->Wait(this->pendingPlayMsg.cast<Message>());
            n_assert(this->pendingPlayMsg->Handled());
        }

        // check if play message has been handled yet
        if (this->pendingPlayMsg->Handled())
        {
            this->cueHandle = this->pendingPlayMsg->GetCueHandle();
            n_assert(this->cueHandle.isvalid());
            this->pendingPlayMsg = 0;
        }
    }
}

} // namespace Audio