//------------------------------------------------------------------------------
//  xactwave.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coreaudio/win360/xactwave.h"
#include "math/scalar.h"

namespace Win360
{
__ImplementClass(Win360::XACTWave, 'XWAV', Base::WaveBase);

using namespace CoreAudio;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
XACTWave::XACTWave() :
    xactWave(0),
    xactVolume(1.0f),
    xactPitch(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
XACTWave::~XACTWave()
{
    n_assert(!this->IsValid());
}

//------------------------------------------------------------------------------
/**
    Set wave volume. 0 means silence, 1 means original volume.
*/
void
XACTWave::SetVolume(float v)
{
    this->xactVolume = v;
    if (0 != this->xactWave)
    {
        HRESULT hr = this->xactWave->SetVolume(this->xactVolume);
        n_assert(SUCCEEDED(hr));
    }
}

//------------------------------------------------------------------------------
/**
    Set wave pitch between -1.0 and 1.0.
*/
void
XACTWave::SetPitch(float p)
{
    // bring to 0..1 range and convert to XACT pitch values
    float l = (n_clamp(p, -1.0f, 1.0f) + 1.0f) * 0.5f;
    this->xactPitch = (XACTPITCH) n_lerp(XACTPITCH_MIN, XACTPITCH_MAX, l);
    if (0 != this->xactWave)
    {
        HRESULT hr = this->xactWave->SetPitch(this->xactPitch);
        n_assert(SUCCEEDED(hr));
    }
}

//------------------------------------------------------------------------------
/**
*/
void
XACTWave::Setup(const Ptr<SoundBank>& soundBank, IndexT waveIndex, bool startPlayback, SizeT loopCount, bool enable3D)
{
    n_assert(!this->IsValid());
    n_assert(soundBank.isvalid());
    n_assert(InvalidIndex != waveIndex);
    n_assert(0 == this->xactWave);

    // call parent class
    WaveBase::Setup(soundBank, waveIndex, startPlayback, loopCount, enable3D);

    // do everything else directly through the XACT wave bank
    IXACT3WaveBank* xactWaveBank = soundBank->GetXACTWaveBank();
    HRESULT hr;
    if (startPlayback)
    {
        hr = xactWaveBank->Play((XACTINDEX)waveIndex, 0, 0, (XACTLOOPCOUNT)loopCount, &this->xactWave);
    }
    else
    {
        hr = xactWaveBank->Prepare((XACTINDEX)waveIndex, 0, 0, (XACTLOOPCOUNT)loopCount, &this->xactWave);
    }
    n_assert(SUCCEEDED(hr));
    n_assert(0 != this->xactWave);

    // set initial pitch and volume
    hr = this->xactWave->SetVolume(this->xactVolume);
    n_assert(SUCCEEDED(hr));
    hr = this->xactWave->SetPitch(this->xactPitch);
    n_assert(SUCCEEDED(hr));
}

//------------------------------------------------------------------------------
/**
*/
void
XACTWave::Discard()
{
    n_assert(0 != this->xactWave);

    this->xactWave->Destroy();
    this->xactWave = 0;

    WaveBase::Discard();
}

//------------------------------------------------------------------------------
/**
*/
void
XACTWave::Play()
{
    n_assert(0 != this->xactWave);
    HRESULT hr = this->xactWave->Play();
    n_assert(SUCCEEDED(hr));
}

//------------------------------------------------------------------------------
/**
    This will stop the wave when it is completed, or for a looping wave,
    at the end of the current iteration.
*/
void
XACTWave::Stop()
{
    n_assert(0 != this->xactWave);
    HRESULT hr = this->xactWave->Stop(XACT_FLAG_STOP_RELEASE);
    n_assert(SUCCEEDED(hr));
}

//------------------------------------------------------------------------------
/**
    This will stop the wave immediately.
*/
void
XACTWave::StopImmediate()
{
    n_assert(0 != this->xactWave);
    HRESULT hr = this->xactWave->Stop(XACT_FLAG_STOP_IMMEDIATE);
    n_assert(SUCCEEDED(hr));
}

//------------------------------------------------------------------------------
/**
*/
void
XACTWave::Pause()
{
    n_assert(0 != this->xactWave);
    HRESULT hr = this->xactWave->Pause(TRUE);
    n_assert(SUCCEEDED(hr));
}

//------------------------------------------------------------------------------
/**
*/
void
XACTWave::Resume()
{
    n_assert(0 != this->xactWave);
    HRESULT hr = this->xactWave->Pause(FALSE);
    n_assert(SUCCEEDED(hr));
}

//------------------------------------------------------------------------------
/**
*/
DWORD
XACTWave::GetXACTWaveState() const
{
    n_assert(0 != this->xactWave);
    DWORD dwState = 0;
    HRESULT hr = this->xactWave->GetState(&dwState);
    n_assert(SUCCEEDED(hr))
    return dwState;
}

//------------------------------------------------------------------------------
/**
*/
bool
XACTWave::IsPreparing() const
{
    return 0 != (this->GetXACTWaveState() & XACT_STATE_PREPARING);
}

//------------------------------------------------------------------------------
/**
*/
bool
XACTWave::IsPrepared() const
{
    return 0 != (this->GetXACTWaveState() & XACT_STATE_PREPARED);
}

//------------------------------------------------------------------------------
/**
*/
bool
XACTWave::IsPlaying() const
{
    return 0 != (this->GetXACTWaveState() & XACT_STATE_PLAYING);
}

//------------------------------------------------------------------------------
/**
*/
bool
XACTWave::IsStopping() const
{
    return 0 != (this->GetXACTWaveState() & XACT_STATE_STOPPING);
}

//------------------------------------------------------------------------------
/**
*/
bool
XACTWave::IsStopped() const
{
    return 0 != (this->GetXACTWaveState() & XACT_STATE_STOPPED);
}

//------------------------------------------------------------------------------
/**
*/
bool
XACTWave::IsPaused() const
{
    return 0 != (this->GetXACTWaveState() & XACT_STATE_PAUSED);
}

//------------------------------------------------------------------------------
/**
*/
SoundState::Mask
XACTWave::GetState() const
{
    DWORD xactState = this->GetXACTWaveState();
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