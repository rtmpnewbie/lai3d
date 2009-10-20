//------------------------------------------------------------------------------
//  animeventvibrationhandler.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "animation/handler/animeventvibrationhandler.h"
#include "graphics/graphicsprotocol.h"
#include "graphics/graphicsinterface.h"
#include "corefx/effects/vibrationfx.h"
#include "corefx/fxserver.h"
#include "animation/animeventregistry.h"

namespace Animation
{
__ImplementClass(Animation::AnimEventVibrationHandler, 'AEVH', Animation::AnimEventHandlerBase);

using namespace InternalGraphics;
using namespace Graphics;

//------------------------------------------------------------------------------
/**
*/
AnimEventVibrationHandler::AnimEventVibrationHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
AnimEventVibrationHandler::~AnimEventVibrationHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
AnimEventVibrationHandler::HandleEvent(const Animation::AnimEventInfo& event)
{
    // first check if we got that attachment event
    if (AnimEventRegistry::Instance()->HasVibrationEvent(event.GetAnimEvent().GetName()))
    {
        // get data
        const AnimEventRegistry::VibrationEvent& vibEvent = AnimEventRegistry::Instance()->GetVibrationEvent(event.GetAnimEvent().GetName());

        // create fx
        Ptr<CoreFX::VibrationFX> vibFx = CoreFX::VibrationFX::Create();
        vibFx->SetHighFreqDuration(vibEvent.highFreqDuration);
        vibFx->SetHighFreqIntensity(vibEvent.highFreqIntensity);
        vibFx->SetLowFreqDuration(vibEvent.lowFreqDuration);
        vibFx->SetLowFreqIntensity(vibEvent.lowFreqIntensity);
        vibFx->SetPlayerIndex(vibEvent.playerIndex);

        // attach to server, fire and forget!
        CoreFX::FxServer::Instance()->AttachEffect(vibFx.cast<CoreFX::Effect>());            
    }    
    return false;
}
} // namespace Animation
