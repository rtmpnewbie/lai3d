#pragma once
//------------------------------------------------------------------------------
/**
    @class Characters::CharacterSkinNodeInstance
    
    Implements per-instance functionality of a character skin node.
    
    (C) 2008 Radon Labs GmbH
*/
#include "models/nodes/shapenodeinstance.h"
#include "characters/characterinstance.h"
#include "characters/skinnedmeshrenderer.h"

//------------------------------------------------------------------------------
namespace Characters
{
class CharacterSkinNodeInstance : public Models::ShapeNodeInstance
{
    __DeclareClass(CharacterSkinNodeInstance);
public:
    /// constructor
    CharacterSkinNodeInstance();
    /// destructor
    virtual ~CharacterSkinNodeInstance();

    /// called when attached to ModelInstance
    virtual void Setup(const Ptr<Models::ModelInstance>& inst, const Ptr<Models::ModelNode>& node, const Ptr<Models::ModelNodeInstance>& parentNodeInst);
    /// called when removed from ModelInstance
    virtual void Discard();
    /// perform per-frame updates
    virtual void Update(Timing::Time time);    
    /// perform rendering
    virtual void Render();

protected:
    Ptr<CharacterInstance> characterInstance;
    Ptr<CoreGraphics::ShaderVariable> jointPaletteShdVar;
    SkinnedMeshRenderer::DrawHandle drawHandle;
};

} // namespace Characters
//------------------------------------------------------------------------------
    