//------------------------------------------------------------------------------
//  particlerendererbase.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "particles/base/particlerendererbase.h"

namespace Base
{
__ImplementClass(Base::ParticleRendererBase, 'PTRB', Core::RefCounted);
__ImplementSingleton(Base::ParticleRendererBase);

using namespace Particles;

//------------------------------------------------------------------------------
/**
*/
ParticleRendererBase::ParticleRendererBase() :
    isValid(false),
    inAttach(false)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
ParticleRendererBase::~ParticleRendererBase()
{
    n_assert(!this->IsValid());
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleRendererBase::Setup()
{
    n_assert(!this->IsValid());
    this->isValid = true;
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleRendererBase::Discard()
{
    n_assert(this->IsValid());
    this->isValid = false;
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleRendererBase::BeginAttach()
{
    n_assert(this->IsValid());
    n_assert(!this->inAttach);
    this->inAttach = true;    
}

//------------------------------------------------------------------------------
/**
*/
ParticleRenderInfo
ParticleRendererBase::AddVisibleParticleSystem(const Ptr<ParticleSystemInstance>& particleSystem)
{
    n_assert(this->IsValid());
    n_assert(this->inAttach);
    n_assert(particleSystem.isvalid());
    return ParticleRenderInfo();
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleRendererBase::EndAttach()
{
    n_assert(this->IsValid());
    n_assert(this->inAttach);
    this->inAttach = false;
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleRendererBase::RenderParticleSystem(const ParticleRenderInfo& renderInfo)
{
    n_assert(this->IsValid());
    n_assert(!this->inAttach);
}

} // namespace Base
