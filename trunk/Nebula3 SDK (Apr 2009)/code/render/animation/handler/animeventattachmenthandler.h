#pragma once
//------------------------------------------------------------------------------
/**
    @class Animation::AnimEventParticleHandler

    This is the godsend animevent handler, to handle vfx hotspots

    (C) 2009 Radon Labs GmbH
*/
#include "animation/animeventhandlerbase.h"

//------------------------------------------------------------------------------
namespace Animation
{
class AnimEventAttachmentHandler : public AnimEventHandlerBase
{
    __DeclareClass(AnimEventAttachmentHandler);

public:
    /// constructor
    AnimEventAttachmentHandler();
    /// destructor
    virtual ~AnimEventAttachmentHandler();  

    /// handle a event
    virtual bool HandleEvent(const Animation::AnimEventInfo& event);    
};

} // namespace Animation
//------------------------------------------------------------------------------