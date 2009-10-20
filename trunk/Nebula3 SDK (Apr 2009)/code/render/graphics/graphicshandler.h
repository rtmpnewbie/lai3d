#pragma once
//------------------------------------------------------------------------------
/**
    @class Graphics::GraphicsHandler
    
    Runs in the graphics thread context, setup the graphics runtime environment
    and processes messages to the graphics thread.

    (C) 2008 Radon Labs GmbH
*/
#include "interface/interfacehandlerbase.h"
#include "messaging/message.h"
#include "graphics/graphicsprotocol.h"
#include "debugrender/debugrenderprotocol.h"
#include "io/console.h"
#include "io/ioserver.h"
#include "coregraphics/renderdevice.h"
#include "coregraphics/displaydevice.h"
#include "coregraphics/transformdevice.h"
#include "coregraphics/shaderserver.h"
#include "coregraphics/shaperenderer.h"
#include "coregraphics/vertexlayoutserver.h"
#include "resources/sharedresourceserver.h"
#include "resources/resourcemanager.h"
#include "models/modelserver.h"
#include "internalgraphics/internalgraphicsserver.h"
#include "lighting/lightserver.h"
#include "lighting/shadowserver.h"
#include "frame/frameserver.h"
#include "timing/slavetime.h"
#include "timing/timeprotocol.h"
#include "threading/safeflag.h"
#include "animation/animeventserver.h"
#include "debug/debugtimer.h"
#include "coregraphics/textrenderer.h"
#include "http/httpserverproxy.h"
#include "characters/skinnedmeshrenderer.h"
#include "coregraphics/mouserenderdevice.h"

#if !__WII__
#include "coreui/coreuirenderer.h"
#include "video/videoserver.h"
#include "corefx/fxserver.h"
#include "vegetation/grassrenderer.h"
#include "vegetation/grassentityeventhandler.h"
#include "particles/particleserver.h"
#endif

//------------------------------------------------------------------------------
namespace Graphics
{
class GraphicsHandler : public Interface::InterfaceHandlerBase
{
    __DeclareClass(GraphicsHandler);
public:
    /// constructor
    GraphicsHandler();
    /// destructor
    virtual ~GraphicsHandler();

    /// open the handler
    virtual void Open();
    /// close the handler
    virtual void Close();
    /// handle a message, return true if handled
    virtual bool HandleMessage(const Ptr<Messaging::Message>& msg);
    /// do per-frame work
    virtual void DoWork();

