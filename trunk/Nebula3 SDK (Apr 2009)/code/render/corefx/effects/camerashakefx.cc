//------------------------------------------------------------------------------
//  corefx/effects/camerashakefx.cc
//  (C) 2005 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "corefx/effects/camerashakefx.h"

namespace CoreFX
{
__ImplementClass(CoreFX::CameraShakeFX, 'FXSH', CoreFX::Effect);

//------------------------------------------------------------------------------
/**
*/
CameraShakeFX::CameraShakeFX() :
    range(1.0f),
    intensity(1.0f),
    curIntensity(0.0f)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
CameraShakeFX::OnStart(Timing::Time time)
{
    Effect::OnStart(time);
    this->curIntensity = this->intensity;
}

//------------------------------------------------------------------------------
/**
*/
void
CameraShakeFX::OnFrame(Timing::Time time)
{
    Effect::OnFrame(time);

    if (this->IsPlaying())
    {
        // drop off shake intensity by time
        Timing::Time age = time - this->startTime;
        if (age <= this->GetDuration())
        {
            this->curIntensity = this->intensity * (1.0f - Math::n_saturate(float(age / this->duration)));
        }
    }
}
}; // namespace CoreFX