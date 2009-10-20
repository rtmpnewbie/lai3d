#pragma once
//------------------------------------------------------------------------------
/**
    @class Animation::AnimEventVibrationHandler
    
    This is the godsend animevent handler, to handle hotspot triggered vibrations
    
    (C) 2009 Radon Labs GmbH
*/
#include "animation/animeventhandlerbase.h"
#include "io/uri.h"
#include "timing/timer.h"

//------------------------------------------------------------------------------
namespace Animation
{
class AnimEventVibrationHandler : public AnimEventHandlerBase
{
    __DeclareClass(AnimEventVibrationHandler);

public:
    /// constructor
    AnimEventVibrationHandler();
    /// destructor
    virtual ~AnimEventVibrationHandler();  

    /// handle a event
    virtual bool HandleEvent(const Animation::AnimEventInfo& event);
};
} // namespace Animation
//------------------------------------------------------------------------------