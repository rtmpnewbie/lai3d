#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "messaging/message.h"
#include "util/stringatom.h"
#include "core/rtti.h"
#include "coregraphics/displaymode.h"
#include "coregraphics/adapter.h"
#include "coregraphics/antialiasquality.h"
#include "coregraphics/pixelformat.h"
#include "coregraphics/adapterinfo.h"
#include "coregraphics/threadsafedisplayeventhandler.h"
#include "coregraphics/threadsaferendereventhandler.h"
#include "internalgraphics/attachmentserver.h"
#include "internalgraphics/internalgraphicsentity.h"
#include "attr/attributecontainer.h"
#include "resources/resourceid.h"
#include "math/matrix44.h"
#include "math/float4.h"
#include "math/quaternion.h"
#include "math/clipstatus.h"
#include "graphics/handle.h"
#include "timing/time.h"
#include "animation/animeventinfo.h"
#include "animation/animeventhandlerbase.h"
#include "coregraphics/mousepointer.h"

//------------------------------------------------------------------------------
namespace Graphics
{
//------------------------------------------------------------------------------
class RegisterRTPlugin : public Messaging::Message
{
    __DeclareClass(RegisterRTPlugin);
    __DeclareMsgId;
public:
    RegisterRTPlugin() 
    { };
public:
    void SetType(const Core::Rtti* val)
    {
        n_assert(!this->handled);
        this->type = val;
    };
    const Core::Rtti* GetType() const
    {
        return this->type;
    };
private:
    const Core::Rtti* type;
};
//------------------------------------------------------------------------------
class UnregisterRTPlugin : public Messaging::Message
{
    __DeclareClass(UnregisterRTPlugin);
    __DeclareMsgId;
public:
    UnregisterRTPlugin() 
    { };
public:
    void SetType(const Core::Rtti* val)
    {
        n_assert(!this->handled);
        this->type = val;
    };
    const Core::Rtti* GetType() const
    {
        return this->type;
    };
private:
    const Core::Rtti* type;
};
//------------------------------------------------------------------------------
class SetupGraphics : public Messaging::Message
{
    __DeclareClass(SetupGraphics);
    __DeclareMsgId;
public:
    SetupGraphics() 
    { };
public:
    void SetAdapter(const CoreGraphics::Adapter::Code& val)
    {
        n_assert(!this->handled);
        this->adapter = val;
    };
    const CoreGraphics::Adapter::Code& GetAdapter() const
    {
        return this->adapter;
    };
private:
    CoreGraphics::Adapter::Code adapter;
public:
    void SetDisplayMode(const CoreGraphics::DisplayMode& val)
    {
        n_assert(!this->handled);
        this->displaymode = val;
    };
    const CoreGraphics::DisplayMode& GetDisplayMode() const
    {
        return this->displaymode;
    };
private:
    CoreGraphics::DisplayMode displaymode;
public:
    void SetAntiAliasQuality(const CoreGraphics::AntiAliasQuality::Code& val)
    {
        n_assert(!this->handled);
        this->antialiasquality = val;
    };
    const CoreGraphics::AntiAliasQuality::Code& GetAntiAliasQuality() const
    {
        return this->antialiasquality;
    };
private:
    CoreGraphics::AntiAliasQuality::Code antialiasquality;
public:
    void SetFullscreen(bool val)
    {
        n_assert(!this->handled);
        this->fullscreen = val;
    };
    bool GetFullscreen() const
    {
        return this->fullscreen;
    };
private:
    bool fullscreen;
public:
    void SetDisplayModeSwitchEnabled(bool val)
    {
        n_assert(!this->handled);
        this->displaymodeswitchenabled = val;
    };
    bool GetDisplayModeSwitchEnabled() const
    {
        return this->displaymodeswitchenabled;
    };
private:
    bool displaymodeswitchenabled;
public:
    void SetTripleBufferingEnabled(bool val)
    {
        n_assert(!this->handled);
        this->triplebufferingenabled = val;
    };
    bool GetTripleBufferingEnabled() const
    {
        return this->triplebufferingenabled;
    };
private:
    bool triplebufferingenabled;
public:
    void SetAlwaysOnTop(bool val)
    {
        n_assert(!this->handled);
        this->alwaysontop = val;
    };
    bool GetAlwaysOnTop() const
    {
        return this->alwaysontop;
    };
private:
    bool alwaysontop;
public:
    void SetVerticalSyncEnabled(bool val)
    {
        n_assert(!this->handled);
        this->verticalsyncenabled = val;
    };
    bool GetVerticalSyncEnabled() const
    {
        return this->verticalsyncenabled;
    };
private:
    bool verticalsyncenabled;
public:
    void SetIconName(const Util::String& val)
    {
        n_assert(!this->handled);
        this->iconname = val;
    };
    const Util::String& GetIconName() const
    {
        return this->iconname;
    };
private:
    Util::String iconname;
public:
    void SetSuccess(bool val)
    {
        n_assert(!this->handled);
        this->success = val;
    };
    bool GetSuccess() const
    {
        n_assert(this->handled);
        return this->success;
    };
private:
    bool success;
public:
    void SetActualDisplayMode(const CoreGraphics::DisplayMode& val)
    {
        n_assert(!this->handled);
        this->actualdisplaymode = val;
    };
    const CoreGraphics::DisplayMode& GetActualDisplayMode() const
    {
        n_assert(this->handled);
        return this->actualdisplaymode;
    };
private:
    CoreGraphics::DisplayMode actualdisplaymode;
};
//------------------------------------------------------------------------------
class AdapterExists : public Messaging::Message
{
    __DeclareClass(AdapterExists);
    __DeclareMsgId;
public:
    AdapterExists() 
    { };
public:
    void SetAdapter(const CoreGraphics::Adapter::Code& val)
    {
        n_assert(!this->handled);
        this->adapter = val;
    };
    const CoreGraphics::Adapter::Code& GetAdapter() const
    {
        return this->adapter;
    };
private:
    CoreGraphics::Adapter::Code adapter;
public:
    void SetResult(bool val)
    {
        n_assert(!this->handled);
        this->result = val;
    };
    bool GetResult() const
    {
        n_assert(this->handled);
        return this->result;
    };
private:
    bool result;
};
//------------------------------------------------------------------------------
class GetAvailableDisplayModes : public Messaging::Message
{
    __DeclareClass(GetAvailableDisplayModes);
    __DeclareMsgId;
public:
    GetAvailableDisplayModes() 
    { };
public:
    void SetAdapter(const CoreGraphics::Adapter::Code& val)
    {
        n_assert(!this->handled);
        this->adapter = val;
    };
    const CoreGraphics::Adapter::Code& GetAdapter() const
    {
        return this->adapter;
    };
private:
    CoreGraphics::Adapter::Code adapter;
public:
    void SetPixelFormat(const CoreGraphics::PixelFormat::Code& val)
    {
        n_assert(!this->handled);
        this->pixelformat = val;
    };
    const CoreGraphics::PixelFormat::Code& GetPixelFormat() const
    {
        return this->pixelformat;
    };
private:
    CoreGraphics::PixelFormat::Code pixelformat;
public:
    void SetResult(const Util::Array<CoreGraphics::DisplayMode>& val)
    {
        n_assert(!this->handled);
        this->result = val;
    };
    const Util::Array<CoreGraphics::DisplayMode>& GetResult() const
    {
        n_assert(this->handled);
        return this->result;
    };
private:
    Util::Array<CoreGraphics::DisplayMode> result;
};
//------------------------------------------------------------------------------
class SupportsDisplayMode : public Messaging::Message
{
    __DeclareClass(SupportsDisplayMode);
    __DeclareMsgId;
public:
    SupportsDisplayMode() 
    { };
public:
    void SetAdapter(const CoreGraphics::Adapter::Code& val)
    {
        n_assert(!this->handled);
        this->adapter = val;
    };
    const CoreGraphics::Adapter::Code& GetAdapter() const
    {
        return this->adapter;
    };
private:
    CoreGraphics::Adapter::Code adapter;
public:
    void SetDisplayMode(const CoreGraphics::DisplayMode& val)
    {
        n_assert(!this->handled);
        this->displaymode = val;
    };
    const CoreGraphics::DisplayMode& GetDisplayMode() const
    {
        return this->displaymode;
    };
private:
    CoreGraphics::DisplayMode displaymode;
public:
    void SetResult(bool val)
    {
        n_assert(!this->handled);
        this->result = val;
    };
    bool GetResult() const
    {
        n_assert(this->handled);
        return this->result;
    };
private:
    bool result;
};
//------------------------------------------------------------------------------
class GetCurrentAdapterDisplayMode : public Messaging::Message
{
    __DeclareClass(GetCurrentAdapterDisplayMode);
    __DeclareMsgId;
public:
    GetCurrentAdapterDisplayMode() 
    { };
public:
    void SetAdapter(const CoreGraphics::Adapter::Code& val)
    {
        n_assert(!this->handled);
        this->adapter = val;
    };
    const CoreGraphics::Adapter::Code& GetAdapter() const
    {
        return this->adapter;
    };
private:
    CoreGraphics::Adapter::Code adapter;
public:
    void SetResult(const CoreGraphics::DisplayMode& val)
    {
        n_assert(!this->handled);
        this->result = val;
    };
    const CoreGraphics::DisplayMode& GetResult() const
    {
        n_assert(this->handled);
        return this->result;
    };
private:
    CoreGraphics::DisplayMode result;
};
//------------------------------------------------------------------------------
class GetAdapterInfo : public Messaging::Message
{
    __DeclareClass(GetAdapterInfo);
    __DeclareMsgId;
public:
    GetAdapterInfo() 
    { };
public:
    void SetAdapter(const CoreGraphics::Adapter::Code& val)
    {
        n_assert(!this->handled);
        this->adapter = val;
    };
    const CoreGraphics::Adapter::Code& GetAdapter() const
    {
        return this->adapter;
    };
private:
    CoreGraphics::Adapter::Code adapter;
public:
    void SetResult(const CoreGraphics::AdapterInfo& val)
    {
        n_assert(!this->handled);
        this->result = val;
    };
    const CoreGraphics::AdapterInfo& GetResult() const
    {
        n_assert(this->handled);
        return this->result;
    };
private:
    CoreGraphics::AdapterInfo result;
};
//------------------------------------------------------------------------------
class AttachDisplayEventHandler : public Messaging::Message
{
    __DeclareClass(AttachDisplayEventHandler);
    __DeclareMsgId;
public:
    AttachDisplayEventHandler() 
    { };
public:
    void SetHandler(const Ptr<CoreGraphics::ThreadSafeDisplayEventHandler>& val)
    {
        n_assert(!this->handled);
        this->handler = val;
    };
    const Ptr<CoreGraphics::ThreadSafeDisplayEventHandler>& GetHandler() const
    {
        return this->handler;
    };
private:
    Ptr<CoreGraphics::ThreadSafeDisplayEventHandler> handler;
};
//------------------------------------------------------------------------------
class RemoveDisplayEventHandler : public Messaging::Message
{
    __DeclareClass(RemoveDisplayEventHandler);
    __DeclareMsgId;
public:
    RemoveDisplayEventHandler() 
    { };
public:
    void SetHandler(const Ptr<CoreGraphics::ThreadSafeDisplayEventHandler>& val)
    {
        n_assert(!this->handled);
        this->handler = val;
    };
    const Ptr<CoreGraphics::ThreadSafeDisplayEventHandler>& GetHandler() const
    {
        return this->handler;
    };
private:
    Ptr<CoreGraphics::ThreadSafeDisplayEventHandler> handler;
};
//------------------------------------------------------------------------------
class AttachRenderEventHandler : public Messaging::Message
{
    __DeclareClass(AttachRenderEventHandler);
    __DeclareMsgId;
public:
    AttachRenderEventHandler() 
    { };
public:
    void SetHandler(const Ptr<CoreGraphics::ThreadSafeRenderEventHandler>& val)
    {
        n_assert(!this->handled);
        this->handler = val;
    };
    const Ptr<CoreGraphics::ThreadSafeRenderEventHandler>& GetHandler() const
    {
        return this->handler;
    };
private:
    Ptr<CoreGraphics::ThreadSafeRenderEventHandler> handler;
};
//------------------------------------------------------------------------------
class RemoveRenderEventHandler : public Messaging::Message
{
    __DeclareClass(RemoveRenderEventHandler);
    __DeclareMsgId;
public:
    RemoveRenderEventHandler() 
    { };
public:
    void SetHandler(const Ptr<CoreGraphics::ThreadSafeRenderEventHandler>& val)
    {
        n_assert(!this->handled);
        this->handler = val;
    };
    const Ptr<CoreGraphics::ThreadSafeRenderEventHandler>& GetHandler() const
    {
        return this->handler;
    };
private:
    Ptr<CoreGraphics::ThreadSafeRenderEventHandler> handler;
};
//------------------------------------------------------------------------------
class CreateGraphicsStage : public Messaging::Message
{
    __DeclareClass(CreateGraphicsStage);
    __DeclareMsgId;
public:
    CreateGraphicsStage() :
        stagebuilderclass(0)
    { };
public:
    void SetName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->name = val;
    };
    const Util::StringAtom& GetName() const
    {
        return this->name;
    };
private:
    Util::StringAtom name;
public:
    void SetStageBuilderClass(const Core::Rtti* val)
    {
        n_assert(!this->handled);
        this->stagebuilderclass = val;
    };
    const Core::Rtti* GetStageBuilderClass() const
    {
        return this->stagebuilderclass;
    };
private:
    const Core::Rtti* stagebuilderclass;
public:
    void SetStageBuilderAttrs(const Attr::AttributeContainer& val)
    {
        n_assert(!this->handled);
        this->stagebuilderattrs = val;
    };
    const Attr::AttributeContainer& GetStageBuilderAttrs() const
    {
        return this->stagebuilderattrs;
    };
private:
    Attr::AttributeContainer stagebuilderattrs;
public:
    void SetResult(const Graphics::Handle& val)
    {
        n_assert(!this->handled);
        this->result = val;
    };
    const Graphics::Handle& GetResult() const
    {
        n_assert(this->handled);
        return this->result;
    };
private:
    Graphics::Handle result;
};
//------------------------------------------------------------------------------
class DiscardGraphicsStage : public Messaging::Message
{
    __DeclareClass(DiscardGraphicsStage);
    __DeclareMsgId;
public:
    DiscardGraphicsStage() 
    { };
public:
    void SetStageHandle(const Graphics::Handle& val)
    {
        n_assert(!this->handled);
        this->stagehandle = val;
    };
    const Graphics::Handle& GetStageHandle() const
    {
        return this->stagehandle;
    };
private:
    Graphics::Handle stagehandle;
};
//------------------------------------------------------------------------------
class CreateGraphicsView : public Messaging::Message
{
    __DeclareClass(CreateGraphicsView);
    __DeclareMsgId;
public:
    CreateGraphicsView() :
        viewclass(0),
        defaultview(false)
    { };
public:
    void SetViewClass(const Core::Rtti* val)
    {
        n_assert(!this->handled);
        this->viewclass = val;
    };
    const Core::Rtti* GetViewClass() const
    {
        return this->viewclass;
    };
private:
    const Core::Rtti* viewclass;
public:
    void SetName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->name = val;
    };
    const Util::StringAtom& GetName() const
    {
        return this->name;
    };
