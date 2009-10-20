#pragma once
#ifndef AUDIO_AUDIOPLAYER_H
#define AUDIO_AUDIOPLAYER_H
//------------------------------------------------------------------------------
/**
    @class Audio::AudioPlayer
    
    Client-side audio player for fire-and-forget sounds. FoF-sounds don't have
    a client-side representation after they are fired, so the application
    cannot manipulate a sound once it is playing.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "coreaudio/cueid.h"
#include "coreaudio/waveid.h"
#include "math/matrix44.h"
#include "math/vector.h"
#include "util/stringatom.h"

//------------------------------------------------------------------------------
namespace Audio
{
class AudioPlayer : public Core::RefCounted
{
    __DeclareClass(AudioPlayer);
    __DeclareSingleton(AudioPlayer);
public:
    /// constructor
    AudioPlayer();
    /// destructor
    virtual ~AudioPlayer();
    
    /// play a 2D-fire-and-forget-cue
    void PlayCue(const CoreAudio::CueId& cueId);
    /// play a 3D-fire-and-forget-cue
    void PlayCue3D(const CoreAudio::CueId& cueId, const Math::matrix44& transform, const Math::vector& velocity);
    /// stop all matching cue instances
    void StopCue(const CoreAudio::CueId& cueId);
    
    /// play a 2D-fire-and-forget-wave
    void PlayWave(const CoreAudio::WaveId& waveId, SizeT loopCount, float volume, float pitch);
    /// play a 3D-fire-and-forget-wave
    void PlayWave3D(const CoreAudio::WaveId& waveId, SizeT loopCount, float volume, float pitch, const Math::matrix44& transform, const Math::vector& velocity);
    /// stop all matching wave instances
    void StopWave(const CoreAudio::WaveId& waveId);

    /// set a global variable
    void SetGlobalVariable(const Util::StringAtom& varName, float varValue);
};

} // namespace Audio
//------------------------------------------------------------------------------
#endif
    