//------------------------------------------------------------------------------
//  skinnedmeshrendererbase.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "characters/base/skinnedmeshrendererbase.h"

namespace Base
{
__ImplementClass(Base::SkinnedMeshRendererBase, 'SMRB', Core::RefCounted);
__ImplementSingleton(Base::SkinnedMeshRendererBase);

using namespace Util;
using namespace Characters;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
SkinnedMeshRendererBase::SkinnedMeshRendererBase() :
    isValid(false),
    inFrame(false)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
SkinnedMeshRendererBase::~SkinnedMeshRendererBase()
{
    if (this->IsValid())
    {
        this->Discard();
    }
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
SkinnedMeshRendererBase::Setup()
{
    n_assert(!this->IsValid());
    this->isValid = true;
    this->inFrame = false;
    this->drawInfos.Clear();
}

//------------------------------------------------------------------------------
/**
*/
void
SkinnedMeshRendererBase::Discard()
{
    n_assert(this->IsValid());
    this->isValid = false;
    this->drawInfos.Clear();
}

//------------------------------------------------------------------------------
/**
*/
void
SkinnedMeshRendererBase::OnBeginFrame()
{
    n_assert(!this->inFrame);
    this->inFrame = true;
}

//------------------------------------------------------------------------------
/**
*/
void
SkinnedMeshRendererBase::OnEndFrame()
{
    n_assert(this->inFrame);
    this->inFrame = false;
    this->drawInfos.Clear();
    this->keyIndexMap.Clear();
}   

//------------------------------------------------------------------------------
/**
    This method should only be called when RequiresSoftwareSkinning()
    returns true!

    If a platform needs to perform once-per-frame actions for character 
    skinning, it should happen in a derived method. NOTE that this method
    may be called more then once-per-frame for the same character instance/mesh
    combo because a skin mesh may consist of different material groups. We don't
    want to software-skin several times in this case.
    
    On platforms with no software skinning this method could be implemented 
    more simple (nothing needs to happen inside UpdateSkinnedMesh()...
*/
SkinnedMeshRendererBase::DrawHandle
SkinnedMeshRendererBase::UpdateSoftwareSkinnedMesh(const Ptr<CharacterInstance>& charInst, const Ptr<CoreGraphics::Mesh>& mesh)
{
    n_assert(this->inFrame);
    
    // check if charInst/mesh was already updated this frame
    DrawKey key(charInst.get(), mesh.get());
    IndexT drawInfoIndex = this->keyIndexMap.FindIndex(key);
    if (InvalidIndex == drawInfoIndex)
    {
        // this is a new skin
        DrawInfo drawInfo;
        drawInfo.charInst = charInst;
        drawInfo.mesh = mesh;
        this->drawInfos.Append(drawInfo);
        drawInfoIndex = this->drawInfos.Size() - 1;                
        this->keyIndexMap.Add(key, drawInfoIndex);
    }
    return drawInfoIndex;
}

//------------------------------------------------------------------------------
/**
    This method should only be called when RequiresSoftwareSkinning()
    returns true!

    Software-skinning platforms call this method with the DrawHandle returned
    by UpdateSoftwareSkinnedMesh() to draw a portion of the skinned mesh/
*/
void 
SkinnedMeshRendererBase::DrawSoftwareSkinnedMesh(DrawHandle h, IndexT primGroupIndex)
{
    n_error("SkinnedMeshRendererBase::DrawSkinnedMesh() called!");
}

//------------------------------------------------------------------------------
/**
    This method should only be called when RequiresSoftwareSkinning()
    returns false!

    This is the skinned-mesh rendering method for platforms which do skinning
    on the GPU.
*/
void
SkinnedMeshRendererBase::DrawGPUSkinnedMesh(const Ptr<Characters::CharacterInstance>& charInst, const Ptr<CoreGraphics::Mesh>& mesh, IndexT primGroupIndex, const Util::Array<IndexT>& jointPalette, const Ptr<CoreGraphics::ShaderVariable>& jointPaletteShdVar)
{
    n_error("SkinnedMeshRendererBase::DrawGPUSkinnedMesh() called!");
}

} // namespace Base