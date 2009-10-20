//------------------------------------------------------------------------------
//  corefx/mixer/effectmixer.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "corefx/mixer/effectmixer.h"

namespace CoreFX
{
__ImplementClass(CoreFX::EffectMixer, 'CFEM', Core::RefCounted);

//------------------------------------------------------------------------------
/**
*/
EffectMixer::EffectMixer()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
EffectMixer::Mix(const Util::Array<Ptr<Effect> >& effects, Timing::Time time)
{
    // override in subclass
    n_error("FxMixerBase::Mix -> Mix-method seems not to be overwritten!");
}

//------------------------------------------------------------------------------
/**
*/
void
EffectMixer::Apply() const
{
    // optionally override in subclass
    n_warning("EffectMixer::Apply() called! (.. as intended?)\n");
}
};
