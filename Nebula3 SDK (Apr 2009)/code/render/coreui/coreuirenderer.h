#pragma once
#ifndef COREGRAPHICS_UIRENDERER_H
#define COREGRAPHICS_UIRENDERER_H
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::UIRenderer
    
    Render ui shapes for visualizations.
    
    (C) 2008 Radon Labs GmbH
*/
#if (__WIN32__ )
#include "coreui/win360/win360coreuirenderer.h"
namespace CoreUI
{
class CoreUIRenderer : public Win32::Win360CoreUIRenderer
{
    __DeclareClass(CoreUIRenderer);
    __DeclareSingleton(CoreUIRenderer);
public:
    /// constructor
    CoreUIRenderer();
    /// destructor
    virtual ~CoreUIRenderer();        
};
} // namespace CoreUI

#elif (__XBOX360__)
#include "coreui/win360/win360coreuirenderer.h"
namespace CoreUI
{
class CoreUIRenderer : public Win32::Win360CoreUIRenderer
{
    __DeclareClass(CoreUIRenderer);
    __DeclareSingleton(CoreUIRenderer);
public:
    /// constructor
    CoreUIRenderer();
    /// destructor
    virtual ~CoreUIRenderer();        
};
} // namespace CoreUI
#else
#error "CoreUIRenderer class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