private:
    Util::StringAtom name;
public:
    void SetStageName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->stagename = val;
    };
    const Util::StringAtom& GetStageName() const
    {
        return this->stagename;
    };
private:
    Util::StringAtom stagename;
public:
    void SetFrameShaderName(const Resources::ResourceId& val)
    {
        n_assert(!this->handled);
        this->frameshadername = val;
    };
    const Resources::ResourceId& GetFrameShaderName() const
    {
        return this->frameshadername;
    };
private:
    Resources::ResourceId frameshadername;
public:
    void SetDefaultView(bool val)
    {
        n_assert(!this->handled);
        this->defaultview = val;
    };
    bool GetDefaultView() const
    {
        return this->defaultview;
    };
private:
    bool defaultview;
public:
    void SetResult(const Graphics::Handle& val)
    {
        n_assert(!this->handled);
        this->result = val;
    };
    const Graphics::Handle& GetResult() const
    {
        n_assert(this->handled);
        return this->result;
    };
private:
    Graphics::Handle result;
};
//------------------------------------------------------------------------------
class DiscardGraphicsView : public Messaging::Message
{
    __DeclareClass(DiscardGraphicsView);
    __DeclareMsgId;
public:
    DiscardGraphicsView() 
    { };
public:
    void SetViewHandle(const Graphics::Handle& val)
    {
        n_assert(!this->handled);
        this->viewhandle = val;
    };
    const Graphics::Handle& GetViewHandle() const
    {
        return this->viewhandle;
    };
private:
    Graphics::Handle viewhandle;
};
//------------------------------------------------------------------------------
class CreateGraphicsEntity : public Messaging::Message
{
    __DeclareClass(CreateGraphicsEntity);
    __DeclareMsgId;
public:
    CreateGraphicsEntity() :
        visible(true)
    { };
public:
    void SetStageName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->stagename = val;
    };
    const Util::StringAtom& GetStageName() const
    {
        return this->stagename;
    };
