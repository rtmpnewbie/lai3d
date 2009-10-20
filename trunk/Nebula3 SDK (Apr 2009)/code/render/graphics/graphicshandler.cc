//------------------------------------------------------------------------------
//  graphicshandler.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/graphicshandler.h"
#include "graphics/graphicsinterface.h"
#include "io/filestream.h"
#include "resources/simpleresourcemapper.h"
#include "coregraphics/streamtextureloader.h"
#include "coregraphics/streammeshloader.h"
#include "coregraphics/shaperenderer.h"
#include "coregraphics/base/shaperendererbase.h"
#include "resources/managedtexture.h"
#include "resources/managedmesh.h"
#include "coreanimation/streamanimationloader.h"
#include "internalgraphics/stagebuilder.h"
#include "internalgraphics/internalstage.h"
#include "internalgraphics/internalview.h"
#include "lighting/internalspotlightentity.h"
#include "lighting/internalpointlightentity.h"
#include "apprender/platformconfig.h"
#include "debugrender/debugshaperenderer.h"
#include "models/nodes/statenodeinstance.h"
#include "coregraphics/debug/displaypagehandler.h"
#include "coregraphics/debug/texturepagehandler.h"
#include "coregraphics/debug/meshpagehandler.h"
#include "coregraphics/debug/shaderpagehandler.h"
#include "internalgraphics/debug/graphicspagehandler.h"
#include "coreanimation/animresource.h"
#include "coreanimation/streamanimationloader.h"
#include "coreanimation/managedanimresource.h"
#include "characters/characterinstance.h"
#include "characters/character.h"
#include "threading/thread.h"

