#pragma once
#ifndef AUDIO_AUDIOEMITTER_H
#define AUDIO_AUDIOEMITTER_H
//------------------------------------------------------------------------------
/**
    @class Audio::AudioEmitter
  
    Encapsulates an 3D-positioned audio effect.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "math/vector.h"
#include "math/matrix44.h"
#include "coreaudio/cueid.h"
#include "coreaudio/soundstate.h"
#include "audio/cuehandle.h"

//------------------------------------------------------------------------------
namespace Audio
{
class PlayCue;
class UpdateCue;
    
class AudioEmitter : public Core::RefCounted
{
    __DeclareClass(AudioEmitter);
public:
    /// constructor
    AudioEmitter();
    /// destructor
    virtual ~AudioEmitter();

    /// setup the audio emitter object
    void Setup();
    /// discard the audio emitter object
    void Discard();
    /// return true if the audio emitter object is currently valid
    bool IsValid() const;

    /// set world space transform (ignored for 2D playback)
    void SetTransform(const Math::matrix44& m);
    /// get world space transform
    const Math::matrix44& GetTransform() const;
    /// set world space velocity (ignored for 2D playback)
    void SetVelocity(const Math::vector& v);
    /// get world space velocity
    const Math::vector& GetVelocity() const;

    /// play a cue
    void Play(const CoreAudio::CueId& cueId);
    /// stop playback, allowing release and transition phase
    void Stop();
    /// pause a cue
    void Pause();
    /// Resume a cue
    void Resume();
    
    /// return true if Play() has been called
    bool IsPlaying() const;
    /// return true if Stop() has been called
    bool IsStopped() const;
    /// return true if Cue was paused()
    bool IsPaused() const;
    
private:
    friend class AudioServer;

    /// called in regular intervals by the audio server
    void OnFrame();
    /// try to validate the cue handle from the pending PlayCue message
    void ValidateCueHandle(bool wait);

    bool isValid;
    CoreAudio::CueId cueId;
    Math::matrix44 transform;
    Math::vector velocity;
    CoreAudio::SoundState::Mask stateMask;
    CueHandle cueHandle;
    Ptr<PlayCue> pendingPlayMsg;
    Ptr<UpdateCue> pendingUpdateMsg;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
AudioEmitter::IsValid() const
{
    return this->isValid;
}

//------------------------------------------------------------------------------
/**
*/
inline void
AudioEmitter::SetTransform(const Math::matrix44& m)
{
    this->transform = m;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
AudioEmitter::GetTransform() const
{
    return this->transform;
}

//------------------------------------------------------------------------------
/**
*/
inline void
AudioEmitter::SetVelocity(const Math::vector& v)
{
    this->velocity = v;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::vector&
AudioEmitter::GetVelocity() const
{
    return this->velocity;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
AudioEmitter::IsPlaying() const
{
    return 0 != (this->stateMask & CoreAudio::SoundState::Playing);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
AudioEmitter::IsStopped() const
{
    // we are stopped either if Stop() has been called, or the 
    // audio-thread-cue has stopped playing
    return 0 != (this->stateMask & CoreAudio::SoundState::Stopped);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
AudioEmitter::IsPaused() const
{
    return 0 != (this->stateMask & CoreAudio::SoundState::Paused);
}

} // namespace Audio
//------------------------------------------------------------------------------
#endif
