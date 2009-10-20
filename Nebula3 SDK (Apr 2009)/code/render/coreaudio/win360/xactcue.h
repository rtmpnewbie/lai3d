#pragma once
#ifndef WIN360_XACTCUE_H
#define WIN360_XACTCUE_H
//------------------------------------------------------------------------------
/**
    @class Win360::XACTCue

    XACT implementation of CoreAudio::Cue. This is in fact a very thin wrapper
    around XACT's cue.
    
    (C) 2008 Radon Labs GmbH
*/
#include "coreaudio/base/cuebase.h"

//------------------------------------------------------------------------------
namespace Win360
{
class XACTCue : public Base::CueBase
{
    __DeclareClass(XACTCue);
public:
    /// constructor
    XACTCue();
    /// destructor
    virtual ~XACTCue();

    /// setup the cue object
    void Setup(const Ptr<CoreAudio::SoundBank>& soundBank, IndexT cueIndex, bool startPlayback, bool enable3D);
    /// discard the cue object
    void Discard();
    /// play a prepared cue
    void Play();
    /// stop the cue, allowing release phase or transition 
    void Stop();
    /// stop the cue immediately
    void StopImmediate();
    /// pause the cue
    void Pause();
    /// resume a paused cue
    void Resume();
    /// update the cue, applies 3d settings
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
    /// get sound state bit mask
    CoreAudio::SoundState::Mask GetState() const;

private:
    /// shortcut method: get XACT Cue state flag mask
    DWORD GetXACTCueState() const;

    IXACT3Cue* xactCue;
};

} // namespace Win360
//------------------------------------------------------------------------------
#endif