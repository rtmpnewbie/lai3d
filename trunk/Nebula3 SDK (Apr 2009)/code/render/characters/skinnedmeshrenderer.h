#pragma once
//------------------------------------------------------------------------------
/**
    @class Characters::SkinnedMeshRenderer
    
    Wrapper class for platform-specific skinned mesh rendering.
    
    (C) 2008 Radon Labs GmbH
*/
#if (__WIN32__ || __XBOX360__)
#include "characters/win360/win360skinnedmeshrenderer.h"
namespace Characters
{
class SkinnedMeshRenderer : public Win360::Win360SkinnedMeshRenderer
{
    __DeclareClass(SkinnedMeshRenderer);
    __DeclareSingleton(SkinnedMeshRenderer);
public:
    /// constructor
    SkinnedMeshRenderer();
    /// destructor
    virtual ~SkinnedMeshRenderer();
};
}
#elif __WII__
#include "characters/wii/wiiskinnedmeshrenderer.h"
namespace Characters
{
class SkinnedMeshRenderer : public Wii::WiiSkinnedMeshRenderer
{
    __DeclareClass(SkinnedMeshRenderer);
    __DeclareSingleton(SkinnedMeshRenderer);
public:
    /// constructor
    SkinnedMeshRenderer();
    /// destructor
    virtual ~SkinnedMeshRenderer();
};
}
#endif
//------------------------------------------------------------------------------
    