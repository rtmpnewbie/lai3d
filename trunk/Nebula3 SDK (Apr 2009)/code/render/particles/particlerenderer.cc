//------------------------------------------------------------------------------
//  particlerenderer.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "particles/particlerenderer.h"

namespace Particles
{
#if __WIN32__
__ImplementClass(Particles::ParticleRenderer, 'PRRD', Direct3D9::D3D9ParticleRenderer);
__ImplementSingleton(Particles::ParticleRenderer);
#elif __XBOX360__
__ImplementClass(Particles::ParticleRenderer, 'PRRD', Xbox360::Xbox360ParticleRenderer);
__ImplementSingleton(Particles::ParticleRenderer);
#else
#error "Particles::ParticleRenderer not implemented on this platform!"
#endif

//------------------------------------------------------------------------------
/**
*/
ParticleRenderer::ParticleRenderer()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
ParticleRenderer::~ParticleRenderer()
{
    __DestructSingleton;
}

} // namespace Particles