private:
    Util::StringAtom stagename;
public:
    void SetTransform(const Math::matrix44& val)
    {
        n_assert(!this->handled);
        this->transform = val;
    };
    const Math::matrix44& GetTransform() const
    {
        return this->transform;
    };
private:
    Math::matrix44 transform;
public:
    void SetVisible(bool val)
    {
        n_assert(!this->handled);
        this->visible = val;
    };
    bool GetVisible() const
    {
        return this->visible;
    };
private:
    bool visible;
public:
    void SetEntityHandle(const Graphics::Handle& val)
    {
        n_assert(!this->handled);
        this->entityhandle = val;
    };
    const Graphics::Handle& GetEntityHandle() const
    {
        n_assert(this->handled);
        return this->entityhandle;
    };
private:
    Graphics::Handle entityhandle;
};
//------------------------------------------------------------------------------
class DiscardGraphicsEntity : public Messaging::Message
{
    __DeclareClass(DiscardGraphicsEntity);
    __DeclareMsgId;
public:
    DiscardGraphicsEntity() 
    { };
public:
    void SetEntityHandle(const Graphics::Handle& val)
    {
        n_assert(!this->handled);
        this->entityhandle = val;
    };
    const Graphics::Handle& GetEntityHandle() const
    {
        return this->entityhandle;
    };
private:
    Graphics::Handle entityhandle;
};
//------------------------------------------------------------------------------
class CreateModelEntity : public CreateGraphicsEntity
{
    __DeclareClass(CreateModelEntity);
    __DeclareMsgId;
public:
    CreateModelEntity() 
    { };
public:
    void SetResourceId(const Resources::ResourceId& val)
    {
        n_assert(!this->handled);
        this->resourceid = val;
    };
    const Resources::ResourceId& GetResourceId() const
    {
        return this->resourceid;
    };
private:
    Resources::ResourceId resourceid;
public:
    void SetAnimationDrivenJointName(const Util::String& val)
    {
        n_assert(!this->handled);
        this->animationdrivenjointname = val;
    };
    const Util::String& GetAnimationDrivenJointName() const
    {
        return this->animationdrivenjointname;
    };
private:
    Util::String animationdrivenjointname;
};
//------------------------------------------------------------------------------
class CreateCameraEntity : public CreateGraphicsEntity
{
    __DeclareClass(CreateCameraEntity);
    __DeclareMsgId;
public:
    CreateCameraEntity() :
        isperspective(true),
        perspfieldofview(0.0f),
        perspaspectratio(0.0f),
        orthowidth(0.0f),
        orthoheight(0.0f)
    { };
public:
    void SetIsPerspective(bool val)
    {
        n_assert(!this->handled);
        this->isperspective = val;
    };
    bool GetIsPerspective() const
    {
        return this->isperspective;
    };
private:
    bool isperspective;
public:
    void SetPerspFieldOfView(float val)
    {
        n_assert(!this->handled);
        this->perspfieldofview = val;
    };
    float GetPerspFieldOfView() const
    {
        return this->perspfieldofview;
    };
private:
    float perspfieldofview;
public:
    void SetPerspAspectRatio(float val)
    {
        n_assert(!this->handled);
        this->perspaspectratio = val;
    };
    float GetPerspAspectRatio() const
    {
        return this->perspaspectratio;
    };
private:
    float perspaspectratio;
public:
    void SetOrthoWidth(float val)
    {
        n_assert(!this->handled);
        this->orthowidth = val;
    };
    float GetOrthoWidth() const
    {
        return this->orthowidth;
    };
private:
    float orthowidth;
public:
    void SetOrthoHeight(float val)
    {
        n_assert(!this->handled);
        this->orthoheight = val;
    };
    float GetOrthoHeight() const
    {
        return this->orthoheight;
    };
private:
    float orthoheight;
public:
    void SetNearPlane(float val)
    {
        n_assert(!this->handled);
        this->nearplane = val;
    };
    float GetNearPlane() const
    {
        return this->nearplane;
    };
private:
    float nearplane;
public:
    void SetFarPlane(float val)
    {
        n_assert(!this->handled);
        this->farplane = val;
    };
    float GetFarPlane() const
    {
        return this->farplane;
    };
private:
    float farplane;
};
//------------------------------------------------------------------------------
class CreateGlobalLightEntity : public CreateGraphicsEntity
{
    __DeclareClass(CreateGlobalLightEntity);
    __DeclareMsgId;
public:
    CreateGlobalLightEntity() 
    { };
public:
    void SetLightColor(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->lightcolor = val;
    };
    const Math::float4& GetLightColor() const
    {
        return this->lightcolor;
    };
private:
    Math::float4 lightcolor;
public:
    void SetProjMapUvOffsetAndScale(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->projmapuvoffsetandscale = val;
    };
    const Math::float4& GetProjMapUvOffsetAndScale() const
    {
        return this->projmapuvoffsetandscale;
    };
private:
    Math::float4 projmapuvoffsetandscale;
public:
    void SetCastShadows(bool val)
    {
        n_assert(!this->handled);
        this->castshadows = val;
    };
    bool GetCastShadows() const
    {
        return this->castshadows;
    };
private:
    bool castshadows;
public:
    void SetBackLightColor(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->backlightcolor = val;
    };
    const Math::float4& GetBackLightColor() const
    {
        return this->backlightcolor;
    };
private:
    Math::float4 backlightcolor;
public:
    void SetAmbientLightColor(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->ambientlightcolor = val;
    };
    const Math::float4& GetAmbientLightColor() const
    {
        return this->ambientlightcolor;
    };
private:
    Math::float4 ambientlightcolor;
public:
    void SetBackLightOffset(float val)
    {
        n_assert(!this->handled);
        this->backlightoffset = val;
    };
    float GetBackLightOffset() const
    {
        return this->backlightoffset;
    };
private:
    float backlightoffset;
};
//------------------------------------------------------------------------------
class CreateSpotLightEntity : public CreateGraphicsEntity
{
    __DeclareClass(CreateSpotLightEntity);
    __DeclareMsgId;
public:
    CreateSpotLightEntity() 
    { };
public:
    void SetLightColor(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->lightcolor = val;
    };
    const Math::float4& GetLightColor() const
    {
        return this->lightcolor;
    };
private:
    Math::float4 lightcolor;
public:
    void SetProjMapUvOffsetAndScale(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->projmapuvoffsetandscale = val;
    };
    const Math::float4& GetProjMapUvOffsetAndScale() const
    {
        return this->projmapuvoffsetandscale;
    };
private:
    Math::float4 projmapuvoffsetandscale;
public:
    void SetCastShadows(bool val)
    {
        n_assert(!this->handled);
        this->castshadows = val;
    };
    bool GetCastShadows() const
    {
        return this->castshadows;
    };
private:
    bool castshadows;
};
//------------------------------------------------------------------------------
class CreatePointLightEntity : public CreateGraphicsEntity
{
    __DeclareClass(CreatePointLightEntity);
    __DeclareMsgId;
public:
    CreatePointLightEntity() 
    { };
public:
    void SetLightColor(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->lightcolor = val;
    };
    const Math::float4& GetLightColor() const
    {
        return this->lightcolor;
    };
private:
    Math::float4 lightcolor;
public:
    void SetProjMapUvOffsetAndScale(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->projmapuvoffsetandscale = val;
    };
    const Math::float4& GetProjMapUvOffsetAndScale() const
    {
        return this->projmapuvoffsetandscale;
    };
private:
    Math::float4 projmapuvoffsetandscale;
public:
    void SetCastShadows(bool val)
    {
        n_assert(!this->handled);
        this->castshadows = val;
    };
    bool GetCastShadows() const
    {
        return this->castshadows;
    };
private:
    bool castshadows;
};
//------------------------------------------------------------------------------
class GraphicsEntityMessage : public Messaging::Message
{
    __DeclareClass(GraphicsEntityMessage);
    __DeclareMsgId;
public:
    GraphicsEntityMessage() 
    { };
public:
    void SetEntityHandle(const Graphics::Handle& val)
    {
        n_assert(!this->handled);
        this->entityhandle = val;
    };
    const Graphics::Handle& GetEntityHandle() const
    {
        return this->entityhandle;
    };
private:
    Graphics::Handle entityhandle;
};
//------------------------------------------------------------------------------
class SetLightColor : public GraphicsEntityMessage
{
    __DeclareClass(SetLightColor);
    __DeclareMsgId;
public:
    SetLightColor() 
    { };
public:
    void SetColor(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->color = val;
    };
    const Math::float4& GetColor() const
    {
        return this->color;
    };
private:
    Math::float4 color;
};
//------------------------------------------------------------------------------
class SetLightCastShadows : public GraphicsEntityMessage
{
    __DeclareClass(SetLightCastShadows);
    __DeclareMsgId;
public:
    SetLightCastShadows() 
    { };
public:
    void SetCastShadows(bool val)
    {
        n_assert(!this->handled);
        this->castshadows = val;
    };
    bool GetCastShadows() const
    {
        return this->castshadows;
    };
private:
    bool castshadows;
};
//------------------------------------------------------------------------------
class SetGlobalBackLightColor : public GraphicsEntityMessage
{
    __DeclareClass(SetGlobalBackLightColor);
    __DeclareMsgId;
public:
    SetGlobalBackLightColor() 
    { };
public:
    void SetColor(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->color = val;
    };
    const Math::float4& GetColor() const
    {
        return this->color;
    };
private:
    Math::float4 color;
};
//------------------------------------------------------------------------------
class SetGlobalAmbientLightColor : public GraphicsEntityMessage
{
    __DeclareClass(SetGlobalAmbientLightColor);
    __DeclareMsgId;
public:
    SetGlobalAmbientLightColor() 
    { };
public:
    void SetColor(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->color = val;
    };
    const Math::float4& GetColor() const
    {
        return this->color;
    };
private:
    Math::float4 color;
};
//------------------------------------------------------------------------------
class SetGlobalBackLightOffset : public GraphicsEntityMessage
{
    __DeclareClass(SetGlobalBackLightOffset);
    __DeclareMsgId;
public:
    SetGlobalBackLightOffset() 
    { };
public:
    void SetOffset(float val)
    {
        n_assert(!this->handled);
        this->offset = val;
    };
    float GetOffset() const
    {
        return this->offset;
    };
private:
    float offset;
};
//------------------------------------------------------------------------------
class SetGlobalLightParams : public GraphicsEntityMessage
{
    __DeclareClass(SetGlobalLightParams);
    __DeclareMsgId;
public:
    SetGlobalLightParams() 
    { };
public:
    void SetLightColor(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->lightcolor = val;
    };
    const Math::float4& GetLightColor() const
    {
        return this->lightcolor;
    };
private:
    Math::float4 lightcolor;
public:
    void SetBackLightColor(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->backlightcolor = val;
    };
    const Math::float4& GetBackLightColor() const
    {
        return this->backlightcolor;
    };
private:
    Math::float4 backlightcolor;
public:
    void SetAmbientLightColor(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->ambientlightcolor = val;
    };
    const Math::float4& GetAmbientLightColor() const
    {
        return this->ambientlightcolor;
    };
private:
    Math::float4 ambientlightcolor;
public:
    void SetBackLightOffset(float val)
    {
        n_assert(!this->handled);
        this->backlightoffset = val;
    };
    float GetBackLightOffset() const
    {
        return this->backlightoffset;
    };
private:
    float backlightoffset;
};
//------------------------------------------------------------------------------
class SetGlobalLightColor : public GraphicsEntityMessage
{
    __DeclareClass(SetGlobalLightColor);
    __DeclareMsgId;
public:
    SetGlobalLightColor() 
    { };
public:
    void SetBackLightColor(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->backlightcolor = val;
    };
    const Math::float4& GetBackLightColor() const
    {
        return this->backlightcolor;
    };
private:
    Math::float4 backlightcolor;
public:
    void SetAmbientLightColor(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->ambientlightcolor = val;
    };
    const Math::float4& GetAmbientLightColor() const
    {
        return this->ambientlightcolor;
    };
private:
    Math::float4 ambientlightcolor;
public:
    void SetBackLightOffset(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->backlightoffset = val;
    };
    const Math::float4& GetBackLightOffset() const
    {
        return this->backlightoffset;
    };
private:
    Math::float4 backlightoffset;
};
//------------------------------------------------------------------------------
class UpdateTransform : public GraphicsEntityMessage
{
    __DeclareClass(UpdateTransform);
    __DeclareMsgId;
public:
    UpdateTransform() 
    { };
public:
    void SetTransform(const Math::matrix44& val)
    {
        n_assert(!this->handled);
        this->transform = val;
    };
    const Math::matrix44& GetTransform() const
    {
        return this->transform;
    };
private:
    Math::matrix44 transform;
};
//------------------------------------------------------------------------------
class SetVisibility : public GraphicsEntityMessage
{
    __DeclareClass(SetVisibility);
    __DeclareMsgId;
public:
    SetVisibility() 
    { };
public:
    void SetVisible(bool val)
    {
        n_assert(!this->handled);
        this->visible = val;
    };
    bool GetVisible() const
    {
        return this->visible;
    };
private:
    bool visible;
};
//------------------------------------------------------------------------------
class AttachCameraToView : public GraphicsEntityMessage
{
    __DeclareClass(AttachCameraToView);
    __DeclareMsgId;
public:
    AttachCameraToView() 
    { };
public:
    void SetViewName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->viewname = val;
    };
    const Util::StringAtom& GetViewName() const
    {
        return this->viewname;
    };
