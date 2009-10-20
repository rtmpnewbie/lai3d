//------------------------------------------------------------------------------
//  win360skinnedmeshrenderer.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "characters/win360/win360skinnedmeshrenderer.h"
#include "coregraphics/shaderserver.h"
#include "coregraphics/renderdevice.h"

namespace Win360
{
__ImplementClass(Win360::Win360SkinnedMeshRenderer, 'XSMR', Base::SkinnedMeshRendererBase);
__ImplementSingleton(Win360::Win360SkinnedMeshRenderer);

using namespace Math;
using namespace Util;
using namespace Characters;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
Win360SkinnedMeshRenderer::Win360SkinnedMeshRenderer()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
Win360SkinnedMeshRenderer::~Win360SkinnedMeshRenderer()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
Win360SkinnedMeshRenderer::DrawGPUSkinnedMesh(const Ptr<CharacterInstance>& charInst,
                                              const Ptr<Mesh>& mesh,
                                              IndexT primGroupIndex,
                                              const Array<IndexT>& jointPalette,
                                              const Ptr<ShaderVariable>& jointPaletteShdVar)
{
    const int maxJointPaletteSize = 72;
    matrix44 jointArray[maxJointPaletteSize];

    // extract joint palette skin matrices from skeleton
    const CharacterSkeletonInstance& skel = charInst->Skeleton();
    IndexT i;
    SizeT numJointsInPalette = jointPalette.Size();
    n_assert(numJointsInPalette <= maxJointPaletteSize);
    for (i = 0; i < numJointsInPalette; i++)
    {
        jointArray[i] = skel.GetJoint(jointPalette[i]).GetSkinMatrix();
    }

    // commit skin matrices into joint palette shader variable
    jointPaletteShdVar->SetMatrixArray(jointArray, numJointsInPalette);
    ShaderServer::Instance()->GetActiveShaderInstance()->Commit();

    // apply mesh data and draw skinned primitives
    mesh->ApplyPrimitives(primGroupIndex);
    RenderDevice::Instance()->Draw();
}

} // namespace Characters