#pragma once
//------------------------------------------------------------------------------
/**
    class Lighting::ShadowServerBase
    
    The ShadowServer setups and controls the global aspects of the dynamic
    shadow system.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "internalgraphics/internalcameraentity.h"
#include "lighting/internalabstractlightentity.h"
#include "lighting/internalgloballightentity.h"
    
//------------------------------------------------------------------------------
namespace Lighting
{
class ShadowServerBase : public Core::RefCounted
{
    __DeclareClass(ShadowServerBase);
    __DeclareSingleton(ShadowServerBase);
public:
    /// constructor
    ShadowServerBase();
    /// destructor
    virtual ~ShadowServerBase();

    /// open the shadow server
    void Open();
    /// close the shadow server
    void Close();
    /// return true if shadow server is open
    bool IsOpen() const;

    /// begin shadow frame
    void BeginFrame(const Ptr<InternalGraphics::InternalCameraEntity>& cameraEntity);
    /// begin attaching visible shadow casting light sources
    void BeginAttachVisibleLights();
    /// attach a visible shadow casting light source
    void AttachVisibleLight(const Ptr<InternalAbstractLightEntity>& lightEntity);
    /// end attaching visible shadow casting light sources
    void EndAttachVisibleLights();
    /// update shadow buffer
    void UpdateShadowBuffers();
    /// end lighting frame
    void EndFrame();
    
protected:
    bool isOpen;
    bool inBeginFrame;
    bool inBeginAttach;
    Ptr<InternalGraphics::InternalCameraEntity> cameraEntity;
    Ptr<InternalGlobalLightEntity> globalLightEntity;
    Util::Array<Ptr<InternalAbstractLightEntity> > localLightEntities;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
ShadowServerBase::IsOpen() const
{
    return this->isOpen;
}

} // namespace Lighting
//------------------------------------------------------------------------------

