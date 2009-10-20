#pragma once
//------------------------------------------------------------------------------
/**
    @class Sequencer::TrackSequencer
    
    (C) 2008 Radon Labs GmbH
*/

#include "foundation/core/refcounted.h"
#include "foundation/timing/time.h"

namespace Sequencer
{
class Track;
class TrackBar;

class TrackSequencer : public Core::RefCounted
{
    __DeclareClass(TrackSequencer);

public:
    /// the sequencer states
    enum TrackSequencerState
    {
        PLAYING,
        PAUSED,
        STOPPED
    };

    /// constructor
    TrackSequencer();
    /// destructor
    virtual ~TrackSequencer();
    /// one frame
    void OnFrame();
	/// called when becomes deactive
	void Discard();
    /// play sequencer
    void Play();
    /// pause sequencer
    void Pause();
    /// stop the sequencer
    void Stop();
    /// abort sequencer
    void Abort();
    /// reset sequencer to start position
    void ResetPosition();
    /// must be used to manually end a
    /// sequence(cutscene) which waits on the last frame 
    void EndWaitingSequence();
    /// goto cinematic frame position
    void GotoPosition(float frame);
    /// get all trackbars handled by this track sequencer
	Util::Array<Ptr<TrackBar> > GetTrackBarsArray();

private:
	/// calculates the current frame position
    float CalcFramePosition();
    /// add a new track
    void AddTrack(const Ptr<Track>& track);
	
	Timing::Time startTime;
	Timing::Time elapsedTime;
	TrackSequencerState currentState;
    int selectionStart;
    int selectionEnd;
	int numOfRepeats;
	int repeatCount;
	bool repeat;
	float framePosition;
	int framesPerSecond;
	int playLength;
	Util::Array<Ptr<Sequencer::Track> > trackArray;
};

}//End Sequencer Namespace