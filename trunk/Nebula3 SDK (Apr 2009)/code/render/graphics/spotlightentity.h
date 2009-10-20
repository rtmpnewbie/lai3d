#pragma once
//------------------------------------------------------------------------------
/**
    @class Graphics::SpotLightEntity
    
    Client-side proxy of a InternalGraphics::InternalSpotLightEntity.
    
    (C) 2008 Radon Labs GmbH
*/
#include "graphics/abstractlightentity.h"

//------------------------------------------------------------------------------
namespace Graphics
{
class SpotLightEntity : public AbstractLightEntity
{
    __DeclareClass(SpotLightEntity);
public:
    /// constructor
    SpotLightEntity();

private:
    /// called by stage when entity should setup itself
    virtual void Setup(const Ptr<Stage>& stage);
};

} // namespace Graphics
//------------------------------------------------------------------------------
