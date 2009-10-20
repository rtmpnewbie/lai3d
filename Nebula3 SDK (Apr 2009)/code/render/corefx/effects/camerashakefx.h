#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreFX::CameraShakeFX

    A shake effect applies a shake shake to the world as long as it's alive.

    The CoreFX::CameraShakeMixer should calculate the final shake values
    over all CameraShakeFX's and apply the modified transform to the current 
    camera

    (C) 2005 Radon Labs GmbH
*/
#include "corefx/effects/effect.h"

//------------------------------------------------------------------------------
namespace CoreFX
{
class CameraShakeFX : public Effect
{
	__DeclareClass(CameraShakeFX);
public:
    /// constructor
    CameraShakeFX();

    /// set range
    void SetRange(float r);
    /// get range
    float GetRange() const;

    /// set intensity
    void SetIntensity(float i);
    /// get intensity
    float GetIntensity() const;

    /// start the effect
    virtual void OnStart(Timing::Time time);
    /// trigger the effect
    virtual void OnFrame(Timing::Time time);

    /// get current intensity, valid after Update has been called
    float GetCurrentIntensity() const;

private:
    float range;
    float intensity;
    float curIntensity;     // current intensity, updated by Trigger
};

//------------------------------------------------------------------------------
/**
*/
inline
void
CameraShakeFX::SetRange(float r)
{
    this->range = r;
}

//------------------------------------------------------------------------------
/**
*/
inline
float
CameraShakeFX::GetRange() const
{
    return this->range;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
CameraShakeFX::SetIntensity(float i)
{
    this->intensity = i;
}

//------------------------------------------------------------------------------
/**
*/
inline
float
CameraShakeFX::GetIntensity() const
{
    return this->intensity;
}

//------------------------------------------------------------------------------
/**
*/
inline
float
CameraShakeFX::GetCurrentIntensity() const
{
    return this->curIntensity;
}

}; // namespace CoreFX
//------------------------------------------------------------------------------