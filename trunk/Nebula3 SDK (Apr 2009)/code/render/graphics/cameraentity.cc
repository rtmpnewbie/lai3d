//------------------------------------------------------------------------------
//  cameraentity.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/cameraentity.h"
#include "graphics/display.h"
#include "graphics/stage.h"

namespace Graphics
{
__ImplementClass(Graphics::CameraEntity, 'GCET', Graphics::GraphicsEntity);

using namespace Util;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
CameraEntity::CameraEntity() :
    viewProjDirty(true)
{
    this->SetType(GraphicsEntityType::Camera);
    float aspectRatio = Display::Instance()->GetDisplayMode().GetAspectRatio();
    this->SetupPerspectiveFov(n_deg2rad(60.0f), aspectRatio, 0.1f, 2500.0f);
    this->viewMatrix = matrix44::identity();
    this->viewProjMatrix = matrix44::identity();
}

//------------------------------------------------------------------------------
/**
*/
CameraEntity::~CameraEntity()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    Setup the server-side camera entity.
*/
void
CameraEntity::Setup(const Ptr<Stage>& stage_)
{
    GraphicsEntity::Setup(stage_);

    // send off an entity creation message
    Ptr<CreateCameraEntity> msg = CreateCameraEntity::Create();
    msg->SetStageName(this->stage->GetName());
    msg->SetTransform(this->transform);
    msg->SetVisible(this->isVisible);
    msg->SetIsPerspective(this->isPersp);
    msg->SetNearPlane(this->zNear);
    msg->SetFarPlane(this->zFar);
    if (this->isPersp)
    {
        msg->SetPerspFieldOfView(this->fov);
        msg->SetPerspAspectRatio(this->aspect);
    }
    else
    {
        msg->SetOrthoWidth(this->nearWidth);
        msg->SetOrthoHeight(this->nearHeight);
    }
    this->SendCreateMsg(msg.cast<CreateGraphicsEntity>());
}

//------------------------------------------------------------------------------
/**
    Property remove from view if we are attached to a view.
*/
void
CameraEntity::Discard()
{
    if (this->IsAttachedToView())
    {
        this->view->SetCameraEntity(0);
    }
    // call parent
    GraphicsEntity::Discard();
}

//------------------------------------------------------------------------------
/**
    Setup camera as perspective projection.
    NOTE: this method is a copy of CameraEntity::SetupPerspectiveFov()!
*/
void
CameraEntity::SetupPerspectiveFov(float fov_, float aspect_, float zNear_, float zFar_)
{
    this->isPersp = true;
    this->zNear   = zNear_;
    this->zFar    = zFar_;
    this->fov     = fov_;
    this->aspect  = aspect_;

    this->projMatrix = matrix44::perspfovrh(this->fov, this->aspect, this->zNear, this->zFar);

    this->nearWidth  = 2.0f * this->zNear / this->projMatrix.getrow0().x();
    this->nearHeight = 2.0f * this->zNear / this->projMatrix.getrow1().y();
    this->farWidth   = (this->nearWidth / this->zNear) * this->zFar;
    this->farHeight  = (this->nearHeight / this->zNear) * this->zFar;

    this->viewProjDirty = true;
}

//------------------------------------------------------------------------------
/**
    Setup camera as orthogonal projection.
    NOTE: this method is a copy of CameraEntity::SetupOrthogonal()!
*/
void
CameraEntity::SetupOrthogonal(float w, float h, float zNear_, float zFar_)
{
    this->isPersp    = false;
    this->zNear      = zNear_;
    this->zFar       = zFar_;
    this->fov        = 0.0f;
    this->aspect     = w / h;
    this->nearWidth  = w;
    this->nearHeight = h;
    this->farWidth   = w;
    this->farHeight  = h;

    this->projMatrix = matrix44::orthorh(w, h, this->zNear, this->zFar);

    this->viewProjDirty = true;
}

//------------------------------------------------------------------------------
/**
    We need to keep track of modifications of the transformation matrix.
*/
void
CameraEntity::OnTransformChanged()
{
    this->viewMatrix = matrix44::inverse(this->GetTransform());
    this->viewProjDirty = true;
    GraphicsEntity::OnTransformChanged();
}

//------------------------------------------------------------------------------
/**
*/
void
CameraEntity::OnAttachToView(const Ptr<View>& view_)
{
    this->view = view_;
    Ptr<AttachCameraToView> msg = AttachCameraToView::Create();
    msg->SetViewName(this->view->GetName());
    this->SendMsg(msg.cast<GraphicsEntityMessage>());
}

//------------------------------------------------------------------------------
/**
*/
void
CameraEntity::OnRemoveFromView(const Ptr<View>& view_)
{
    n_assert(this->view == view_);
    Ptr<RemoveCameraFromView> msg = RemoveCameraFromView::Create();
    msg->SetViewName(this->view->GetName());
    this->SendMsg(msg.cast<GraphicsEntityMessage>());
    this->view = 0;
}

//------------------------------------------------------------------------------
/**
    Updates the view-projection matrix.
*/
void
CameraEntity::UpdateViewProjMatrix()
{
    n_assert(this->viewProjDirty);
    this->viewProjDirty = false;
    this->viewProjMatrix = matrix44::multiply(this->viewMatrix, this->projMatrix);
}

} // namespace Graphics
