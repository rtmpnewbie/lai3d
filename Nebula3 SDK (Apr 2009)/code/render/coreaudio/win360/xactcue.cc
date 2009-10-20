//------------------------------------------------------------------------------
//  xactcue.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coreaudio/win360/xactcue.h"
#include "coreaudio/audiodevice.h"

namespace Win360
{
__ImplementClass(Win360::XACTCue, 'XCUE', Base::CueBase);

using namespace CoreAudio;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
XACTCue::XACTCue() :
    xactCue(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
XACTCue::~XACTCue()
{
    n_assert(!this->IsValid());
}

//------------------------------------------------------------------------------
/**
*/
void
XACTCue::Setup(const Ptr<SoundBank>& soundBank, IndexT cueIndex, bool startPlayback, bool enable3D)
{
    n_assert(!this->IsValid());
    n_assert(soundBank.isvalid());
    n_assert(InvalidIndex != cueIndex);
    n_assert(0 == this->xactCue);
    
    // call parent class
    CueBase::Setup(soundBank, cueIndex, startPlayback, enable3D);
    
    // do everything else directly through the XACT sound bank
    IXACT3SoundBank* xactSoundBank = soundBank->GetXACTSoundBank();
    HRESULT hr;
    if (startPlayback)
    {
        hr = xactSoundBank->Play((XACTINDEX)cueIndex, 0, 0, &this->xactCue);
    }
    else
    {
        hr = xactSoundBank->Prepare((XACTINDEX)cueIndex, 0, 0, &this->xactCue);
    }
    n_assert(SUCCEEDED(hr));
    n_assert(0 != this->xactCue);
}

//------------------------------------------------------------------------------
/**
*/
void
XACTCue::Discard()
{
    n_assert(0 != this->xactCue);
    
    this->xactCue->Destroy();
    this->xactCue = 0;

    CueBase::Discard();
}

//------------------------------------------------------------------------------
/**
*/
void
XACTCue::Play()
{
    n_assert(0 != this->xactCue);
    HRESULT hr = this->xactCue->Play();
    n_assert(SUCCEEDED(hr));
}

//------------------------------------------------------------------------------
/**
*/
void
XACTCue::Stop()
{
    n_assert(0 != this->xactCue);
    HRESULT hr = this->xactCue->Stop(0);
    n_assert(SUCCEEDED(hr));
}

//------------------------------------------------------------------------------
/**
*/
void
XACTCue::StopImmediate()
{
    n_assert(0 != this->xactCue);
    HRESULT hr = this->xactCue->Stop(XACT_FLAG_CUE_STOP_IMMEDIATE);
    n_assert(SUCCEEDED(hr));
}

//------------------------------------------------------------------------------
/**
*/
void
XACTCue::Pause()
{
    n_assert(0 != this->xactCue);
    HRESULT hr = this->xactCue->Pause(TRUE);
    n_assert(SUCCEEDED(hr));
}

//------------------------------------------------------------------------------
/**
*/
void
XACTCue::Resume()
{
    n_assert(0 != this->xactCue);
    HRESULT hr = this->xactCue->Pause(FALSE);
    n_assert(SUCCEEDED(hr));
}

//------------------------------------------------------------------------------
/**
    This method is called once per-frame by the AudioDevice for
    all active cues. This is the place where the cue should update its
    3D parameters.
*/
void
XACTCue::Update()
{
    if (this->is3DEnabled && this->IsPlaying())
    {
        n_assert(0 != this->xactCue);
        AudioDevice* audioDevice = AudioDevice::Instance();

        // setup an X3DAudio listener structure
        X3DAUDIO_LISTENER x3dListener = { 0 };
        const matrix44& listenerTransform = audioDevice->GetListenerTransform();
        const vector& listenerVelocity = audioDevice->GetListenerVelocity();
        x3dListener.OrientFront = (X3DAUDIO_VECTOR&) listenerTransform.get_zaxis();
        x3dListener.OrientTop   = (X3DAUDIO_VECTOR&) listenerTransform.get_yaxis();
        x3dListener.Position    = (X3DAUDIO_VECTOR&) listenerTransform.get_position();
        x3dListener.Velocity    = (X3DAUDIO_VECTOR&) listenerVelocity;

        // setup an X3DAudio emitter structure
        X3DAUDIO_EMITTER x3dEmitter = { 0 };
        x3dEmitter.OrientFront  = (X3DAUDIO_VECTOR&) this->transform.get_zaxis();
        x3dEmitter.OrientTop    = (X3DAUDIO_VECTOR&) this->transform.get_yaxis();
        x3dEmitter.Position     = (X3DAUDIO_VECTOR&) this->transform.get_position();
        x3dEmitter.Velocity     = (X3DAUDIO_VECTOR&) this->velocity;
        x3dEmitter.ChannelCount = 1;
        x3dEmitter.CurveDistanceScaler = 1.0f;
        x3dEmitter.DopplerScaler = 1.0f;

        // use the prepared dspSettings structure in AudioDevice
        // to hold the result, this is a bit dirty...
        X3DAUDIO_DSP_SETTINGS& dspSettings = audioDevice->X3DAudioDspSettings();
        
        // let XACT do the 3D calculation
        HRESULT hr = XACT3DCalculate(audioDevice->x3dAudioHandle,   // X3DInstance
                                     &x3dListener,                  // pListener
                                     &x3dEmitter,                   // pEmitter
                                     &dspSettings);                 // pDSPSettings
        n_assert(SUCCEEDED(hr));

        // apply the computation result
        hr = XACT3DApply(&dspSettings, this->xactCue);
    }
}

//------------------------------------------------------------------------------
/**
*/
DWORD
XACTCue::GetXACTCueState() const
{
    n_assert(0 != this->xactCue);
    DWORD dwState = 0;
    HRESULT hr = this->xactCue->GetState(&dwState);
    n_assert(SUCCEEDED(hr))
    return dwState;
}

//------------------------------------------------------------------------------
/**
*/
bool
XACTCue::IsPreparing() const
{
    return 0 != (this->GetXACTCueState() & XACT_CUESTATE_PREPARING);
}

//------------------------------------------------------------------------------
/**
*/
bool
XACTCue::IsPrepared() const
{
    return 0 != (this->GetXACTCueState() & XACT_CUESTATE_PREPARED);
}

//------------------------------------------------------------------------------
/**
*/
bool
XACTCue::IsPlaying() const
{
    return 0 != (this->GetXACTCueState() & XACT_CUESTATE_PLAYING);
}

//------------------------------------------------------------------------------
/**
*/
bool
XACTCue::IsStopping() const
{
    return 0 != (this->GetXACTCueState() & XACT_CUESTATE_STOPPING);
}

//------------------------------------------------------------------------------
/**
*/
bool
XACTCue::IsStopped() const
{
    return 0 != (this->GetXACTCueState() & XACT_CUESTATE_STOPPED);
}

//------------------------------------------------------------------------------
/**
*/
bool
XACTCue::IsPaused() const
{
    return 0 != (this->GetXACTCueState() & XACT_CUESTATE_PAUSED);
}

//------------------------------------------------------------------------------
/**
*/
SoundState::Mask
XACTCue::GetState() const
{
    DWORD xactState = this->GetXACTCueState();
    SoundState::Mask mask = 0;
    if (xactState & XACT_CUESTATE_CREATED)      mask |= SoundState::Initial;
    if (xactState & XACT_CUESTATE_PREPARING)    mask |= SoundState::Preparing;
    if (xactState & XACT_CUESTATE_PREPARED)     mask |= SoundState::Prepared;
    if (xactState & XACT_CUESTATE_PLAYING)      mask |= SoundState::Playing;
    if (xactState & XACT_CUESTATE_STOPPING)     mask |= SoundState::Stopping;
    if (xactState & XACT_CUESTATE_STOPPED)      mask |= SoundState::Stopped;
    if (xactState & XACT_CUESTATE_PAUSED)       mask |= SoundState::Paused;
    return mask;
}

} // namespace Win360