private:
    Util::StringAtom viewname;
};
//------------------------------------------------------------------------------
class RemoveCameraFromView : public GraphicsEntityMessage
{
    __DeclareClass(RemoveCameraFromView);
    __DeclareMsgId;
public:
    RemoveCameraFromView() 
    { };
public:
    void SetViewName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->viewname = val;
    };
    const Util::StringAtom& GetViewName() const
    {
        return this->viewname;
    };
private:
    Util::StringAtom viewname;
};
//------------------------------------------------------------------------------
class UpdateInstanceShaderVariable : public GraphicsEntityMessage
{
    __DeclareClass(UpdateInstanceShaderVariable);
    __DeclareMsgId;
public:
    UpdateInstanceShaderVariable() 
    { };
public:
    void SetNodeInstanceName(const Util::String& val)
    {
        n_assert(!this->handled);
        this->nodeinstancename = val;
    };
    const Util::String& GetNodeInstanceName() const
    {
        return this->nodeinstancename;
    };
private:
    Util::String nodeinstancename;
public:
    void SetSemantic(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->semantic = val;
    };
    const Util::StringAtom& GetSemantic() const
    {
        return this->semantic;
    };
private:
    Util::StringAtom semantic;
public:
    void SetValue(const Util::Variant& val)
    {
        n_assert(!this->handled);
        this->value = val;
    };
    const Util::Variant& GetValue() const
    {
        return this->value;
    };
private:
    Util::Variant value;
};
//------------------------------------------------------------------------------
class AnimPlayOnce : public GraphicsEntityMessage
{
    __DeclareClass(AnimPlayOnce);
    __DeclareMsgId;
public:
    AnimPlayOnce() :
        starttime(0),
        fadeintime(0),
        fadeouttime(0),
        timeoffset(0),
        blendpriority(0),
        blendweight(1.0f),
        timefactor(1.0)
    { };
public:
    void SetClipName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->clipname = val;
    };
    const Util::StringAtom& GetClipName() const
    {
        return this->clipname;
    };
