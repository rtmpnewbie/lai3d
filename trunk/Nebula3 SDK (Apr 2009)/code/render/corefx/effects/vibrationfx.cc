//------------------------------------------------------------------------------
//  corefx/effects/camerashakefx.cc
//  (C) 2005 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "corefx/effects/vibrationfx.h"
#include "vibration/vibrationplayer.h"

namespace CoreFX
{
__ImplementClass(CoreFX::VibrationFX, 'VIFX', CoreFX::Effect);

//------------------------------------------------------------------------------
/**
*/
VibrationFX::VibrationFX() :    highFreqIntensity(0),
                                lowFreqIntensity(0),
                                highFreqDuration(0),
                                lowFreqDuration(0),
                                curHighFreqIntensity(0),
                                curLowFreqIntensity(0),
                                playerIndex(InvalidIndex)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
VibrationFX::OnStart(Timing::Time time)
{
    Effect::OnStart(time);
    this->curHighFreqIntensity = this->highFreqIntensity;
    this->curLowFreqIntensity = this->lowFreqIntensity;

    // set master duration
    this->duration = this->lowFreqDuration > this->highFreqDuration ? this->lowFreqDuration : this->highFreqDuration;

    Vibration::VibrationPlayer::Instance()->AddEnvelopeVibration(this->playerIndex, 
                                this->highFreqIntensity, 0, this->highFreqDuration, 0,
                                this->lowFreqIntensity, 0, this->lowFreqDuration, 0);
}

//------------------------------------------------------------------------------
/**
*/
void
VibrationFX::OnFrame(Timing::Time time)
{
    Effect::OnFrame(time);

    //if (this->IsPlaying())
    //{
    //    // drop off vibration intensity by time
    //    Timing::Time age = time - this->startTime;
    //    if (age <= this->GetDuration())
    //    {
    //        this->curHighFreqIntensity = this->highFreqIntensity * (1.0f - Math::n_saturate(float(age / this->highFreqDuration)));
    //        this->curLowFreqIntensity = this->lowFreqIntensity * (1.0f - Math::n_saturate(float(age / this->lowFreqDuration)));
    //    }
    //    else
    //    {
    //        this->curHighFreqIntensity = 0;
    //        this->curLowFreqIntensity = 0;
    //    }
    //}
    //else
    //{
    //    this->curHighFreqIntensity = 0;
    //    this->curLowFreqIntensity = 0;
    //}
}
}; // namespace CoreFX