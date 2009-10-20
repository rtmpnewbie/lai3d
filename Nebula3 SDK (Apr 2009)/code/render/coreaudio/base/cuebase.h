#pragma once
#ifndef BASE_CUEBASE_H
#define BASE_CUEBASE_H
//------------------------------------------------------------------------------
/**
    @class Base::CueBase
  
    An audio cue is a "highlevel sound effect" and is actually a wrapper
    around one or several waves. See the XACT docs for more details.
    It is important to note that cues are one-use-only. After a cue has
    stopped playing it cannot be reused to play for another time. Instead
    everytime a cue is played, a new cue object will be instantiated.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "coreaudio/cueid.h"
#include "coreaudio/soundbank.h"
#include "math/matrix44.h"
#include "math/vector.h"
#include "coreaudio/soundstate.h"

//------------------------------------------------------------------------------
namespace Base
{
class CueBase : public Core::RefCounted
{
    __DeclareClass(CueBase);
public:
    /// constructor
    CueBase();
    /// destructor
    virtual ~CueBase();

    /// setup the cue object
    void Setup(const Ptr<CoreAudio::SoundBank>& soundBank, IndexT cueIndex, bool startPlayback, bool enable3D);
    /// discard the cue object
    void Discard();
    /// return true if the cue object has been setup
    bool IsValid() const;
    /// get the sound bank this cue is part of
    const Ptr<CoreAudio::SoundBank>& GetSoundBank() const;
    /// get the cue index in the sound bank
    IndexT GetCueIndex() const;
    /// get the cue's CueInfo object
    const CoreAudio::CueInfo& GetCueInfo() const;

    /// set world space transform
    void SetTransform(const Math::matrix44& m);
    /// get world space transform
    const Math::matrix44& GetTransform() const;
    /// set world space velocity
    void SetVelocity(const Math::vector& v);
    /// get world space velocity
    const Math::vector& GetVelocity() const;

    /// play a prepared cue
    void Play();
    /// stop the cue, allowing release phase are transition 
    void Stop();
    /// stop the cue immediately
    void StopImmediate();
    /// pause the cue
    void Pause();
    /// resume a paused cue
    void Resume();
    /// update the cue, this is called per-frame by the audio device to apply 3d settings
    void Update();

    /// return true if the cue is currently preparing
    bool IsPreparing() const;
    /// return true if the cue is prepared (ready for playback)
    bool IsPrepared() const;
    /// return true if the cue is currently playing
    bool IsPlaying() const;
    /// return true if the cue is currently stopping
    bool IsStopping() const;
    /// return true if the cue is stopped
    bool IsStopped() const;
    /// return true if cue is currently paused
    bool IsPaused() const;
    /// return true if this is a 3D cue
    bool Is3DEnabled() const;
    /// get sound state bit mask
    CoreAudio::SoundState::Mask GetState() const;

protected:
    /// shortcut method, get XACT cue state flag mask
    DWORD GetXACTCueState() const;

    Ptr<CoreAudio::SoundBank> soundBank;
    IndexT cueIndex;
    Math::matrix44 transform;
    Math::vector velocity;
    bool is3DEnabled;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
CueBase::IsValid() const
{
    return (InvalidIndex != this->cueIndex);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
CueBase::Is3DEnabled() const
{
    return this->is3DEnabled;
}

//------------------------------------------------------------------------------
/**
*/
inline void
CueBase::SetTransform(const Math::matrix44& m)
{
    this->transform = m;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
CueBase::GetTransform() const
{
    return this->transform;
}

//------------------------------------------------------------------------------
/**
*/
inline void
CueBase::SetVelocity(const Math::vector& v)
{
    this->velocity = v;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::vector&
CueBase::GetVelocity() const
{
    return this->velocity;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreAudio::SoundBank>&
CueBase::GetSoundBank() const
{
    return this->soundBank;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT
CueBase::GetCueIndex() const
{
    return this->cueIndex;
}

} // namespace 
//------------------------------------------------------------------------------
#endif
