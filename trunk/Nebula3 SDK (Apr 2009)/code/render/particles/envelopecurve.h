#pragma once
//------------------------------------------------------------------------------
/**
    @class Particles::EnvelopeCurve
    
    An Attack/Sustain/Release envelope curve class with optional
    sine/cosine modulation. Used for animated particle emitter attributes.
    
    (C) 2008 Radon Labs GmbH
*/
#include "math/scalar.h"

//------------------------------------------------------------------------------
namespace Particles
{
class EnvelopeCurve
{
public:
    /// modulation enumerator
    enum ModFunc
    {
        Sine = 0,
        Cosine,
    };

    /// default constructor
    EnvelopeCurve();
    /// set parameters
    void Setup(float val0, float val1, float val2, float val3, float keyPos0, float keyPos1, float freq, float amp, ModFunc mod);
    /// sample at specific time (0..1)
    float Sample(float t) const;
    /// sample from t=0 to t=1 into array of values
    void PreSample(float* sampleBuffer, SizeT numSamples, SizeT sampleStride) const;
    /// get the max of val0, val1, val2, val3
    float GetMaxValue() const;

private:
    float values[4];
    float keyPos0;
    float keyPos1;
    float frequency;
    float amplitude;
    ModFunc modFunction;
};

//------------------------------------------------------------------------------
/**
*/
inline 
EnvelopeCurve::EnvelopeCurve() :
    keyPos0(0.25f),
    keyPos1(0.75f),
    frequency(0.0f),
    amplitude(0.0f),
    modFunction(Sine)
{
    this->values[0] = 0.0f;
    this->values[1] = 0.0f;
    this->values[2] = 0.0f;
    this->values[3] = 0.0f;
}

//------------------------------------------------------------------------------
/**
*/
inline void
EnvelopeCurve::Setup(float val0, float val1, float val2, float val3, float keyp0, float keyp1, float freq, float amp, ModFunc mod)
{
    this->values[0] = val0;
    this->values[1] = val1;
    this->values[2] = val2;
    this->values[3] = val3;
    this->keyPos0 = keyp0;
    this->keyPos1 = keyp1;
    this->frequency = freq;
    this->amplitude = amp;
    this->modFunction = mod;
}

//------------------------------------------------------------------------------
/**
    NOTE: Sampling a single value is relatively expensive. Consider
    pre-sampling into a lookup table!
*/
inline float
EnvelopeCurve::Sample(float t) const
{
    t = Math::n_saturate(t);

    float value;
    if (t < this->keyPos0)
    {
        value = Math::n_lerp(this->values[0], this->values[1], t / this->keyPos0);
    }
    else if (t < this->keyPos1)
    {
        value = Math::n_lerp(this->values[1], this->values[2], (t - this->keyPos0) / (this->keyPos1 - this->keyPos0));
    }
    else
    {
        value = Math::n_lerp(this->values[2], this->values[3], (t - this->keyPos1) / (1.0f - this->keyPos1));
    }
    if (this->amplitude > 0.0f)
    {
        if (Sine == this->modFunction)
        {
            value += Math::n_sin(t * 2.0f * N_PI * this->frequency) * this->amplitude;
        }
        else
        {
            value += Math::n_cos(t * 2.0f * N_PI * this->frequency) * this->amplitude;
        }
    }
    return value;
}

//------------------------------------------------------------------------------
/**
    This samples N values from t=0 to t=1 into an array. The array can then
    be used as a lookup table.
*/
inline void
EnvelopeCurve::PreSample(float* sampleBuffer, SizeT numSamples, SizeT sampleStride) const
{
    float t = 0.0f;
    float d = 1.0f / numSamples;
    IndexT sampleIndex = 0;
    IndexT i;
    for (i = 0; i < numSamples; i++)
    {
        sampleBuffer[sampleIndex] = this->Sample(t);
        sampleIndex += sampleStride;
        t += d;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline float
EnvelopeCurve::GetMaxValue() const
{
    return max(this->values[0], max(this->values[1], max(this->values[2], this->values[3])));
}

} // namespace Particles
//------------------------------------------------------------------------------
