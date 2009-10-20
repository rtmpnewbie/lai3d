//------------------------------------------------------------------------------
//  rendertargetbase.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/base/rendertargetbase.h"
#include "resources/sharedresourceserver.h"

namespace Base
{
__ImplementClass(Base::RenderTargetBase, 'RTGB', Core::RefCounted);

using namespace CoreGraphics;
using namespace Resources;

//------------------------------------------------------------------------------
/**
*/
RenderTargetBase::RenderTargetBase() :
    batchType(BatchType::InvalidBatchType),
    width(0),
    height(0),
    resolveTextureDimensionsValid(false),
    resolveTextureWidth(0),
    resolveTextureHeight(0),
    resolveRectValid(false),
    resolveRect(0, 0, 0, 0),
    mrtIndex(0),
#if NEBULA3_DEBUG
    clearColor(0.0f, 0.0f, 1.0f, 0.0f),
#else // on release clear defaultrendertarget to black for nice loadingscreen fading
    clearColor(0.0f, 0.0f, 0.0f, 0.0f),
#endif
    clearDepth(1.0f),
    clearStencil(0),
    antiAliasQuality(AntiAliasQuality::None),
    numColorBuffers(0),
    mipMapsEnabled(false),
    isValid(false),    
    inBeginPass(false),
    inBeginBatch(false),
    hasDepthStencilBuffer(false),
    isDefaultRenderTarget(false),
    resolveCpuAccess(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
RenderTargetBase::~RenderTargetBase()
{
    n_assert(!this->isValid);
}

//------------------------------------------------------------------------------
/**
*/
void
RenderTargetBase::Setup()
{
    n_assert(!this->isValid);
    this->isValid = true;
}

//------------------------------------------------------------------------------
/**
*/
void
RenderTargetBase::Discard()
{
    n_assert(this->isValid);
    n_assert(!this->inBeginPass);
    n_assert(!this->inBeginBatch);
    if (this->resolveTexture.isvalid())
    {
        SharedResourceServer* resServer = SharedResourceServer::Instance();
        if (resServer->HasSharedResource(this->resolveTexture->GetResourceId()))
        {
            resServer->UnregisterSharedResource(this->resolveTexture->GetResourceId());
        }
        this->resolveTexture = 0;
    }
    this->isValid = false;
}

//------------------------------------------------------------------------------
/**
*/
void
RenderTargetBase::BeginPass()
{
    n_assert(this->isValid);
    n_assert(!this->inBeginPass);
    n_assert(!this->inBeginBatch);
    this->inBeginPass = true;
}

//------------------------------------------------------------------------------
/**
*/
void
RenderTargetBase::BeginBatch(BatchType::Code t)
{
    n_assert(this->inBeginPass);
    n_assert(!this->inBeginBatch);
    this->inBeginBatch = true;
    this->batchType = t;
}

//------------------------------------------------------------------------------
/**
*/
void
RenderTargetBase::EndBatch()
{
    n_assert(this->inBeginBatch);
    this->inBeginBatch = false; 
    this->batchType = BatchType::InvalidBatchType;
}

//------------------------------------------------------------------------------
/**
*/
void
RenderTargetBase::EndPass()
{
    n_assert(this->isValid);
    n_assert(this->inBeginPass);
    n_assert(!this->inBeginBatch);
    this->inBeginPass = false;
}

//------------------------------------------------------------------------------
/**
*/
void
RenderTargetBase::GenerateMipLevels()
{
    n_assert(this->mipMapsEnabled);
}

//------------------------------------------------------------------------------
/**
*/
void
RenderTargetBase::ResolveDepthBuffer()
{
    n_error("RenderTargetBase::ResolveDepthBuffer not implemented!");
}

} // namespace Base