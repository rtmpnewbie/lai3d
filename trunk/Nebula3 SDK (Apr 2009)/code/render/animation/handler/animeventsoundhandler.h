#pragma once
//------------------------------------------------------------------------------
/**
    @class Animation::AnimEventSoundHandler
    
    This is the godsend animevent handler, to handle sound anim events
    
    (C) 2009 Radon Labs GmbH
*/
#include "animation/animeventhandlerbase.h"
#include "io/uri.h"

//------------------------------------------------------------------------------
namespace Animation
{
class AnimEventSoundHandler : public AnimEventHandlerBase
{
    __DeclareClass(AnimEventSoundHandler);
public:
    /// constructor
    AnimEventSoundHandler();
    /// destructor
    virtual ~AnimEventSoundHandler();  

    /// handle a event
    virtual bool HandleEvent(const Animation::AnimEventInfo& event);
};
} // namespace Animation
//------------------------------------------------------------------------------