#pragma once
#ifndef WIN360_D3D9TRANSFORMDEVICE_H
#define WIN360_D3D9TRANSFORMDEVICE_H
//------------------------------------------------------------------------------
/**
    @class Win360::D3D9TransformDevice
    
    D3D9/Xbox360 version of TransformDevice.
    
    (C) 2007 Radon Labs GmbH
*/
#include "coregraphics/base/transformdevicebase.h"
#include "coregraphics/shadervariable.h"
#include "core/singleton.h"
#include "math/matrix44.h"

//------------------------------------------------------------------------------
namespace Win360
{
class D3D9TransformDevice : public Base::TransformDeviceBase
{
    __DeclareClass(D3D9TransformDevice);
    __DeclareSingleton(D3D9TransformDevice);
public:
    /// constructor
    D3D9TransformDevice();
    /// destructor
    virtual ~D3D9TransformDevice();

    /// open the transform device
    virtual bool Open();
    /// close the transform device
    virtual void Close();

    /// updates shared shader variables dependent on view matrix
    virtual void ApplyViewSettings();
    /// apply current modelviewproj and model matrix in shader
    virtual void ApplyModelTransforms();

private:
    /// shared shader variables
    Ptr<CoreGraphics::ShaderVariable> shdEyePos;
    Ptr<CoreGraphics::ShaderVariable> viewMatrix;
    Ptr<CoreGraphics::ShaderVariable> invViewMatrix;
    Ptr<CoreGraphics::ShaderVariable> displayResolution;
    Ptr<CoreGraphics::ShaderVariable> halfPixelSize;

    CoreGraphics::ShaderVariable::Semantic mvpSemantic;
    CoreGraphics::ShaderVariable::Semantic modelSemantic;
    CoreGraphics::ShaderVariable::Semantic viewSemantic;
    CoreGraphics::ShaderVariable::Semantic invViewSemantic;
    CoreGraphics::ShaderVariable::Semantic viewProjSemantic;
    CoreGraphics::ShaderVariable::Semantic eyePosSemantic;
    CoreGraphics::ShaderVariable::Semantic displayResolutionSemantic;
    CoreGraphics::ShaderVariable::Semantic halfPixelSizeSemantic;
};


} // namespace Win360
//------------------------------------------------------------------------------
#endif
    