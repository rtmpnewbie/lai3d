//------------------------------------------------------------------------------
//  sequenzermanager.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "trackbar.h"

#include "foundation/core/ptr.h"

namespace Sequencer
{
__ImplementClass(Sequencer::TrackBar, 'STBA', Core::RefCounted);

//------------------------------------------------------------------------------
/**
*/
TrackBar::TrackBar():
		startFrame(0),
		endFrame(0),
		isMuted(false),
		isPlaying(false),
		hasExecuted(false)
{

}

//------------------------------------------------------------------------------
/**
*/
TrackBar::~TrackBar()
{

}

//------------------------------------------------------------------------------
/** This method is called when deattached from its track.
    Will unload all subsystems.
*/
void 
TrackBar::Discard()
{
    this->track = 0;
	//this->isSetup = false;
    //this->isLoad = false;
}

//------------------------------------------------------------------------------
/** This method is called when the play position reaches the
    beginning of the track bar.
*/
void
TrackBar::OnEnter()
{

}

//------------------------------------------------------------------------------
/** This method is called when the play position reaches the
    end of the track bar.
*/
void
TrackBar::OnExit()
{
    this->hasExecuted = true;
}

//------------------------------------------------------------------------------
/**
*/
void
TrackBar::OnAbort()
{
    this->hasExecuted = true;
}

//------------------------------------------------------------------------------
/** This method should be called per frame. It handles the TrackBars
    according to the sequencer frame time position.
*/
void
TrackBar::OnSequenceFrame(Timing::Time localFrameTime)
{
	//empty
}

//------------------------------------------------------------------------------
/** called every frame, when the sequencer is not playing
*/
void
TrackBar::OnIdle(Timing::Time localFrameTime)
{
	//empty
}

//------------------------------------------------------------------------------
/** called every frame after OnIdle(), when the sequencer is not playing
*/
void
TrackBar::OnIdleAfter(Timing::Time localFrameTime)
{
	//empty
}

}//End namespace Sequencer