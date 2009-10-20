//------------------------------------------------------------------------------
//  animeventsoundhandler.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "animation/handler/animeventsoundhandler.h"
#include "internalgraphics/internalgraphicsserver.h"
#include "internalgraphics/internalgraphicsentity.h"
#include "audio/audiointerface.h"
#include "audio/audioprotocol.h"
#include "coreaudio/cueid.h"
#include "animation/animeventregistry.h"

namespace Animation
{
__ImplementClass(Animation::AnimEventSoundHandler, 'AESH', Animation::AnimEventHandlerBase);

using namespace InternalGraphics;
using namespace Audio;

//------------------------------------------------------------------------------
/**
*/
AnimEventSoundHandler::AnimEventSoundHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
AnimEventSoundHandler::~AnimEventSoundHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
AnimEventSoundHandler::HandleEvent(const Animation::AnimEventInfo& event)
{
    // first check if we got that attachment event
    
    if (AnimEventRegistry::Instance()->HasSoundEvent(event.GetAnimEvent().GetName()))
    {
        const Util::StringAtom& cueName = AnimEventRegistry::Instance()->GetSoundEvent(event.GetAnimEvent().GetName());
        
        // get entity
        Ptr<InternalGraphicsEntity> entity = InternalGraphicsServer::Instance()->GetEntityById(event.GetEntityId());

        // play 
        Ptr<Audio::PlayCue> msg = Audio::PlayCue::Create();
        msg->SetCueId(cueName);
        msg->SetEnable3D(true);
        msg->SetTransform(entity->GetTransform());
        msg->SetVelocity(Math::vector::nullvec());      // TODO <- velocity
        AudioInterface::Instance()->Send(msg.cast<Messaging::Message>());
    }
    
    return false;
}
} // namespace Animation
