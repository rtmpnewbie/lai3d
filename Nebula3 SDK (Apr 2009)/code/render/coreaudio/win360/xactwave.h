#pragma once
#ifndef WIN360_XACTWAVE_H
#define WIN360_XACTWAVE_H
//------------------------------------------------------------------------------
/**
    @class Win360::XACTWave
  
    XACT implementation of CoreAudio::Wave.
    
    (C) 2008 Radon Labs GmbH
*/    
#include "coreaudio/base/wavebase.h"

//------------------------------------------------------------------------------
namespace Win360
{
class XACTWave : public Base::WaveBase
{
    __DeclareClass(XACTWave);
public:
    /// constructor
    XACTWave();
    /// destructor
    virtual ~XACTWave();

    /// setup the wave object
    void Setup(const Ptr<CoreAudio::SoundBank>& soundBank, const IndexT waveIndex, bool startPlayback, SizeT loopCount, bool enable3D);
    /// discard the wave object
    void Discard();

    /// set volume 
    void SetVolume(float v);
    /// set pitch
    void SetPitch(float p);

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
    /// get sound state bit mask
    CoreAudio::SoundState::Mask GetState() const;

private:
    /// shortcut method, get XACT wave state flag mask
    DWORD GetXACTWaveState() const;

    IXACT3Wave* xactWave;
    XACTVOLUME xactVolume;
    XACTPITCH xactPitch;
};

} // namespace Win360
//------------------------------------------------------------------------------
#endif
