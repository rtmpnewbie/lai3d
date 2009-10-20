#pragma once
//------------------------------------------------------------------------------
/**
    @class Base::ParticleRendererBase
    
    Base class for platform-specific particle renders.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "particles/particlesysteminstance.h"
#include "particles/particlerenderinfo.h"

//------------------------------------------------------------------------------
namespace Base
{
class ParticleRendererBase : public Core::RefCounted
{
    __DeclareClass(ParticleRendererBase);
    __DeclareSingleton(ParticleRendererBase);
public:
    /// constructor
    ParticleRendererBase();
    /// destructor
    virtual ~ParticleRendererBase();

    /// setup the particle renderer
    void Setup();
    /// discard the particle renderer
    void Discard();
    /// return true if particle renderer has been setup
    bool IsValid() const;

    /// begin adding visible particle systems
    void BeginAttach();
    /// attach a visible particle system instance
    Particles::ParticleRenderInfo AddVisibleParticleSystem(const Ptr<Particles::ParticleSystemInstance>& particleSystem);
    /// finish adding visible particle sytems
    void EndAttach();

    /// render particles of previously attached particle system
    void RenderParticleSystem(const Particles::ParticleRenderInfo& renderInfo);

protected:
    bool isValid;
    bool inAttach;
};    

//------------------------------------------------------------------------------
/**
*/
inline bool
ParticleRendererBase::IsValid() const
{
    return this->isValid;
}

} // namespace ParticleRendererBase
//------------------------------------------------------------------------------
 