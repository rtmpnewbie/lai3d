#pragma once
//------------------------------------------------------------------------------
/**
    @class Lighting::InternalAbstractLightEntity
    
    Base class for light sources. Light sources do not directly influence
    the render pipeline (like manipulating shader variables, etc...). This
    will be handled by the LightServer and ShadowServer singletons which
    may implement platform specific lighting models.
    
    (C) 2007 Radon Labs GmbH
*/
#include "internalgraphics/internalgraphicsentity.h"
#include "lighting/lighttype.h"

//------------------------------------------------------------------------------
namespace Lighting
{
class InternalAbstractLightEntity : public InternalGraphics::InternalGraphicsEntity
{
    __DeclareClass(InternalAbstractLightEntity);
public:
    /// constructor
    InternalAbstractLightEntity();
    /// destructor
    virtual ~InternalAbstractLightEntity();

    /// get the light type
    LightType::Code GetLightType() const;
    /// set primary light color
    void SetColor(const Math::float4& c);
    /// get primary light color
    const Math::float4& GetColor() const;
    /// enable/disable shadow casting
    void SetCastShadows(bool b);
    /// get shadow casting flag
    bool GetCastShadows() const;   
    /// set projection map UV offset and scale (xy->offset, zw->scale)
    void SetProjMapUvOffsetAndScale(const Math::float4& v);
    /// get projection map UV offset and scale
    const Math::float4& GetProjMapUvOffsetAndScale() const;

    /// get inverse transform (transforms from world to light space)
    const Math::matrix44& GetInvTransform() const;
    /// get light-projection matrix (transforms from light space to light projection space)
    const Math::matrix44& GetProjTransform() const;
    /// get world-to-light-projection transform (transform from world to light projection space)
    const Math::matrix44& GetInvLightProjTransform() const;

    /// set shadow buffer uv rectangle (optionally set by light/shadow servers)
    void SetShadowBufferUvOffsetAndScale(const Math::float4& uvOffset);
    /// get shadow buffer uv rectangle
    const Math::float4& GetShadowBufferUvOffsetAndScale() const;    
   
protected:
    /// called when transform matrix changed
    virtual void OnTransformChanged();
    /// set the light type (must be called from sub-classes constructor
    void SetLightType(LightType::Code c);

    LightType::Code lightType;
    Math::matrix44 invTransform;
    Math::matrix44 projTransform;
    Math::matrix44 invLightProjTransform;
    Math::float4 color;
    Math::float4 projMapUvOffsetAndScale;
    Math::float4 shadowBufferUvOffsetAndScale;
    bool castShadows;
};

//------------------------------------------------------------------------------
/**
*/
inline void
InternalAbstractLightEntity::SetLightType(LightType::Code c)
{
    this->lightType = c;
}

//------------------------------------------------------------------------------
/**
*/
inline LightType::Code
InternalAbstractLightEntity::GetLightType() const
{
    return this->lightType;
}

//------------------------------------------------------------------------------
/**
*/
inline void
InternalAbstractLightEntity::SetColor(const Math::float4& c)
{
    this->color = c;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::float4&
InternalAbstractLightEntity::GetColor() const
{
    return this->color;
}

//------------------------------------------------------------------------------
/**
*/
inline void
InternalAbstractLightEntity::SetProjMapUvOffsetAndScale(const Math::float4& v)
{
    this->projMapUvOffsetAndScale = v;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::float4&
InternalAbstractLightEntity::GetProjMapUvOffsetAndScale() const
{
    return this->projMapUvOffsetAndScale;
}

//------------------------------------------------------------------------------
/**
*/
inline void
InternalAbstractLightEntity::SetShadowBufferUvOffsetAndScale(const Math::float4& v)
{
    this->shadowBufferUvOffsetAndScale = v;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::float4&
InternalAbstractLightEntity::GetShadowBufferUvOffsetAndScale() const
{
    return this->shadowBufferUvOffsetAndScale;
}

//------------------------------------------------------------------------------
/**
*/
inline void
InternalAbstractLightEntity::SetCastShadows(bool b)
{
    this->castShadows = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
InternalAbstractLightEntity::GetCastShadows() const
{
    return this->castShadows;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
InternalAbstractLightEntity::GetInvTransform() const
{
    return this->invTransform;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
InternalAbstractLightEntity::GetProjTransform() const
{
    return this->projTransform;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
InternalAbstractLightEntity::GetInvLightProjTransform() const
{
    return this->invLightProjTransform;
}

} // namespace Lighting
//------------------------------------------------------------------------------

    