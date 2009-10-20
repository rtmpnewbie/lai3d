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
class TrackBar;

class Track : public Core::RefCounted
{
    __DeclareClass(Track);

public:
    /// constructor
    Track();
    /// destructor
    virtual ~Track();

    /// called every frame, when the sequencer is not playing
    virtual void OnIdle(Timing::Time localFrameTime);
    /// called every frame after OnIdle(), when the sequencer is not playing
    virtual void OnIdleAfter(Timing::Time localFrameTime);
	/// call per-frame
	void OnSequenceFrame(Timing::Time frameTime);
    /// called when deattached from the track sequencer
    void Discard();
    /// get muted status
    bool IsMuted() const;
	/// get track bar element at position
    Ptr<TrackBar> GetTrackBar(int index) const;
    /// get number of track bars
    int GetNumOfTrackBars() const;
    /// add a track bar
    void AddTrackBar(const Ptr<TrackBar>& trackBar);

private:
    Util::Array<Ptr<Sequencer::TrackBar> > trackBarArray;
	Util::Array<Ptr<TrackBar>> activeTrackBars;
    Ptr<TrackBar> inactivTrackBarTransition;
    Ptr<TrackBar> activeTrackBar;
    bool muted;
};

//------------------------------------------------------------------------------
/**
*/
inline
bool
Track::IsMuted() const
{
    return this->muted;
}

}//End Sequencer Namespace