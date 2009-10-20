//------------------------------------------------------------------------------
//  NViewer3Application.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tools/nviewer3/nviewer3application.h"

namespace Tools
{
using namespace Graphics;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
NViewer3Application::NViewer3Application()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
NViewer3Application::~NViewer3Application()
{
    if (this->IsOpen())
    {
        this->Close();
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
NViewer3Application::Open()
{
    n_assert(!this->IsOpen());
    if (ViewerApplication::Open())
    {
        // setup lights
        matrix44 lightTransform = matrix44::rotationx(n_deg2rad(-45.0f));
        this->globalLight = GlobalLightEntity::Create();
        this->globalLight->SetTransform(lightTransform);
        this->globalLight->SetColor(float4(1.0f, 1.0f, 1.0f, 1.0f));
        this->globalLight->SetAmbientLightColor(float4(0.0f, 0.0f, 0.0f, 1.0f));
        this->globalLight->SetBackLightColor(float4(0.1f, 0.1f, 0.1f, 1.0f));
        this->globalLight->SetCastShadows(false);
        this->stage->AttachEntity(this->globalLight.cast<GraphicsEntity>());
        
        // wait for resources to be loaded
        GraphicsInterface::Instance()->WaitForPendingResources();

        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
NViewer3Application::Close()
{
    this->stage->RemoveEntity(this->globalLight.cast<GraphicsEntity>());
    this->globalLight = 0;

    ViewerApplication::Close();
}

} // namespace Tools