private:
    Util::StringAtom clipname;
public:
    void SetCategory(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->category = val;
    };
    const Util::StringAtom& GetCategory() const
    {
        return this->category;
    };
private:
    Util::StringAtom category;
public:
    void SetStartTime(const Timing::Tick& val)
    {
        n_assert(!this->handled);
        this->starttime = val;
    };
    const Timing::Tick& GetStartTime() const
    {
        return this->starttime;
    };
private:
    Timing::Tick starttime;
public:
    void SetFadeInTime(const Timing::Tick& val)
    {
        n_assert(!this->handled);
        this->fadeintime = val;
    };
    const Timing::Tick& GetFadeInTime() const
    {
        return this->fadeintime;
    };
private:
    Timing::Tick fadeintime;
public:
    void SetFadeOutTime(const Timing::Tick& val)
    {
        n_assert(!this->handled);
        this->fadeouttime = val;
    };
    const Timing::Tick& GetFadeOutTime() const
    {
        return this->fadeouttime;
    };
private:
    Timing::Tick fadeouttime;
public:
    void SetTimeOffset(const Timing::Tick& val)
    {
        n_assert(!this->handled);
        this->timeoffset = val;
    };
    const Timing::Tick& GetTimeOffset() const
    {
        return this->timeoffset;
    };
private:
    Timing::Tick timeoffset;
public:
    void SetBlendPriority(int val)
    {
        n_assert(!this->handled);
        this->blendpriority = val;
    };
    int GetBlendPriority() const
    {
        return this->blendpriority;
    };
private:
    int blendpriority;
public:
    void SetBlendWeight(float val)
    {
        n_assert(!this->handled);
        this->blendweight = val;
    };
    float GetBlendWeight() const
    {
        return this->blendweight;
    };
private:
    float blendweight;
public:
    void SetTimeFactor(const Timing::Time& val)
    {
        n_assert(!this->handled);
        this->timefactor = val;
    };
    const Timing::Time& GetTimeFactor() const
    {
        return this->timefactor;
    };
private:
    Timing::Time timefactor;
};
//------------------------------------------------------------------------------
class AnimPlayLoop : public GraphicsEntityMessage
{
    __DeclareClass(AnimPlayLoop);
    __DeclareMsgId;
public:
    AnimPlayLoop() :
        loopcount(0),
        starttime(0),
        fadeintime(0),
        fadeouttime(0),
        timeoffset(0),
        blendpriority(0),
        blendweight(1.0f),
        timefactor(1.0)
    { };
public:
    void SetClipName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->clipname = val;
    };
    const Util::StringAtom& GetClipName() const
    {
        return this->clipname;
    };
private:
    Util::StringAtom clipname;
