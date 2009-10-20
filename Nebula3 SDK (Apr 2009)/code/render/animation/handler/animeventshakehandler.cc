//------------------------------------------------------------------------------
//  animeventshakehandler.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "animation/handler/animeventshakehandler.h"
#include "internalgraphics/internalgraphicsserver.h"
#include "internalgraphics/internalgraphicsentity.h"
#include "graphics/graphicsprotocol.h"
#include "graphics/graphicsinterface.h"
#include "corefx/effects/camerashakefx.h"
#include "corefx/fxserver.h"
#include "animation/animeventregistry.h"

namespace Animation
{
__ImplementClass(Animation::AnimEventShakeHandler, 'ASHA', Animation::AnimEventHandlerBase);

using namespace InternalGraphics;
using namespace Graphics;

//------------------------------------------------------------------------------
/**
*/
AnimEventShakeHandler::AnimEventShakeHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
AnimEventShakeHandler::~AnimEventShakeHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
AnimEventShakeHandler::HandleEvent(const Animation::AnimEventInfo& event)
{
    //// first check if we got that attachment event    
    if (AnimEventRegistry::Instance()->HasCamShakeEvent(event.GetAnimEvent().GetName()))
    {
        // get data
        const AnimEventRegistry::ShakeEvent& timingEvent = AnimEventRegistry::Instance()->GetCamShakeEvent(event.GetAnimEvent().GetName());

        // get position of shake
        Math::matrix44 transform;
        transform = InternalGraphics::InternalGraphicsServer::Instance()->GetEntityById(event.GetEntityId())->GetTransform();
        
        // create fx
        Ptr<CoreFX::CameraShakeFX> shakeFx = CoreFX::CameraShakeFX::Create();
        shakeFx->SetIntensity(timingEvent.intensity);
        shakeFx->SetRange(timingEvent.range);
        shakeFx->SetDuration(timingEvent.duration);
        shakeFx->SetTransform(transform);        

        // attach to server, fire and forget!
        CoreFX::FxServer::Instance()->AttachEffect(shakeFx.cast<CoreFX::Effect>());            
    }    
    return false;
}
} // namespace Animation
