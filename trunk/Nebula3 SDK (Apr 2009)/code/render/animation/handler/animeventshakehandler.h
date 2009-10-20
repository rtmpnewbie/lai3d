#pragma once
//------------------------------------------------------------------------------
/**
    @class Animation::AnimEventShakeHandler
    
    This is the godsend animevent handler, to handle camera shake events
    
    (C) 2009 Radon Labs GmbH
*/
#include "animation/animeventhandlerbase.h"

//------------------------------------------------------------------------------
namespace Animation
{
class AnimEventShakeHandler : public AnimEventHandlerBase
{
    __DeclareClass(AnimEventShakeHandler);

public:
    /// constructor
    AnimEventShakeHandler();
    /// destructor
    virtual ~AnimEventShakeHandler();  

    /// handle a event
    virtual bool HandleEvent(const Animation::AnimEventInfo& event);
};
} // namespace Animation
//------------------------------------------------------------------------------