//------------------------------------------------------------------------------
//  frameshaderloader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "frame/frameshaderloader.h"
#include "io/ioserver.h"
#include "coregraphics/rendertarget.h"
#include "coregraphics/displaydevice.h"
#include "coregraphics/shaderserver.h"
#include "coregraphics/renderdevice.h"
#include "models/modelnodetype.h"
#include "resources/sharedresourceserver.h"

namespace Frame
{
using namespace Resources;
using namespace CoreGraphics;
using namespace Models;
using namespace Util;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
Array<Ptr<FrameShader> >
FrameShaderLoader::LoadFrameShaders(const URI& uri)
{
    Array<Ptr<FrameShader> > result;
    Ptr<Stream> stream = IoServer::Instance()->CreateStream(uri);
    Ptr<XmlReader> xmlReader = XmlReader::Create();
    xmlReader->SetStream(stream);
    if (xmlReader->Open())
    {
        // make sure it's a valid frame shader file
        if (!xmlReader->HasNode("/Nebula3/FrameShader"))
        {
            n_error("FrameShaderLoader: '%s' is not a valid frame shader!", uri.AsString().AsCharPtr());
            return result;
        }
        xmlReader->SetToNode("/Nebula3");

        // for each frame shader definition in the file...
        if (xmlReader->SetToFirstChild("FrameShader")) do
        {
            Ptr<FrameShader> frameShader = FrameShader::Create();
            ParseFrameShader(xmlReader, frameShader);
            result.Append(frameShader);
        }
        while (xmlReader->SetToNextChild("FrameShader"));
        xmlReader->Close();
    }
    else
    {
        n_error("FrameShaderLoader: failed to load frame shader '%s'!", uri.AsString().AsCharPtr());
    }
    return result;
}

//------------------------------------------------------------------------------
/**
*/
void
FrameShaderLoader::ParseFrameShader(const Ptr<XmlReader>& xmlReader, const Ptr<FrameShader>& frameShader)
{
    frameShader->SetName(xmlReader->GetString("name"));
    
    // parse render target declarations
    if (xmlReader->SetToFirstChild("DeclareRenderTarget")) do
    {
        ParseRenderTarget(xmlReader, frameShader);
    }
    while (xmlReader->SetToNextChild("DeclareRenderTarget"));

    // parse multiple render target declarations
    if (xmlReader->SetToFirstChild("DeclareMRT")) do
    {
        ParseMultipleRenderTarget(xmlReader, frameShader);
    }
    while (xmlReader->SetToNextChild("DeclareMRT"));

    // parse frame passes
    if (xmlReader->SetToFirstChild("Pass")) do
    {
        ParseFramePass(xmlReader, frameShader);
    }
    while (xmlReader->SetToNextChild("Pass"));

    // parse posteffects
    if (xmlReader->SetToFirstChild("PostEffect")) do
    {
        ParsePostEffect(xmlReader, frameShader);
    }
    while (xmlReader->SetToNextChild("PostEffect"));
}

//------------------------------------------------------------------------------
/**
*/
void
FrameShaderLoader::ParseRenderTarget(const Ptr<XmlReader>& xmlReader, const Ptr<FrameShader>& frameShader)
{
    n_assert(DisplayDevice::Instance()->IsOpen());

    // create and configure a new render target
    Ptr<RenderTarget> renderTarget = RenderTarget::Create();
    const DisplayMode& displayMode = DisplayDevice::Instance()->GetDisplayMode();
    String name = xmlReader->GetString("name");
    renderTarget->SetResolveTextureResourceId(name);
    renderTarget->AddColorBuffer(PixelFormat::FromString(xmlReader->GetString("format")));
    if (xmlReader->HasAttr("format2"))
    {
        renderTarget->AddColorBuffer(PixelFormat::FromString(xmlReader->GetString("format2")));
    }
    if (xmlReader->HasAttr("format3"))
    {
        renderTarget->AddColorBuffer(PixelFormat::FromString(xmlReader->GetString("format3")));
    }
    if (xmlReader->HasAttr("format4"))
    {
        renderTarget->AddColorBuffer(PixelFormat::FromString(xmlReader->GetString("format4")));
    }
    if (xmlReader->HasAttr("depth"))
    {
        if (xmlReader->GetBool("depth"))
        {
            renderTarget->AddDepthStencilBuffer();
        }
    }
    if (xmlReader->HasAttr("depthTexture"))
    {
        n_assert2(xmlReader->HasAttr("depth") && xmlReader->GetBool("depth"), "Depthbuffer must be declared for a resolve!");
        renderTarget->SetResolveDepthTextureResourceId(xmlReader->GetString("depthTexture"));
    }
    if (xmlReader->HasAttr("width"))
    {
        renderTarget->SetWidth(xmlReader->GetInt("width"));
    }
    if (xmlReader->HasAttr("height"))
    {
        renderTarget->SetHeight(xmlReader->GetInt("height"));
    }
    if (xmlReader->HasAttr("relWidth"))
    {
        renderTarget->SetWidth(uint(float(displayMode.GetWidth()) * xmlReader->GetFloat("relWidth")));
    }
    if (xmlReader->HasAttr("relHeight"))
    {
        renderTarget->SetHeight(uint(float(displayMode.GetHeight()) * xmlReader->GetFloat("relHeight")));
    }
    if (xmlReader->HasAttr("msaa"))
    {
        if (xmlReader->GetBool("msaa"))
        {
            renderTarget->SetAntiAliasQuality(DisplayDevice::Instance()->GetAntiAliasQuality());
        }
    }
    if (xmlReader->HasAttr("cpuAccess"))
    {
        renderTarget->SetResolveTargetCpuAccess(xmlReader->GetBool("cpuAccess"));
    }
    renderTarget->Setup();
    frameShader->AddRenderTarget(name, renderTarget);
}

//------------------------------------------------------------------------------
/**
*/
Ptr<ShaderVariableInstance>
FrameShaderLoader::ParseShaderVariableInstance(const Ptr<XmlReader>& xmlReader, const Ptr<ShaderInstance>& shd)
{
    /// create a shader variable instance by semantic
    String semantic = xmlReader->GetString("sem");
    if (!shd->HasVariableBySemantic(semantic))
    {
        n_error("FrameShaderLoader: shader '%s' has no variable '%s' (%s, line %d)",
            shd->GetOriginalShader()->GetResourceId().Value().AsCharPtr(),
            semantic.AsCharPtr(),
            xmlReader->GetStream()->GetURI().AsString().AsCharPtr(),
            xmlReader->GetCurrentNodeLineNumber());
    }
    const Ptr<ShaderVariable>& shdVar = shd->GetVariableBySemantic(semantic);
    Ptr<ShaderVariableInstance> shdVarInst = shdVar->CreateInstance();

    /// get the default value of the shader variable
    String valueStr = xmlReader->GetString("value");
    switch (shdVarInst->GetShaderVariable()->GetType())
    {
        case ShaderVariable::IntType:
            shdVarInst->SetInt(valueStr.AsInt());
            break;

        case ShaderVariable::FloatType:
            shdVarInst->SetFloat(valueStr.AsFloat());
            break;

        case ShaderVariable::VectorType:
            shdVarInst->SetFloat4(valueStr.AsFloat4());
            break;

        case ShaderVariable::MatrixType:
            shdVarInst->SetMatrix(valueStr.AsMatrix44());
            break;

        case ShaderVariable::BoolType:
            shdVarInst->SetBool(valueStr.AsBool());
            break;

        case ShaderVariable::TextureType:
            shdVarInst->SetTexture(SharedResourceServer::Instance()->LookupSharedResource(valueStr).downcast<Texture>());            
            break;
    }
    return shdVarInst;
}

//------------------------------------------------------------------------------
/**
*/
void
FrameShaderLoader::ParseFramePass(const Ptr<XmlReader>& xmlReader, const Ptr<FrameShader>& frameShader)
{
    // create frame pass, optionally of requested class (default is FramePass)
    String framePassClass = xmlReader->GetOptString("class", "Frame::FramePass");
    Ptr<FramePass> framePass = (FramePass*) Core::Factory::Instance()->Create(framePassClass);
    n_assert(framePass->IsA(Frame::FramePass::RTTI));
    
    framePass->SetName(xmlReader->GetString("name"));

    // setup the pass shader
    ResourceId shdResId = ResourceId("shd:" + xmlReader->GetString("shader"));
    Ptr<ShaderInstance> shader = ShaderServer::Instance()->CreateShaderInstance(shdResId);
    framePass->SetShader(shader);
    
    // setup the render target (if not render to default render target)
    bool useDefaultRendertarget = true;
    if (xmlReader->HasAttr("renderTarget"))
    {
        n_assert2(!xmlReader->HasAttr("multipleRenderTarget"), "Rendertarget and multiple rendertarget are exclusivly!");
        useDefaultRendertarget = false;
        String rtName = xmlReader->GetString("renderTarget");
        if (frameShader->HasRenderTarget(rtName))
        {
            framePass->SetRenderTarget(frameShader->GetRenderTargetByName(rtName));
        }
        else
        {
            n_error("FrameShaderLoader: render target '%s' not declared (%s, line %d)",
                xmlReader->GetStream()->GetURI().AsString().AsCharPtr(), 
                xmlReader->GetCurrentNodeLineNumber());
        }    
    }
    if (xmlReader->HasAttr("multipleRenderTarget"))
    {
        useDefaultRendertarget = false;
        String rtName = xmlReader->GetString("multipleRenderTarget");
        if (frameShader->HasMultipleRenderTarget(rtName))
        {
            framePass->SetMultipleRenderTarget(frameShader->GetMultipleRenderTargetByName(rtName));
        }
        else
        {
            n_error("FrameShaderLoader: mutliple render target '%s' not declared (%s, line %d)",
                xmlReader->GetStream()->GetURI().AsString().AsCharPtr(), 
                xmlReader->GetCurrentNodeLineNumber());
        } 
    }
    
    if (useDefaultRendertarget)
    {
        framePass->SetRenderTarget(RenderDevice::Instance()->GetDefaultRenderTarget());
    }

    // setup the clear color, depth and stencil (if defined)
    if (xmlReader->HasAttr("clearColor"))
    {
        // clear color for all rendertargets
        framePass->SetClearColor(xmlReader->GetFloat4("clearColor"));
    }
    if (xmlReader->HasAttr("clearDepth"))
    {
        framePass->SetClearDepth(xmlReader->GetFloat("clearDepth"));
    }
    if (xmlReader->HasAttr("clearStencil"))
    {
        framePass->SetClearStencil(uchar(xmlReader->GetInt("clearStencil")));
    }

    // add shader variable instances
    if (xmlReader->SetToFirstChild("ApplyShaderVariable")) do
    {
        Ptr<ShaderVariableInstance> var = ParseShaderVariableInstance(xmlReader, shader);
        framePass->AddVariable(var);
    }
    while (xmlReader->SetToNextChild("ApplyShaderVariable"));

    // add render batches
    if (xmlReader->SetToFirstChild("Batch")) do
    {
        Ptr<FrameBatch> frameBatch = ParseFrameBatch(xmlReader);
        framePass->AddBatch(frameBatch);
    }
    while (xmlReader->SetToNextChild("Batch"));

    // add frame pass to frame shader
    frameShader->AddFramePass(framePass);

#if NEBULA3_ENABLE_PROFILING
    framePass->SetFramePassDebugTimer(Util::String("FramePass_") + framePass->GetName().Value());
#endif
}

//------------------------------------------------------------------------------
/**
*/
Ptr<FrameBatch>
FrameShaderLoader::ParseFrameBatch(const Ptr<XmlReader>& xmlReader)
{
    Ptr<FrameBatch> frameBatch = FrameBatch::Create();
    
    // setup the batch shader
    ResourceId shdResId = ResourceId("shd:" + xmlReader->GetString("shader"));
    Ptr<ShaderInstance> shader = ShaderServer::Instance()->CreateShaderInstance(shdResId);
    frameBatch->SetShader(shader);

    // setup batch type, model node filter, lighting and sorting mode
    frameBatch->SetType(BatchType::FromString(xmlReader->GetString("type")));
    if (xmlReader->HasAttr("nodeFilter"))
    {
        frameBatch->SetNodeFilter(ModelNodeType::FromName(xmlReader->GetString("nodeFilter")));
    }
    if (xmlReader->HasAttr("lighting"))
    {
        frameBatch->SetLightingMode(LightingMode::FromString(xmlReader->GetString("lighting")));
    }
    if (xmlReader->HasAttr("sorting"))
    {
        frameBatch->SetSortingMode(SortingMode::FromString(xmlReader->GetString("sorting")));
    }
    if (xmlReader->HasAttr("shdFeatures"))
    {
        frameBatch->SetShaderFeatures(ShaderServer::Instance()->FeatureStringToMask(xmlReader->GetString("shdFeatures")));
    }

    // add shader variable instances
    if (xmlReader->SetToFirstChild("ApplyShaderVariable")) do
    {
        Ptr<ShaderVariableInstance> var = ParseShaderVariableInstance(xmlReader, shader);
        frameBatch->AddVariable(var);
    }
    while (xmlReader->SetToNextChild("ApplyShaderVariable"));

#if NEBULA3_ENABLE_PROFILING
    // add debug timer
    Util::String name;
    name.Format("FrameBatch:%s_%s_%s_%s_%s_%s", 
        xmlReader->GetString("shader").AsCharPtr(),
        xmlReader->GetString("type").AsCharPtr(),
        xmlReader->HasAttr("nodeFilter") ? xmlReader->GetString("nodeFilter").AsCharPtr() : "",
        xmlReader->HasAttr("lighting") ? xmlReader->GetString("lighting").AsCharPtr() : "",
        xmlReader->HasAttr("sorting") ? xmlReader->GetString("sorting").AsCharPtr() : "",
        xmlReader->HasAttr("shdFeatures") ? xmlReader->GetString("shdFeatures").AsCharPtr() : "");
    frameBatch->SetBatchDebugTimer(name);
#endif

    return frameBatch;
}

//------------------------------------------------------------------------------
/**
*/
void
FrameShaderLoader::ParsePostEffect(const Ptr<XmlReader>& xmlReader, const Ptr<FrameShader>& frameShader)
{
    // create and configure a new post effect
    Ptr<FramePostEffect> framePostEffect = FramePostEffect::Create();
    framePostEffect->SetName(xmlReader->GetString("name"));

    // setup the pass shader
    ResourceId shdResId = ResourceId("shd:" + xmlReader->GetString("shader"));
    Ptr<ShaderInstance> shader = ShaderServer::Instance()->CreateShaderInstance(shdResId);
    framePostEffect->SetShader(shader);

    // setup the render target (if not render to default render target)
    bool useDefaultRendertarget = true;
    if (xmlReader->HasAttr("renderTarget"))
    {
        n_assert2(!xmlReader->HasAttr("multipleRenderTarget"), "Rendertarget and multiple rendertarget are exclusivly!");
        useDefaultRendertarget = false;
        String rtName = xmlReader->GetString("renderTarget");
        if (frameShader->HasRenderTarget(rtName))
        {
            framePostEffect->SetRenderTarget(frameShader->GetRenderTargetByName(rtName));
        }
        else
        {
            n_error("FrameShaderLoader: render target '%s' not declared (%s, line %d)",
                xmlReader->GetStream()->GetURI().AsString().AsCharPtr(), 
                xmlReader->GetCurrentNodeLineNumber());
        }    
    }
    if (xmlReader->HasAttr("multipleRenderTarget"))
    {
        useDefaultRendertarget = false;
        String rtName = xmlReader->GetString("multipleRenderTarget");
        if (frameShader->HasMultipleRenderTarget(rtName))
        {
            framePostEffect->SetMultipleRenderTarget(frameShader->GetMultipleRenderTargetByName(rtName));
        }
        else
        {
            n_error("FrameShaderLoader: mutliple render target '%s' not declared (%s, line %d)",
                xmlReader->GetStream()->GetURI().AsString().AsCharPtr(), 
                xmlReader->GetCurrentNodeLineNumber());
        } 
    }

    if (useDefaultRendertarget)
    {
        framePostEffect->SetRenderTarget(RenderDevice::Instance()->GetDefaultRenderTarget());
    }

    // add shader variable instances
    if (xmlReader->SetToFirstChild("ApplyShaderVariable")) do
    {
        Ptr<ShaderVariableInstance> var = ParseShaderVariableInstance(xmlReader, shader);
        framePostEffect->AddVariable(var);
    }
    while (xmlReader->SetToNextChild("ApplyShaderVariable"));

    // add render batches
    if (xmlReader->SetToFirstChild("Batch")) do
    {
        Ptr<FrameBatch> frameBatch = ParseFrameBatch(xmlReader);
        framePostEffect->AddBatch(frameBatch);
    }
    while (xmlReader->SetToNextChild("Batch"));

    // add post effect to frame shader
    frameShader->AddPostEffect(framePostEffect);
}

//------------------------------------------------------------------------------
/**
*/
void 
FrameShaderLoader::ParseMultipleRenderTarget(const Ptr<IO::XmlReader>& xmlReader, const Ptr<FrameShader>& frameShader)
{
    n_assert(DisplayDevice::Instance()->IsOpen());

    // create and configure a new render target
    Ptr<MultipleRenderTarget> multipleRenderTarget = MultipleRenderTarget::Create();
    String name = xmlReader->GetString("name");

    // collect all rendertargets 
    if (xmlReader->SetToFirstChild("RenderTarget")) do
    {
        String rtName = xmlReader->GetString("name");
        if (frameShader->HasRenderTarget(rtName))
        {
            multipleRenderTarget->AddRenderTarget(frameShader->GetRenderTargetByName(rtName));
            Math::float4 color = Math::point::origin();
            if (xmlReader->HasAttr("clearColor"))
            {
                color = xmlReader->GetFloat4("clearColor");
            }
            multipleRenderTarget->SetClearColor(multipleRenderTarget->GetNumRendertargets() - 1, color);
            if (xmlReader->HasAttr("clearDepth"))
            {
                multipleRenderTarget->SetClearDepth(xmlReader->GetFloat("clearDepth"));
            }
            if (xmlReader->HasAttr("clearStencil"))
            {
                multipleRenderTarget->SetClearStencil((uchar)xmlReader->GetInt("clearStencil"));
            }
        }
        else
        {
            n_error("FrameShaderLoader: render target '%s' not declared (%s, line %d)",
                xmlReader->GetStream()->GetURI().AsString().AsCharPtr(), 
                xmlReader->GetCurrentNodeLineNumber());
        }   
    }
    while (xmlReader->SetToNextChild("RenderTarget"));

    frameShader->AddMultipleRenderTarget(name, multipleRenderTarget);
}

} // namespace Frame