//------------------------------------------------------------------------------
//  d3d9transformdevice.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/win360/d3d9transformdevice.h"
#include "coregraphics/shaderserver.h"
#include "coregraphics/displaydevice.h"

namespace Win360
{
__ImplementClass(Win360::D3D9TransformDevice, 'D9TD', Base::TransformDeviceBase);
__ImplementSingleton(Win360::D3D9TransformDevice);

using namespace Util;
using namespace Math;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
D3D9TransformDevice::D3D9TransformDevice() :
    mvpSemantic("ModelViewProjection"),
    modelSemantic("Model"),
    viewSemantic("View"),
    invViewSemantic("InvView"),
    viewProjSemantic("ViewProjection"),
    eyePosSemantic("EyePos"),
    displayResolutionSemantic("DisplayResolution"),
    halfPixelSizeSemantic("HalfPixelSize")
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
D3D9TransformDevice::~D3D9TransformDevice()
{
    // empty
}


//------------------------------------------------------------------------------
/**
*/
bool
D3D9TransformDevice::Open()
{
    ShaderServer* shdServer = ShaderServer::Instance();
    this->shdEyePos = shdServer->GetSharedVariableBySemantic(this->eyePosSemantic);
    this->viewMatrix = shdServer->GetSharedVariableBySemantic(this->viewSemantic);
    this->invViewMatrix = shdServer->GetSharedVariableBySemantic(this->invViewSemantic);
    this->displayResolution = shdServer->GetSharedVariableBySemantic(this->displayResolutionSemantic);
    this->halfPixelSize = shdServer->GetSharedVariableBySemantic(this->halfPixelSizeSemantic);
    return TransformDeviceBase::Open();
}

//------------------------------------------------------------------------------
/**
*/
void
D3D9TransformDevice::Close()
{
    this->shdEyePos = 0;
    this->viewMatrix = 0;
    TransformDeviceBase::Close();
}

//------------------------------------------------------------------------------
/**
*/
void
D3D9TransformDevice::ApplyViewSettings()
{
    // update shared shader variable eye pos from current view matrix
    this->shdEyePos->SetFloat4(this->GetInvViewTransform().getrow3());
    
    // apply view matrix to shader
    this->viewMatrix->SetMatrix(this->GetViewTransform());   
    this->invViewMatrix->SetMatrix(this->GetInvViewTransform());

    // @todo: NEED TO SET FROM RENDER TARGET RESOLUTION?
    // set display resolution variable
    DisplayMode mode = DisplayDevice::Instance()->GetDisplayMode();
    this->displayResolution->SetFloat4(Math::float4((float)mode.GetWidth(), (float)mode.GetHeight(), mode.GetAspectRatio(), 0.0f));

    // @todo: NEED TO SET FROM RENDER TARGET RESOLUTION?
    // set half pixel size for lookup in pixelshaders into screen mapped textures
    float xHalfSize = 0.5f / mode.GetWidth();
    float yHalfSize = 0.5f / mode.GetHeight();
    this->halfPixelSize->SetFloat4(Math::float4(xHalfSize, yHalfSize,0,0));
}

//------------------------------------------------------------------------------
/**
*/
void
D3D9TransformDevice::ApplyModelTransforms()
{    
    const Ptr<ShaderInstance>& shaderInst = ShaderServer::Instance()->GetActiveShaderInstance();    
    
    // apply ModelViewProjection 
    n_assert(shaderInst->HasVariableBySemantic(this->mvpSemantic));
    const Ptr<ShaderVariable>& modelViewProjMatrix = shaderInst->GetVariableBySemantic(mvpSemantic);       
    modelViewProjMatrix->SetMatrix(this->GetModelViewProjTransform());

    // apply optional ViewProjection
    if (shaderInst->HasVariableBySemantic(this->viewProjSemantic))
    {
        const Ptr<ShaderVariable>& viewProj = shaderInst->GetVariableBySemantic(this->viewProjSemantic);
        viewProj->SetMatrix(this->GetViewProjTransform());
    }

    // apply optional Model matrix to shader
    if (shaderInst->HasVariableBySemantic(this->modelSemantic))
    {
        const Ptr<ShaderVariable>& model = shaderInst->GetVariableBySemantic(this->modelSemantic);
        model->SetMatrix(this->GetModelTransform());
    }
}

} // namespace Win360