public:
    void SetCategory(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->category = val;
    };
    const Util::StringAtom& GetCategory() const
    {
        return this->category;
    };
private:
    Util::StringAtom category;
public:
    void SetLoopCount(uint val)
    {
        n_assert(!this->handled);
        this->loopcount = val;
    };
    uint GetLoopCount() const
    {
        return this->loopcount;
    };
private:
    uint loopcount;
public:
    void SetStartTime(const Timing::Tick& val)
    {
        n_assert(!this->handled);
        this->starttime = val;
    };
    const Timing::Tick& GetStartTime() const
    {
        return this->starttime;
    };
private:
    Timing::Tick starttime;
public:
    void SetFadeInTime(const Timing::Tick& val)
    {
        n_assert(!this->handled);
        this->fadeintime = val;
    };
    const Timing::Tick& GetFadeInTime() const
    {
        return this->fadeintime;
    };
private:
    Timing::Tick fadeintime;
public:
    void SetFadeOutTime(const Timing::Tick& val)
    {
        n_assert(!this->handled);
        this->fadeouttime = val;
    };
    const Timing::Tick& GetFadeOutTime() const
    {
        return this->fadeouttime;
    };
private:
    Timing::Tick fadeouttime;
public:
    void SetTimeOffset(const Timing::Tick& val)
    {
        n_assert(!this->handled);
        this->timeoffset = val;
    };
    const Timing::Tick& GetTimeOffset() const
    {
        return this->timeoffset;
    };
private:
    Timing::Tick timeoffset;
public:
    void SetBlendPriority(int val)
    {
        n_assert(!this->handled);
        this->blendpriority = val;
    };
    int GetBlendPriority() const
    {
        return this->blendpriority;
    };
private:
    int blendpriority;
public:
    void SetBlendWeight(float val)
    {
        n_assert(!this->handled);
        this->blendweight = val;
    };
    float GetBlendWeight() const
    {
        return this->blendweight;
    };
private:
    float blendweight;
public:
    void SetTimeFactor(const Timing::Time& val)
    {
        n_assert(!this->handled);
        this->timefactor = val;
    };
    const Timing::Time& GetTimeFactor() const
    {
        return this->timefactor;
    };
private:
    Timing::Time timefactor;
};
//------------------------------------------------------------------------------
class AnimPlayDuration : public GraphicsEntityMessage
{
    __DeclareClass(AnimPlayDuration);
    __DeclareMsgId;
public:
    AnimPlayDuration() :
        duration(0),
        starttime(0),
        fadeintime(0),
        fadeouttime(0),
        timeoffset(0),
        blendpriority(0),
        blendweight(1.0f),
        timefactor(1.0)
    { };
public:
    void SetClipName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->clipname = val;
    };
    const Util::StringAtom& GetClipName() const
    {
        return this->clipname;
    };
private:
    Util::StringAtom clipname;
public:
    void SetCategory(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->category = val;
    };
    const Util::StringAtom& GetCategory() const
    {
        return this->category;
    };
private:
    Util::StringAtom category;
public:
    void SetDuration(const Timing::Tick& val)
    {
        n_assert(!this->handled);
        this->duration = val;
    };
    const Timing::Tick& GetDuration() const
    {
        return this->duration;
    };
private:
    Timing::Tick duration;
public:
    void SetStartTime(const Timing::Tick& val)
    {
        n_assert(!this->handled);
        this->starttime = val;
    };
    const Timing::Tick& GetStartTime() const
    {
        return this->starttime;
    };
private:
    Timing::Tick starttime;
public:
    void SetFadeInTime(const Timing::Tick& val)
    {
        n_assert(!this->handled);
        this->fadeintime = val;
    };
    const Timing::Tick& GetFadeInTime() const
    {
        return this->fadeintime;
    };
private:
    Timing::Tick fadeintime;
public:
    void SetFadeOutTime(const Timing::Tick& val)
    {
        n_assert(!this->handled);
        this->fadeouttime = val;
    };
    const Timing::Tick& GetFadeOutTime() const
    {
        return this->fadeouttime;
    };
private:
    Timing::Tick fadeouttime;
public:
    void SetTimeOffset(const Timing::Tick& val)
    {
        n_assert(!this->handled);
        this->timeoffset = val;
    };
    const Timing::Tick& GetTimeOffset() const
    {
        return this->timeoffset;
    };
private:
    Timing::Tick timeoffset;
public:
    void SetBlendPriority(int val)
    {
        n_assert(!this->handled);
        this->blendpriority = val;
    };
    int GetBlendPriority() const
    {
        return this->blendpriority;
    };
private:
    int blendpriority;
public:
    void SetBlendWeight(float val)
    {
        n_assert(!this->handled);
        this->blendweight = val;
    };
    float GetBlendWeight() const
    {
        return this->blendweight;
    };
private:
    float blendweight;
public:
    void SetTimeFactor(const Timing::Time& val)
    {
        n_assert(!this->handled);
        this->timefactor = val;
    };
    const Timing::Time& GetTimeFactor() const
    {
        return this->timefactor;
    };
private:
    Timing::Time timefactor;
};
//------------------------------------------------------------------------------
class AnimModifyBlendWeight : public GraphicsEntityMessage
{
    __DeclareClass(AnimModifyBlendWeight);
    __DeclareMsgId;
public:
    AnimModifyBlendWeight() :
        blendweight(1.0f)
    { };
public:
    void SetClipName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->clipname = val;
    };
    const Util::StringAtom& GetClipName() const
    {
        return this->clipname;
    };
private:
    Util::StringAtom clipname;
public:
    void SetCategory(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->category = val;
    };
    const Util::StringAtom& GetCategory() const
    {
        return this->category;
    };
private:
    Util::StringAtom category;
public:
    void SetBlendWeight(float val)
    {
        n_assert(!this->handled);
        this->blendweight = val;
    };
    float GetBlendWeight() const
    {
        return this->blendweight;
    };
private:
    float blendweight;
};
//------------------------------------------------------------------------------
class AnimModifyTimeFactor : public GraphicsEntityMessage
{
    __DeclareClass(AnimModifyTimeFactor);
    __DeclareMsgId;
public:
    AnimModifyTimeFactor() :
        timefactor(1.0)
    { };
public:
    void SetClipName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->clipname = val;
    };
    const Util::StringAtom& GetClipName() const
    {
        return this->clipname;
    };
private:
    Util::StringAtom clipname;
public:
    void SetCategory(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->category = val;
    };
    const Util::StringAtom& GetCategory() const
    {
        return this->category;
    };
private:
    Util::StringAtom category;
public:
    void SetTimeFactor(const Timing::Time& val)
    {
        n_assert(!this->handled);
        this->timefactor = val;
    };
    const Timing::Time& GetTimeFactor() const
    {
        return this->timefactor;
    };
private:
    Timing::Time timefactor;
};
//------------------------------------------------------------------------------
class AnimStopClip : public GraphicsEntityMessage
{
    __DeclareClass(AnimStopClip);
    __DeclareMsgId;
public:
    AnimStopClip() 
    { };
public:
    void SetClipName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->clipname = val;
    };
    const Util::StringAtom& GetClipName() const
    {
        return this->clipname;
    };
