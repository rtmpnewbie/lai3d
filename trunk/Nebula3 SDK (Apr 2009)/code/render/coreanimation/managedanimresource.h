#pragma once
#ifndef COREANIMATION_MANAGEDANIMRESOURCE_H
#define COREANIMATION_MANAGEDANIMRESOURCE_H
//------------------------------------------------------------------------------
/**
    @class CoreAnimation::ManagedAnimation
    
    Managed wrapper for AnimResource class.
    
    (C) 2008 Radon Labs GmbH
*/
#include "resources/managedresource.h"
#include "coreanimation/animresource.h"

//------------------------------------------------------------------------------
namespace CoreAnimation
{
class ManagedAnimResource : public Resources::ManagedResource
{
    __DeclareClass(ManagedAnimResource);
public:
    /// get contained anim resource
    const Ptr<AnimResource>& GetAnimResource() const;
};

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<AnimResource>&
ManagedAnimResource::GetAnimResource() const
{
    return this->GetResource().downcast<AnimResource>();
}

} // namespace CoreAnimation
//------------------------------------------------------------------------------
#endif
    