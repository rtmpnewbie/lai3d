//------------------------------------------------------------------------------
//  fxfeature/fxmanager.cc
//  (C) 2009 RadonLabs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "fxfeature/fxmanager.h"
#include "corefx/mixer/camerashakemixer.h"
#include "corefx/effects/camerashakefx.h"
#include "corefx/effects/vibrationfx.h"
#include "corefx/corefxprotocol.h"
#include "graphics/graphicsinterface.h"

namespace FxFeature
{
__ImplementClass(FxFeature::FxManager, 'VFVS', Core::RefCounted);
__ImplementSingleton(FxFeature::FxManager);

using namespace CoreFX;

//------------------------------------------------------------------------------
/**
*/
FxManager::FxManager() :
    isOpen(false)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
FxManager::~FxManager()
{
    if (this->isOpen)
    {
        this->Close();
    }
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
bool
FxManager::Open()
{
    n_assert(!this->isOpen);
    this->isOpen = true;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
FxManager::Close()
{
    n_assert(this->isOpen);
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
void
FxManager::SetupCameraShakeMixer(const Math::vector& maxDisplacement, const Math::vector& maxThumble)
{
    // create mixer
    Ptr<CameraShakeMixer> camMixer = CameraShakeMixer::Create();
    camMixer->SetMaxDisplacement(maxDisplacement);
    camMixer->SetMaxTumble(maxThumble);

    // send to render thread
    Ptr<AttachEffectMixer> attachMsg = AttachEffectMixer::Create();
    attachMsg->SetEffectMixer(camMixer.cast<EffectMixer>());
    Graphics::GraphicsInterface::Instance()->SendBatched(attachMsg.cast<Messaging::Message>());
}

//------------------------------------------------------------------------------
/**
*/
void
FxManager::EmitCameraShakeEffect(float intensity,
                                 Timing::Time duration,
                                 float range,
                                 const Math::matrix44& transform)
{
    // create effect
    Ptr<CameraShakeFX> camShake = CameraShakeFX::Create();
    camShake->SetIntensity(intensity);
    camShake->SetDuration(duration);
    camShake->SetRange(range);
    camShake->SetTransform(transform);
    
    // send
    Ptr<AttachEffect> attachMsg = AttachEffect::Create();
    attachMsg->SetEffect(camShake.cast<Effect>());
    Graphics::GraphicsInterface::Instance()->SendBatched(attachMsg.cast<Messaging::Message>());
}

//------------------------------------------------------------------------------
/**
*/
void
FxManager::EmitVibrationEffect( float highFreqIntensity,
                                Timing::Time highFreqDuration,
                                float lowFreqIntensity,
                                Timing::Time lowFreqDuration,
                                IndexT playerIndex)
{
    // create fx
    Ptr<VibrationFX> vibFx = VibrationFX::Create();
    vibFx->SetHighFreqDuration(highFreqDuration);
    vibFx->SetHighFreqIntensity(highFreqIntensity);
    vibFx->SetLowFreqDuration(lowFreqDuration);
    vibFx->SetLowFreqIntensity(lowFreqIntensity);
    vibFx->SetPlayerIndex(playerIndex);

    // send
    Ptr<AttachEffect> attachMsg = AttachEffect::Create();
    attachMsg->SetEffect(vibFx.cast<Effect>());
    Graphics::GraphicsInterface::Instance()->SendBatched(attachMsg.cast<Messaging::Message>());
}
} // namespace FxFeature