namespace Graphics
{
__ImplementClass(Graphics::GraphicsHandler, 'GHDL', Messaging::Handler);

#define __GRASSRENDERING_ENABLED__ (0)

using namespace IO;
using namespace CoreGraphics;
using namespace CoreAnimation;
using namespace Resources;
using namespace Math;
using namespace Models;
using namespace InternalGraphics;
using namespace Lighting;
using namespace Frame;
using namespace Interface;
using namespace Util;
using namespace Debug;
using namespace Messaging;
using namespace Animation;
using namespace Timing;
using namespace Characters;

#if !__WII__
using namespace Vegetation;
using namespace Particles;
#endif

//------------------------------------------------------------------------------
/**
*/
GraphicsHandler::GraphicsHandler() :
    isGraphicsRuntimeValid(false),
    masterTime(0.0),
    scaledTime(0.0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
GraphicsHandler::~GraphicsHandler()
{
}

//------------------------------------------------------------------------------
/**
    Waits for all resources to be loaded.
    THIS METHOD IS CALLED IN THE MAIN THREAD CONTEXT!
*/
void
GraphicsHandler::WaitForPendingResources()
{
    // setting this flag causes the DoWork-loop to check for pending
    // resources and to set the pending resource event when all
    // pending resources have been loaded
    this->checkPendingResourcesFlag.Set();

    // now wait for the DoWork-loop to signal the event
    this->pendingResourcesEvent.Wait();
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::Open()
{
    n_assert(!this->IsOpen());
    n_assert(!this->isGraphicsRuntimeValid);
    InterfaceHandlerBase::Open();

    // crate slave timer
    this->slaveTimer = Timing::SlaveTime::Create();

    // at first create slave 
    // setup core runtime
    this->ioServer = IO::IoServer::Create();

#if __NEBULA3_HTTP__
    // setup http page handlers
    this->httpServerProxy = Http::HttpServerProxy::Create();
    this->httpServerProxy->Open();
    this->httpServerProxy->AttachRequestHandler(Debug::DisplayPageHandler::Create());
    this->httpServerProxy->AttachRequestHandler(Debug::MeshPageHandler::Create());
    this->httpServerProxy->AttachRequestHandler(Debug::ShaderPageHandler::Create());
    this->httpServerProxy->AttachRequestHandler(Debug::TexturePageHandler::Create());
    this->httpServerProxy->AttachRequestHandler(Debug::GraphicsPageHandler::Create());
#endif

    // setup the required objects, but do not open them, this will
    // happen at a later time when the SetupGrapics message is received
    this->renderDevice = RenderDevice::Create();
    this->displayDevice = DisplayDevice::Create();
    this->transformDevice = TransformDevice::Create();
    this->shaderServer = ShaderServer::Create();
    this->shapeRenderer = ShapeRenderer::Create();
    this->textRenderer = TextRenderer::Create();
    this->vertexLayoutServer = VertexLayoutServer::Create();
    this->sharedResourceServer = SharedResourceServer::Create();
    this->resourceManager = ResourceManager::Create();
    this->modelServer = ModelServer::Create();
    this->graphicsServer = InternalGraphicsServer::Create();
    this->lightServer = LightServer::Create();
    this->shadowServer = ShadowServer::Create();
    this->frameServer = FrameServer::Create();
    this->attachmentServer = AttachmentServer::Create();
    this->animEventServer = Animation::AnimEventServer::Create();
    this->skinnedMeshRenderer = Characters::SkinnedMeshRenderer::Create();
    this->mouseRenderDevice = MouseRenderDevice::Create();

    #if !__WII__
	this->uiRenderer = CoreUI::CoreUIRenderer::Create();
    this->fxServer = CoreFX::FxServer::Create();
    this->videoServer = Video::VideoServer::Create();

    #if __GRASSRENDERING_ENABLED__
    this->grassRenderer = GrassRenderer::Create();
    this->grassEntityHandler = GrassEntityEventHandler::Create();
    this->graphicsServer->AttachEntityEventHandler(InternalGraphicsEntityType::Model, this->grassEntityHandler.cast<InternalGraphicsEntityHandler>());
    #endif
    this->particleServer = ParticleServer::Create();
    #endif // __WII__

    // setup debug timers and counters
    _setup_timer(GraphicsFrameTime);
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::Close()
{
    n_assert(this->IsOpen());

    _discard_timer(GraphicsFrameTime);
    
    if (this->isGraphicsRuntimeValid)
    {
        this->ShutdownGraphicsRuntime();
    }

    #if !__WII__    
    #if __GRASSRENDERING_ENABLED__
    this->graphicsServer->RemoveEntityEventHandler(this->grassEntityHandler.cast<InternalGraphicsEntityHandler>());
    this->grassEntityHandler = 0;
    if (this->grassRenderer->IsOpen())
    {
        this->grassRenderer->Close();
    }
    #endif
    this->particleServer = 0;
    this->grassRenderer = 0;
	this->uiRenderer = 0;
    this->fxServer = 0;
    this->videoServer = 0;
    #endif // __WII__
    
    this->mouseRenderDevice = 0;
    this->skinnedMeshRenderer = 0;
    this->frameServer = 0;
    this->shadowServer = 0;
    this->lightServer = 0;
    this->graphicsServer = 0;
    this->modelServer = 0;
    this->resourceManager = 0;
    this->sharedResourceServer = 0;
    this->vertexLayoutServer = 0;
    this->shapeRenderer = 0;
    this->shaderServer = 0;
    this->transformDevice = 0;
    this->displayDevice = 0;
    this->renderDevice = 0;
    this->textRenderer = 0;
    this->attachmentServer = 0;
    this->animEventServer = 0;

#if __NEBULA3_HTTP__
    this->httpServerProxy->Close();
    this->httpServerProxy = 0;
#endif 

    this->ioServer = 0;

    this->slaveTimer = 0;

    InterfaceHandlerBase::Close();
}

//------------------------------------------------------------------------------
/**
    Setup the graphics runtime, this method is called when the
    SetupGraphics message is received from the main thread. The method
    expects that the DisplayDevice has been configured with the 
    desired display settings.
*/
void
GraphicsHandler::SetupGraphicsRuntime()
{
    n_assert(!this->isGraphicsRuntimeValid);
    n_assert(!this->displayDevice->IsOpen());
    n_assert(!this->renderDevice->IsOpen());
    n_assert(!this->transformDevice->IsOpen());
    n_assert(!this->shaderServer->IsOpen());
    n_assert(!this->shapeRenderer->IsOpen());
    n_assert(!this->vertexLayoutServer->IsOpen());
    n_assert(!this->sharedResourceServer->IsOpen());
    n_assert(!this->resourceManager->IsOpen());
    n_assert(!this->modelServer->IsOpen());
    n_assert(!this->graphicsServer->IsOpen());
    n_assert(!this->lightServer->IsOpen());
    n_assert(!this->shadowServer->IsOpen());
    n_assert(!this->frameServer->IsOpen());
    n_assert(!this->attachmentServer->IsOpen());
    n_assert(!this->skinnedMeshRenderer->IsValid());
    
    if (!this->displayDevice->Open())
    {
        n_error("GraphicsHandler: failed to open display device!");
    }
    #if __WII__
    this->displayDevice->GetScreenShotHelper().EnableScreenShotMode();
    #endif
    
    if (!this->renderDevice->Open())
    {
        n_error("GraphicsHandler: failed to open render device!");
    }
    this->vertexLayoutServer->Open();
    this->shaderServer->Open();
    this->transformDevice->Open();
    this->shapeRenderer->Open();
    this->sharedResourceServer->Open();
    this->resourceManager->Open();
    this->frameServer->Open();
    this->textRenderer->Open();
    this->skinnedMeshRenderer->Setup();
    this->mouseRenderDevice->Setup();

    // setup resource mapper for textures
    // FIXME: should be configurable!
    Ptr<SimpleResourceMapper> texMapper = SimpleResourceMapper::Create();
    texMapper->SetPlaceholderResourceId(ResourceId(String(PLACEHOLDER_TEXTURENAME) + String(TEXTURE_EXTENSION)));
    texMapper->SetResourceClass(Texture::RTTI);
    texMapper->SetResourceLoaderClass(StreamTextureLoader::RTTI);
    texMapper->SetManagedResourceClass(ManagedTexture::RTTI);
    this->resourceManager->AttachMapper(texMapper.upcast<ResourceMapper>());

    // setup resource mapper for meshes
    // FIXME: should be configurable!
    Ptr<SimpleResourceMapper> meshMapper = SimpleResourceMapper::Create();
    meshMapper->SetPlaceholderResourceId(ResourceId(PLACEHOLDER_MESHNAME));
    meshMapper->SetResourceClass(Mesh::RTTI);
    meshMapper->SetResourceLoaderClass(StreamMeshLoader::RTTI);
    meshMapper->SetManagedResourceClass(ManagedMesh::RTTI);
    this->resourceManager->AttachMapper(meshMapper.upcast<ResourceMapper>());

    // setup resource mapper for animations
    // FIXME: should be configurable!
    Ptr<SimpleResourceMapper> animMapper = SimpleResourceMapper::Create();
    animMapper->SetResourceClass(AnimResource::RTTI);
    animMapper->SetResourceLoaderClass(StreamAnimationLoader::RTTI);
    animMapper->SetManagedResourceClass(ManagedAnimResource::RTTI);
    this->resourceManager->AttachMapper(animMapper.upcast<ResourceMapper>());

    this->modelServer->Open();
    this->graphicsServer->Open();
    this->lightServer->Open();
    this->shadowServer->Open();    
    this->attachmentServer->Open();

    #if !__WII__
    this->videoServer->Open();
    this->particleServer->Open();    
    this->uiRenderer->Open();
    this->fxServer->Open();
    #endif

    this->isGraphicsRuntimeValid = true;
}

//------------------------------------------------------------------------------
/**
    Shutdown the graphics runtime, this is called from the Close() method.
*/
void
GraphicsHandler::ShutdownGraphicsRuntime()
{
    n_assert(this->isGraphicsRuntimeValid);
    this->isGraphicsRuntimeValid = false;

    #if !__WII__
    #if __GRASSRENDERING_ENABLED__    
    if (this->grassRenderer->IsOpen())
    {
        this->grassRenderer->Close();
    }
    #endif
    this->particleServer->Close();
	this->uiRenderer->Close();
    this->videoServer->Close();
    #endif // __WII__
    
    this->mouseRenderDevice->Discard();
    this->skinnedMeshRenderer->Discard();
    this->textRenderer->Close();
    this->shadowServer->Close();
    this->lightServer->Close();
    this->graphicsServer->Close();
    this->modelServer->Close();
    this->resourceManager->Close();
    this->frameServer->Close();
    this->sharedResourceServer->Close();
    this->shapeRenderer->Close();
    this->shaderServer->Close();
    this->vertexLayoutServer->Close();
    this->renderDevice->Close();
    this->displayDevice->Close();
    this->attachmentServer->Close();
}

//------------------------------------------------------------------------------
/**
*/
bool
GraphicsHandler::HandleMessage(const Ptr<Message>& msg)
{
    n_assert(msg.isvalid());
    if (msg->CheckId(SetupGraphics::Id))
    {
        this->OnSetupGraphics(msg.cast<SetupGraphics>());
    }
    else if (msg->CheckId(AdapterExists::Id))
    {
        this->OnAdapterExists(msg.cast<AdapterExists>());
    }
    else if (msg->CheckId(GetAvailableDisplayModes::Id))
    {
        this->OnGetAvailableDisplayModes(msg.cast<GetAvailableDisplayModes>());
    }
    else if (msg->CheckId(GetCurrentAdapterDisplayMode::Id))
    {
        this->OnGetCurrentAdapterDisplayMode(msg.cast<GetCurrentAdapterDisplayMode>());
    }
    else if (msg->CheckId(GetAdapterInfo::Id))
    {
        this->OnGetAdapterInfo(msg.cast<GetAdapterInfo>());
    }
    else if (msg->CheckId(AttachDisplayEventHandler::Id))
    {
        this->OnAttachDisplayEventHandler(msg.cast<AttachDisplayEventHandler>());
    }
    else if (msg->CheckId(RemoveDisplayEventHandler::Id))
    {
        this->OnRemoveDisplayEventHandler(msg.cast<RemoveDisplayEventHandler>());
    }
    else if (msg->CheckId(AttachRenderEventHandler::Id))
    {
        this->OnAttachRenderEventHandler(msg.cast<AttachRenderEventHandler>());
    }
    else if (msg->CheckId(RemoveRenderEventHandler::Id))
    {
        this->OnRemoveRenderEventHandler(msg.cast<RemoveRenderEventHandler>());
    }
    else if (msg->CheckId(CreateGraphicsStage::Id))
    {
        this->OnCreateGraphicsStage(msg.cast<CreateGraphicsStage>());
    }
    else if (msg->CheckId(DiscardGraphicsStage::Id))
    {
        this->OnDiscardGraphicsStage(msg.cast<DiscardGraphicsStage>());
    }
    else if (msg->CheckId(CreateGraphicsView::Id))
    {
        this->OnCreateGraphicsView(msg.cast<CreateGraphicsView>());
    }
    else if (msg->CheckId(DiscardGraphicsView::Id))
    {
        this->OnDiscardGraphicsView(msg.cast<DiscardGraphicsView>());
    }
    else if (msg->CheckId(CreateCameraEntity::Id))
    {
        this->OnCreateCameraEntity(msg.cast<CreateCameraEntity>());
    }
    else if (msg->CheckId(DiscardGraphicsEntity::Id))
    {
        this->OnDiscardGraphicsEntity(msg.cast<DiscardGraphicsEntity>());
    }
    else if (msg->CheckId(CreateModelEntity::Id))
    {
        this->OnCreateModelEntity(msg.cast<CreateModelEntity>());
    }
    else if (msg->CheckId(CreateGlobalLightEntity::Id))
    {
        this->OnCreateGlobalLightEntity(msg.cast<CreateGlobalLightEntity>());
    }
    else if (msg->CheckId(CreateSpotLightEntity::Id))
    {
        this->OnCreateSpotLightEntity(msg.cast<CreateSpotLightEntity>());
    }
    else if (msg->CheckId(CreatePointLightEntity::Id))
    {
        this->OnCreatePointLightEntity(msg.cast<CreatePointLightEntity>());
    }
	else if (msg->CheckId(UpdateTransform::Id))
    {
        this->OnUpdateTransform(msg.cast<UpdateTransform>());
    }
    else if (msg->CheckId(AttachCameraToView::Id))
    {
        this->OnAttachCameraToView(msg.cast<AttachCameraToView>());
    }
    else if (msg->CheckId(RemoveCameraFromView::Id))
    {
        this->OnRemoveCameraFromView(msg.cast<RemoveCameraFromView>());
    }
    else if (msg->CheckId(SetVisibility::Id))
    {
        this->OnSetVisibility(msg.cast<SetVisibility>());
    }
    else if (msg->CheckId(UpdateInstanceShaderVariable::Id))
    {
        this->OnUpdateInstanceShaderVariable(msg.cast<UpdateInstanceShaderVariable>());
    }
    else if (msg->CheckId(AnimPlayOnce::Id))
    {
        this->OnAnimPlayOnce(msg.cast<AnimPlayOnce>());
    }
    else if (msg->CheckId(AnimPlayLoop::Id))
    {
        this->OnAnimPlayLoop(msg.cast<AnimPlayLoop>());
    }
    else if (msg->CheckId(AnimPlayDuration::Id))
    {
        this->OnAnimPlayDuration(msg.cast<AnimPlayDuration>());
    }
    else if (msg->CheckId(AnimModifyBlendWeight::Id))
    {
        this->OnAnimModifyBlendWeight(msg.cast<AnimModifyBlendWeight>());
    }
    else if (msg->CheckId(AnimModifyTimeFactor::Id))
    {
        this->OnAnimModifyTimeFactor(msg.cast<AnimModifyTimeFactor>());
    }
    else if (msg->CheckId(AnimStopClip::Id))
    {
        this->OnAnimStopClip(msg.cast<AnimStopClip>());
    }
    else if (msg->CheckId(AnimStopCategory::Id))
    {
        this->OnAnimStopCategory(msg.cast<AnimStopCategory>());
    }
    else if (msg->CheckId(AnimStopAll::Id))
    {
        this->OnAnimStopAll(msg.cast<AnimStopAll>());
    }
    else if (msg->CheckId(ApplySkinList::Id))
    {
        this->OnApplySkinList(msg.cast<ApplySkinList>());
    }
    else if (msg->CheckId(ShowSkin::Id))
    {
        this->OnShowSkin(msg.cast<ShowSkin>());
    }
    else if (msg->CheckId(HideSkin::Id))
    {
        this->OnHideSkin(msg.cast<HideSkin>());
    }
    else if (msg->CheckId(SetupGrassFromStage::Id))
    {
        this->OnSetupGrassFromStage(msg.cast<SetupGrassFromStage>());
    }
    else if (msg->CheckId(BindAttachment::Id))
    {
        this->OnAttachEntity(msg.cast<BindAttachment>());
    }
    else if (msg->CheckId(UnbindAttachment::Id))
    {
        this->OnDetachEntity(msg.cast<UnbindAttachment>());
    }   
    else if (msg->CheckId(GetAnimationDrivenVector::Id))
    {
        this->OnGetAnimationDrivenMovement(msg.cast<GetAnimationDrivenVector>());
    }
    else if (msg->CheckId(SetAnimationDrivenFlag::Id))
    {
        this->OnSetAnimationDrivenFlag(msg.cast<SetAnimationDrivenFlag>());
    }    
    else if (msg->CheckId(GetActiveAnimEvents::Id))
    {
        this->OnGetActiveAnimEvents(msg.cast<GetActiveAnimEvents>());
    }
    else if (msg->CheckId(GetMaximumKeyDuration::Id))
    {
        this->OnGetMaximumKeyDuration(msg.cast<GetMaximumKeyDuration>());
    }
    else if (msg->CheckId(GetJointTransform::Id))
    {
        this->OnGetJointTransform(msg.cast<GetJointTransform>());
    }
    else if (msg->CheckId(AnimGetClipDuration::Id))
    {
        this->OnAnimGetClipDuration(msg.cast<AnimGetClipDuration>());
    }
    else if (msg->CheckId(RegisterAnimEventHandler::Id))
    {
        this->OnRegisterAnimEventHandler(msg.cast<RegisterAnimEventHandler>());
    }
    else if (msg->CheckId(UnregisterAnimEventHandler::Id))
    {
        this->OnUnregisterAnimEventHandler(msg.cast<UnregisterAnimEventHandler>()); 
    }
    else if (msg->CheckId(SetLightColor::Id))
    {
        this->OnSetLightColor(msg.cast<SetLightColor>());
    }
    else if (msg->CheckId(SetLightCastShadows::Id))
    {
        this->OnSetLightCastShadows(msg.cast<SetLightCastShadows>());
    }
    else if (msg->CheckId(SetGlobalBackLightColor::Id))
    {
        this->OnSetGlobalBackLightColor(msg.cast<SetGlobalBackLightColor>());
    }
    else if (msg->CheckId(SetGlobalAmbientLightColor::Id))
    {
        this->OnSetGlobalAmbientLightColor(msg.cast<SetGlobalAmbientLightColor>());
    }
    else if (msg->CheckId(SetGlobalBackLightOffset::Id))
    {
        this->OnSetGlobalBackLightOffset(msg.cast<SetGlobalBackLightOffset>());
    }
    else if (msg->CheckId(SetGlobalLightParams::Id))
    {
        this->OnSetGlobalLightParams(msg.cast<SetGlobalLightParams>());
    }
    else if (msg->CheckId(PreloadMousePointerTextures::Id))
    {
        this->OnPreloadMousePointerTextures(msg.cast<PreloadMousePointerTextures>());
    }
    else if (msg->CheckId(UpdateMousePointers::Id))
    {
        this->OnUpdateMousePointers(msg.cast<UpdateMousePointers>());
    }
	else if (msg->CheckId(Timing::SyncTime::Id))
    {
        this->OnSyncTime(msg.cast<Timing::SyncTime>());
    }
    else if (msg->CheckId(RegisterRTPlugin::Id))
    {        
        this->OnRegisterRTPlugin(msg.cast<RegisterRTPlugin>());
    }
    else if (msg->CheckId(UnregisterRTPlugin::Id))
    {
        this->OnUnregisterRTPlugin(msg.cast<UnregisterRTPlugin>());
    }
    else if (msg->CheckId(SaveScreenshot::Id))
    {
        this->OnSaveScreenshot(msg.cast<SaveScreenshot>());
    }
    else
    {
        // unknown message
        return false;
    }
    // fallthrough: message was handled
    return true;
}

//------------------------------------------------------------------------------
/**
    This is the per-frame method which implements the asynchronous render-loop.
*/
void
GraphicsHandler::DoWork()
{
    if (this->isGraphicsRuntimeValid)
    {
        _start_timer(GraphicsFrameTime);
        n_assert(this->IsOpen());

        // at first check our slave timer
        this->slaveTimer->UpdateTimes();
        this->masterTime = this->slaveTimer->GetTime(Timing::TimingType::MasterTime);
        this->scaledTime = this->slaveTimer->GetTime(Timing::TimingType::ScaledTime);

        // debug render
        //n_printf("----> master: %f, scale: %f\n", (float)this->masterTime, (float)this->scaledTime);

        // first signal the global frameEvent, to notify other threads
        // that we are finished with message processing
        GraphicsInterface::Instance()->SignalFrameEvent();

        // need to check for asynchronous pending resources?
        if (this->checkPendingResourcesFlag.Test())
        {            
            if (ResourceManager::Instance()->CheckPendingResources())
            {
                this->checkPendingResourcesFlag.Clear();
                this->pendingResourcesEvent.Signal();
            }
        }

        // handle CreateModelEntities which have been queued until
        // the ModelEntities are ready for use (resources have been loaded)
        this->HandlePendingModelEntities();

        // now apply attachment positions
        this->attachmentServer->OnFrame(this->scaledTime);
        
        // and update anim event handler
        this->animEventServer->OnFrame(this->scaledTime);

        // update fx stuff
        #if !__WII__
        this->fxServer->OnFrame(this->scaledTime);
        #endif

        #if __GRASSRENDERING_ENABLED__
        // update grass textures and meshes
        if (this->grassRenderer->IsOpen())
        {
            this->grassRenderer->Update();
        }
        #endif
        
        // finally, render the frame
        this->graphicsServer->OnFrame(this->scaledTime);
       
        _stop_timer(GraphicsFrameTime);
    }

#if __NEBULA3_HTTP__
    // allow render-thread HttpRequests to be processed
    this->httpServerProxy->HandlePendingRequests();
#endif
}

//------------------------------------------------------------------------------
/**
    This method goes through the pending ModelEntityCreate messages which
    are waiting for their resources to have loaded, and sets those messages
    to "handled" whose entities have finished loading.
*/
void
GraphicsHandler::HandlePendingModelEntities()
{
    // start at the end since we will remove entities from the array
    IndexT i;
    for (i = this->pendingModelEntities.Size() - 1; i != InvalidIndex; --i)
    {
        const KeyValuePair<Ptr<InternalModelEntity>,Ptr<CreateModelEntity> >& kvp = this->pendingModelEntities[i];
        const Ptr<InternalModelEntity>& modelEntity = kvp.Key();
        const Ptr<CreateModelEntity>& msg = kvp.Value();
        if (modelEntity->IsValid())
        {
            msg->SetDeferredHandled(true);
            this->pendingModelEntities.EraseIndex(i);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnSetupGraphics(const Ptr<SetupGraphics>& msg)
{
    n_assert(!this->isGraphicsRuntimeValid);

    // configure the display device and setup the graphics runtime
    DisplayDevice* disp = this->displayDevice;
    disp->SetAdapter(msg->GetAdapter());
    disp->SetDisplayMode(msg->GetDisplayMode());
    disp->SetAntiAliasQuality(msg->GetAntiAliasQuality());
    disp->SetFullscreen(msg->GetFullscreen());
    disp->SetDisplayModeSwitchEnabled(msg->GetDisplayModeSwitchEnabled());
    disp->SetTripleBufferingEnabled(msg->GetTripleBufferingEnabled());
    disp->SetAlwaysOnTop(msg->GetAlwaysOnTop());
    disp->SetVerticalSyncEnabled(msg->GetVerticalSyncEnabled());
    disp->SetIconName(msg->GetIconName());
    this->SetupGraphicsRuntime();
    msg->SetActualDisplayMode(disp->GetDisplayMode());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnAdapterExists(const Ptr<AdapterExists>& msg)
{
    msg->SetResult(this->displayDevice->AdapterExists(msg->GetAdapter()));
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnGetAvailableDisplayModes(const Ptr<GetAvailableDisplayModes>& msg)
{
    Adapter::Code adapter = msg->GetAdapter();
    PixelFormat::Code pixelFormat = msg->GetPixelFormat();
    msg->SetResult(this->displayDevice->GetAvailableDisplayModes(adapter, pixelFormat));
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnGetCurrentAdapterDisplayMode(const Ptr<GetCurrentAdapterDisplayMode>& msg)
{
    Adapter::Code adapter = msg->GetAdapter();
    msg->SetResult(this->displayDevice->GetCurrentAdapterDisplayMode(adapter));
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnGetAdapterInfo(const Ptr<GetAdapterInfo>& msg)
{
    Adapter::Code adapter = msg->GetAdapter();
    msg->SetResult(this->displayDevice->GetAdapterInfo(adapter));
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnAttachDisplayEventHandler(const Ptr<AttachDisplayEventHandler>& msg)
{
    this->displayDevice->AttachEventHandler(msg->GetHandler().upcast<DisplayEventHandler>());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnRemoveDisplayEventHandler(const Ptr<RemoveDisplayEventHandler>& msg)
{
    this->displayDevice->RemoveEventHandler(msg->GetHandler().upcast<DisplayEventHandler>());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnAttachRenderEventHandler(const Ptr<AttachRenderEventHandler>& msg)
{
    this->renderDevice->AttachEventHandler(msg->GetHandler().upcast<RenderEventHandler>());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnRemoveRenderEventHandler(const Ptr<RemoveRenderEventHandler>& msg)
{
    this->renderDevice->RemoveEventHandler(msg->GetHandler().upcast<RenderEventHandler>());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnCreateGraphicsStage(const Ptr<CreateGraphicsStage>& msg)
{
    const StringAtom& name = msg->GetName();
    const Core::Rtti* stageBuilderClass = msg->GetStageBuilderClass();
    n_assert(stageBuilderClass && stageBuilderClass->IsDerivedFrom(StageBuilder::RTTI));
    const Attr::AttributeContainer& stageBuilderAttrs = msg->GetStageBuilderAttrs();
    Ptr<StageBuilder> stageBuilder = (StageBuilder*) stageBuilderClass->Create();
    stageBuilder->SetAttributes(stageBuilderAttrs);
    Ptr<InternalStage> stage = InternalGraphicsServer::Instance()->CreateStage(name, stageBuilder);
    msg->SetResult(stage.get());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnDiscardGraphicsStage(const Ptr<DiscardGraphicsStage>& msg)
{
    Ptr<InternalStage> stage = msg->GetStageHandle().cast<InternalStage>();
    n_assert(stage.isvalid() && stage->IsA(InternalStage::RTTI));
    InternalGraphicsServer::Instance()->DiscardStage(stage);

    // clear pending modelentities of this stage
    this->ClearPendingModelEntitiesOfStage(stage->GetName());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnCreateGraphicsView(const Ptr<CreateGraphicsView>& msg)
{
    const Core::Rtti* viewClass = msg->GetViewClass();
    n_assert(viewClass && viewClass->IsDerivedFrom(InternalView::RTTI));
    const StringAtom& viewName = msg->GetName();
    const StringAtom& stageName = msg->GetStageName();
    const ResourceId& frameShaderName = msg->GetFrameShaderName();
    bool isDefaultView = msg->GetDefaultView();

    const Ptr<FrameShader>& frameShader = FrameServer::Instance()->GetFrameShaderByName(frameShaderName);
    const Ptr<InternalStage>& stage = InternalGraphicsServer::Instance()->GetStageByName(stageName);
    Ptr<InternalView> view = InternalGraphicsServer::Instance()->CreateView(*viewClass, viewName, isDefaultView);
    view->SetStage(stage);
    view->SetFrameShader(frameShader);

    msg->SetResult(view.get());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnDiscardGraphicsView(const Ptr<DiscardGraphicsView>& msg)
{
    Ptr<InternalView> view = msg->GetViewHandle().cast<InternalView>();
    n_assert(view.isvalid() && view->IsA(InternalView::RTTI));
    InternalGraphicsServer::Instance()->DiscardView(view);
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnCreateCameraEntity(const Ptr<CreateCameraEntity>& msg)
{
    Ptr<InternalCameraEntity> cameraEntity = InternalCameraEntity::Create();
    cameraEntity->SetTransform(msg->GetTransform());
    cameraEntity->SetVisible(msg->GetVisible());
    if (msg->GetIsPerspective())
    {
        cameraEntity->SetupPerspectiveFov(msg->GetPerspFieldOfView(), 
                                          msg->GetPerspAspectRatio(), 
                                          msg->GetNearPlane(), 
                                          msg->GetFarPlane());
    }
    else
    {
        cameraEntity->SetupOrthogonal(msg->GetOrthoWidth(),
                                      msg->GetOrthoHeight(),
                                      msg->GetNearPlane(),
                                      msg->GetFarPlane());
    }

    // attach camera entity to stage
    const Ptr<InternalStage>& stage = InternalGraphicsServer::Instance()->GetStageByName(msg->GetStageName());
    stage->AttachEntity(cameraEntity.cast<InternalGraphicsEntity>());

    // set return value
    msg->SetEntityHandle(cameraEntity.get());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnDiscardGraphicsEntity(const Ptr<DiscardGraphicsEntity>& msg)
{
    Ptr<InternalGraphicsEntity> entity = msg->GetEntityHandle().cast<InternalGraphicsEntity>();
    n_assert(entity.isvalid() && entity->IsA(InternalGraphicsEntity::RTTI));
    Ptr<InternalStage> stage = entity->GetStage();
    n_assert(stage.isvalid());

    // HOTFIX: if camera entity is attached to view, remove any pending modelentities
    if (entity->IsA(InternalCameraEntity::RTTI))
    {
        if (entity.cast<InternalCameraEntity>()->IsAttachedToView())
        {
            this->ClearPendingModelEntitiesOfStage(stage->GetName());
        }        
    }
    // remove entity from stage
    stage->RemoveEntity(entity);    
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnCreateModelEntity(const Ptr<CreateModelEntity>& msg)
{
    // create a new model entity
    Ptr<InternalModelEntity> modelEntity = InternalModelEntity::Create();
    modelEntity->SetTransform(msg->GetTransform());
    modelEntity->SetVisible(msg->GetVisible());
    modelEntity->SetResourceId(msg->GetResourceId());
    modelEntity->SetAnimationJointName(msg->GetAnimationDrivenJointName());

    // lookup stage and attach entity
    const Ptr<InternalStage>& stage = InternalGraphicsServer::Instance()->GetStageByName(msg->GetStageName());
    stage->AttachEntity(modelEntity.cast<InternalGraphicsEntity>());

    // set return value
    msg->SetEntityHandle(modelEntity.get());

    // we will only set the entity to "handled" when the entity 
    // is actually ready for use (because of asynchronous resource loading
    // this may be at some later time)
    msg->SetDeferred(true);
    this->pendingModelEntities.Append(KeyValuePair<Ptr<InternalModelEntity>,Ptr<CreateModelEntity> >(modelEntity,msg));
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnCreateGlobalLightEntity(const Ptr<CreateGlobalLightEntity>& msg)
{
    // create a new global light entity
    Ptr<Lighting::InternalGlobalLightEntity> lightEntity = Lighting::InternalGlobalLightEntity::Create();
    lightEntity->SetTransform(msg->GetTransform());
    lightEntity->SetVisible(msg->GetVisible());
    lightEntity->SetColor(msg->GetLightColor());
    lightEntity->SetBackLightColor(msg->GetBackLightColor());
    lightEntity->SetCastShadows(msg->GetCastShadows());
    lightEntity->SetProjMapUvOffsetAndScale(msg->GetProjMapUvOffsetAndScale());
    lightEntity->SetAmbientLightColor(msg->GetAmbientLightColor());
    lightEntity->SetBackLightOffset(msg->GetBackLightOffset());
    
    // lookup stage and attach entity
    const Ptr<InternalStage>& stage = InternalGraphicsServer::Instance()->GetStageByName(msg->GetStageName());
    stage->AttachEntity(lightEntity.cast<InternalGraphicsEntity>());

    // set return value
    msg->SetEntityHandle(lightEntity.get());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnCreateSpotLightEntity(const Ptr<CreateSpotLightEntity>& msg)
{
    // create a new spotlight entity
    Ptr<Lighting::InternalSpotLightEntity> lightEntity = Lighting::InternalSpotLightEntity::Create();
    lightEntity->SetTransform(msg->GetTransform());
    lightEntity->SetVisible(msg->GetVisible());
    lightEntity->SetColor(msg->GetLightColor());
    lightEntity->SetCastShadows(msg->GetCastShadows());
    lightEntity->SetProjMapUvOffsetAndScale(msg->GetProjMapUvOffsetAndScale());

    // lookup stage and attach entity
    const Ptr<InternalStage>& stage = InternalGraphicsServer::Instance()->GetStageByName(msg->GetStageName());
    stage->AttachEntity(lightEntity.cast<InternalGraphicsEntity>());

    // set return value
    msg->SetEntityHandle(lightEntity.get());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnCreatePointLightEntity(const Ptr<CreatePointLightEntity>& msg)
{
    // create a new spotlight entity
    Ptr<Lighting::InternalPointLightEntity> lightEntity = Lighting::InternalPointLightEntity::Create();
    lightEntity->SetTransform(msg->GetTransform());
    lightEntity->SetVisible(msg->GetVisible());
    lightEntity->SetColor(msg->GetLightColor());
    lightEntity->SetCastShadows(msg->GetCastShadows());
    lightEntity->SetProjMapUvOffsetAndScale(msg->GetProjMapUvOffsetAndScale());

    // lookup stage and attach entity
    const Ptr<InternalStage>& stage = InternalGraphicsServer::Instance()->GetStageByName(msg->GetStageName());
    stage->AttachEntity(lightEntity.cast<InternalGraphicsEntity>());

    // set return value
    msg->SetEntityHandle(lightEntity.get());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnUpdateTransform(const Ptr<UpdateTransform>& msg)
{
    Ptr<InternalGraphicsEntity> entity = msg->GetEntityHandle().cast<InternalGraphicsEntity>();
    n_assert(entity.isvalid() && entity->IsA(InternalGraphicsEntity::RTTI));
    entity->SetTransform(msg->GetTransform());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnSetVisibility(const Ptr<SetVisibility>& msg)
{
    Ptr<InternalGraphicsEntity> entity = msg->GetEntityHandle().cast<InternalGraphicsEntity>();
    n_assert(entity.isvalid() && entity->IsA(InternalGraphicsEntity::RTTI));
    entity->SetVisible(msg->GetVisible());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnAttachCameraToView(const Ptr<AttachCameraToView>& msg)
{
    Ptr<InternalCameraEntity> entity = msg->GetEntityHandle().cast<InternalCameraEntity>();
    n_assert(entity.isvalid() && entity->IsA(InternalCameraEntity::RTTI));
    const StringAtom& viewName = msg->GetViewName();
    n_assert(InternalGraphicsServer::Instance()->HasView(viewName));
    InternalGraphicsServer::Instance()->GetViewByName(viewName)->SetCameraEntity(entity);
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnRemoveCameraFromView(const Ptr<RemoveCameraFromView>& msg)
{
    Ptr<InternalCameraEntity> entity = msg->GetEntityHandle().cast<InternalCameraEntity>();
    n_assert(entity.isvalid() && entity->IsA(InternalCameraEntity::RTTI));
    const StringAtom& viewName = msg->GetViewName();
    n_assert(InternalGraphicsServer::Instance()->HasView(viewName));
    const Ptr<InternalView>& view = InternalGraphicsServer::Instance()->GetViewByName(viewName);
    if (view->GetCameraEntity().isvalid())
    {
        n_assert(view->GetCameraEntity() == entity);
        view->SetCameraEntity(0);
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
GraphicsHandler::OnUpdateInstanceShaderVariable(const Ptr<Graphics::UpdateInstanceShaderVariable>& msg)
{
    Ptr<InternalGraphicsEntity> entity = msg->GetEntityHandle().cast<InternalGraphicsEntity>();
    n_assert(entity.isvalid() && entity->IsA(InternalGraphicsEntity::RTTI));
    n_assert(entity->GetType() == InternalGraphics::InternalGraphicsEntityType::Model);
    Ptr<InternalModelEntity> modelEntity = entity.cast<InternalModelEntity>();
    // is modelentity deleted, and msg out-of-date, return handled = true to remove msg from list
    if (!modelEntity->IsActive())
    {
        return true;
    }

    if (modelEntity->GetModelResourceState() == Resources::Resource::Loaded)
    {
        const Ptr<ModelInstance>& modelInst =  modelEntity->GetModelInstance();    
        if (modelInst.isvalid())
        {
            const Util::String& name = msg->GetNodeInstanceName();
            const Ptr<ModelNodeInstance>& nodeInst = modelInst->LookupNodeInstance(name);
            n_assert(nodeInst->IsA(StateNodeInstance::RTTI));
            const Ptr<StateNodeInstance>& stateNodeInst = nodeInst.cast<StateNodeInstance>();
            Ptr<CoreGraphics::ShaderVariableInstance> var;
            if (stateNodeInst->HasShaderVariableInstance(ShaderVariable::Semantic(msg->GetSemantic())))
            {
                var = stateNodeInst->GetShaderVariableInstance(ShaderVariable::Semantic(msg->GetSemantic()));        
            }
            else
            {
                var = stateNodeInst->CreateShaderVariableInstance(ShaderVariable::Semantic(msg->GetSemantic()));
            }

            const Util::Variant& value = msg->GetValue();
            var->SetValue(value);   

            return true;
        }
    }

    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnAnimPlayOnce(const Ptr<AnimPlayOnce>& msg)
{
    Ptr<InternalModelEntity> entity = msg->GetEntityHandle().cast<InternalModelEntity>();
    n_assert(entity.isvalid() && entity->IsA(InternalModelEntity::RTTI));
    if (entity->HasCharacter())
    {
        entity->GetCharacterInstance()->AnimController().PlayOnce(msg->GetClipName(),
                                                                  msg->GetCategory(),
                                                                  msg->GetStartTime(),
                                                                  msg->GetFadeInTime(),
                                                                  msg->GetFadeOutTime(),
                                                                  msg->GetTimeOffset(),
                                                                  msg->GetBlendPriority(),
                                                                  msg->GetBlendWeight(),
                                                                  msg->GetTimeFactor());
    }
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnAnimPlayLoop(const Ptr<AnimPlayLoop>& msg)
{
    Ptr<InternalModelEntity> entity = msg->GetEntityHandle().cast<InternalModelEntity>();
    n_assert(entity.isvalid() && entity->IsA(InternalModelEntity::RTTI));
    if (entity->HasCharacter())
    {
        entity->GetCharacterInstance()->AnimController().PlayLoop(msg->GetClipName(),
                                                                  msg->GetCategory(),
                                                                  msg->GetLoopCount(),
                                                                  msg->GetStartTime(),
                                                                  msg->GetFadeInTime(),
                                                                  msg->GetFadeOutTime(),
                                                                  msg->GetTimeOffset(),
                                                                  msg->GetBlendPriority(),
                                                                  msg->GetBlendWeight(),
                                                                  msg->GetTimeFactor());
    }
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnAnimPlayDuration(const Ptr<AnimPlayDuration>& msg)
{
    Ptr<InternalModelEntity> entity = msg->GetEntityHandle().cast<InternalModelEntity>();
    n_assert(entity.isvalid() && entity->IsA(InternalModelEntity::RTTI));
    if (entity->HasCharacter())
    {
        entity->GetCharacterInstance()->AnimController().PlayDuration(msg->GetClipName(),
                                                                      msg->GetCategory(),
                                                                      msg->GetDuration(),
                                                                      msg->GetStartTime(),
                                                                      msg->GetFadeInTime(),
                                                                      msg->GetFadeOutTime(),
                                                                      msg->GetTimeOffset(),
                                                                      msg->GetBlendPriority(),
                                                                      msg->GetBlendWeight(),
                                                                      msg->GetTimeFactor());
    }
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnAnimStopClip(const Ptr<Graphics::AnimStopClip>& msg)
{
    Ptr<InternalModelEntity> entity = msg->GetEntityHandle().cast<InternalModelEntity>();
    n_assert(entity.isvalid() && entity->IsA(InternalModelEntity::RTTI));
    if (entity->HasCharacter())
    {
        entity->GetCharacterInstance()->AnimController().StopClips(msg->GetClipName());
    }
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnAnimStopCategory(const Ptr<Graphics::AnimStopCategory>& msg)
{
    Ptr<InternalModelEntity> entity = msg->GetEntityHandle().cast<InternalModelEntity>();
    n_assert(entity.isvalid() && entity->IsA(InternalModelEntity::RTTI));
    if (entity->HasCharacter())
    {
        entity->GetCharacterInstance()->AnimController().StopCategory(msg->GetCategory());
    }
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnAnimStopAll(const Ptr<Graphics::AnimStopAll>& msg)
{
    Ptr<InternalModelEntity> entity = msg->GetEntityHandle().cast<InternalModelEntity>();
    n_assert(entity.isvalid() && entity->IsA(InternalModelEntity::RTTI));
    if (entity->HasCharacter())
    {
        entity->GetCharacterInstance()->AnimController().StopAll();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnApplySkinList(const Ptr<Graphics::ApplySkinList>& msg)
{
    Ptr<InternalModelEntity> entity = msg->GetEntityHandle().cast<InternalModelEntity>();
    n_assert(entity.isvalid() && entity->IsA(InternalModelEntity::RTTI));
    if (entity->HasCharacter())
    {
        entity->GetCharacterInstance()->SkinSet().ApplySkinList(msg->GetSkinList());
    }
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnShowSkin(const Ptr<Graphics::ShowSkin>& msg)
{
    Ptr<InternalModelEntity> entity = msg->GetEntityHandle().cast<InternalModelEntity>();
    n_assert(entity.isvalid() && entity->IsA(InternalModelEntity::RTTI));
    if (entity->HasCharacter())
    {
        entity->GetCharacterInstance()->SkinSet().AddSkin(msg->GetSkin());
    }
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnHideSkin(const Ptr<Graphics::HideSkin>& msg)
{
    Ptr<InternalModelEntity> entity = msg->GetEntityHandle().cast<InternalModelEntity>();
    n_assert(entity.isvalid() && entity->IsA(InternalModelEntity::RTTI));
    if (entity->HasCharacter())
    {
        entity->GetCharacterInstance()->SkinSet().RemoveSkin(msg->GetSkin());
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
GraphicsHandler::OnSetupGrassFromStage(const Ptr<Graphics::SetupGrassFromStage>& msg)
{
    // after loading grass patches we can start the grass renderer

#if __GRASSRENDERING_ENABLED__
    GrassRenderer::Instance()->Open();
#endif
}

//------------------------------------------------------------------------------
/**
*/
void 
GraphicsHandler::OnAttachEntity(const Ptr<Graphics::BindAttachment>& msg)
{
    // cast handles, attachment server just works with internalgraphicspointer
    const Ptr<InternalGraphicsEntity>& internalAttachedPtr = msg->GetEntityToAttachHandle().cast<InternalGraphicsEntity>();
    const Ptr<InternalModelEntity>& internalBasePtr = msg->GetBaseEntityHandle().cast<InternalModelEntity>();

    AttachmentServer::Instance()->AttachEntity(msg->GetOffset(),
                                               msg->GetClearType(),
                                               msg->GetJoint(),
                                               internalAttachedPtr,
                                               internalBasePtr,
                                               msg->GetKeepLocal(),
                                               msg->GetRotation());
}

//------------------------------------------------------------------------------
/**
*/
void 
GraphicsHandler::OnDetachEntity(const Ptr<Graphics::UnbindAttachment>& msg)
{
    // cast handles, attachment server just works with internalgraphicspointer
    const Ptr<InternalGraphicsEntity>& internalAttachedPtr = msg->GetEntityToAttachHandle().cast<InternalGraphicsEntity>();
    const Ptr<InternalModelEntity>& internalBasePtr = msg->GetBaseEntityHandle().cast<InternalModelEntity>();

    AttachmentServer::Instance()->DetachEntity(msg->GetClearType(),
                                               msg->GetJoint(),
                                               internalAttachedPtr,
                                               internalBasePtr);
}

//------------------------------------------------------------------------------
/**
*/
void 
GraphicsHandler::OnAnimModifyBlendWeight(const Ptr<Graphics::AnimModifyBlendWeight>& msg)
{
    Ptr<InternalModelEntity> entity = msg->GetEntityHandle().cast<InternalModelEntity>();
    n_assert(entity.isvalid() && entity->IsA(InternalModelEntity::RTTI));
    if (entity->HasCharacter())
    {
        const AnimSequencer& sequencer = entity->GetCharacterInstance()->AnimController().AnimSequencer();
        Util::Array<Ptr<AnimJob> > animJobs = sequencer.GetAnimJobsByName(msg->GetClipName());
        IndexT i;
        for (i = 0; i < animJobs.Size(); ++i)
        {
        	animJobs[i]->SetBlendWeight(msg->GetBlendWeight());
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
GraphicsHandler::OnGetAnimationDrivenMovement(const Ptr<Graphics::GetAnimationDrivenVector>& msg)
{
    Ptr<InternalModelEntity> entity = msg->GetEntityHandle().cast<InternalModelEntity>();
    n_assert(entity.isvalid() && entity->IsA(InternalModelEntity::RTTI));
    if (entity->HasCharacter() && !msg->Handled())
    {
        msg->SetMovement(entity->GetAnimationDrivenMovement());
        msg->SetClipStatus(entity->GetClipStatus());
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
GraphicsHandler::OnSetAnimationDrivenFlag(const Ptr<Graphics::SetAnimationDrivenFlag>& msg)
{
    Ptr<InternalModelEntity> entity = msg->GetEntityHandle().cast<InternalModelEntity>();
    n_assert(entity.isvalid() && entity->IsA(InternalModelEntity::RTTI));
    if (entity->HasCharacter())
    {
        entity->SetAnimationJointName(msg->GetJointName());
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
GraphicsHandler::OnAnimModifyTimeFactor(const Ptr<Graphics::AnimModifyTimeFactor>& msg)
{
    Ptr<InternalModelEntity> entity = msg->GetEntityHandle().cast<InternalModelEntity>();
    n_assert(entity.isvalid() && entity->IsA(InternalModelEntity::RTTI));
    if (entity->HasCharacter())
    {
        const AnimSequencer& sequencer = entity->GetCharacterInstance()->AnimController().AnimSequencer();
        Util::Array<Ptr<AnimJob> > animJobs = sequencer.GetAnimJobsByName(msg->GetClipName());
        IndexT i;
        for (i = 0; i < animJobs.Size(); ++i)
        {
            animJobs[i]->SetTimeFactor(msg->GetTimeFactor());
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
GraphicsHandler::OnGetActiveAnimEvents(const Ptr<Graphics::GetActiveAnimEvents>& msg)
{
    // calc time diff between render and app thread
    Timing::Tick diff = Timing::SecondsToTicks(this->scaledTime) - msg->GetApplicationTime();

    Ptr<InternalModelEntity> entity = msg->GetEntityHandle().cast<InternalModelEntity>();
    n_assert(entity.isvalid() && entity->IsA(InternalModelEntity::RTTI));
    if (entity->HasCharacter())
    {
        // collect data
        const AnimSequencer& sequencer = entity->GetCharacterInstance()->AnimController().AnimSequencer();
        Timing::Tick startTimeInTicks = msg->GetStartTime() + diff;
        Timing::Tick endTimeInTicks = msg->GetEndTime() + diff;
        const Util::Array<Util::StringAtom>& cats = msg->GetCategories();    //optional, if empty msg will return all events
        bool justDominatingClip = msg->GetUseJustDominatingClip();

        // is category set?
        Util::Dictionary<Util::StringAtom, Util::Array<AnimEventInfo> >events;
        Util::StringAtom cat("");
        IndexT catIndex = 0;
        if (cats.Size() > 0)
        {
            cat = cats[catIndex].Value();
        }       

        // do for all categories
        do 
        {
            if(msg->GetOneKeyDuration())
            {
                endTimeInTicks = startTimeInTicks + sequencer.GetMaximumKeyDuration();
                const Util::Array<Animation::AnimEventInfo>& newEvents = sequencer.EmitAnimEvents(startTimeInTicks, endTimeInTicks, justDominatingClip, cat.Value());
                events.Add(cat, newEvents);
                msg->SetEndTime(endTimeInTicks);
            }
            else
            {
                const Util::Array<Animation::AnimEventInfo>& newEvents = sequencer.EmitAnimEvents(startTimeInTicks, endTimeInTicks, justDominatingClip, cat.Value());
                events.Add(cat, newEvents);
            }

            // check if there are more categories
            if (cats.Size() > ++catIndex)
            {
                cat = cats[catIndex].Value();
            }
        } while (catIndex < cats.Size());
        msg->SetAnimEvents(events);
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
GraphicsHandler::OnGetMaximumKeyDuration(const Ptr<Graphics::GetMaximumKeyDuration>& msg)
{
    Ptr<InternalModelEntity> entity = msg->GetEntityHandle().cast<InternalModelEntity>();
    n_assert(entity.isvalid() && entity->IsA(InternalModelEntity::RTTI));
    if (entity->HasCharacter())
    {
        const AnimSequencer& sequencer = entity->GetCharacterInstance()->AnimController().AnimSequencer();
        msg->SetDuration(sequencer.GetMaximumKeyDuration());
    } 
}

//------------------------------------------------------------------------------
/**
*/
void 
GraphicsHandler::OnGetJointTransform(const Ptr<Graphics::GetJointTransform>& msg)
{
    Ptr<InternalModelEntity> entity = msg->GetEntityHandle().cast<InternalModelEntity>();
    n_assert(entity.isvalid() && entity->IsA(InternalModelEntity::RTTI));
    if (entity->HasCharacter())
    {
        IndexT jointIndex = entity->GetCharacter()->Skeleton().GetJointIndexByName(msg->GetJointName());
        const Characters::CharacterJointInstance& joint = entity->GetCharacterInstance()->Skeleton().GetJoint(jointIndex);
        msg->SetTransform(joint.GetJointMatrix());
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
GraphicsHandler::OnAnimGetClipDuration(const Ptr<Graphics::AnimGetClipDuration>& msg)
{
    Ptr<InternalModelEntity> entity = msg->GetEntityHandle().cast<InternalModelEntity>();
    n_assert(entity.isvalid() && entity->IsA(InternalModelEntity::RTTI));
    if (entity->HasCharacter())
    {
        const AnimSequencer& sequencer = entity->GetCharacterInstance()->AnimController().AnimSequencer();
        const AnimClip& clip = sequencer.GetAnimResource()->GetClipByName(msg->GetClipName());
        msg->SetDuration(clip.GetClipDuration());
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
GraphicsHandler::OnRegisterAnimEventHandler(const Ptr<Graphics::RegisterAnimEventHandler>& msg)
{
    Animation::AnimEventServer::Instance()->RegisterAnimEventHandler(msg->GetAnimEventHandler());
}

//------------------------------------------------------------------------------
/**
*/
void 
GraphicsHandler::OnUnregisterAnimEventHandler(const Ptr<Graphics::UnregisterAnimEventHandler>& msg)
{
    Animation::AnimEventServer::Instance()->UnregisterAnimEventHandler(msg->GetCategoryName());  
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnSetLightColor(const Ptr<Graphics::SetLightColor>& msg)
{
    Ptr<InternalAbstractLightEntity> entity = msg->GetEntityHandle().downcast<InternalAbstractLightEntity>();
    entity->SetColor(msg->GetColor());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnSetLightCastShadows(const Ptr<Graphics::SetLightCastShadows>& msg)
{
    Ptr<InternalAbstractLightEntity> entity = msg->GetEntityHandle().downcast<InternalAbstractLightEntity>();
    entity->SetCastShadows(msg->GetCastShadows());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnSetGlobalBackLightColor(const Ptr<Graphics::SetGlobalBackLightColor>& msg)
{
    Ptr<InternalGlobalLightEntity> entity = msg->GetEntityHandle().downcast<InternalGlobalLightEntity>();
    entity->SetBackLightColor(msg->GetColor());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnSetGlobalAmbientLightColor(const Ptr<Graphics::SetGlobalAmbientLightColor>& msg)
{
    Ptr<InternalGlobalLightEntity> entity = msg->GetEntityHandle().downcast<InternalGlobalLightEntity>();
    entity->SetAmbientLightColor(msg->GetColor());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnSetGlobalBackLightOffset(const Ptr<Graphics::SetGlobalBackLightOffset>& msg)
{
    Ptr<InternalGlobalLightEntity> entity = msg->GetEntityHandle().downcast<InternalGlobalLightEntity>();
    entity->SetBackLightOffset(msg->GetOffset());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnSetGlobalLightParams(const Ptr<Graphics::SetGlobalLightParams>& msg)
{
    Ptr<InternalGlobalLightEntity> entity = msg->GetEntityHandle().downcast<InternalGlobalLightEntity>();
    entity->SetColor(msg->GetLightColor());
    entity->SetBackLightColor(msg->GetBackLightColor());
    entity->SetAmbientLightColor(msg->GetAmbientLightColor());
    entity->SetBackLightOffset(msg->GetBackLightOffset());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnPreloadMousePointerTextures(const Ptr<Graphics::PreloadMousePointerTextures>& msg)
{
    MouseRenderDevice::Instance()->PreloadTextures(msg->GetResourceIds());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnUpdateMousePointers(const Ptr<Graphics::UpdateMousePointers>& msg)
{
    MouseRenderDevice::Instance()->UpdatePointers(msg->GetPointers());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnRegisterRTPlugin(const Ptr<Graphics::RegisterRTPlugin>& msg)
{
    RenderModules::RTPluginRegistry::Instance()->RegisterRTPlugin(msg->GetType());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnUnregisterRTPlugin(const Ptr<Graphics::UnregisterRTPlugin>& msg)
{
    RenderModules::RTPluginRegistry::Instance()->UnregisterRTPlugin(msg->GetType());
}

//------------------------------------------------------------------------------
/**
    On discard stage, remove all pending modelentities for this stage
*/
void 
GraphicsHandler::ClearPendingModelEntitiesOfStage(const Util::StringAtom& stagename)
{
    // start at the end since we will remove entities from the array
    IndexT i;
    for (i = this->pendingModelEntities.Size() - 1; i != InvalidIndex; --i)
    {
        const KeyValuePair<Ptr<InternalModelEntity>,Ptr<CreateModelEntity> >& kvp = this->pendingModelEntities[i];        
        const Ptr<CreateModelEntity>& msg = kvp.Value();        
        if (msg->GetStageName() == stagename)
        {   
            msg->SetDeferredHandled(true);
            this->pendingModelEntities.EraseIndex(i);        
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnSyncTime(const Ptr<Timing::SyncTime>& msg)
{
    // update the slave time
    const Util::Dictionary<Timing::TimingType::Code, Timing::Time>& times = msg->GetTimes();
    IndexT index;
    for (index = 0; index < Timing::TimingType::NumTimingTypes; index++)
    {
        IndexT dictIndex = times.FindIndex((Timing::TimingType::Code)index);
        n_assert(InvalidIndex != dictIndex);
        this->slaveTimer->SyncTime(times.ValueAtIndex(dictIndex), (Timing::TimingType::Code)index);
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
GraphicsHandler::OnSaveScreenshot(const Ptr<Graphics::SaveScreenshot>& msg)
{
    // create stream to save screenshot to
    Ptr<IO::Stream> outStream = IO::IoServer::Instance()->CreateStream(msg->GetUri());
    n_assert(outStream.isvalid());
    ImageFileFormat::Code fmt = ImageFileFormat::FromString(msg->GetFileformat());
    // for wrong fileformat use png as default
    if (ImageFileFormat::InvalidImageFileFormat == fmt)
    {
        fmt = ImageFileFormat::PNG;
    }
    // do screenshot and save to stream
    RenderDevice::Instance()->SaveScreenshot(fmt, outStream);
}

} // namespace Graphics

