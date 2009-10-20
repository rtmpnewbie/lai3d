#pragma once
//------------------------------------------------------------------------------
/**
    @class Animation::AnimEventTimingHandler
    
    This is the godsend animevent handler, to handle timefactor events
    
    (C) 2009 Radon Labs GmbH
*/
#include "animation/animeventhandlerbase.h"
#include "animation/animeventregistry.h"
#include "timing/timer.h"

//------------------------------------------------------------------------------
namespace Animation
{
class AnimEventTimingHandler : public AnimEventHandlerBase
{
    __DeclareClass(AnimEventTimingHandler);

public:
    /// constructor
    AnimEventTimingHandler();
    /// destructor
    virtual ~AnimEventTimingHandler();  

    /// handle a event
    virtual bool HandleEvent(const Animation::AnimEventInfo& event);

    /// check if we have to reset time factor
    virtual void OnFrame(Timing::Time time);

private:  
    AnimEventRegistry::TimingEvent curTiming;
    bool curTimingActive;
    float curFactor;

    Timing::Timer timer;                              // we need an own timer, to prevent influencing our own time
};
} // namespace Animation
//------------------------------------------------------------------------------