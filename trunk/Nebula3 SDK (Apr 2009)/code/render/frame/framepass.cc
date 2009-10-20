//------------------------------------------------------------------------------
//  framepass.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "frame/framepass.h"
#include "coregraphics/renderdevice.h"
#include "rendermodules/rt/rtpluginregistry.h"

namespace Frame
{
__ImplementClass(Frame::FramePass, 'FPSS', Core::RefCounted);

using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
FramePass::FramePass() :
    clearDepth(1.0f),
    clearStencil(0),
    extraClearColor(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
FramePass::~FramePass()
{
    // make sure Discard() has been called
    n_assert(!this->shader.isvalid());
    n_assert(!this->renderTarget.isvalid());
    n_assert(!this->multipleRenderTarget.isvalid());
    n_assert(this->shaderVariables.IsEmpty());
    n_assert(this->batches.IsEmpty());
}

//------------------------------------------------------------------------------
/**
*/
void
FramePass::Discard()
{
    if (this->shader.isvalid())
    {
        this->shader->Discard();
        this->shader = 0;
    }
    if (this->renderTarget.isvalid())
    {
        if (this->renderTarget->IsValid())
        {
            this->renderTarget->Discard();
        }
        this->renderTarget = 0;
    }
    if (this->multipleRenderTarget.isvalid())
    {
        this->multipleRenderTarget = 0;
    }
    this->shaderVariables.Clear();
    IndexT i;
    for (i = 0; i < this->batches.Size(); i++)
    {
        this->batches[i]->Discard();
    }
    this->batches.Clear();

    _discard_timer(debugTimer);
}

//------------------------------------------------------------------------------
/**
*/
void
FramePass::Render()
{
    _start_timer(this->debugTimer);

    n_assert(this->renderTarget.isvalid() ||this->multipleRenderTarget.isvalid());
    RenderDevice* renderDevice = RenderDevice::Instance();

    if (this->renderTarget.isvalid())
    {
        n_assert(!this->multipleRenderTarget.isvalid());
        // update render targets
        this->renderTarget->SetClearColor(this->clearColor);
        this->renderTarget->SetClearDepth(this->clearDepth);
        this->renderTarget->SetClearStencil(this->clearStencil);  
    }  
    else if (this->multipleRenderTarget.isvalid())
    {
        n_assert(!this->renderTarget.isvalid());

    }

    // apply shader variables
    IndexT varIndex;
    for (varIndex = 0; varIndex < this->shaderVariables.Size(); varIndex++)
    {
        this->shaderVariables[varIndex]->Apply();
    }

    // render batches...
    if (this->renderTarget.isvalid())
    {
        renderDevice->BeginPass(this->renderTarget, this->shader);
    }
    else
    {
        renderDevice->BeginPass(this->multipleRenderTarget, this->shader);
    }
    RenderModules::RTPluginRegistry* rtPluginRegistry = RenderModules::RTPluginRegistry::Instance();
    IndexT batchIndex;
    for (batchIndex = 0; batchIndex < this->batches.Size(); batchIndex++)
    {
        this->batches[batchIndex]->Render();
        rtPluginRegistry->OnRenderFrameBatch(this->batches[batchIndex]);
    }
    renderDevice->EndPass();

    _stop_timer(this->debugTimer);
}

//------------------------------------------------------------------------------
/**
*/
#if NEBULA3_ENABLE_PROFILING
void 
FramePass::SetFramePassDebugTimer(const Util::String& name)
{
    this->debugTimer = Debug::DebugTimer::Create();
    this->debugTimer->Setup(name);
}
#endif
} // namespace Frame
