#pragma once
#ifndef BASE_WAVEBASE_H
#define BASE_WAVEBASE_H
//------------------------------------------------------------------------------
/**
    @class Base::WaveBase
  
    A wave object represents a low level sound effect.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "coreaudio/waveid.h"
#include "coreaudio/soundbank.h"
#include "math/matrix44.h"
#include "math/vector.h"
#include "coreaudio/soundstate.h"

//------------------------------------------------------------------------------
namespace Base
{
class WaveBase : public Core::RefCounted
{
    __DeclareClass(WaveBase);
public:
    /// constructor
    WaveBase();
    /// destructor
    virtual ~WaveBase();

    /// setup the wave object
    void Setup(const Ptr<CoreAudio::SoundBank>& soundBank, const IndexT waveIndex, bool startPlayback, SizeT loopCount, bool enable3D);
    /// discard the wave object
    void Discard();
    /// return true if the wave object has been setup
    bool IsValid() const;
    /// get the sound bank this wave is part of
    const Ptr<CoreAudio::SoundBank>& GetSoundBank() const;
    /// get the wave index in the sound bank
    IndexT GetWaveIndex() const;
    /// get the Wave's WaveInfo object
    const CoreAudio::WaveInfo& GetWaveInfo() const;

    /// set volume 
    void SetVolume(float v);
    /// set pitch
    void SetPitch(float p);
    /// set world space transform
    void SetTransform(const Math::matrix44& m);
    /// get world space transform
    const Math::matrix44& GetTransform() const;
    /// set world space velocity
    void SetVelocity(const Math::vector& v);
    /// get world space velocity
    const Math::vector& GetVelocity() const;

    /// play a prepared wave
    void Play();
    /// stop, but first play wave to completion or end of current iteration (for looping waves)
    void Stop();
    /// stop immediately
    void StopImmediate();
    /// pause the wave
    void Pause();
    /// resume a paused wave
    void Resume();

    /// return true if the wave is currently preparing
    bool IsPreparing() const;
    /// return true if the wave is prepared (ready for playback)
    bool IsPrepared() const;
    /// return true if the wave is currently playing
    bool IsPlaying() const;
    /// return true if the wave is currently stopping
    bool IsStopping() const;
    /// return true if the wave is stopped
    bool IsStopped() const;
    /// return true if wave is currently paused
    bool IsPaused() const;
    /// return true if this is a 3D wave
    bool Is3DEnabled() const;
    /// get sound state bit mask
    CoreAudio::SoundState::Mask GetState() const;

protected:
    Ptr<CoreAudio::SoundBank> soundBank;
    IndexT waveIndex;
    bool is3DEnabled;
    Math::matrix44 transform;
    Math::vector velocity;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
WaveBase::IsValid() const
{
    return (InvalidIndex != this->waveIndex);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
WaveBase::Is3DEnabled() const
{
    return this->is3DEnabled;
}

//------------------------------------------------------------------------------
/**
*/
inline void
WaveBase::SetTransform(const Math::matrix44& m)
{
    this->transform = m;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
WaveBase::GetTransform() const
{
    return this->transform;
}

//------------------------------------------------------------------------------
/**
*/
inline void
WaveBase::SetVelocity(const Math::vector& v)
{
    this->velocity = v;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::vector&
WaveBase::GetVelocity() const
{
    return this->velocity;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreAudio::SoundBank>&
WaveBase::GetSoundBank() const
{
    return this->soundBank;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT
WaveBase::GetWaveIndex() const
{
    return this->waveIndex;
}

} // namespace Base
//------------------------------------------------------------------------------
#endif

