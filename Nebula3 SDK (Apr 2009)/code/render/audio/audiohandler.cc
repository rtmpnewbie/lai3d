//------------------------------------------------------------------------------
//  audiohandler.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "audio/audiohandler.h"
#include "coreaudio/debug/coreaudiopagehandler.h"

namespace Audio
{
__ImplementClass(Audio::AudioHandler, 'AUDH', Interface::InterfaceHandlerBase);

using namespace Util;
using namespace CoreAudio;
using namespace Interface;
using namespace Timing;
using namespace Messaging;

// trigger the audio engine at most N times per second!
const Time AudioHandler::AudioTriggerInterval = 0.1;

//------------------------------------------------------------------------------
/**
*/
AudioHandler::AudioHandler() :
    isAudioRuntimeValid(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
AudioHandler::~AudioHandler()
{
    // empty
}            

//------------------------------------------------------------------------------
/**
*/
void
AudioHandler::Open()
{
    n_assert(!this->IsOpen());
    n_assert(!this->isAudioRuntimeValid);
    InterfaceHandlerBase::Open();

    // setup core runtime
    this->ioServer = IO::IoServer::Create();

#if __NEBULA3_HTTP__
    // setup http page handlers
    this->httpServerProxy = Http::HttpServerProxy::Create();
    this->httpServerProxy->Open();
    this->httpServerProxy->AttachRequestHandler(Debug::CoreAudioPageHandler::Create());
#endif

    // create the audio device, but don't open it yet
    this->audioDevice = AudioDevice::Create();

    // start the timer for measuring elapsed time
    this->timer.Start();
    this->time = this->timer.GetTime();
}

//------------------------------------------------------------------------------
/**
*/
void
AudioHandler::Close()
{
    n_assert(this->IsOpen());

    if (this->isAudioRuntimeValid)
    {
        this->ShutdownAudioRuntime();
    }
    this->audioDevice = 0;
#if __NEBULA3_HTTP__
    this->httpServerProxy->Close();
    this->httpServerProxy = 0;
#endif
    this->ioServer = 0;

    InterfaceHandlerBase::Close();
}

//------------------------------------------------------------------------------
/**
    Per-frame method. This is called by the when a new batch of messages
    is received, or at most after every 10th second.
*/
void
AudioHandler::DoWork()
{
    if (this->isAudioRuntimeValid)
    {
        // check if it's time to update the audio device
        Time curTime = this->timer.GetTime();
        Time diffTime = curTime - this->time;
        if (diffTime >= AudioTriggerInterval)
        {
            this->audioDevice->OnFrame();
        }
    }

#if __NEBULA3_HTTP__
    // allow audio-thread HttpRequests to be processed
    this->httpServerProxy->HandlePendingRequests();
#endif
}

//------------------------------------------------------------------------------
/**
    Setup the audio runtime, this method is called when the
    SetupAudio message is received from the main thread. 
*/
void
AudioHandler::SetupAudioRuntime(IndexT adapterIndex, const IO::URI& globalSettingsURI)
{
    n_assert(!this->isAudioRuntimeValid);
    n_assert(!this->audioDevice->IsOpen());

    this->audioDevice->SetAdapter(adapterIndex);
    this->audioDevice->SetGlobalSettingsURI(globalSettingsURI);
    this->audioDevice->Open();
    n_assert(this->audioDevice->IsOpen());

    this->isAudioRuntimeValid = true;
}

//------------------------------------------------------------------------------
/**
    Shutdown the audio runtime.
*/
void
AudioHandler::ShutdownAudioRuntime()
{
    n_assert(this->isAudioRuntimeValid);
    n_assert(this->audioDevice->IsOpen());
    this->audioDevice->Close();
    this->isAudioRuntimeValid = false;
}

//------------------------------------------------------------------------------
/**
*/
bool
AudioHandler::HandleMessage(const Ptr<Message>& msg)
{
    n_assert(msg.isvalid());
    if (msg->CheckId(SetupAudio::Id))
    {
        this->OnSetupAudio(msg.cast<SetupAudio>());
    }
    else if (msg->CheckId(GetAudioAdapterInfos::Id))
    {
        this->OnGetAudioAdapterInfos(msg.cast<GetAudioAdapterInfos>());
    }
    else if (msg->CheckId(LoadSoundBank::Id))
    {
        this->OnLoadSoundBank(msg.cast<LoadSoundBank>());
    }
    else if (msg->CheckId(UnloadSoundBank::Id))
    {
        this->OnUnloadSoundBank(msg.cast<UnloadSoundBank>());
    }
    else if (msg->CheckId(PlayCue::Id))
    {
        this->OnPlayCue(msg.cast<PlayCue>());
    }
    else if (msg->CheckId(UpdateCue::Id))
    {
        this->OnUpdateCue(msg.cast<UpdateCue>());
    }
    else if (msg->CheckId(StopCue::Id))
    {
        this->OnStopCue(msg.cast<StopCue>());
    }
    else if (msg->CheckId(StopCueInstances::Id))
    {
        this->OnStopCueInstances(msg.cast<StopCueInstances>());
    }
    else if (msg->CheckId(PlayWave::Id))
    {
        this->OnPlayWave(msg.cast<PlayWave>());
    }
    else if (msg->CheckId(StopWaveInstances::Id))
    {
        this->OnStopWaveInstances(msg.cast<StopWaveInstances>());
    }
    else if (msg->CheckId(UpdateListener::Id))
    {
        this->OnUpdateListener(msg.cast<UpdateListener>());
    }
    else if (msg->CheckId(PauseCue::Id))
    {
        this->OnPauseCue(msg.cast<PauseCue>());
    }
    else if (msg->CheckId(ResumeCue::Id))
    {
        this->OnResumeCue(msg.cast<ResumeCue>());
    }
    else if (msg->CheckId(SetGlobalVariable::Id))
    {
        this->OnSetGlobalVariable(msg.cast<SetGlobalVariable>());
    }
    else if (msg->CheckId(GetGlobalVariable::Id))
    {
        this->OnGetGlobalVariable(msg.cast<GetGlobalVariable>());
    }
    else
    {
        // unknown message
        return false;
    }
    // fallthrough: message was handled
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
AudioHandler::OnSetupAudio(const Ptr<SetupAudio>& msg)
{
    n_assert(!this->isAudioRuntimeValid);
    this->SetupAudioRuntime(msg->GetAdapterIndex(), msg->GetGlobalSettingsURI());
}

//------------------------------------------------------------------------------
/**
*/
void
AudioHandler::OnGetAudioAdapterInfos(const Ptr<Audio::GetAudioAdapterInfos>& msg)
{
    Array<AudioAdapterInfo> infos;
    IndexT i;
    SizeT num = this->audioDevice->GetNumAdapters();
    infos.Reserve(num);
    for (i = 0; i < num; i++)
    {
        infos.Append(this->audioDevice->GetAdapterInfo(i));
    }
    msg->SetResult(infos);
}

//------------------------------------------------------------------------------
/**
*/
void
AudioHandler::OnLoadSoundBank(const Ptr<Audio::LoadSoundBank>& msg)
{
    msg->SetResult(this->audioDevice->LoadSoundBank(msg->GetResourceId(), msg->GetStreaming()));
}

//------------------------------------------------------------------------------
/**
*/
void
AudioHandler::OnUnloadSoundBank(const Ptr<Audio::UnloadSoundBank>& msg)
{
    this->audioDevice->UnloadSoundBank(msg->GetResourceId());
}

//------------------------------------------------------------------------------
/**
*/
void
AudioHandler::OnPlayCue(const Ptr<Audio::PlayCue>& msg)
{
    msg->SetCueHandle(this->audioDevice->PlayCue(msg->GetCueId(),
                                                 msg->GetStartPlaying(),
                                                 msg->GetEnable3D(),
                                                 msg->GetTransform(),
                                                 msg->GetVelocity()).cast<Core::RefCounted>());
}

//------------------------------------------------------------------------------
/**
*/
void
AudioHandler::OnUpdateCue(const Ptr<Audio::UpdateCue>& msg)
{
    Ptr<Cue> cue = msg->GetCueHandle().downcast<Cue>();
    n_assert(cue.isvalid());
    if (cue->IsValid())
    {
        cue->SetTransform(msg->GetTransform());
        cue->SetVelocity(msg->GetVelocity());
        msg->SetStateMask(cue->GetState());
    }
}

//------------------------------------------------------------------------------
/**
*/
void
AudioHandler::OnStopCue(const Ptr<Audio::StopCue>& msg)
{
    Ptr<Cue> cue = msg->GetCueHandle().downcast<Cue>();
    n_assert(cue.isvalid());
    if (cue->IsValid())
    {
        cue->Stop();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
AudioHandler::OnStopCueInstances(const Ptr<Audio::StopCueInstances>& msg)
{
    this->audioDevice->StopCue(msg->GetCueId(), msg->GetStopImmediate());
}

//------------------------------------------------------------------------------
/**
*/
void
AudioHandler::OnPlayWave(const Ptr<Audio::PlayWave>& msg)
{
    msg->SetWaveHandle(this->audioDevice->PlayWave(msg->GetWaveId(),
                                                   msg->GetStartPlaying(),
                                                   msg->GetLoopCount(),
                                                   msg->GetVolume(),
                                                   msg->GetPitch(),
                                                   msg->GetEnable3D(),
                                                   msg->GetTransform(),
                                                   msg->GetVelocity()).cast<Core::RefCounted>());
}

//------------------------------------------------------------------------------
/**
*/
void
AudioHandler::OnStopWaveInstances(const Ptr<Audio::StopWaveInstances>& msg)
{
    this->audioDevice->StopWave(msg->GetWaveId(), msg->GetStopImmediate());
}

//------------------------------------------------------------------------------
/**
*/
void
AudioHandler::OnUpdateListener(const Ptr<Audio::UpdateListener>& msg)
{
    this->audioDevice->UpdateListener(msg->GetTransform(), msg->GetVelocity());
}

//------------------------------------------------------------------------------
/**
*/
void
AudioHandler::OnPauseCue(const Ptr<Audio::PauseCue>& msg)
{
    Ptr<Cue> cue = msg->GetCueHandle().downcast<Cue>();
    n_assert(cue.isvalid());
    if (cue->IsValid() && !cue->IsPaused())
    {
        cue->Pause();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
AudioHandler::OnResumeCue(const Ptr<Audio::ResumeCue>& msg)
{
    Ptr<Cue> cue = msg->GetCueHandle().downcast<Cue>();
    n_assert(cue.isvalid());
    if (cue->IsValid() && cue->IsPaused())
    {
        cue->Resume();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
AudioHandler::OnSetGlobalVariable(const Ptr<Audio::SetGlobalVariable>& msg)
{
    n_assert(msg->GetVariableName().IsValid());
    this->audioDevice->SetGlobalVariable(msg->GetVariableName(), msg->GetVaribaleValue());
}

//------------------------------------------------------------------------------
/**
*/
void
AudioHandler::OnGetGlobalVariable(const Ptr<Audio::GetGlobalVariable>& msg)
{
    n_assert(msg->GetVariableName().IsValid());
    msg->SetVaribaleValue(this->audioDevice->GetGlobalVariable(msg->GetVariableName()));
}
} // namespace Audio
