#pragma once
//------------------------------------------------------------------------------
/**
    @class BaseGameFeature::AnimEventManager
  
    This application side manager, polls every frame all anim events.
    
    (C) 2009 Radon Labs GmbH
*/    
#include "game/manager.h"
#include "core/singleton.h"
#include "util/dictionary.h"
#include "graphics/graphicsprotocol.h"

// forward decls
namespace Game
{
    class Entity;
}

//------------------------------------------------------------------------------
namespace BaseGameFeature
{
class AnimEventManager : public Game::Manager
{
    __DeclareClass(AnimEventManager);
    __DeclareSingleton(AnimEventManager);
public:
    /// constructor
    AnimEventManager();
    /// destructor
    virtual ~AnimEventManager();

    /// called when attached to game server
    virtual void OnActivate();
    /// called before frame by the game server
    virtual void OnBeginFrame();
    /// called after frame by the game server
    virtual void OnEndFrame();

    /// register an entity for observing anim events
    void RegisterEntity(const Ptr<Game::Entity>& entity, const Util::StringAtom& category);
    /// unregister an entity from observing
    void UnregisterEntity(const Ptr<Game::Entity>& entity, const Util::StringAtom& category);

    /// get all anim events for entity
    const Util::Array<Animation::AnimEventInfo>& GetAnimEvents(const Ptr<Game::Entity>& entity, const Util::StringAtom& category);

private:
    /// structure for collecting and holding anim event data
    struct AnimEventData 
    { 
        Util::Array<Util::StringAtom> registredCategories;

        Timing::Tick lastCheckEndTime;                                                              // last check time
        Timing::Tick keyDuration;                                                                   // key duration used as interval for checking
        Util::Dictionary<Util::StringAtom, Util::Array<Animation::AnimEventInfo> > animEvents;      // last anim events by category

        Ptr<Graphics::GetActiveAnimEvents> getEventsMsg;                                            // pending anim event message
        Ptr<Graphics::GetMaximumKeyDuration> getMaxDurMsg;                                          // pending duration message
    };

    Util::Dictionary<Ptr<Game::Entity>, AnimEventData> animEventData;
};

} // namespace BaseGameFeature
//------------------------------------------------------------------------------