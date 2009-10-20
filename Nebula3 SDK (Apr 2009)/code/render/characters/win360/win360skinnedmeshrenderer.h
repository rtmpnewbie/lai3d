#pragma once
//------------------------------------------------------------------------------
/** 
    @class Win360::Win360SkinnedMeshRenderer
  
    D3D9/Xbox360 implemention of SkinnedMeshRenderer.
    
    (C) 2008 Radon Labs GmbH
*/    
#include "characters/base/skinnedmeshrendererbase.h"

//------------------------------------------------------------------------------
namespace Win360
{
class Win360SkinnedMeshRenderer : public Base::SkinnedMeshRendererBase
{
    __DeclareClass(Win360SkinnedMeshRenderer);
    __DeclareSingleton(Win360SkinnedMeshRenderer);
public:
    /// constructor
    Win360SkinnedMeshRenderer();
    /// destructor
    virtual ~Win360SkinnedMeshRenderer();
    /// skinning happens on the GPU
    bool RequiresSoftwareSkinning() const;
    /// draw a skinned mesh
    void DrawGPUSkinnedMesh(const Ptr<Characters::CharacterInstance>& charInst, const Ptr<CoreGraphics::Mesh>& mesh, IndexT primGroupIndex, const Util::Array<IndexT>& jointPalette, const Ptr<CoreGraphics::ShaderVariable>& jointPaletteShdVar);
};

//------------------------------------------------------------------------------
/**
*/
inline bool
Win360SkinnedMeshRenderer::RequiresSoftwareSkinning() const
{
    return false;
}

} // namespace Win360
//------------------------------------------------------------------------------
