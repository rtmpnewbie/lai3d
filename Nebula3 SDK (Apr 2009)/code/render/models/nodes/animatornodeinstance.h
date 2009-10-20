#pragma once
#ifndef ANIMATOR_ANIMATORNODEINSTANCE_H
#define ANIMATOR_ANIMATORNODEINSTANCE_H
//------------------------------------------------------------------------------
/**
    @class Animator::AnimatorInstance
    
    (C) 2008 Radon Labs GmbH
*/

#include "models/modelnodeinstance.h"
#include "timing/time.h"
#include "models/nodes/animlooptype.h"
#include "models/nodes/animkeyarray.h"
#include "models/nodes/animkey.h"
#include "math/vector.h"
#include "coregraphics/shadervariable.h"
#include "coregraphics/shadervariableinstance.h"

namespace Models
{
class AnimatorNodeInstance : public Models::ModelNodeInstance
{
__DeclareClass(AnimatorNodeInstance);
public:
    /// Constructor
    AnimatorNodeInstance();
    /// Destructor
    virtual ~AnimatorNodeInstance();
    /// setup the model node instance
    virtual void Setup(const Ptr<ModelInstance>& inst, const Ptr<ModelNode>& node, const Ptr<ModelNodeInstance>& parentNodeInst);    
    /// perform per-frame updates
    virtual void Update(Timing::Time time);

private:
    /// called by scene node objects which wish to be animated by this object
    void Animate(Timing::Time time);
    /// called when the node becomes visible with current time
    virtual void OnShow(Timing::Time time);

    struct AnimatedNode
    {
        Ptr<ModelNodeInstance> node;
        Ptr<CoreGraphics::ShaderVariableInstance> shaderVar;
    };

    Util::Array<Util::Array<AnimatedNode> > animSection;
    Timing::Time startTime;
};
__RegisterClass(AnimatorNodeInstance);

};
#endif