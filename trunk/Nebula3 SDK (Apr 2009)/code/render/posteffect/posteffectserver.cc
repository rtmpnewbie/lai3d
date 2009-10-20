//------------------------------------------------------------------------------
//  posteffectserver.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "posteffect/posteffectserver.h"
#include "internalgraphics/internalgraphicsserver.h"
#include "internalgraphics/internalview.h"
#include "internalgraphics/internalmodelentity.h"
#include "coregraphics/shaderserver.h"
#include "apprender/platformconfig.h"
#include "lighting/internalgloballightentity.h"
#include "models/nodes/statenodeinstance.h"
#include "resources/managedtexture.h"
#include "resources/resourcemanager.h"

namespace PostEffect
{
__ImplementClass(PostEffect::PostEffectServer, 'POSR', Core::RefCounted);
__ImplementSingleton(PostEffectServer);

using namespace Util;
using namespace CoreGraphics;
using namespace InternalGraphics;
using namespace Resources;
using namespace Math;
using namespace Models;

//------------------------------------------------------------------------------
/**
*/
PostEffectServer::PostEffectServer():
    currentFadeValue(1.0f),
    fadeTime(0.0),
    curFadeMode(NoFade),
    lastTime(0.0),
    postEffects(NumPostEffectTypes)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
PostEffectServer::~PostEffectServer()
{
    this->Close();

    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void 
PostEffectServer::Open()
{
    const Ptr<ShaderServer> shdServer = ShaderServer::Instance();

    // lookup the shared post effect fade variable
    this->fadeShaderVariable = shdServer->GetSharedVariableBySemantic(ShaderVariable::Semantic("FadeValue"));
    
    // color stuff
    this->saturationShaderVariable = shdServer->GetSharedVariableBySemantic(ShaderVariable::Semantic("Saturation"));
    this->balanceShaderVariable = shdServer->GetSharedVariableBySemantic(ShaderVariable::Semantic("Balance"));

    // fog stuff
    this->fogColorVariable = shdServer->GetSharedVariableBySemantic(ShaderVariable::Semantic("FogColor"));
    this->fogDistancesVariable = shdServer->GetSharedVariableBySemantic(ShaderVariable::Semantic("FogDistances"));

    // hdr stuff
    this->hdrColorVariable = shdServer->GetSharedVariableBySemantic(ShaderVariable::Semantic("HdrBloomColor"));
    this->hdrScaleVariable = shdServer->GetSharedVariableBySemantic(ShaderVariable::Semantic("HdrBloomScale"));
    this->hdrThresholdVariable = shdServer->GetSharedVariableBySemantic(ShaderVariable::Semantic("HdrBrightPassThreshold"));

    // dof stuff
    this->dofVariable = shdServer->GetSharedVariableBySemantic(ShaderVariable::Semantic("DoFDistances"));
}

//------------------------------------------------------------------------------
/**
*/
bool
PostEffectServer::FindCurrentSkyEntities()
{
    // find the right shader variable instance
    const Ptr<InternalGraphicsServer>& gfxServer = InternalGraphicsServer::Instance();
    const Ptr<InternalView>& view = gfxServer->GetDefaultView();
    if(view.isvalid())
    {
        const Ptr<InternalStage>& stage = view->GetStage();
        if(stage.isvalid())
        {
            const Util::Array<Ptr<InternalGraphicsEntity>>& entities = stage->GetEntitiesByType(InternalGraphicsEntityType::Model);

            IndexT index;
            for(index = 0; index < entities.Size(); index++)
            {
                const Ptr<InternalModelEntity> internalModel = entities[index].downcast<InternalModelEntity>();

                // check if this is our sky
                if("mdl:skies/sky.n3" == internalModel->GetResourceId().Value())
                {
                    const Ptr<ModelInstance>& modelInstance = internalModel->GetModelInstance();
                    if (modelInstance.isvalid())
                    {
                        const Array<Ptr<ModelNodeInstance>>& modelNodeInstances = modelInstance->GetNodeInstances();
                        IndexT modelNodeInstIndex;
                        for (modelNodeInstIndex = 0; modelNodeInstIndex < modelNodeInstances.Size(); modelNodeInstIndex++)
                        {
                            const Ptr<ModelNodeInstance>& nodeInst = modelNodeInstances[modelNodeInstIndex];
                            if (nodeInst->IsA(StateNodeInstance::RTTI))
                            {
                                const Ptr<StateNodeInstance>& stateNodeInst = nodeInst.downcast<StateNodeInstance>();
                                this->skyBaseTexture = stateNodeInst->CreateShaderVariableInstance(ShaderVariable::Semantic("DiffMap0"));
                                this->skyBlendTexture = stateNodeInst->CreateShaderVariableInstance(ShaderVariable::Semantic("DiffMap1"));
                                this->skyBlendFactor = stateNodeInst->CreateShaderVariableInstance(ShaderVariable::Semantic("Intensity2"));
                                this->skyBrightness = stateNodeInst->CreateShaderVariableInstance(ShaderVariable::Semantic("Intensity1"));
                                this->skyContrast = stateNodeInst->CreateShaderVariableInstance(ShaderVariable::Semantic("Intensity0"));

                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void 
PostEffectServer::Close()
{
    // delete all posteffect structs
    IndexT index;
    for (index = 0; index < this->postEffects.Size(); index++)
    {
        ParameterSet set;
        this->postEffects[index] = set;
    }

    this->curFadeMode = NoFade;
    this->currentFadeValue = 1.0f;
    this->fadeShaderVariable = 0;
    this->saturationShaderVariable = 0;
    this->balanceShaderVariable = 0;
    this->fogDistancesVariable = 0;
    this->fogColorVariable = 0;
    this->hdrScaleVariable = 0;
    this->hdrColorVariable = 0;
    this->hdrThresholdVariable = 0;
    this->dofVariable = 0;
    this->skyContrast = 0;
    this->skyBrightness = 0;
    this->skyBlendFactor = 0;
    this->skyBlendTexture = 0;
    this->skyBaseTexture = 0;
    this->globalLight = 0;

    this->UnloadTextures();
}

//------------------------------------------------------------------------------
/**
*/
void 
PostEffectServer::OnFrame(Timing::Time time)
{
    // if the shader variables are not yet connected to the sky shader, try
    if(!this->skyBlendTexture.isvalid() || !this->skyBaseTexture.isvalid())
    {
        this->FindCurrentSkyEntities();
        // mabye the sky is still invalid
        return;
    }

    // if no global light entity set, find the new one
    //   (can be used for setting a new, in case of changing the view
    //    the old one should be reseted and here we will find the new one)
    if(!this->globalLight.isvalid())
    {
        this->globalLight = InternalGraphics::InternalGraphicsServer::Instance()->GetCurrentGlobalLightEntity();
        // return, the light still can be invalid
        return;
    }

    Timing::Time frameTime = time - this->lastTime;
    this->lastTime = time;

    // compute simple fading
    if (this->curFadeMode != NoFade)
    {
        this->currentFadeValue = this->ComputeFadeValue(frameTime);
        // apply fadeValue on shader variable
        this->fadeShaderVariable->SetFloat(this->currentFadeValue);
    }

    // now compute further post effect blendings
    IndexT index = 0;
    for(index; index < NumPostEffectTypes; index++)
    {
        if(this->ComputeBlending((PostEffectType)index))
        {
            this->ApplyParameters((PostEffectType)index);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
PostEffectServer::ComputeBlending(PostEffectType type)
{    
    const Ptr<ParamBase>& target = this->postEffects[(int)type].target;
    const Ptr<ParamBase>& current = this->postEffects[(int)type].current;
    
    //n_assert(current.isvalid());
    if(target.isvalid())
    {
        // if this is the first blend request
        if(!current.isvalid())
        {
            // set current, apply instantly and stop blending
            this->postEffects[type].current = target;
            this->ApplyParameters(type);
            this->StopBlending(type);
            return false;
        }
        else if(!current->Equals(target, TINY))
        {
            // how much have we progressed since last time?
            // and how much time left to the end?
            Timing::Time timeDiff = this->lastTime - this->postEffects[(int)type].lastTime;
            Timing::Time timeToEnd = max(this->postEffects[(int)type].blendEndTime - this->lastTime, 0.0001);

            // get normalized lerp value and perform blending
            float lerpVal = n_saturate(float(timeDiff / timeToEnd));
            current->BlendTo( target , lerpVal);

            // store current time for parameterblend
            this->postEffects[(int)type].lastTime = this->lastTime;

            return true;
        }
        else
        {
            this->StopBlending(type);
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
PostEffectServer::ApplyParameters(PostEffectType type)
{   
    switch(type)
    {
        case Color:
            this->ApplyColorParameters();
            break;
        case DoF:
            this->ApplyDepthOfFieldParameters();
            break;
        case Fog:
            this->ApplyFogParameters();
            break;
        case Hdr:
            this->ApplyHdrParameters();
            break;
        case Sky:
            this->ApplySkyParameters();
            break;
        case Light:
            this->ApplyLightParameters();
            break;
        default:
            n_error("Wrong PostEffectParameter Type set!");
            break;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
PostEffectServer::ApplyColorParameters()
{   
    this->balanceShaderVariable->SetFloat4(this->postEffects[Color].current.cast<ColorParams>()->GetColorBalance());
    this->saturationShaderVariable->SetFloat(this->postEffects[Color].current.cast<ColorParams>()->GetColorSaturation());
}

//------------------------------------------------------------------------------
/**
*/
void
PostEffectServer::ApplyDepthOfFieldParameters()
{
    // TODO
    Math::vector dofDistances(  this->postEffects[DoF].current.cast<DepthOfFieldParams>()->GetFocusDistance(),
                                this->postEffects[DoF].current.cast<DepthOfFieldParams>()->GetFocusLength(),
                                this->postEffects[DoF].current.cast<DepthOfFieldParams>()->GetFilterSize());
    this->dofVariable->SetFloat4(dofDistances);
}

//------------------------------------------------------------------------------
/**
*/
void
PostEffectServer::ApplyFogParameters()
{
    this->fogColorVariable->SetFloat4(this->postEffects[Fog].current.cast<FogParams>()->GetFogColorAndIntensity());
    // build distances stuff
    Math::float4 fogDistances(  this->postEffects[Fog].current.cast<FogParams>()->GetFogNearDistance(),
                                this->postEffects[Fog].current.cast<FogParams>()->GetFogFarDistance(),
                                this->postEffects[Fog].current.cast<FogParams>()->GetFogHeight(),
                                this->postEffects[Fog].current.cast<FogParams>()->GetFogFarDistance() - this->postEffects[Fog].current.cast<FogParams>()->GetFogNearDistance());
    this->fogDistancesVariable->SetFloat4(fogDistances);
}

//------------------------------------------------------------------------------
/**
*/
void
PostEffectServer::ApplyHdrParameters()
{
    this->hdrColorVariable->SetFloat4(this->postEffects[Hdr].current.cast<HdrParams>()->GetHdrBloomColor());
    this->hdrScaleVariable->SetFloat(this->postEffects[Hdr].current.cast<HdrParams>()->GetHdrBloomIntensity());
    this->hdrThresholdVariable->SetFloat(this->postEffects[Hdr].current.cast<HdrParams>()->GetHdrBloomThreshold());
}

//------------------------------------------------------------------------------
/**
*/
void
PostEffectServer::ApplyLightParameters()
{
    // do not apply any shader variables,
    // modify globallight instead
    const Ptr<LightParams>& paras = this->postEffects[Light].current.cast<LightParams>();
    this->globalLight->SetColor(paras->GetLightColor());
    this->globalLight->SetTransform(paras->GetLightTransform());
    this->globalLight->SetBackLightColor(paras->GetLightOppositeColor());
    // TODO: rename backlightfactor in application layer and corresponding xml tables into backlightoffset
    this->globalLight->SetBackLightOffset(paras->GetBackLightFactor());
    this->globalLight->SetAmbientLightColor(paras->GetLightAmbientColor());
}

//------------------------------------------------------------------------------
/**
*/
void
PostEffectServer::ApplySkyParameters()
{
    // get textures
    const Ptr<SkyParams>& targetPara = this->postEffects[Sky].target.cast<SkyParams>();
    const Ptr<SkyParams>& currentPara = this->postEffects[Sky].current.cast<SkyParams>();

    this->skyBrightness->SetFloat(currentPara->GetSkyBrightness());
    this->skyContrast->SetFloat(currentPara->GetSkyContrast());
    
    // if blending finished, apply blend texture as current
    if(1.0f - currentPara->GetTextureBlendFactor() <= N_TINY || targetPara == currentPara)
    {        
        // set current as target, reset blend factor and delete target
        currentPara->ResetTextureBlendFactor();
        currentPara->SetSkyTexturePath(targetPara->GetSkyTexturePath());
        this->StopBlending(Sky);

        // set base texture, other one is not needed
        this->skyBaseTexture->SetTexture(this->texturePool[currentPara->GetSkyTexturePath()]->GetTexture());
        this->skyBlendTexture->SetTexture(this->texturePool[currentPara->GetSkyTexturePath()]->GetTexture());
        this->skyBlendFactor->SetFloat(currentPara->GetTextureBlendFactor());
    }
    else
    {
        // set base and blend texture
        this->skyBlendTexture->SetTexture(this->texturePool[targetPara->GetSkyTexturePath()]->GetTexture());
        this->skyBaseTexture->SetTexture(this->texturePool[currentPara->GetSkyTexturePath()]->GetTexture());
        this->skyBlendFactor->SetFloat(currentPara->GetTextureBlendFactor());
    }
}

//------------------------------------------------------------------------------
/**
*/
void
PostEffectServer::StartBlending(const Ptr<ParamBase>& target, Timing::Time fadeTime, PostEffectType postEffectType)
{
    //n_printf("PostEffectServer: Starting Blending (%i) at '%f' - fadeTime (%f)\n", (int)postEffectType, this->lastTime, fadeTime);

    // update times
    this->postEffects[postEffectType].blendTime = fadeTime;
    this->postEffects[postEffectType].blendEndTime = this->lastTime + fadeTime;
    this->postEffects[postEffectType].lastTime = this->lastTime;

    // set new target
    this->postEffects[postEffectType].target = target;
}

//------------------------------------------------------------------------------
/**
*/
void
PostEffectServer::StopBlending(PostEffectType postEffectType)
{
    //n_printf("PostEffectServer: Stopping Blending (%i) at '%f'\n", (int)postEffectType, this->lastTime);

    this->postEffects[(int)postEffectType].target = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
PostEffectServer::PreloadTexture(const Util::String &resource)
{
    // soft check
    if(!this->texturePool.Contains(resource))
    {
        // append textureextension dependend on platform
        ResourceId fullTexResId = String(resource + TEXTURE_EXTENSION);
        Ptr<ManagedTexture> managedTexture = ResourceManager::Instance()->CreateManagedResource(Texture::RTTI, fullTexResId).downcast<ManagedTexture>();
        this->texturePool.Add(resource, managedTexture);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
PostEffectServer::UnloadTexture(const Util::String &resource)
{
    // append textureextension dependend on platform
    this->texturePool.Erase(resource);
}

//------------------------------------------------------------------------------
/**
*/
void
PostEffectServer::UnloadTextures()
{
    this->texturePool.Clear();
}

//------------------------------------------------------------------------------
/**
*/
float 
PostEffectServer::ComputeFadeValue(Timing::Time frameTime)
{
    float result = this->currentFadeValue;
    if (this->curFadeMode == FadeOut)
    {
        const float targetValue = 0.0f;
        if (this->fadeTime == 0.0f)
        {
            result = targetValue;
        }
        if (result <= targetValue)
        {
            this->curFadeMode = NoFade;
            return targetValue;
        }
        else
        {
            // always compute time in ratio of full black and full transparent
            float stepSize = (float)frameTime / (float)this->fadeTime; 
            result -= stepSize;
        }
    }
    else
    {
        const float targetValue = 1.0f;
        if (this->fadeTime == 0.0f)
        {
            result = targetValue;
        }
        if (result >= targetValue)
        {
            this->curFadeMode = NoFade;
            return targetValue;
        }
        else
        {
            // always compute time in ratio of full black and full transparent
            float stepSize = (float)frameTime / (float)this->fadeTime; 
            result += stepSize;
        }
    }

    return result;
}
} // namespace PostEffect
