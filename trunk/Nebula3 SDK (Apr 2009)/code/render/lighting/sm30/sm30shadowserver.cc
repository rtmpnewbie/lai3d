//------------------------------------------------------------------------------
//  sm30shadowserver.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "lighting/sm30/sm30shadowserver.h"
#include "frame/frameserver.h"
#include "coregraphics/transformdevice.h"
#include "models/visresolver.h"
#include "internalgraphics/internalmodelentity.h"

namespace Lighting
{
__ImplementClass(Lighting::SM30ShadowServer, 'S3SS', Lighting::ShadowServerBase);

using namespace Math;
using namespace Util;
using namespace Frame;
using namespace Resources;
using namespace CoreGraphics;
using namespace InternalGraphics;
using namespace Models;

//------------------------------------------------------------------------------
/**
*/
SM30ShadowServer::SM30ShadowServer()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
SM30ShadowServer::~SM30ShadowServer()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
SM30ShadowServer::Open()
{
    // call parent class
    ShadowServerBase::Open();

    // load the ShadowBuffer frame shader
    FrameServer* frameServer = FrameServer::Instance();
    if (!frameServer->HasFrameShader(ResourceId("VSMShadowMapping")))
    {
        n_error("SM30ShadowServer::Open(): Frame shader 'VSMShadowMapping' not found!");
    }
    this->frameShader = frameServer->GetFrameShaderByName(ResourceId("VSMShadowMapping"));
    n_assert(this->frameShader.isvalid());
    this->finalPostEffect = this->frameShader->GetPostEffectByName(ResourceId("Final"));

    // setup the shadow buffer render target, this is a single
    // render target which contains the shadow buffer data for
    // all shadow casting light sources
    const Ptr<RenderTarget>& origRt = this->frameShader->GetRenderTargetByName(ResourceId("VSMDownscale"));
    SizeT rtWidth = origRt->GetWidth();
    SizeT rtHeight = origRt->GetHeight();
    SizeT resolveWidth = rtWidth * MaxNumShadowLights;
    SizeT resolveHeight = rtHeight;
    PixelFormat::Code pixelFormat = origRt->GetColorBufferFormat(0);
    
    // create a shadow buffer for up to MaxNumShadowLights local light sources
    this->localLightShadowBuffer = RenderTarget::Create();
    this->localLightShadowBuffer->SetWidth(rtWidth);
    this->localLightShadowBuffer->SetHeight(rtHeight);
    this->localLightShadowBuffer->SetAntiAliasQuality(AntiAliasQuality::None);
    this->localLightShadowBuffer->AddColorBuffer(pixelFormat);
    this->localLightShadowBuffer->SetMipMapsEnabled(true);
    this->localLightShadowBuffer->SetResolveTextureResourceId(ResourceId("ShadowBuffer"));
    this->localLightShadowBuffer->SetResolveTextureWidth(resolveWidth);
    this->localLightShadowBuffer->SetResolveTextureHeight(resolveHeight);
    this->localLightShadowBuffer->Setup();

    // create a shadow buffer for Parallel-Split-Shadow-Maps (used for the global light source)
    // NOTE: PSSM-ShadowBuffer must be same size as LocalLight-ShadowBuffer
    /*this->pssmShadowBuffer = RenderTarget::Create();
    this->pssmShadowBuffer->SetWidth(rtWidth);
    this->pssmShadowBuffer->SetHeight(rtHeight);
    this->pssmShadowBuffer->SetAntiAliasQuality(AntiAliasQuality::None);
    this->pssmShadowBuffer->AddColorBuffer(pixelFormat);
    this->pssmShadowBuffer->SetMipMapsEnabled(true);
    this->pssmShadowBuffer->SetResolveTextureResourceId(ResourceId("PSSMShadowBuffer"));
    this->pssmShadowBuffer->SetResolveTextureWidth(rtWidth * PSSMUtil::NumSplits);
    this->pssmShadowBuffer->SetResolveTextureHeight(rtHeight);
    this->pssmShadowBuffer->Setup();*/
}

//------------------------------------------------------------------------------
/**
*/
void
SM30ShadowServer::Close()
{
    // release shadow buffers
    this->localLightShadowBuffer->Discard();
    this->localLightShadowBuffer = 0;
    /*this->pssmShadowBuffer->Discard();
    this->pssmShadowBuffer = 0;*/

    // release frame shader
    this->finalPostEffect = 0;
    this->frameShader->Discard();
    this->frameShader = 0;

    // call parent class
    ShadowServerBase::Close();
}

//------------------------------------------------------------------------------
/**
    This method updates the internal shadow buffer render taregets.
*/
void
SM30ShadowServer::UpdateShadowBuffers()
{
    n_assert(this->inBeginFrame);
    n_assert(!this->inBeginAttach);

    // update local lights shadow buffer
    if (this->localLightEntities.Size() > 0)
    {
        this->UpdateLocalLightShadowBuffers();
    }

    // update global ligth parallel-split-shadow-map shadow buffers
    if (this->globalLightEntity.isvalid())
    {
        //this->UpdatePSSMShadowBuffers();
    }
}

//------------------------------------------------------------------------------
/**
    Update the local lights shadow buffer.
*/
void
SM30ShadowServer::UpdateLocalLightShadowBuffers()
{
    VisResolver* visResolver = VisResolver::Instance();
    TransformDevice* transDev = TransformDevice::Instance();

    // store original view and projection transform
    matrix44 origView = transDev->GetViewTransform();
    matrix44 origProj = transDev->GetProjTransform();

    // for each shadow casting light...
    SizeT numLights = this->localLightEntities.Size();
    if (numLights > MaxNumShadowLights)
    {
        numLights = MaxNumShadowLights;
    }
    IndexT lightIndex;
    for (lightIndex = 0; lightIndex < numLights; lightIndex++)
    {
        // render shadow casters in current light volume to shadow buffer
        const Ptr<InternalAbstractLightEntity>& lightEntity = this->localLightEntities[lightIndex];
        transDev->SetViewTransform(lightEntity->GetInvTransform());
        transDev->SetProjTransform(lightEntity->GetProjTransform());

        // perform visibility resolve for current light
        visResolver->BeginResolve();
        const Array<Ptr<InternalGraphicsEntity> >& visLinks = lightEntity->GetLinks(InternalGraphicsEntity::LightLink);
        IndexT linkIndex;
        for (linkIndex = 0; linkIndex < visLinks.Size(); linkIndex++)
        {
            const Ptr<InternalGraphicsEntity>& curEntity = visLinks[linkIndex];
            n_assert(InternalGraphicsEntityType::Model == curEntity->GetType());
            const Ptr<InternalModelEntity>& modelEntity = curEntity.downcast<InternalModelEntity>();
            visResolver->AttachVisibleModelInstance(modelEntity->GetModelInstance());
        }
        visResolver->EndResolve();

        // prepare shadow buffer render target for rendering and 
        // patch current shadow buffer render target into the frame shader
        Math::rectangle<int> resolveRect;
        resolveRect.left   = lightIndex * this->localLightShadowBuffer->GetWidth();
        resolveRect.right  = resolveRect.left + this->localLightShadowBuffer->GetWidth();
        resolveRect.top    = 0;
        resolveRect.bottom = this->localLightShadowBuffer->GetHeight();
        this->localLightShadowBuffer->SetResolveRect(resolveRect);
        this->finalPostEffect->SetRenderTarget(this->localLightShadowBuffer);

        // render the resolved model node instances into the main shadow buffer
        this->frameShader->Render();

        // patch shadow buffer and shadow buffer uv offset into the light source  
        // uvOffset.xy is offset
        // uvOffset.zw is modulate
        // also moves projection space coords into uv space
        float shadowBufferHoriPixelSize = 1.0f / this->localLightShadowBuffer->GetResolveTextureWidth();
        Math::float4 uvOffset;
        uvOffset.x() = float(lightIndex) / float(MaxNumShadowLights);
        uvOffset.y() = 0.0f;
        uvOffset.z() = (1.0f - shadowBufferHoriPixelSize) / float(MaxNumShadowLights);
        uvOffset.w() = 1.0f;
        lightEntity->SetShadowBufferUvOffsetAndScale(uvOffset);
    }

    // generate mip levels for the shadow buffer
    this->localLightShadowBuffer->GenerateMipLevels();

    // restore original view and projection transforms
    transDev->SetViewTransform(origView);
    transDev->SetProjTransform(origProj);
}

//------------------------------------------------------------------------------
/**
    Update the parallel-split-shadow-map shadow buffers for the
    global light source.
*/
void
SM30ShadowServer::UpdatePSSMShadowBuffers()
{
    n_assert(this->globalLightEntity.isvalid());
    VisResolver* visResolver = VisResolver::Instance();
    TransformDevice* transDev = TransformDevice::Instance();

    // store original view and projection transform
    matrix44 origView = transDev->GetViewTransform();
    matrix44 origProj = transDev->GetProjTransform();

    // compute split view volumes
    this->pssmUtil.SetCameraEntity(this->cameraEntity);
    this->pssmUtil.SetLightDir(this->globalLightEntity->GetLightDirection());
    this->pssmUtil.Compute();

    // render shadow casters for each view volume split
    IndexT splitIndex;
    for (splitIndex = 0; splitIndex < PSSMUtil::NumSplits; splitIndex++)
    {
        // get the view and projection matrices of the current view volume split
        const matrix44& splitLightProjMatrix = this->pssmUtil.GetSplitLightProjTransform(splitIndex);
        const matrix44& splitLightMatrix = this->pssmUtil.GetSplitLightTransform(splitIndex);
        const matrix44& splitProjMatrix = this->pssmUtil.GetSplitProjTransform(splitIndex);
        
        // update transform device with view and projection matrix of current split volume
        transDev->SetViewTransform(splitLightMatrix);
        transDev->SetProjTransform(splitProjMatrix);

        // perform visibility resolve
        visResolver->BeginResolve();
        const Array<Ptr<InternalGraphicsEntity> >& visLinks = this->globalLightEntity->GetLinks(InternalGraphicsEntity::LightLink);
        IndexT linkIndex;
        for (linkIndex = 0; linkIndex < visLinks.Size(); linkIndex++)
        {
            const Ptr<InternalGraphicsEntity>& curEntity = visLinks[linkIndex];
            n_assert(InternalGraphicsEntityType::Model == curEntity->GetType());

            // test if the current graphics entity falls into the current split volume
            // (FIXME: need to check against extruded shadow bounding box!)
            if (curEntity->GetGlobalBoundingBox().clipstatus(splitLightProjMatrix) != ClipStatus::Outside)
            {
                const Ptr<InternalModelEntity>& modelEntity = curEntity.downcast<InternalModelEntity>();
                visResolver->AttachVisibleModelInstance(modelEntity->GetModelInstance());
            }
        }
        visResolver->EndResolve();

        // prepare shadow buffer render target for rendering and 
        // patch shadow buffer render target into the frame shader
        Math::rectangle<int> resolveRect;
        resolveRect.left   = splitIndex * this->pssmShadowBuffer->GetWidth();
        resolveRect.right  = resolveRect.left + this->pssmShadowBuffer->GetWidth();
        resolveRect.top    = 0;
        resolveRect.bottom = this->pssmShadowBuffer->GetHeight();
        this->pssmShadowBuffer->SetResolveRect(resolveRect);
        this->finalPostEffect->SetRenderTarget(this->pssmShadowBuffer);

        // render the resolved model node instances into the main shadow buffer
        this->frameShader->Render();
    }

    // generate mip levels for PSSM shadow buffer
    this->pssmShadowBuffer->GenerateMipLevels();

    // restore original view and projection transforms
    transDev->SetViewTransform(origView);
    transDev->SetProjTransform(origProj);
    this->pssmUtil.SetCameraEntity(0);
}

//------------------------------------------------------------------------------
/**
    Get raw pointer to array of PSSM split distances.
*/
const float*
SM30ShadowServer::GetPSSMSplitDistances() const
{
    return this->pssmUtil.GetSplitDistances();
}

//------------------------------------------------------------------------------
/**
    Get raw pointer to array of PSSM split LightProjTransform matrices.
*/
const Math::matrix44*
SM30ShadowServer::GetPSSMSplitLightProjTransforms() const
{
    return this->pssmUtil.GetSplitLightProjTransforms();
}

} // namespace Lighting
