#pragma once
#ifndef RESOURCES_MANAGEDTEXTURE_H
#define RESOURCES_MANAGEDTEXTURE_H
//------------------------------------------------------------------------------
/**
    @class Resources::ManagedTexture
    
    A specialized managed resource for texture resources.
    
    (C) 2007 Radon Labs GmbH
*/
#include "resources/managedresource.h"
#include "coregraphics/texture.h"

//------------------------------------------------------------------------------
namespace Resources
{
class ManagedTexture : public ManagedResource
{
    __DeclareClass(ManagedTexture);
public:
    /// get contained texture resource
    const Ptr<CoreGraphics::Texture>& GetTexture() const;
};

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::Texture>&
ManagedTexture::GetTexture() const
{
    return this->GetResource().downcast<CoreGraphics::Texture>();
}

} // namespace Resources
//------------------------------------------------------------------------------
#endif
    