#pragma once
//------------------------------------------------------------------------------
/**
    @class Managers::SequencerManager
    
    (C) 2008 Radon Labs GmbH
*/

#include "application/game/manager.h"
#include "core/singleton.h"

namespace Sequencer
{
class TrackSequencer;

class SequencerManager : public Game::Manager
{
    __DeclareClass(SequencerManager);
    __DeclareSingleton(SequencerManager);

public:
    /// constructor
    SequencerManager();
    /// destructor
    virtual ~SequencerManager();

    /// called when removed from game server
    virtual void OnDeactivate();
    /// called after world is loaded
    virtual void OnStart();
    /// called per-frame by the game server
    virtual void OnFrame();

    /// attach a track sequencer
    void AttachTrackSequencer(Ptr<Sequencer::TrackSequencer> sequencer);
    /// remove a track sequencer
    void RemoveTrackSequencer(Ptr<Sequencer::TrackSequencer> sequencer);
    /// attach a cutscene
    void AttachCutscene(Ptr<Sequencer::TrackSequencer> cutscene);
    /// remove a cutscene
    void RemoveCutscene(Ptr<Sequencer::TrackSequencer> cutscene);

    /// get the remote sequencer
    Ptr<Sequencer::TrackSequencer> GetRemoteTrackSequencer() const;
    /// if a sequence is running
    bool IsSequenceRunning() const;
    /// switch to a new selection. given by a trackbars id.
	void SwitchSelectedTrackBars(const Util::Array<Util::String>& trackBarList);

private:
    Util::Array<Ptr<Sequencer::TrackSequencer> > activeSequencerPool;
    Util::Array<Ptr<Sequencer::TrackSequencer> > inactiveSequencerPool;
    Util::Array<Ptr<Sequencer::TrackSequencer> > cutsceneSequencerPool;

    bool isSequenceRunning;
};

//------------------------------------------------------------------------------
/**
*/
inline
bool
SequencerManager::IsSequenceRunning() const
{
    return this->isSequenceRunning;
}

}//End Sequencer Namespace