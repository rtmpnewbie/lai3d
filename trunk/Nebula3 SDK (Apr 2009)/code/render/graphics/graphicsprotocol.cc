//------------------------------------------------------------------------------
//  MACHINE GENERATED, DON'T EDIT!
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphicsprotocol.h"

namespace Graphics
{
    __ImplementClass(Graphics::RegisterRTPlugin, 'rrtp', Messaging::Message);
    __ImplementMsgId(RegisterRTPlugin);
    __ImplementClass(Graphics::UnregisterRTPlugin, 'urtp', Messaging::Message);
    __ImplementMsgId(UnregisterRTPlugin);
    __ImplementClass(Graphics::SetupGraphics, 'supg', Messaging::Message);
    __ImplementMsgId(SetupGraphics);
    __ImplementClass(Graphics::AdapterExists, 'aext', Messaging::Message);
    __ImplementMsgId(AdapterExists);
    __ImplementClass(Graphics::GetAvailableDisplayModes, 'gadm', Messaging::Message);
    __ImplementMsgId(GetAvailableDisplayModes);
    __ImplementClass(Graphics::SupportsDisplayMode, 'spdm', Messaging::Message);
    __ImplementMsgId(SupportsDisplayMode);
    __ImplementClass(Graphics::GetCurrentAdapterDisplayMode, 'gcdm', Messaging::Message);
    __ImplementMsgId(GetCurrentAdapterDisplayMode);
    __ImplementClass(Graphics::GetAdapterInfo, 'gadi', Messaging::Message);
    __ImplementMsgId(GetAdapterInfo);
    __ImplementClass(Graphics::AttachDisplayEventHandler, 'adeh', Messaging::Message);
    __ImplementMsgId(AttachDisplayEventHandler);
    __ImplementClass(Graphics::RemoveDisplayEventHandler, 'rdeh', Messaging::Message);
    __ImplementMsgId(RemoveDisplayEventHandler);
    __ImplementClass(Graphics::AttachRenderEventHandler, 'areh', Messaging::Message);
    __ImplementMsgId(AttachRenderEventHandler);
    __ImplementClass(Graphics::RemoveRenderEventHandler, 'rreh', Messaging::Message);
    __ImplementMsgId(RemoveRenderEventHandler);
    __ImplementClass(Graphics::CreateGraphicsStage, 'cgst', Messaging::Message);
    __ImplementMsgId(CreateGraphicsStage);
    __ImplementClass(Graphics::DiscardGraphicsStage, 'digs', Messaging::Message);
    __ImplementMsgId(DiscardGraphicsStage);
    __ImplementClass(Graphics::CreateGraphicsView, 'cgvw', Messaging::Message);
    __ImplementMsgId(CreateGraphicsView);
    __ImplementClass(Graphics::DiscardGraphicsView, 'dgvw', Messaging::Message);
    __ImplementMsgId(DiscardGraphicsView);
    __ImplementClass(Graphics::CreateGraphicsEntity, 'cget', Messaging::Message);
    __ImplementMsgId(CreateGraphicsEntity);
    __ImplementClass(Graphics::DiscardGraphicsEntity, 'dcge', Messaging::Message);
    __ImplementMsgId(DiscardGraphicsEntity);
    __ImplementClass(Graphics::CreateModelEntity, 'cmde', CreateGraphicsEntity);
    __ImplementMsgId(CreateModelEntity);
    __ImplementClass(Graphics::CreateCameraEntity, 'cpce', CreateGraphicsEntity);
    __ImplementMsgId(CreateCameraEntity);
    __ImplementClass(Graphics::CreateGlobalLightEntity, 'cgle', CreateGraphicsEntity);
    __ImplementMsgId(CreateGlobalLightEntity);
    __ImplementClass(Graphics::CreateSpotLightEntity, 'csle', CreateGraphicsEntity);
    __ImplementMsgId(CreateSpotLightEntity);
    __ImplementClass(Graphics::CreatePointLightEntity, 'cple', CreateGraphicsEntity);
    __ImplementMsgId(CreatePointLightEntity);
    __ImplementClass(Graphics::GraphicsEntityMessage, 'gemg', Messaging::Message);
    __ImplementMsgId(GraphicsEntityMessage);
    __ImplementClass(Graphics::SetLightColor, 'slcl', GraphicsEntityMessage);
    __ImplementMsgId(SetLightColor);
    __ImplementClass(Graphics::SetLightCastShadows, 'slcs', GraphicsEntityMessage);
    __ImplementMsgId(SetLightCastShadows);
    __ImplementClass(Graphics::SetGlobalBackLightColor, 'sgbl', GraphicsEntityMessage);
    __ImplementMsgId(SetGlobalBackLightColor);
    __ImplementClass(Graphics::SetGlobalAmbientLightColor, 'sgal', GraphicsEntityMessage);
    __ImplementMsgId(SetGlobalAmbientLightColor);
    __ImplementClass(Graphics::SetGlobalBackLightOffset, 'sgbo', GraphicsEntityMessage);
    __ImplementMsgId(SetGlobalBackLightOffset);
    __ImplementClass(Graphics::SetGlobalLightParams, 'sglp', GraphicsEntityMessage);
    __ImplementMsgId(SetGlobalLightParams);
    __ImplementClass(Graphics::SetGlobalLightColor, 'sglc', GraphicsEntityMessage);
    __ImplementMsgId(SetGlobalLightColor);
    __ImplementClass(Graphics::UpdateTransform, 'sttr', GraphicsEntityMessage);
    __ImplementMsgId(UpdateTransform);
    __ImplementClass(Graphics::SetVisibility, 'svsb', GraphicsEntityMessage);
    __ImplementMsgId(SetVisibility);
    __ImplementClass(Graphics::AttachCameraToView, 'actv', GraphicsEntityMessage);
    __ImplementMsgId(AttachCameraToView);
    __ImplementClass(Graphics::RemoveCameraFromView, 'rcfv', GraphicsEntityMessage);
    __ImplementMsgId(RemoveCameraFromView);
    __ImplementClass(Graphics::UpdateInstanceShaderVariable, 'uisv', GraphicsEntityMessage);
    __ImplementMsgId(UpdateInstanceShaderVariable);
    __ImplementClass(Graphics::AnimPlayOnce, 'anpo', GraphicsEntityMessage);
    __ImplementMsgId(AnimPlayOnce);
    __ImplementClass(Graphics::AnimPlayLoop, 'anpl', GraphicsEntityMessage);
    __ImplementMsgId(AnimPlayLoop);
    __ImplementClass(Graphics::AnimPlayDuration, 'anpd', GraphicsEntityMessage);
    __ImplementMsgId(AnimPlayDuration);
    __ImplementClass(Graphics::AnimModifyBlendWeight, 'anmb', GraphicsEntityMessage);
    __ImplementMsgId(AnimModifyBlendWeight);
    __ImplementClass(Graphics::AnimModifyTimeFactor, 'anmt', GraphicsEntityMessage);
    __ImplementMsgId(AnimModifyTimeFactor);
    __ImplementClass(Graphics::AnimStopClip, 'ansn', GraphicsEntityMessage);
    __ImplementMsgId(AnimStopClip);
    __ImplementClass(Graphics::AnimStopCategory, 'ansc', GraphicsEntityMessage);
    __ImplementMsgId(AnimStopCategory);
    __ImplementClass(Graphics::AnimStopAll, 'ansa', GraphicsEntityMessage);
    __ImplementMsgId(AnimStopAll);
    __ImplementClass(Graphics::AnimGetClipDuration, 'agcd', GraphicsEntityMessage);
    __ImplementMsgId(AnimGetClipDuration);
    __ImplementClass(Graphics::ApplySkinList, 'askl', GraphicsEntityMessage);
    __ImplementMsgId(ApplySkinList);
    __ImplementClass(Graphics::ShowSkin, 'swsk', GraphicsEntityMessage);
    __ImplementMsgId(ShowSkin);
    __ImplementClass(Graphics::HideSkin, 'hdsk', GraphicsEntityMessage);
    __ImplementMsgId(HideSkin);
    __ImplementClass(Graphics::SetupGrassFromStage, 'sgfs', Messaging::Message);
    __ImplementMsgId(SetupGrassFromStage);
    __ImplementClass(Graphics::SetTimeFactor, 'stif', Messaging::Message);
    __ImplementMsgId(SetTimeFactor);
    __ImplementClass(Graphics::UnbindAttachment, 'clat', Messaging::Message);
    __ImplementMsgId(UnbindAttachment);
    __ImplementClass(Graphics::BindAttachment, 'aten', UnbindAttachment);
    __ImplementMsgId(BindAttachment);
    __ImplementClass(Graphics::GetAnimationDrivenVector, 'gadp', GraphicsEntityMessage);
    __ImplementMsgId(GetAnimationDrivenVector);
    __ImplementClass(Graphics::SetAnimationDrivenFlag, 'sand', GraphicsEntityMessage);
    __ImplementMsgId(SetAnimationDrivenFlag);
    __ImplementClass(Graphics::GetActiveAnimEvents, 'gaae', GraphicsEntityMessage);
    __ImplementMsgId(GetActiveAnimEvents);
    __ImplementClass(Graphics::GetMaximumKeyDuration, 'gmkd', GraphicsEntityMessage);
    __ImplementMsgId(GetMaximumKeyDuration);
    __ImplementClass(Graphics::GetJointTransform, 'gjtr', GraphicsEntityMessage);
    __ImplementMsgId(GetJointTransform);
    __ImplementClass(Graphics::RegisterAnimEventHandler, 'raeh', Messaging::Message);
    __ImplementMsgId(RegisterAnimEventHandler);
    __ImplementClass(Graphics::UnregisterAnimEventHandler, 'ureh', Messaging::Message);
    __ImplementMsgId(UnregisterAnimEventHandler);
    __ImplementClass(Graphics::PreloadMousePointerTextures, 'plpt', Messaging::Message);
    __ImplementMsgId(PreloadMousePointerTextures);
    __ImplementClass(Graphics::UpdateMousePointers, 'unpt', Messaging::Message);
    __ImplementMsgId(UpdateMousePointers);
    __ImplementClass(Graphics::SaveScreenshot, 'scre', Messaging::Message);
    __ImplementMsgId(SaveScreenshot);
} // Graphics

namespace Commands
{
} // namespace Commands
//------------------------------------------------------------------------------
