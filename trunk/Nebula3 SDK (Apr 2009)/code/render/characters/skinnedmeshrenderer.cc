//------------------------------------------------------------------------------
//  skinnedmeshrenderer.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "characters/skinnedmeshrenderer.h"

namespace Characters
{
#if (__WIN32__ || __XBOX360__)
__ImplementClass(Characters::SkinnedMeshRenderer, 'SMRR', Win360::Win360SkinnedMeshRenderer);
#elif __WII__
__ImplementClass(Characters::SkinnedMeshRenderer, 'SMRR', Wii::WiiSkinnedMeshRenderer);
#else
#error "RenderDevice class not implemented on this platform!"
#endif

__ImplementSingleton(Characters::SkinnedMeshRenderer);

//------------------------------------------------------------------------------
/**
*/
SkinnedMeshRenderer::SkinnedMeshRenderer()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
SkinnedMeshRenderer::~SkinnedMeshRenderer()
{
    __DestructSingleton;
}

} // namespace Characters
