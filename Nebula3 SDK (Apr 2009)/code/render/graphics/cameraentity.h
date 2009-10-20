#pragma once
//------------------------------------------------------------------------------
/**
    @class Graphics::CameraEntity
    
    Client-side proxy of a InternalGraphics::InternalCameraEntity. NOTE: all getter-methods
    of this class return client-side cached values, not the actual server-side
    values. Thus they may be off by some amount, since the render thread
    may run at a different frame rate then the client thread!
    
    (C) 2007 Radon Labs GmbH
*/
#include "graphics/graphicsentity.h"
#include "graphics/view.h"

//------------------------------------------------------------------------------
namespace Graphics
{
class CameraEntity : public GraphicsEntity
{
    __DeclareClass(CameraEntity);
public:
    /// constructor
    CameraEntity();
    /// destructor
    virtual ~CameraEntity();

    /// return true if this camera is currently attached to a view
    bool IsAttachedToView() const;
    /// get the view this camera is currently attached to
    const Ptr<View>& GetView() const;

    /// setup camera for perspective field-of-view projection transform
    void SetupPerspectiveFov(float fov, float aspect, float zNear, float zFar);
    /// setup camera for orthogonal projection transform
    void SetupOrthogonal(float w, float h, float zNear, float zFar);

    /// get projection matrix
    const Math::matrix44& GetProjTransform() const;
    /// get view transform (inverse transform)
    const Math::matrix44& GetViewTransform() const;
    /// get view projection matrix (non-const!)
    const Math::matrix44& GetViewProjTransform();

    /// return true if this is a perspective projection
    bool IsPerspective() const;
    /// return true if this is an orthogonal transform
    bool IsOrthogonal() const;
    /// get near plane distance
    float GetZNear() const;
    /// get far plane distance
    float GetZFar() const;
    /// get field-of-view (only if perspective)
    float GetFov() const;
    /// get aspect ration (only if perspective)
    float GetAspect() const;
    /// get width of near plane
    float GetNearWidth() const;
    /// get height of near plane
    float GetNearHeight() const;
    /// get width of far plane
    float GetFarWidth() const;
    /// get height of far plane
    float GetFarHeight() const;

protected:
    friend class View;

    /// called by stage when entity should setup itself
    virtual void Setup(const Ptr<Stage>& stage);
    /// called by stage when entity should discard itself
    virtual void Discard();
    /// called when transform matrix changed
    virtual void OnTransformChanged();
    /// update the view projection matrix
    void UpdateViewProjMatrix();
    /// called by view when this camera entity should become the view's camera
    void OnAttachToView(const Ptr<View>& view);
    /// called by view when this camera is no longer the view's camera
    void OnRemoveFromView(const Ptr<View>& view);    
    
    Math::matrix44 projMatrix;
    Math::matrix44 viewMatrix;
    Math::matrix44 viewProjMatrix;
    bool viewProjDirty;

    bool isPersp;
    float zNear;
    float zFar;
    float fov;
    float aspect;
    float nearWidth;
    float nearHeight;
    float farWidth;
    float farHeight;

    Ptr<View> view;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
CameraEntity::IsAttachedToView() const
{
    return this->view.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<View>&
CameraEntity::GetView() const
{
    return this->view;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
CameraEntity::GetProjTransform() const
{
    return this->projMatrix;
}

//------------------------------------------------------------------------------
/**
    NOTE: The matrix returned here may be off a little bit from view
    matrix used in the render thread because the game thread may run
    at a slower frame rate then rendering!
    It is not possible to get an exact view matrix outside of the render
    thread.
*/
inline const Math::matrix44&
CameraEntity::GetViewTransform() const
{
    return this->viewMatrix;
}

//------------------------------------------------------------------------------
/**
    NOTE: The matrix returned here may be off a little bit from viewProj
    matrix used in the render thread because the game thread may run
    at a slower frame rate then rendering!
    It is not possible to get an exact viewProj matrix outside of the render
    thread.
*/
inline const Math::matrix44&
CameraEntity::GetViewProjTransform()
{
    if (this->viewProjDirty)
    {
        this->UpdateViewProjMatrix();
    }
    return this->viewProjMatrix;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
CameraEntity::IsPerspective() const
{
    return this->isPersp;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
CameraEntity::IsOrthogonal() const
{
    return !this->isPersp;
}

//------------------------------------------------------------------------------
/**
*/
inline float
CameraEntity::GetZNear() const
{
    return this->zNear;
}

//------------------------------------------------------------------------------
/**
*/
inline float
CameraEntity::GetZFar() const
{
    return this->zFar;
}

//------------------------------------------------------------------------------
/**
*/
inline float
CameraEntity::GetFov() const
{
    return this->fov;
}

//------------------------------------------------------------------------------
/**
*/
inline float
CameraEntity::GetAspect() const
{
    return this->aspect;
}

//------------------------------------------------------------------------------
/**
*/
inline float
CameraEntity::GetNearWidth() const
{
    return this->nearWidth;
}

//------------------------------------------------------------------------------
/**
*/
inline float
CameraEntity::GetNearHeight() const
{
    return this->nearHeight;
}

//------------------------------------------------------------------------------
/**
*/
inline float
CameraEntity::GetFarWidth() const
{
    return this->farWidth;
}

//------------------------------------------------------------------------------
/**
*/
inline float
CameraEntity::GetFarHeight() const
{
    return this->farHeight;
}

} // namespace Graphics
//------------------------------------------------------------------------------
