#pragma once
//------------------------------------------------------------------------------
/**
    @class FxFeature::EffectProperty
  
    This is the base class for game entities, to emit some effects, camouflaged
    as anim events. Override this class in your application to implement special
    behavior.

    (C) 2009 Radon Labs GmbH
*/
#include "game/property.h"
#include "animation/handler/animeventattachmenthandler.h"
#include "animation/handler/animeventshakehandler.h"
#include "animation/handler/animeventsoundhandler.h"
#include "animation/handler/animeventtiminghandler.h"
#include "animation/handler/animeventvibrationhandler.h"

//------------------------------------------------------------------------------
namespace FxFeature
{
class EffectProperty : public Game::Property
{
	__DeclareClass(EffectProperty);
public:
    /// constructor
    EffectProperty();
    /// destructor
    virtual ~EffectProperty();
    
    /// handle a single message
    virtual void HandleMessage(const Ptr<Messaging::Message>& msg);    

protected:
    /// do some shakes
    virtual void EmitCameraShakeEvent(const Util::StringAtom& eventName) const;
    /// do some timing events
    virtual void EmitTimingEvent(const Util::StringAtom& eventName) const;
    /// do some vibration
    virtual void EmitVibrationEvent(const Util::StringAtom& eventName, IndexT playerIndex) const;
    /// do some sound 
    virtual void EmitSoundEvent(const Util::StringAtom& eventName) const;
    /// do some attachment effects
    virtual void EmitAttachmentEvent(const Util::StringAtom& eventName) const;    
};
__RegisterClass(EffectProperty);
}; // namespace FxFeature