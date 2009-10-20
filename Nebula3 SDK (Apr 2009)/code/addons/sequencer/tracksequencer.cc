//------------------------------------------------------------------------------
//  sequenzermanager.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tracksequencer.h"

#include "track.h"
#include "trackbar.h"
#include "sequencermanager.h"
#include "foundation/timing/mastertime.h"

namespace Sequencer
{
__ImplementClass(Sequencer::TrackSequencer, 'TSEQ',Core::RefCounted);

//------------------------------------------------------------------------------
/**
*/
TrackSequencer::TrackSequencer():
		startTime(0),
		elapsedTime(0),
		currentState(STOPPED),
		framePosition(0.0f),
		playLength(300),
		framesPerSecond(30)
{

}

//------------------------------------------------------------------------------
/**
*/
TrackSequencer::~TrackSequencer()
{

}

//------------------------------------------------------------------------------
/** This method should be called per frame. It handles the TrackBars
    according to the sequencer frame time position.
*/
void TrackSequencer::OnFrame()
{
    // play if the sequence is playing or when its a cutscene 
    // when standing on the last frame
    if (this->currentState == PLAYING)
    {
    //    this->runningTime += deltaTime;
		this->elapsedTime = Timing::MasterTime::Instance()->GetTime() - this->startTime;
        this->CalcFramePosition();
        
        if (this->framePosition < this->playLength)
        {
            // play inside selection if repeat is on
            if (((	this->framePosition + 1.0f) >= this->selectionEnd)
				&&	this->repeat
                &&	(this->selectionEnd != this->selectionStart)
                &&	((this->numOfRepeats < 1) || (this->repeatCount != this->numOfRepeats)))
            {
                if (this->numOfRepeats > 0)
                {
                    this->repeatCount++;
                }
                float diff = this->framePosition - (float)this->selectionEnd;
                this->GotoPosition(((float)this->selectionStart) + diff);
            }
            else
            {
                // call all tracks
                int i;
                int numOfTracks = this->trackArray.Size();
                for (i=0; i<numOfTracks; i++)
                {
                    this->trackArray[i]->OnSequenceFrame(this->framePosition);
                }
            }
        }
        // play end is reached stop
        else if ((	this->repeat)
				&&	((this->numOfRepeats < 1) || (this->repeatCount != this->numOfRepeats)))
        {
            if (this->numOfRepeats > 0)
            {
                this->repeatCount++;
            }
            float diff = this->framePosition - (float)this->playLength;
            this->GotoPosition(diff);
        }
        else
        {
            this->currentState = STOPPED;
            // for cutscenes only to fix the problem with entities 
            // disappearing after finishing a cutscene-sequence
            //if its a which has ended cutscene set to lastframe, exception for remote cutscene-sequences
//******************************************************
/*			if(this->isCutscene && this != Managers::SequencerManager::Instance()->GetRemoteTrackSequencer())
            {
                this->GotoPosition((float)this->playLength);
            }
*/
//******************************************************
		}
    }
    // call OnIdle/OnIdleAfter for all tracks
    // only call OnIdle for the remotesequencer, 
    // a normal sequence will not stand still at a play position 
    else if (this == SequencerManager::Instance()->GetRemoteTrackSequencer())
    {
        this->CalcFramePosition();
        int i;
        int numOfTracks = this->trackArray.Size();
        //OnIdle calls
        for (i=0; i<numOfTracks; i++)
        {
            if(false == this->trackArray[i]->IsMuted())
            {
                this->trackArray[i]->OnIdle(this->framePosition);
            }
        }
        //OnIdleAfter() calls
        for (i=0; i<numOfTracks; i++)
        {
            if(false == this->trackArray[i]->IsMuted())
            {
                this->trackArray[i]->OnIdleAfter(this->framePosition);
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
TrackSequencer::Discard()
{
    int index;
    for(index = 0; index < trackArray.Size(); index++)
    {
        this->trackArray[index]->Discard();
    }
    this->trackArray.Clear();
}

//------------------------------------------------------------------------------
/** This method starts the sequencer play back.
*/
void TrackSequencer::Play()
{
    //recalculate start time realtive to current system time
	this->startTime = Timing::MasterTime::Instance()->GetTime() - this->elapsedTime;
    this->currentState = PLAYING;
}

//------------------------------------------------------------------------------
/** This method pauses the sequencer.
*/
void TrackSequencer::Pause()
{
    this->currentState = PAUSED;
}

//------------------------------------------------------------------------------
/** Will stop the sequence. Unlikley to resetposition(0) it
    will not call a OnSequence frame.
*/
void TrackSequencer::Stop()
{
    //reset starttime and elapsed time
    this->startTime		= Timing::MasterTime::Instance()->GetTime();
    this->elapsedTime	= 0;
    this->framePosition = 0;
    // reset repeat count
    this->repeatCount	= 1;

    this->currentState	= STOPPED;
}

//------------------------------------------------------------------------------
/**
*/
void
TrackSequencer::Abort()
{
	Util::Array<Ptr<TrackBar>> trackBarArray = this->GetTrackBarsArray();
    //loop thorugh all trackbars look if they are properly executed
    int index;
    for(index = 0; index < trackBarArray.Size(); index++)
    {
        if(false == trackBarArray[index]->HasExecuted())
        {
            trackBarArray[index]->OnAbort();
        }
    }
    this->currentState = STOPPED;
}

//------------------------------------------------------------------------------
/** Get the current cinematic frame number
*/
float TrackSequencer::CalcFramePosition()
{
    this->framePosition = (float)this->elapsedTime * this->framesPerSecond;
    return this->framePosition;
}

//------------------------------------------------------------------------------
/** This method jumps to a frame.
*/
void TrackSequencer::GotoPosition(float frame)
{
    //calculate current time in the sequence
    this->elapsedTime = frame/(float)this->framesPerSecond;
    //calculate start position realtive to system time
	this->startTime = Timing::MasterTime::Instance()->GetTime() - this->elapsedTime;
    this->framePosition = frame;
    // call all tracks
    int i;
    int numOfTracks = this->trackArray.Size();
    for (i=0; i<numOfTracks; i++)
    {
        this->trackArray[i]->OnSequenceFrame(this->framePosition);
    }
}

//------------------------------------------------------------------------------
/** This method resets the sequencer to the start.
*/
void TrackSequencer::ResetPosition()
{
    this->GotoPosition(0);
}

//------------------------------------------------------------------------------
/**
*/
void TrackSequencer::EndWaitingSequence()
{
	Util::Array<Ptr<TrackBar>> trackBarArray = this->GetTrackBarsArray();
    //loop thorugh all trackbars look if they are properly executed
    IndexT i;
    for(i = 0; i < trackBarArray.Size(); i++)
    {
        if(false == trackBarArray[i]->HasExecuted())
        {
            trackBarArray[i]->OnExit();
        }
    }
}

//------------------------------------------------------------------------------
/** This method creates a new track.
*/
void TrackSequencer::AddTrack(const Ptr<Track>& track)
{
    n_assert(!this->trackArray.Find(track));
    this->trackArray.Append( track );
}

//------------------------------------------------------------------------------
/**
*/
Util::Array<Ptr<TrackBar>> 
TrackSequencer::GetTrackBarsArray()
{
    Util::Array<Ptr<TrackBar>> output;
    IndexT i;
    for(i = 0; i < this->trackArray.Size();i++)
    {
        IndexT u;
        for(u = 0; u < this->trackArray[i]->GetNumOfTrackBars(); u++)
        {
            output.Append(this->trackArray[i]->GetTrackBar(u));
        }
    }
    return output;
}

}//End namespace Sequencer