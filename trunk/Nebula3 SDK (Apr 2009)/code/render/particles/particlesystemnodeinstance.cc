//------------------------------------------------------------------------------
//  particlesystemnodeinstance.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "particles/particlesystemnodeinstance.h"
#include "particles/particlesystemnode.h"
#include "coregraphics/transformdevice.h"
#include "particles/particlerenderer.h"

// DEBUG
#include "models/model.h"

namespace Particles
{
__ImplementClass(Particles::ParticleSystemNodeInstance, 'PSNI', Models::StateNodeInstance);

using namespace Models;
using namespace Util;
using namespace CoreGraphics;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
ParticleSystemNodeInstance::ParticleSystemNodeInstance()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ParticleSystemNodeInstance::~ParticleSystemNodeInstance()
{
    n_assert(!this->particleSystemInstance.isvalid());
}    

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemNodeInstance::OnNotifyVisible(IndexT frameIndex)
{
    // check if node is inside lod distances or if no lod is used
    if (this->InsideLodDistances())
    {
        // just tell our model node that we are a visible instance
        this->modelNode->AddVisibleNodeInstance(frameIndex, this);
    }    
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemNodeInstance::Setup(const Ptr<ModelInstance>& inst, const Ptr<ModelNode>& node, const Ptr<ModelNodeInstance>& parentNodeInst)
{
    n_assert(!this->particleSystemInstance.isvalid());

    // up to parent class
    StateNodeInstance::Setup(inst, node, parentNodeInst);

    // setup a new particle system instance
    this->particleSystemInstance = ParticleSystemInstance::Create();
    const Ptr<ParticleSystemNode>& particleSystemNode = node.downcast<ParticleSystemNode>();
    const Ptr<ParticleSystem>& particleSystem = particleSystemNode->GetParticleSystem();
    this->particleSystemInstance->Setup(particleSystem);
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemNodeInstance::Discard()
{
    n_assert(this->particleSystemInstance.isvalid());

    // discard our particle system instance
    this->particleSystemInstance->Discard();
    this->particleSystemInstance = 0;

    // up to parent-class
    StateNodeInstance::Discard();
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemNodeInstance::Update(Timing::Time time)
{
    // reset the render-info structure
    this->renderInfo.Clear();

    // call parent class
    StateNodeInstance::Update(time);    

    // update particle system with new model transform
    this->particleSystemInstance->SetTransform(this->modelTransform);

    // updating happens in 2 stages:
    // 1) within activity distance: particles are emitted and updated
    // 2) in view volume: particle system is added for rendering
    const point& eyePos = TransformDevice::Instance()->GetInvViewTransform().get_position();
    const point& myPos  = this->modelTransform.get_position();
    float dist = float4(myPos - eyePos).length();
    float activityDist = this->particleSystemInstance->GetParticleSystem()->GetEmitterAttrs().GetFloat(EmitterAttrs::ActivityDistance);
    if (dist <= activityDist)
    {
        // alright, we're within the activity distance, update the particle system
        this->particleSystemInstance->Update(time);

        // check if we're also in the view volume, and if yes, 
        // register the particle system for rendering
        // FIXME: use actual particle bounding box!!!
        const bbox& globalBox = this->particleSystemInstance->GetBoundingBox();
        const matrix44& viewProj = TransformDevice::Instance()->GetViewProjTransform();
        if (ClipStatus::Outside != globalBox.clipstatus(viewProj))
        {
            // yes, we're visible
            this->renderInfo = ParticleRenderer::Instance()->AddVisibleParticleSystem(this->particleSystemInstance);
        }
        else
        {
            // FIXME DEBUG
            // n_printf("%f: Particle system invisible (clip) %s!\n", time, this->modelNode->GetName().Value().AsCharPtr());
        }
    }
    else
    {
        // FIXME DEBUG
        //n_printf("Particle system invisible (activity dist) %s!\n", this->modelNode->GetName().Value().AsCharPtr());
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
ParticleSystemNodeInstance::Render()
{
    StateNodeInstance::Render();
    ParticleRenderer::Instance()->RenderParticleSystem(this->renderInfo);
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemNodeInstance::RenderDebug()
{
    StateNodeInstance::RenderDebug();
    this->particleSystemInstance->RenderDebug();
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemNodeInstance::OnShow(Timing::Time time)
{
    this->particleSystemInstance->Start();
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemNodeInstance::OnHide(Timing::Time time)
{
    // FIXME: should stop immediately?
    if (this->particleSystemInstance->IsPlaying())
    {
        this->particleSystemInstance->Stop();
    }
}

//------------------------------------------------------------------------------
/**
*/
bool 
ParticleSystemNodeInstance::InsideLodDistances()
{
    const Ptr<TransformNode>& transformNode = this->modelNode.cast<TransformNode>();
    if (transformNode->LodDistancesUsed())
    {
        const point& eyePos = TransformDevice::Instance()->GetInvViewTransform().get_position();
        const point& modelPos = this->GetModelTransform().get_position();
        vector distVec = eyePos - modelPos;
        float dist = distVec.length();
        if (dist > transformNode->GetMinDistance()
            && dist < transformNode->GetMaxDistance())
        {
            return true;
        }
    }
    else
    {
        return true;
    }
    return false;
}
} // namespace Particles