private:
    Util::StringAtom clipname;
};
//------------------------------------------------------------------------------
class AnimStopCategory : public GraphicsEntityMessage
{
    __DeclareClass(AnimStopCategory);
    __DeclareMsgId;
public:
    AnimStopCategory() 
    { };
public:
    void SetCategory(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->category = val;
    };
    const Util::StringAtom& GetCategory() const
    {
        return this->category;
    };
private:
    Util::StringAtom category;
};
//------------------------------------------------------------------------------
class AnimStopAll : public GraphicsEntityMessage
{
    __DeclareClass(AnimStopAll);
    __DeclareMsgId;
public:
    AnimStopAll() 
    { };
};
//------------------------------------------------------------------------------
class AnimGetClipDuration : public GraphicsEntityMessage
{
    __DeclareClass(AnimGetClipDuration);
    __DeclareMsgId;
public:
    AnimGetClipDuration() 
    { };
public:
    void SetClipName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->clipname = val;
    };
    const Util::StringAtom& GetClipName() const
    {
        return this->clipname;
    };
private:
    Util::StringAtom clipname;
public:
    void SetDuration(const Timing::Tick& val)
    {
        n_assert(!this->handled);
        this->duration = val;
    };
    const Timing::Tick& GetDuration() const
    {
        n_assert(this->handled);
        return this->duration;
    };
private:
    Timing::Tick duration;
};
//------------------------------------------------------------------------------
class ApplySkinList : public GraphicsEntityMessage
{
    __DeclareClass(ApplySkinList);
    __DeclareMsgId;
public:
    ApplySkinList() 
    { };
public:
    void SetSkinList(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->skinlist = val;
    };
    const Util::StringAtom& GetSkinList() const
    {
        return this->skinlist;
    };
private:
    Util::StringAtom skinlist;
};
//------------------------------------------------------------------------------
class ShowSkin : public GraphicsEntityMessage
{
    __DeclareClass(ShowSkin);
    __DeclareMsgId;
public:
    ShowSkin() 
    { };
public:
    void SetSkin(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->skin = val;
    };
    const Util::StringAtom& GetSkin() const
    {
        return this->skin;
    };
private:
    Util::StringAtom skin;
};
//------------------------------------------------------------------------------
class HideSkin : public GraphicsEntityMessage
{
    __DeclareClass(HideSkin);
    __DeclareMsgId;
public:
    HideSkin() 
    { };
public:
    void SetSkin(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->skin = val;
    };
    const Util::StringAtom& GetSkin() const
    {
        return this->skin;
    };
private:
    Util::StringAtom skin;
};
//------------------------------------------------------------------------------
class SetupGrassFromStage : public Messaging::Message
{
    __DeclareClass(SetupGrassFromStage);
    __DeclareMsgId;
public:
    SetupGrassFromStage() 
    { };
public:
    void SetStageName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->stagename = val;
    };
    const Util::StringAtom& GetStageName() const
    {
        return this->stagename;
    };
