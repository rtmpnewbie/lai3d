//------------------------------------------------------------------------------
//  particlesystem.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "particles/particlesystem.h"

namespace Particles
{
__ImplementClass(Particles::ParticleSystem, 'PSYS', Core::RefCounted);

using namespace CoreGraphics;

// update particles with a locked step rate of 60 fps
const float ParticleSystem::StepTime = 1.0f / 30.0f;

//------------------------------------------------------------------------------
/**
*/
ParticleSystem::ParticleSystem() :
    isValid(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ParticleSystem::~ParticleSystem()
{
    if (this->IsValid())
    {
        this->Discard();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystem::Setup(const Ptr<Mesh>& mesh, IndexT primGrpIndex, const EmitterAttrs& attrs)
{
    n_assert(!this->IsValid());

    this->emitterAttrs = attrs;
    this->envelopeSampleBuffer.Setup(attrs, NumEnvelopeSamples);
    this->emitterMesh.Setup(mesh, primGrpIndex);
    this->isValid = true;
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystem::Discard()
{
    n_assert(this->IsValid());
    this->isValid = false;
    this->emitterMesh.Discard();
    this->envelopeSampleBuffer.Discard();
}

//------------------------------------------------------------------------------
/**
*/
SizeT
ParticleSystem::GetMaxNumParticles() const
{
    float maxFreq = this->emitterAttrs.GetEnvelope(EmitterAttrs::EmissionFrequency).GetMaxValue();
    float maxLifeTime = this->emitterAttrs.GetEnvelope(EmitterAttrs::LifeTime).GetMaxValue();
    return 1 + SizeT(maxFreq * maxLifeTime);
}

} // namespace Particles