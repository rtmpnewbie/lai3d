//------------------------------------------------------------------------------
//  corefx/mixer/camerashakemixer.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "corefx/mixer/camerashakemixer.h"
#include "corefx/effects/camerashakefx.h"
#include "math/point.h"

namespace CoreFX
{
__ImplementClass(CoreFX::CameraShakeMixer, 'CSHM', CoreFX::EffectMixer);

using namespace Math;
//------------------------------------------------------------------------------
/**
*/
CameraShakeMixer::CameraShakeMixer() :  maxDisplacement(1.0f, 1.0f, 1.0f),
                                        maxTumble(10.0f, 10.0f, 10.0f),
                                        curDisplace(0, 0, 0),
                                        curTumble(0, 0, 0),
                                        lastTime(0.0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
CameraShakeMixer::Mix(const Util::Array<Ptr<Effect> >& effects, Timing::Time time)
{
    // just do if camera exists
    if (InternalGraphics::InternalGraphicsServer::Instance()->GetDefaultView().isvalid() &&
        InternalGraphics::InternalGraphicsServer::Instance()->GetDefaultView()->GetCameraEntity().isvalid())
    {
        // at first calculate accumulated shake intensity at camera position
        const Math::matrix44& camMatrix = this->GetCameraTransform();
        const Math::point& camPos = camMatrix.get_position();
        float shakeIntensity = this->ComputeShakeIntensityAtPosition(effects, camPos);

        // only update displace and tumble at some relatively low frame rate
        // this prevents the shake effect from "flickering" if the 
        // frame rate is very high    
        if ((0.0 == this->lastTime) || ((time - this->lastTime) > 0.01))
        {
            this->lastTime = time;

            // compute a random displacement vector
            this->curDisplace.set(((n_rand() * 2.0f) - 1.0f) * this->maxDisplacement.x(), 
                                  ((n_rand() * 2.0f) - 1.0f) * this->maxDisplacement.y(), 
                                  ((n_rand() * 2.0f) - 1.0f) * this->maxDisplacement.z());


            // compute random tumble angles
            this->curTumble.set(((n_rand() * 2.0f) - 1.0f) * this->maxTumble.x(), 
                                ((n_rand() * 2.0f) - 1.0f) * this->maxTumble.y(), 
                                ((n_rand() * 2.0f) - 1.0f) * this->maxTumble.z());
        }   
        
        // update the current incoming camera matrix by the displace and tumble vectors
        if (shakeIntensity > 0.0f)
        {
            matrix44 shakeMatrix;
            shakeMatrix.rotationx(this->curTumble.x() * shakeIntensity);
            shakeMatrix.rotationy(this->curTumble.y() * shakeIntensity);
            shakeMatrix.rotationz(this->curTumble.z() * shakeIntensity);
            shakeMatrix.translate(Math::vector::transform((this->curDisplace * shakeIntensity), Math::matrix44(camMatrix.get_xaxis(),
                                                                                                               camMatrix.get_yaxis(),
                                                                                                               camMatrix.get_zaxis(),
                                                                                                               Math::point::origin())));            
            this->shakeCameraMatrix = Math::matrix44::multiply(shakeMatrix, camMatrix);
        }
        else
        {
            this->shakeCameraMatrix = camMatrix;
        }
    }
}

//------------------------------------------------------------------------------
/**
    apply camera transform
*/
void
CameraShakeMixer::Apply() const
{
    if (InternalGraphics::InternalGraphicsServer::Instance()->GetDefaultView().isvalid() &&
        InternalGraphics::InternalGraphicsServer::Instance()->GetDefaultView()->GetCameraEntity().isvalid())
    {
        this->SetCameraTransform(this->shakeCameraMatrix);
    }
}

//------------------------------------------------------------------------------
/**
    Computes the accumulated shake intensity at a given position.
*/
float
CameraShakeMixer::ComputeShakeIntensityAtPosition(const Util::Array<Ptr<Effect> >& effects, const Math::vector& pos) const
{
    float shake = 0.0f;
    int i;
    int num = effects.Size();
    for (i = 0; i < num; i++)
    {
        if (effects[i]->IsA(CameraShakeFX::RTTI))
        {
            const Ptr<CameraShakeFX>& curShake = effects[i].cast<CameraShakeFX>();
            if (curShake->IsPlaying())
            {
                Math::vector distVec = pos - curShake->GetTransform().get_position();
                float absDist = distVec.length();
                if (absDist < curShake->GetRange())
                {
                    float attenuate = 1.0f - Math::n_saturate(absDist / curShake->GetRange());
                    shake += attenuate * curShake->GetCurrentIntensity();
                }
            }
        }
    }
    return shake;
}
};