private:
    Util::StringAtom stagename;
};
//------------------------------------------------------------------------------
class SetTimeFactor : public Messaging::Message
{
    __DeclareClass(SetTimeFactor);
    __DeclareMsgId;
public:
    SetTimeFactor() 
    { };
public:
    void SetFactor(const Timing::Time& val)
    {
        n_assert(!this->handled);
        this->factor = val;
    };
    const Timing::Time& GetFactor() const
    {
        return this->factor;
    };
private:
    Timing::Time factor;
};
//------------------------------------------------------------------------------
class UnbindAttachment : public Messaging::Message
{
    __DeclareClass(UnbindAttachment);
    __DeclareMsgId;
public:
    UnbindAttachment() :
        cleartype(InternalGraphics::AttachmentServer::ClearNone)
    { };
public:
    void SetBaseEntityHandle(const Graphics::Handle& val)
    {
        n_assert(!this->handled);
        this->baseentityhandle = val;
    };
    const Graphics::Handle& GetBaseEntityHandle() const
    {
        return this->baseentityhandle;
    };
private:
    Graphics::Handle baseentityhandle;
public:
    void SetEntityToAttachHandle(const Graphics::Handle& val)
    {
        n_assert(!this->handled);
        this->entitytoattachhandle = val;
    };
    const Graphics::Handle& GetEntityToAttachHandle() const
    {
        return this->entitytoattachhandle;
    };
private:
    Graphics::Handle entitytoattachhandle;
public:
    void SetJoint(const Util::String& val)
    {
        n_assert(!this->handled);
        this->joint = val;
    };
    const Util::String& GetJoint() const
    {
        return this->joint;
    };
private:
    Util::String joint;
public:
    void SetClearType(const InternalGraphics::AttachmentServer::ClearType& val)
    {
        n_assert(!this->handled);
        this->cleartype = val;
    };
    const InternalGraphics::AttachmentServer::ClearType& GetClearType() const
    {
        return this->cleartype;
    };
private:
    InternalGraphics::AttachmentServer::ClearType cleartype;
};
//------------------------------------------------------------------------------
class BindAttachment : public UnbindAttachment
{
    __DeclareClass(BindAttachment);
    __DeclareMsgId;
public:
    BindAttachment() 
    { };
public:
    void SetKeepLocal(bool val)
    {
        n_assert(!this->handled);
        this->keeplocal = val;
    };
    bool GetKeepLocal() const
    {
        return this->keeplocal;
    };
private:
    bool keeplocal;
public:
    void SetRotation(const InternalGraphics::AttachmentServer::AttachmentRotation& val)
    {
        n_assert(!this->handled);
        this->rotation = val;
    };
    const InternalGraphics::AttachmentServer::AttachmentRotation& GetRotation() const
    {
        return this->rotation;
    };
private:
    InternalGraphics::AttachmentServer::AttachmentRotation rotation;
public:
    void SetOffset(const Math::matrix44& val)
    {
        n_assert(!this->handled);
        this->offset = val;
    };
    const Math::matrix44& GetOffset() const
    {
        return this->offset;
    };
private:
    Math::matrix44 offset;
};
//------------------------------------------------------------------------------
class GetAnimationDrivenVector : public GraphicsEntityMessage
{
    __DeclareClass(GetAnimationDrivenVector);
    __DeclareMsgId;
public:
    GetAnimationDrivenVector() 
    { };
public:
    void SetMovement(const Math::vector& val)
    {
        n_assert(!this->handled);
        this->movement = val;
    };
    const Math::vector& GetMovement() const
    {
        n_assert(this->handled);
        return this->movement;
    };
private:
    Math::vector movement;
public:
    void SetClipStatus(const Math::ClipStatus::Type& val)
    {
        n_assert(!this->handled);
        this->clipstatus = val;
    };
    const Math::ClipStatus::Type& GetClipStatus() const
    {
        n_assert(this->handled);
        return this->clipstatus;
    };
private:
    Math::ClipStatus::Type clipstatus;
};
//------------------------------------------------------------------------------
class SetAnimationDrivenFlag : public GraphicsEntityMessage
{
    __DeclareClass(SetAnimationDrivenFlag);
    __DeclareMsgId;
public:
    SetAnimationDrivenFlag() 
    { };
public:
    void SetAnimationDriven(bool val)
    {
        n_assert(!this->handled);
        this->animationdriven = val;
    };
    bool GetAnimationDriven() const
    {
        return this->animationdriven;
    };
private:
    bool animationdriven;
public:
    void SetJointName(const Util::String& val)
    {
        n_assert(!this->handled);
        this->jointname = val;
    };
    const Util::String& GetJointName() const
    {
        return this->jointname;
    };
private:
    Util::String jointname;
};
//------------------------------------------------------------------------------
class GetActiveAnimEvents : public GraphicsEntityMessage
{
    __DeclareClass(GetActiveAnimEvents);
    __DeclareMsgId;
public:
    GetActiveAnimEvents() :
        onekeyduration(false),
        usejustdominatingclip(false)
    { };
public:
    void SetApplicationTime(const Timing::Tick& val)
    {
        n_assert(!this->handled);
        this->applicationtime = val;
    };
    const Timing::Tick& GetApplicationTime() const
    {
        return this->applicationtime;
    };
private:
    Timing::Tick applicationtime;
public:
    void SetStartTime(const Timing::Tick& val)
    {
        n_assert(!this->handled);
        this->starttime = val;
    };
    const Timing::Tick& GetStartTime() const
    {
        return this->starttime;
    };
private:
    Timing::Tick starttime;
public:
    void SetEndTime(const Timing::Tick& val)
    {
        n_assert(!this->handled);
        this->endtime = val;
    };
    const Timing::Tick& GetEndTime() const
    {
        return this->endtime;
    };
private:
    Timing::Tick endtime;
public:
    void SetOneKeyDuration(bool val)
    {
        n_assert(!this->handled);
        this->onekeyduration = val;
    };
    bool GetOneKeyDuration() const
    {
        return this->onekeyduration;
    };
private:
    bool onekeyduration;
public:
    void SetCategories(const Util::Array<Util::StringAtom>& val)
    {
        n_assert(!this->handled);
        this->categories = val;
    };
    const Util::Array<Util::StringAtom>& GetCategories() const
    {
        return this->categories;
    };
private:
    Util::Array<Util::StringAtom> categories;
public:
    void SetUseJustDominatingClip(bool val)
    {
        n_assert(!this->handled);
        this->usejustdominatingclip = val;
    };
    bool GetUseJustDominatingClip() const
    {
        return this->usejustdominatingclip;
    };
private:
    bool usejustdominatingclip;
public:
    void SetAnimEvents(const Util::Dictionary<Util::StringAtom, Util::Array<Animation::AnimEventInfo> >& val)
    {
        n_assert(!this->handled);
        this->animevents = val;
    };
    const Util::Dictionary<Util::StringAtom, Util::Array<Animation::AnimEventInfo> >& GetAnimEvents() const
    {
        n_assert(this->handled);
        return this->animevents;
    };
private:
    Util::Dictionary<Util::StringAtom, Util::Array<Animation::AnimEventInfo> > animevents;
};
//------------------------------------------------------------------------------
class GetMaximumKeyDuration : public GraphicsEntityMessage
{
    __DeclareClass(GetMaximumKeyDuration);
    __DeclareMsgId;
public:
    GetMaximumKeyDuration() 
    { };
public:
    void SetDuration(const Timing::Tick& val)
    {
        n_assert(!this->handled);
        this->duration = val;
    };
    const Timing::Tick& GetDuration() const
    {
        n_assert(this->handled);
        return this->duration;
    };
private:
    Timing::Tick duration;
};
//------------------------------------------------------------------------------
class GetJointTransform : public GraphicsEntityMessage
{
    __DeclareClass(GetJointTransform);
    __DeclareMsgId;
public:
    GetJointTransform() 
    { };
public:
    void SetJointName(const Util::String& val)
    {
        n_assert(!this->handled);
        this->jointname = val;
    };
    const Util::String& GetJointName() const
    {
        return this->jointname;
    };
private:
    Util::String jointname;
public:
    void SetTransform(const Math::matrix44& val)
    {
        n_assert(!this->handled);
        this->transform = val;
    };
    const Math::matrix44& GetTransform() const
    {
        n_assert(this->handled);
        return this->transform;
    };
private:
    Math::matrix44 transform;
};
//------------------------------------------------------------------------------
class RegisterAnimEventHandler : public Messaging::Message
{
    __DeclareClass(RegisterAnimEventHandler);
    __DeclareMsgId;
public:
    RegisterAnimEventHandler() 
    { };
public:
    void SetAnimEventHandler(const Ptr<Animation::AnimEventHandlerBase>& val)
    {
        n_assert(!this->handled);
        this->animeventhandler = val;
    };
    const Ptr<Animation::AnimEventHandlerBase>& GetAnimEventHandler() const
    {
        return this->animeventhandler;
    };
private:
    Ptr<Animation::AnimEventHandlerBase> animeventhandler;
};
//------------------------------------------------------------------------------
class UnregisterAnimEventHandler : public Messaging::Message
{
    __DeclareClass(UnregisterAnimEventHandler);
    __DeclareMsgId;
public:
    UnregisterAnimEventHandler() 
    { };
public:
    void SetCategoryName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->categoryname = val;
    };
    const Util::StringAtom& GetCategoryName() const
    {
        return this->categoryname;
    };
private:
    Util::StringAtom categoryname;
};
//------------------------------------------------------------------------------
class PreloadMousePointerTextures : public Messaging::Message
{
    __DeclareClass(PreloadMousePointerTextures);
    __DeclareMsgId;
public:
    PreloadMousePointerTextures() 
    { };
public:
    void SetResourceIds(const Util::Array<Resources::ResourceId>& val)
    {
        n_assert(!this->handled);
        this->resourceids = val;
    };
    const Util::Array<Resources::ResourceId>& GetResourceIds() const
    {
        return this->resourceids;
    };
private:
    Util::Array<Resources::ResourceId> resourceids;
};
//------------------------------------------------------------------------------
class UpdateMousePointers : public Messaging::Message
{
    __DeclareClass(UpdateMousePointers);
    __DeclareMsgId;
public:
    UpdateMousePointers() 
    { };
public:
    void SetPointers(const Util::Array<CoreGraphics::MousePointer>& val)
    {
        n_assert(!this->handled);
        this->pointers = val;
    };
    const Util::Array<CoreGraphics::MousePointer>& GetPointers() const
    {
        return this->pointers;
    };
private:
    Util::Array<CoreGraphics::MousePointer> pointers;
};
//------------------------------------------------------------------------------
class SaveScreenshot : public Messaging::Message
{
    __DeclareClass(SaveScreenshot);
    __DeclareMsgId;
public:
    SaveScreenshot() 
    { };
public:
    void SetFileformat(const Util::String& val)
    {
        n_assert(!this->handled);
        this->fileformat = val;
    };
    const Util::String& GetFileformat() const
    {
        return this->fileformat;
    };
private:
    Util::String fileformat;
public:
    void SetUri(const IO::URI& val)
    {
        n_assert(!this->handled);
        this->uri = val;
    };
    const IO::URI& GetUri() const
    {
        return this->uri;
    };
private:
    IO::URI uri;
};
} // namespace Graphics
//------------------------------------------------------------------------------
