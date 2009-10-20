#pragma once
//------------------------------------------------------------------------------
/**
    @class Particles::ParticleSystemNodeInstance
    
    Wraps a ParticleSystemInstance into a ModelNodeInstance.
    
    (C) 2008 Radon Labs GmbH
*/
#include "models/nodes/statenodeinstance.h"
#include "particles/particlesysteminstance.h"
#include "particles/particlerenderinfo.h"

//------------------------------------------------------------------------------
namespace Particles
{
class ParticleSystemNodeInstance : public Models::StateNodeInstance
{
    __DeclareClass(ParticleSystemNodeInstance);
public:
    /// constructor
    ParticleSystemNodeInstance();
    /// destructor
    virtual ~ParticleSystemNodeInstance();

    /// perform per-frame updates
    virtual void Update(Timing::Time time);
    /// perform rendering
    virtual void Render();

    /// get the node's particle system instance
    const Ptr<ParticleSystemInstance>& GetParticleSystemInstance() const;

protected:
    /// called when attached to ModelInstance
    virtual void Setup(const Ptr<Models::ModelInstance>& inst, const Ptr<Models::ModelNode>& node, const Ptr<Models::ModelNodeInstance>& parentNodeInst);
    /// called when removed from ModelInstance
    virtual void Discard();
    /// notify that we are visible
    virtual void OnNotifyVisible(IndexT frameIndex);
    /// render node specific debug shape
    virtual void RenderDebug();
    /// called when the node becomes visible with current time
    virtual void OnShow(Timing::Time time);
    /// called when the node becomes invisible
    virtual void OnHide(Timing::Time time);
    /// check lod distances
    bool InsideLodDistances();

    Ptr<ParticleSystemInstance> particleSystemInstance;
    ParticleRenderInfo renderInfo;
};

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<ParticleSystemInstance>&
ParticleSystemNodeInstance::GetParticleSystemInstance() const
{
    return this->particleSystemInstance;
}

} // namespace Particles
//------------------------------------------------------------------------------
    