    /// wait for pending resources, this method is called in the main thread context!
    void WaitForPendingResources();

private:
    /// handle pending model entity create messages
    void HandlePendingModelEntities();
    /// setup the required graphics runtime
    void SetupGraphicsRuntime();
    /// shutdown the graphics runtime
    void ShutdownGraphicsRuntime();
    /// handle SetupGraphics message
    void OnSetupGraphics(const Ptr<Graphics::SetupGraphics>& msg);
    /// handle AdapterExists message
    void OnAdapterExists(const Ptr<Graphics::AdapterExists>& msg);
    /// handle GetAvailableDisplayModes message
    void OnGetAvailableDisplayModes(const Ptr<Graphics::GetAvailableDisplayModes>& msg);
    /// handle GetCurrentAdapterDisplayMode message
    void OnGetCurrentAdapterDisplayMode(const Ptr<Graphics::GetCurrentAdapterDisplayMode>& msg);
    /// handle GetAdapterInfo message
    void OnGetAdapterInfo(const Ptr<Graphics::GetAdapterInfo>& msg);
    /// handle AttachDisplayEventHandler message
    void OnAttachDisplayEventHandler(const Ptr<Graphics::AttachDisplayEventHandler>& msg);
    /// handle RemoveDisplayEventHandler message
    void OnRemoveDisplayEventHandler(const Ptr<Graphics::RemoveDisplayEventHandler>& msg);
    /// handle AttachRenderEventHandler message
    void OnAttachRenderEventHandler(const Ptr<Graphics::AttachRenderEventHandler>& msg);
    /// handle RemoveRenderEventHandler message
    void OnRemoveRenderEventHandler(const Ptr<Graphics::RemoveRenderEventHandler>& msg);
    /// handle CreateGraphicsStage message
    void OnCreateGraphicsStage(const Ptr<Graphics::CreateGraphicsStage>& msg);
    /// handle a DiscardGraphicsStage message
    void OnDiscardGraphicsStage(const Ptr<Graphics::DiscardGraphicsStage>& msg);
    /// handle CreateGraphicsView message
    void OnCreateGraphicsView(const Ptr<Graphics::CreateGraphicsView>& msg);
    /// handle DiscardGraphicsView message
    void OnDiscardGraphicsView(const Ptr<Graphics::DiscardGraphicsView>& msg);
    /// handle CreateCameraEntity message
    void OnCreateCameraEntity(const Ptr<Graphics::CreateCameraEntity>& msg);
    /// handle CreateModelEntity message
    void OnCreateModelEntity(const Ptr<Graphics::CreateModelEntity>& msg);
    /// handle CreateGlobalLightEntity message
    void OnCreateGlobalLightEntity(const Ptr<Graphics::CreateGlobalLightEntity>& msg);
    /// handle CreateSpotLightEntity message
    void OnCreateSpotLightEntity(const Ptr<Graphics::CreateSpotLightEntity>& msg);
    /// handle CreateSpotLightEntity message
    void OnCreatePointLightEntity(const Ptr<Graphics::CreatePointLightEntity>& msg);
    /// handle DiscardGraphicsEntity message
    void OnDiscardGraphicsEntity(const Ptr<Graphics::DiscardGraphicsEntity>& msg);
    /// handle UpdateTransform message
    void OnUpdateTransform(const Ptr<Graphics::UpdateTransform>& msg);
    /// handle AttachCameraToView message
    void OnAttachCameraToView(const Ptr<Graphics::AttachCameraToView>& msg);
    /// handle RemoveCameraFromView message
    void OnRemoveCameraFromView(const Ptr<Graphics::RemoveCameraFromView>& msg);
    /// handle SetVisibility message
    void OnSetVisibility(const Ptr<Graphics::SetVisibility>& msg);
    /// on update instance variable
    bool OnUpdateInstanceShaderVariable(const Ptr<Graphics::UpdateInstanceShaderVariable>& msg);
    /// handle AnimPlayOnce message
    void OnAnimPlayOnce(const Ptr<Graphics::AnimPlayOnce>& msg);
    /// handle AnimPlayLoop message
    void OnAnimPlayLoop(const Ptr<Graphics::AnimPlayLoop>& msg);
    /// handle AnimPlayDuration message
    void OnAnimPlayDuration(const Ptr<Graphics::AnimPlayDuration>& msg);
    /// handle AnimModifyBlendWeight message
    void OnAnimModifyBlendWeight(const Ptr<Graphics::AnimModifyBlendWeight>& msg);
    /// handle AnimModifyTimeFactor message
    void OnAnimModifyTimeFactor(const Ptr<Graphics::AnimModifyTimeFactor>& msg);
    /// handle AnimStopClip message
    void OnAnimStopClip(const Ptr<Graphics::AnimStopClip>& msg);
    /// handle AnimStopCategory message
    void OnAnimStopCategory(const Ptr<Graphics::AnimStopCategory>& msg);
    /// handle AnimStopAll message
    void OnAnimStopAll(const Ptr<Graphics::AnimStopAll>& msg);
    /// handle ApplySkinList message
    void OnApplySkinList(const Ptr<Graphics::ApplySkinList>& msg);
    /// handle ShowSkin message
    void OnShowSkin(const Ptr<Graphics::ShowSkin>& msg);
    /// handle HideSkin message
    void OnHideSkin(const Ptr<Graphics::HideSkin>& msg);
    /// setup grass from stage
    void OnSetupGrassFromStage(const Ptr<Graphics::SetupGrassFromStage>& msg);
    /// handle attach attachment message
    void OnAttachEntity(const Ptr<Graphics::BindAttachment>& msg);
    /// handle detach attachment message
    void OnDetachEntity(const Ptr<Graphics::UnbindAttachment>& msg);
    /// handle animation driven movement
    void OnGetAnimationDrivenMovement(const Ptr<Graphics::GetAnimationDrivenVector>& msg);
    /// handle set animation driven flag
    void OnSetAnimationDrivenFlag(const Ptr<Graphics::SetAnimationDrivenFlag>& msg);
    /// handle get anim events msg
    void OnGetActiveAnimEvents(const Ptr<Graphics::GetActiveAnimEvents>& msg);
    /// handle get maximum key duration msg
    void OnGetMaximumKeyDuration(const Ptr<Graphics::GetMaximumKeyDuration>& msg);
    /// handle get joint transform msg
    void OnGetJointTransform(const Ptr<Graphics::GetJointTransform>& msg);
    /// handle get clip duration
    void OnAnimGetClipDuration(const Ptr<Graphics::AnimGetClipDuration>& msg);
    /// handle SetLightColor
    void OnSetLightColor(const Ptr<Graphics::SetLightColor>& msg);
    /// handle SetLightCastShadows
    void OnSetLightCastShadows(const Ptr<Graphics::SetLightCastShadows>& msg);
    /// handle SetGlobalBackLightColor
    void OnSetGlobalBackLightColor(const Ptr<Graphics::SetGlobalBackLightColor>& msg);
    /// handle SetGlobalAmbientLightColor
    void OnSetGlobalAmbientLightColor(const Ptr<Graphics::SetGlobalAmbientLightColor>& msg);
    /// handle SetGlobalBackLightOffset
    void OnSetGlobalBackLightOffset(const Ptr<Graphics::SetGlobalBackLightOffset>& msg);
    /// handle SetGlobalLightParams
    void OnSetGlobalLightParams(const Ptr<Graphics::SetGlobalLightParams>& msg);
    /// handle PreloadMousePointerTextures
    void OnPreloadMousePointerTextures(const Ptr<Graphics::PreloadMousePointerTextures>& msg);
    /// handle UpdateMousePointers
    void OnUpdateMousePointers(const Ptr<Graphics::UpdateMousePointers>& msg);
    /// handle SaveScreenshot
    void OnSaveScreenshot(const Ptr<Graphics::SaveScreenshot>& msg);

