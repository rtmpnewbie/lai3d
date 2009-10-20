#pragma once
#ifndef INTERNALGRAPHICS_INTERNALCAMERAENTITY_H
#define INTERNALGRAPHICS_INTERNALCAMERAENTITY_H
//------------------------------------------------------------------------------
/**
    @class InternalGraphics::InternalCameraEntity
  
    Represents a camera attached to a graphics stage. Any number of
    cameras can be attached to a stage.
    
    (C) 2007 Radon Labs GmbH
*/    
#include "internalgraphics/internalgraphicsentity.h"

//------------------------------------------------------------------------------
namespace InternalGraphics
{
class InternalView;

class InternalCameraEntity : public InternalGraphicsEntity
{
    __DeclareClass(InternalCameraEntity);
public:
    /// constructor
    InternalCameraEntity();
    /// destructor
    virtual ~InternalCameraEntity();
    
    /// return true if camera is attached to a View
    bool IsAttachedToView() const;
    /// compute clip status against bounding box
    virtual Math::ClipStatus::Type ComputeClipStatus(const Math::bbox& box);

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
    friend class InternalView;

    /// called before entity is destroyed
    virtual void OnDeactivate();
    /// called by View when camera is attached to that view
    void OnAttachToView(const Ptr<InternalView>& view);
    /// called by View when camera becomes detached from view
    void OnRemoveFromView(const Ptr<InternalView>& view);
    /// called when transform matrix changed
    virtual void OnTransformChanged();
    /// update the view projection matrix
    void UpdateViewProjMatrix();

    Ptr<InternalView> view;

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
};

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
InternalCameraEntity::GetProjTransform() const
{
    return this->projMatrix;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
InternalCameraEntity::GetViewTransform() const
{
    return this->viewMatrix;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
InternalCameraEntity::GetViewProjTransform()
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
InternalCameraEntity::IsPerspective() const
{
    return this->isPersp;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
InternalCameraEntity::IsOrthogonal() const
{
    return !this->isPersp;
}

//------------------------------------------------------------------------------
/**
*/
inline float
InternalCameraEntity::GetZNear() const
{
    return this->zNear;
}

//------------------------------------------------------------------------------
/**
*/
inline float
InternalCameraEntity::GetZFar() const
{
    return this->zFar;
}

//------------------------------------------------------------------------------
/**
*/
inline float
InternalCameraEntity::GetFov() const
{
    return this->fov;
}

//------------------------------------------------------------------------------
/**
*/
inline float
InternalCameraEntity::GetAspect() const
{
    return this->aspect;
}

//------------------------------------------------------------------------------
/**
*/
inline float
InternalCameraEntity::GetNearWidth() const
{
    return this->nearWidth;
}

//------------------------------------------------------------------------------
/**
*/
inline float
InternalCameraEntity::GetNearHeight() const
{
    return this->nearHeight;
}

//------------------------------------------------------------------------------
/**
*/
inline float
InternalCameraEntity::GetFarWidth() const
{
    return this->farWidth;
}

//------------------------------------------------------------------------------
/**
*/
inline float
InternalCameraEntity::GetFarHeight() const
{
    return this->farHeight;
}

} // namespace InternalGraphics
//------------------------------------------------------------------------------
#endif
