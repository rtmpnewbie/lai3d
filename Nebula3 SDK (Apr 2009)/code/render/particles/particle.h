#pragma once
//------------------------------------------------------------------------------
/**
    @class Particles::Particle
    
    The particle structure holds the current state of a single particle.

    (C) 2008 Radon Labs GmbH
*/
#include "math/float4.h"

//------------------------------------------------------------------------------
namespace Particles
{
struct Particle
{
    Math::float4 position;
    Math::float4 startPosition;
    Math::float4 stretchPosition;
    Math::float4 velocity;
    Math::float4 acceleration;
    Math::float4 uvMinMax;
    Math::float4 color;
    float rotation;      
    float rotationVariation;
    float size;
    float sizeVariation;
    float oneDivLifeTime;
    float relAge;                       // between 0 and 1, particle is dead if age > 1.0
    float age;                          // absolute age
};

} // namespace Particles
//------------------------------------------------------------------------------
