//------------------------------------------------------------------------------
//  characterskinnodeinstance.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "characters/characterskinnodeinstance.h"
#include "characters/characterskinnode.h"
#include "resources/managedmesh.h"
#include "models/modelinstance.h"
#include "characters/characternodeinstance.h"
#include "coregraphics/shader.h"
#include "coregraphics/renderdevice.h"
#include "characters/skinnedmeshrenderer.h"

namespace Characters
{
__ImplementClass(Characters::CharacterSkinNodeInstance, 'CSNI', Models::ShapeNodeInstance);

using namespace Util;
using namespace Resources;
using namespace CoreGraphics;
using namespace Models;

//------------------------------------------------------------------------------
/**
*/
CharacterSkinNodeInstance::CharacterSkinNodeInstance()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
CharacterSkinNodeInstance::~CharacterSkinNodeInstance()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterSkinNodeInstance::Setup(const Ptr<ModelInstance>& inst, 
                                 const Ptr<ModelNode>& node, 
                                 const Ptr<ModelNodeInstance>& parentNodeInst)
{
    // need to call parent class first
    ShapeNodeInstance::Setup(inst, node, parentNodeInst);

    // lookup pointer to our character instance object
    this->characterInstance = inst->GetRootNodeInstance().downcast<CharacterNodeInstance>()->GetCharacterInstance();

    // lookup joint palette shader variable
    if (!SkinnedMeshRenderer::Instance()->RequiresSoftwareSkinning())
    {
        ShaderVariable::Semantic jointPaletteSemantic("JointPalette");
        const Ptr<ShaderInstance>& shdInst = node.downcast<StateNode>()->GetShaderInstance();
        if (shdInst->HasVariableBySemantic(jointPaletteSemantic))
        {
            this->jointPaletteShdVar = shdInst->GetVariableBySemantic(jointPaletteSemantic);
        }
        else
        {
            n_printf("WARNING: shader '%s' has no joint palette!\n", shdInst->GetOriginalShader()->GetResourceId().Value().AsCharPtr());
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterSkinNodeInstance::Discard()
{
    this->characterInstance = 0;
    this->jointPaletteShdVar = 0;
    ShapeNodeInstance::Discard();
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterSkinNodeInstance::Update(Timing::Time time)
{
    // on software-skinned platforms, we need to update the skin mesh
    SkinnedMeshRenderer* smRenderer = SkinnedMeshRenderer::Instance();
    if (smRenderer->RequiresSoftwareSkinning())
    {
        this->drawHandle = SkinnedMeshRenderer::InvalidDrawHandle;
        const Ptr<CharacterSkinNode>& myNode = this->modelNode.cast<CharacterSkinNode>();
        const Ptr<ManagedMesh> managedMesh = myNode->GetManagedMesh();
        if (managedMesh->GetState() == Resource::Loaded)
        {
            const Ptr<Mesh>& mesh = managedMesh->GetMesh();
            this->drawHandle = smRenderer->UpdateSoftwareSkinnedMesh(this->characterInstance, mesh);
        }
    }
    ShapeNodeInstance::Update(time);       
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterSkinNodeInstance::Render()
{
    // different code paths for software and GPU-skinned platforms
    SkinnedMeshRenderer* smRenderer = SkinnedMeshRenderer::Instance();
    if (smRenderer->RequiresSoftwareSkinning())
    {
        // software-skinned path (e.g. on Wii)
        if (this->drawHandle != SkinnedMeshRenderer::InvalidDrawHandle)
        {        
            const Ptr<CharacterSkinNode>& myNode = this->modelNode.cast<CharacterSkinNode>();
            IndexT fragIndex;
            SizeT numFragments = myNode->GetNumFragments();
            for (fragIndex = 0; fragIndex < numFragments; fragIndex++)
            {
                IndexT primGroupIndex = myNode->GetFragmentPrimGroupIndex(fragIndex);
                smRenderer->DrawSoftwareSkinnedMesh(this->drawHandle, primGroupIndex);
            }
        }
    }
    else
    {
        // GPU-skinned path
        const Ptr<CharacterSkinNode>& myNode = this->modelNode.cast<CharacterSkinNode>();
        const Ptr<ManagedMesh> managedMesh = myNode->GetManagedMesh();
        if (managedMesh->GetState() == Resource::Loaded)
        {
            const Ptr<Mesh>& mesh = managedMesh->GetMesh();
    
            // mesh available, render actual skin fragments
            IndexT fragIndex;
            SizeT numFragments = myNode->GetNumFragments();
            for (fragIndex = 0; fragIndex < numFragments; fragIndex++)
            {
                IndexT primGroupIndex = myNode->GetFragmentPrimGroupIndex(fragIndex);
                const Array<IndexT>& jointPalette = myNode->GetFragmentJointPalette(fragIndex);    
                smRenderer->DrawGPUSkinnedMesh(this->characterInstance,
                                               mesh,
                                               primGroupIndex,
                                               jointPalette,
                                               this->jointPaletteShdVar);
            }
        }
    }
}
 
} // namespace Characters
