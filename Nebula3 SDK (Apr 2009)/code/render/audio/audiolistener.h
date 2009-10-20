#pragma once
#ifndef AUDIO_AUDIOLISTENER_H
#define AUDIO_AUDIOLISTENER_H
//------------------------------------------------------------------------------
/**
    @class Audio::AudioListener
    
    Client-side singleton which defines the position, orientation and
    velocity of the audio listener.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "math/matrix44.h"
#include "math/vector.h"

//------------------------------------------------------------------------------
namespace Audio
{
class AudioListener : public Core::RefCounted
{
    __DeclareClass(AudioListener);
    __DeclareSingleton(AudioListener);
public:
    /// constructor
    AudioListener();
    /// destructor
    virtual ~AudioListener();
    
    /// set world space transform
    void SetTransform(const Math::matrix44& m);
    /// get world space transform
    const Math::matrix44& GetTransform() const;
    /// set world space velocity
    void SetVelocity(const Math::vector& v);
    /// get world space velocity
    const Math::vector& GetVelocity() const;

    /// called per frame by AudioServer
    void OnFrame();

private:
    Math::matrix44 transform;
    Math::vector velocity;
};

//------------------------------------------------------------------------------
/**
*/
inline void
AudioListener::SetTransform(const Math::matrix44& m)
{
    this->transform = m;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
AudioListener::GetTransform() const
{
    return this->transform;
}

//------------------------------------------------------------------------------
/**
*/
inline void
AudioListener::SetVelocity(const Math::vector& v)
{
    this->velocity = v;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::vector&
AudioListener::GetVelocity() const
{
    return this->velocity;
}

} // namespace Audio
//------------------------------------------------------------------------------
#endif
    