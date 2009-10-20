//------------------------------------------------------------------------------
//  particlesysteminstance.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "particles/particlesysteminstance.h"
#include "math/polar.h"
#include "coregraphics/shaperenderer.h"
#include "threading/thread.h"

namespace Particles
{
__ImplementClass(Particles::ParticleSystemInstance, 'PSIN', Core::RefCounted);

using namespace Math;
using namespace Util;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
ParticleSystemInstance::ParticleSystemInstance() :
    transform(matrix44::identity()),
    velocity(vector::nullvec()),   
    windVector(0.5f, 0.0f, 0.0f),
    curStepTime(0.0f),
    lastEmissionTime(0.0f),
    timeSinceEmissionStart(0.0f),
    stateMask(ParticleSystemState::Initial),
    stateChangeMask(0),
    emissionCounter(0),
    firstEmissionFrame(false),
    numLivingParticles(0),
    boundingBoxDirty(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ParticleSystemInstance::~ParticleSystemInstance()
{
    n_assert(!this->IsValid());
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemInstance::Setup(const Ptr<ParticleSystem>& psys)
{
    n_assert(!this->IsValid());

    this->stateMask = ParticleSystemState::Initial;
    this->stateChangeMask = 0;
    this->curStepTime = 0.0f;
    this->lastEmissionTime = 0.0f;
    this->timeSinceEmissionStart = 0.0f;
    this->firstEmissionFrame = true;
    this->numLivingParticles = 0;
    this->particleSystem = psys;
    this->particles.SetCapacity(this->particleSystem->GetMaxNumParticles());
}
    
//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemInstance::Discard()
{
    n_assert(this->IsValid());
    this->particleSystem = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemInstance::Start()
{
    n_assert(this->IsValid());
    this->stateChangeMask = ParticleSystemState::Playing;
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemInstance::Stop()
{
    n_assert(this->IsValid());
    this->stateChangeMask = ParticleSystemState::Stopped;
}

//------------------------------------------------------------------------------
/**
    The Update() method must be called once per frame with a global time 
    stamp. If the particle system is currently in the playing state, 
    the method will emit new particles as needed, and update the current
    state of existing particles.
*/
void
ParticleSystemInstance::Update(Timing::Time time)
{
    // clear dirty flag
    this->boundingBoxDirty = false;

    n_assert(this->IsValid());
    float fltTime = float(time);

    // time exepction?
    float timeDiff = fltTime - this->curStepTime;
    if ((timeDiff < -(ParticleSystem::StepTime * 2.0f)) || (timeDiff > 0.5f))
    {
        this->curStepTime = fltTime - ParticleSystem::StepTime;
    }

    // update the state of the particle system (started, stopped, etc...)
    this->UpdateState(fltTime);

    // if we are currently playing, update the particle system in discrete time steps,
    // but never more then 5 times in a row (which would mean we're running 
    // at 12 fps)
    if (this->IsPlaying())
    {
        IndexT curStep = 0;
        while (this->curStepTime <= fltTime)
        {
            this->EmitParticles(ParticleSystem::StepTime);
            this->StepParticles(ParticleSystem::StepTime);
            this->curStepTime += ParticleSystem::StepTime;
            curStep++;
        }
    }
}

//------------------------------------------------------------------------------
/**
    Update the current state of the particle system (Started, Stopping, 
    Stopped, etc...).
*/
void
ParticleSystemInstance::UpdateState(float time)
{
    // first check if a state switch has been requested
    if (0 != this->stateChangeMask)
    {
        // if requested to play, and not already playing...
        if ((0 != (ParticleSystemState::Playing & this->stateChangeMask)) && !this->IsPlaying())
        {
            this->curStepTime = time;
            this->lastEmissionTime = 0.0f;
            this->timeSinceEmissionStart = 0.0f;
            this->firstEmissionFrame = true;
            this->numLivingParticles = 0;
            this->particles.Reset();
            this->stateMask = ParticleSystemState::Playing;
        }
        
        // if requested to stop, and currently playing...
        if ((0 != (ParticleSystemState::Stopped & this->stateChangeMask)) && this->IsPlaying())
        {
            this->stateMask |= ParticleSystemState::Stopping;
        }
    }

    // if we're currently in Stopping state, and no more particles are alive,
    // go into Stopped state, the numLivingParticles member is 
    // updated by the StepParticles method
    if (this->IsStopping())
    {
        if (0 == this->numLivingParticles)
        {
            this->stateMask = ParticleSystemState::Stopped;

            // set box dirty, and reset box 
            this->boundingBoxDirty = true;
            this->boundingBox = Math::bbox();
        }
    }

    // reset the stateChangeMask
    this->stateChangeMask = 0;
}

//------------------------------------------------------------------------------
/**
    Emits new particles if necessary. This method takes the start time,
    curStepTime, startDelay, precalcTime and lastEmissionTime into account.
*/
void
ParticleSystemInstance::EmitParticles(float stepTime)
{
    const EmitterAttrs& emAttrs = this->particleSystem->GetEmitterAttrs();
    const EnvelopeSampleBuffer& sampleBuffer = this->particleSystem->GetEnvelopeSampleBuffer();

    // get the (wrapped around if looping) time since emission has started
    this->timeSinceEmissionStart += stepTime;
    float emDuration = emAttrs.GetFloat(EmitterAttrs::EmissionDuration);
    float startDelay = emAttrs.GetFloat(EmitterAttrs::StartDelay);
    float loopTime = emDuration + startDelay;
    bool looping = emAttrs.GetBool(EmitterAttrs::Looping);
    if (looping && (this->timeSinceEmissionStart > loopTime))
    {
        // a wrap-around
        this->timeSinceEmissionStart = n_fmod(this->timeSinceEmissionStart, loopTime);
    }

    // if we are before the start delay, we definitely don't need to emit anything
    if (this->timeSinceEmissionStart < startDelay)
    {
        // we're before the start delay
        return;
    }
    else if ((this->timeSinceEmissionStart > loopTime) && !looping)
    {
        // we're past the emission time
        if (0 == (this->stateMask & (ParticleSystemState::Stopping|ParticleSystemState::Stopped)))
        {
            this->Stop();
        }
        return;
    }

    // compute the relative emission time (0.0 .. 1.0)
    float relEmissionTime = (this->timeSinceEmissionStart - startDelay) / emDuration;
    IndexT emSampleIndex = IndexT(relEmissionTime * (ParticleSystem::NumEnvelopeSamples-1));

    // lookup current emission frequency
    float emFrequency = sampleBuffer.LookupSamples(emSampleIndex)[EmitterAttrs::EmissionFrequency];
    if (emFrequency > N_TINY)
    {
        float emTimeStep = 1.0f / emFrequency;

        // if we haven't emitted particles in this run yet, we need to compute 
        // the precalc-time and initialize the lastEmissionTime
        if (this->firstEmissionFrame)
        {
            this->firstEmissionFrame = false;

            // handle pre-calc if necessary, this will instantly produce
            // a number of particles to let the particle system
            // appear as if has been emitting for quite a while already
            float preCalcTime = emAttrs.GetFloat(EmitterAttrs::PrecalcTime);
            if (preCalcTime > N_TINY)
            {
                // during pre-calculation we need to update the particle system,
                // but we do this with a lower step-rate for better performance
                // (but less precision)
                this->lastEmissionTime = 0.0f;
                float updateTime = 0.0f;
                float updateStep = 0.05f;
                if (updateStep < emTimeStep)
                {
                    updateStep = emTimeStep;
                }
                while (this->lastEmissionTime < preCalcTime)
                {
                    this->EmitParticle(emSampleIndex, 0.0f);
                    this->lastEmissionTime += emTimeStep;
                    updateTime += emTimeStep;
                    if (updateTime >= updateStep)
                    {
                        this->StepParticles(updateStep);
                        updateTime = 0.0f;
                    }
                }
            }

            // setup the lastEmissionTime for particle emission so that at least
            // one frame's worth of particles is emitted, this is necessary
            // for very short-emitting particles
            this->lastEmissionTime = this->curStepTime;
            if (stepTime > emTimeStep)
            {
                this->lastEmissionTime -= (stepTime + N_TINY);
            }
            else
            {
                this->lastEmissionTime -= (emTimeStep + N_TINY);
            }
        }

        // handle the "normal" particle emission while the particle system is emitting
        while ((this->lastEmissionTime + emTimeStep) <= this->curStepTime)
        {
            this->lastEmissionTime += emTimeStep;
            this->EmitParticle(emSampleIndex, this->curStepTime - this->lastEmissionTime);
        }
    }
}

//------------------------------------------------------------------------------
/**
    Emits a single particle at the current position. The emission
    sample index is used to lookup sampled envelope curves which are
    modulated over the emission duration or particle lifetime.
*/
void
ParticleSystemInstance::EmitParticle(IndexT emissionSampleIndex, float initialAge)
{
    n_assert(initialAge >= 0.0f);

    Particle particle;
    const EmitterMesh& emMesh = this->particleSystem->GetEmitterMesh();
    const EmitterAttrs emAttrs = this->particleSystem->GetEmitterAttrs();
    const EnvelopeSampleBuffer& envSampleBuffer = this->particleSystem->GetEnvelopeSampleBuffer();
    float* particleEnvSamples = envSampleBuffer.LookupSamples(0);
    float* emissionEnvSamples = envSampleBuffer.LookupSamples(emissionSampleIndex);

    // lookup pseudo-random emitter vertex from the emitter mesh
    const float4* emVertex = emMesh.GetEmitterVertex(this->emissionCounter++);

    // setup particle position and start position
    particle.position = float4::transform(emVertex[0], this->transform);
    particle.startPosition = particle.position;
    particle.stretchPosition = particle.position;

    // compute emission direction
    float4 normal, tangent, emNormal;
    normal = emVertex[1];
    tangent = emVertex[2];
    float minSpread = emissionEnvSamples[EmitterAttrs::SpreadMin];
    float maxSpread = emissionEnvSamples[EmitterAttrs::SpreadMax];
    float theta = n_deg2rad(n_lerp(minSpread, maxSpread, n_rand()));
    float rho = n_deg2rad(360.0f) * n_rand();
    matrix44 rot = matrix44::multiply(matrix44::rotationaxis(tangent, theta), matrix44::rotationaxis(normal, rho));
    emNormal = float4::transform(normal, rot);

    // compute start velocity
    float velocityVariation = 1.0f - (n_rand() * emAttrs.GetFloat(EmitterAttrs::VelocityRandomize));
    float startVelocity = emissionEnvSamples[EmitterAttrs::StartVelocity] * velocityVariation;

    // setup particle velocity vector, acceleration
    particle.velocity = emNormal * startVelocity;
    particle.acceleration = vector::nullvec();

    // setup uvMinMax to a random texture tile
    // FIXME: what's up with the horizontal flip?
    float texTile = n_clamp(emAttrs.GetFloat(EmitterAttrs::TextureTile), 1.0f, 16.0f);
    float step = 1.0f / texTile;
    float tileIndex = floorf(n_rand() * texTile);
    float vMin = step * tileIndex;
    float vMax = vMin + step;
    particle.uvMinMax.set(1.0f, 1.0f - vMin, 0.0f, 1.0f - vMax);

    // setup initial particle color
    particle.color.load(&(particleEnvSamples[EmitterAttrs::Red]));

    // setup rotation and rotationVariation
    float startRotMin = emAttrs.GetFloat(EmitterAttrs::StartRotationMin);
    float startRotMax = emAttrs.GetFloat(EmitterAttrs::StartRotationMax);
    particle.rotation = n_lerp(startRotMin, startRotMax, n_rand());
    float rotVar = 1.0f - (n_rand() * emAttrs.GetFloat(EmitterAttrs::RotationRandomize));
    if (emAttrs.GetBool(EmitterAttrs::RandomizeRotation) && (n_rand() < 0.5f))
    {
        rotVar = -rotVar;
    }
    particle.rotationVariation = rotVar;

    // setup particle size and size variation
    particle.size = particleEnvSamples[EmitterAttrs::Size];
    particle.sizeVariation = 1.0f - (n_rand() * emAttrs.GetFloat(EmitterAttrs::SizeRandomize));

    // setup particle age and oneDivLifetime
    particle.oneDivLifeTime = 1.0f / emissionEnvSamples[EmitterAttrs::LifeTime];
    particle.relAge = initialAge * particle.oneDivLifeTime;
    particle.age = initialAge;

    // add the new particle to the particle ring buffer
    this->particles.Add(particle);
}

//------------------------------------------------------------------------------
/**
    Updates the state of living particles, and keeps track of the number
    of currently living particles in the numLivingParticles member.

    FIXME: 
        - handle age as integer number of steps?
        - update a bounding box?
*/
void
ParticleSystemInstance::StepParticles(float stepTime)
{
    const EmitterAttrs emAttrs = this->particleSystem->GetEmitterAttrs();
    const EnvelopeSampleBuffer& envSampleBuffer = this->particleSystem->GetEnvelopeSampleBuffer();

    this->numLivingParticles = 0;
    this->boundingBox.begin_extend();
    float4 gravity(0.0f, emAttrs.GetFloat(EmitterAttrs::Gravity), 0.0f, 0.0f);
    bool stretchToStart = emAttrs.GetBool(EmitterAttrs::StretchToStart);
    const float stretchTime = emAttrs.GetFloat(EmitterAttrs::ParticleStretch);
    float curStretchTime = 0.0f;

    // for each living particle...
    IndexT i;
    for (i = 0; i < this->particles.Size(); i++)
    {
        Particle& particle = this->particles[i];

        // update particle's age, and only update the other
        // parameters if the particle is still alive
        particle.age += stepTime;
        particle.relAge += stepTime * particle.oneDivLifeTime;
        if (particle.relAge < 1.0f)
        {
            IndexT sampleIndex = IndexT(particle.relAge * ParticleSystem::NumEnvelopeSamples);
            n_assert(sampleIndex < ParticleSystem::NumEnvelopeSamples);
            float* samples = envSampleBuffer.LookupSamples(sampleIndex);

            // compute current particle acceleration
            particle.acceleration = this->windVector * samples[EmitterAttrs::AirResistance];
            particle.acceleration += gravity;
            particle.acceleration *= samples[EmitterAttrs::Mass];

            // fix stretch time (if particle stretch is enabled
            if (stretchTime > 0.0f)
            {
                curStretchTime = (stretchTime > particle.age) ? particle.age : stretchTime;
            }

            // update position, velocity, rotation
            particle.position += particle.velocity * samples[EmitterAttrs::VelocityFactor] * stepTime;
            particle.velocity += particle.acceleration * stepTime;
            if (stretchToStart)
            {
                // NOTE: don't support particle rotation in stretch modes
                particle.stretchPosition = particle.startPosition;
            }
            else if (curStretchTime > 0.0f)
            {
                // NOTE: don't support particle rotation in stretch modes
                // ???
                particle.stretchPosition = particle.position -
                                           (particle.velocity - particle.acceleration * curStretchTime * 0.5f) *
                                           (stretchTime * samples[EmitterAttrs::VelocityFactor]);
            }                
            else
            {
                particle.stretchPosition = particle.position;
                particle.rotation += particle.rotationVariation * samples[EmitterAttrs::RotationVelocity] * stepTime;
            }

            // update particle rotation, size and color
            particle.color.load(&(samples[EmitterAttrs::Red]));
            particle.size = samples[EmitterAttrs::Size] * particle.sizeVariation;

            // update bounding box
            this->boundingBox.extend(particle.position);
            this->boundingBoxDirty = true;

            // update living particle counter
            this->numLivingParticles++;
        }
        // NOTE: we cannot simply stop traversing when the first dead
        // particle is encountered due to the possibly randomized life time
        // of the particles
    }
}

//------------------------------------------------------------------------------
/**
    Render a debug visualization of the particle system.
*/
void
ParticleSystemInstance::RenderDebug()
{
    if (this->numLivingParticles > 0)
    {
        Array<float4> lineList;
        lineList.Reserve(this->numLivingParticles * 6);
        vector xAxis(1.0f, 0.0f, 0.0f);
        vector yAxis(0.0f, 1.0f, 0.0f);
        vector zAxis(0.0f, 0.0f, 1.0f);

        // render each living particle as a x/y/z cross
        IndexT i;
        for (i = 0; i < this->particles.Size(); i++)
        {
            const Particle& particle = this->particles[i];
            if (particle.relAge < 1.0f)
            {
                // cross 1
                lineList.Append(particle.position - xAxis * particle.size);
                lineList.Append(particle.position + xAxis * particle.size);
                lineList.Append(particle.position - yAxis * particle.size);
                lineList.Append(particle.position + yAxis * particle.size);
                lineList.Append(particle.position - zAxis * particle.size);
                lineList.Append(particle.position + zAxis * particle.size);

                // connection
                lineList.Append(particle.position);
                lineList.Append(particle.stretchPosition);
            }
        }
        if (!lineList.IsEmpty())
        {
            RenderShape particlesShape;
            particlesShape.SetupPrimitives(Threading::Thread::GetMyThreadId(),
                                           matrix44::identity(),
                                           PrimitiveTopology::LineList,                        
                                           lineList.Size() / 2,
                                           &(lineList.Front()),
                                           4,
                                           float4(1.0f, 0.0f, 0.0f, 1.0f));
            ShapeRenderer::Instance()->AddShape(particlesShape);
            /*
            Shape boxShape;
            boxShape.SetupSimpleShape(Threading::Thread::GetMyThreadId(),
                                      Shape::Box,
                                      this->boundingBox.to_matrix44(),
                                      float4(1.0f, 0.0f, 0.0f, 0.5f));
            ShapeRenderer::Instance()->AddShape(boxShape);
            */
        }
    }
}

} // namespace Particles