    /// handle time sync (should be called as FIRST message)
    void OnSyncTime(const Ptr<Timing::SyncTime>& msg);         

    /// handle register new animeventhandler
    void OnRegisterAnimEventHandler(const Ptr<Graphics::RegisterAnimEventHandler>& msg);
    /// handle unregister new animeventhandler
    void OnUnregisterAnimEventHandler(const Ptr<Graphics::UnregisterAnimEventHandler>& msg);

    /// handle RegisterRTPlugin
    void OnRegisterRTPlugin(const Ptr<Graphics::RegisterRTPlugin>& msg);
    /// handle UnregisterRTPlugin
    void OnUnregisterRTPlugin(const Ptr<Graphics::UnregisterRTPlugin>& msg);

    /// clear pending model entities on 
    void ClearPendingModelEntitiesOfStage(const Util::StringAtom& stagename);

    bool isGraphicsRuntimeValid;
    Ptr<Timing::SlaveTime> slaveTimer;
    Timing::Time masterTime;
    Timing::Time scaledTime;

    Ptr<IO::IoServer> ioServer;
    Ptr<CoreGraphics::RenderDevice> renderDevice;
    Ptr<CoreGraphics::DisplayDevice> displayDevice;
    Ptr<CoreGraphics::TransformDevice> transformDevice;
    Ptr<CoreGraphics::ShaderServer> shaderServer;
    Ptr<CoreGraphics::ShapeRenderer> shapeRenderer;
    Ptr<CoreGraphics::VertexLayoutServer> vertexLayoutServer;
    Ptr<CoreGraphics::TextRenderer> textRenderer;
    Ptr<CoreGraphics::MouseRenderDevice> mouseRenderDevice;
    Ptr<Resources::SharedResourceServer> sharedResourceServer;
    Ptr<Resources::ResourceManager> resourceManager;
    Ptr<Models::ModelServer> modelServer;
    Ptr<InternalGraphics::InternalGraphicsServer> graphicsServer;
    Ptr<InternalGraphics::AttachmentServer> attachmentServer;
    Ptr<Lighting::LightServer> lightServer;
    Ptr<Lighting::ShadowServer> shadowServer;
    Ptr<Frame::FrameServer> frameServer;
    Ptr<Animation::AnimEventServer> animEventServer;
    Ptr<Characters::SkinnedMeshRenderer> skinnedMeshRenderer;

    #if !__WII__
    Ptr<Particles::ParticleServer> particleServer;
    Ptr<Vegetation::GrassRenderer> grassRenderer;
    Ptr<Vegetation::GrassEntityEventHandler> grassEntityHandler;
    Ptr<CoreUI::CoreUIRenderer> uiRenderer;
    Ptr<CoreFX::FxServer> fxServer;
    Ptr<Video::VideoServer> videoServer;
    #endif

    Util::Array<Util::KeyValuePair<Ptr<InternalGraphics::InternalModelEntity>,Ptr<CreateModelEntity> > > pendingModelEntities;
    
    _declare_timer(GraphicsFrameTime);

#if __NEBULA3_HTTP__
    Ptr<Http::HttpServerProxy> httpServerProxy;
#endif 

    Threading::SafeFlag checkPendingResourcesFlag;
    Threading::Event pendingResourcesEvent;
};
} // namespace GraphicsHandler
//------------------------------------------------------------------------------

