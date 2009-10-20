#pragma once
//------------------------------------------------------------------------------
/**
    @class Sequencer::TrackSequencer
    
    (C) 2008 Radon Labs GmbH
*/

#include "foundation/core/refcounted.h"
#include "track.h"

namespace Sequencer
{
class TrackBar : public Core::RefCounted
{
    __DeclareClass(TrackBar);

public:
    /// constructor
    TrackBar();
    /// destructor
    virtual ~TrackBar();
	/// called by sequencer when end frame is reached
    virtual void OnExit();
    /// called by sequencer when start frame is reached
    virtual void OnEnter();
    /// called by sequencer per frame if track bar is in time line
    virtual void OnSequenceFrame(Timing::Time localFrameTime);
    /// called every frame, when the sequencer is not playing
    virtual void OnIdle(Timing::Time localFrameTime);
    /// called every frame after OnIdle(), when the sequencer is not playing
    virtual void OnIdleAfter(Timing::Time localFrameTime);
    /// called when deattached from its track. Will unload all subsystems.
    virtual void Discard();
    /// called by seqeuencer when the sequence is aborted
    virtual void OnAbort();
    /// set start frame
    void SetStartFrame(int frame);
    /// get start frame
    int GetStartFrame() const;
    /// set end frame
    void SetEndFrame(int frame);
    /// get end frame
    int GetEndFrame() const;
	/// set playing state
    void SetIsPlaying(bool isPlaying);
    /// get playing state
    bool GetIsPlaying() const;
    /// set mute state
    void SetMuted(bool mute);
    /// get mute state
    bool GetMuted() const;
    /// get executing status
    bool HasExecuted() const;

protected:
	int startFrame;
    int endFrame;
    Ptr<Sequencer::Track> track;
    bool isMuted;
    bool isPlaying;
    bool hasExecuted;
};

//------------------------------------------------------------------------------
/**
*/
inline 
bool
TrackBar::GetMuted() const
{
    return this->isMuted;
}

//------------------------------------------------------------------------------
/**
*/
inline 
void
TrackBar::SetStartFrame(int frame)
{
    this->startFrame = frame;
}

//------------------------------------------------------------------------------
/**
*/
inline 
int
TrackBar::GetStartFrame() const
{
    return this->startFrame;
}

//------------------------------------------------------------------------------
/**
*/
inline 
void
TrackBar::SetEndFrame(int frame)
{
    this->endFrame = frame;
}

//------------------------------------------------------------------------------
/**
*/
inline 
int
TrackBar::GetEndFrame() const
{
    return this->endFrame;
}

//------------------------------------------------------------------------------
/**
*/
inline 
void
TrackBar::SetIsPlaying(bool isPlaying)
{
    this->isPlaying = isPlaying;
}

//------------------------------------------------------------------------------
/**
*/
inline 
bool
TrackBar::GetIsPlaying() const
{
    return this->isPlaying;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool 
TrackBar::HasExecuted() const
{
    return this->hasExecuted;
}

}//End Sequencer Namespace