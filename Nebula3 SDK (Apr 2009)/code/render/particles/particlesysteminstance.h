#pragma once
//------------------------------------------------------------------------------
/**
    @class Particles::ParticleSystemInstance
    
    The per-instance object of a ParticleSystem. This is where actual
    particles are created and updated.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "particles/particlesystem.h"
#include "particles/particlesystemstate.h"
#include "particles/particle.h"
#include "math/matrix44.h"
#include "util/ringbuffer.h"
#include "timing/time.h"

//------------------------------------------------------------------------------
namespace Particles
{
class ParticleSystemInstance : public Core::RefCounted
{
    __DeclareClass(ParticleSystemInstance);
public:
    /// constructor
    ParticleSystemInstance();
    /// destructor
    virtual ~ParticleSystemInstance();

    /// setup the particle system instance
    void Setup(const Ptr<ParticleSystem>& particleSystem);
    /// discard the particle system instance
    void Discard();
    /// return true if the object has been setup
    bool IsValid() const;

    /// get pointer to particle system object
    const Ptr<ParticleSystem>& GetParticleSystem() const;
    /// set current world-space transform
    void SetTransform(const Math::matrix44& m);
    /// get current world-space transform
    const Math::matrix44& GetTransform() const;
    /// set current world-space velocity
    void SetVelocity(const Math::vector& v);
    /// get current world-space velocity
    const Math::vector& GetVelocity() const;
    /// set local wind vector
    void SetWindVector(const Math::vector& v);
    /// get local wind vector
    const Math::vector& GetWindVector() const;
    
    /// start emitting particles
    void Start();
    /// stop emitting particles
    void Stop();

    /// currently emitting particles?
    bool IsPlaying() const;
    /// stopped, but some particles still alive?
    bool IsStopping() const;
    /// stopped, and no particles alive?
    bool IsStopped() const;
    /// get current state mask
    ParticleSystemState::Mask GetState() const;

    /// update the particle system instance, call once per frame
    void Update(Timing::Time time);
    /// render the particle system instance, only call when the particle system is visible
    void Render();
    /// debug-visualize the particle system
    void RenderDebug();
    /// get the current bounding box (valid after Update())
    const Math::bbox& GetBoundingBox() const;
    /// read-access to the particle ring buffer
    const Util::RingBuffer<Particle>& GetParticleBuffer() const;

    /// get dirty state of bounding box
    bool IsBoundingBoxDirty() const;

private:
    /// emit new particles for the current frame
    void EmitParticles(float stepTime);
    /// emit a new single particle at the current position and time
    void EmitParticle(IndexT emissionSampleIndex, float initialAge);
    /// update the per-frame state of the particle system (Playing, Stopping, etc...)
    void UpdateState(float time);
    /// perform a single time-step-update on particles
    void StepParticles(float stepTime);

    Ptr<ParticleSystem> particleSystem;
    Math::matrix44 transform;
    Math::vector velocity;
    Math::vector windVector;
    Math::bbox boundingBox;
    bool boundingBoxDirty;
    float curStepTime;
    float lastEmissionTime;
    float timeSinceEmissionStart;
    Util::RingBuffer<Particle> particles;
    ParticleSystemState::Mask stateMask;
    ParticleSystemState::Mask stateChangeMask;
    IndexT emissionCounter;
    bool firstEmissionFrame;
    uint numLivingParticles;
};

//------------------------------------------------------------------------------
/**
*/
inline
bool
ParticleSystemInstance::IsBoundingBoxDirty() const
{
    return this->boundingBoxDirty;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<ParticleSystem>&
ParticleSystemInstance::GetParticleSystem() const
{
    return this->particleSystem;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
ParticleSystemInstance::IsValid() const
{
    return this->particleSystem.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
inline bool
ParticleSystemInstance::IsPlaying() const
{
    return 0 != (ParticleSystemState::Playing & this->stateMask);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
ParticleSystemInstance::IsStopping() const
{
    return 0 != (ParticleSystemState::Stopping & this->stateMask);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
ParticleSystemInstance::IsStopped() const
{
    return 0 != (ParticleSystemState::Stopped & this->stateMask);
}

//------------------------------------------------------------------------------
/**
*/
inline ParticleSystemState::Mask
ParticleSystemInstance::GetState() const
{
    return this->stateMask;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ParticleSystemInstance::SetTransform(const Math::matrix44& m)
{
    this->transform = m;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
ParticleSystemInstance::GetTransform() const
{
    return this->transform;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ParticleSystemInstance::SetVelocity(const Math::vector& v)
{
    this->velocity = v;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::vector&
ParticleSystemInstance::GetVelocity() const
{
    return this->velocity;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ParticleSystemInstance::SetWindVector(const Math::vector& v)
{
    this->windVector = v;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::vector&
ParticleSystemInstance::GetWindVector() const
{
    return this->windVector;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::bbox&
ParticleSystemInstance::GetBoundingBox() const
{
    return this->boundingBox;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::RingBuffer<Particle>&
ParticleSystemInstance::GetParticleBuffer() const
{
    return this->particles;
}

} // namespace Particles
//------------------------------------------------------------------------------
    