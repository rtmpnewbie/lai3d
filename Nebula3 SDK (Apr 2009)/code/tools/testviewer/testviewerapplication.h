#pragma once
#ifndef TOOLS_TESTVIEWER_H
#define TOOLS_TESTVIEWER_H
//------------------------------------------------------------------------------
/**
    @class Tools::TestViewer
    
    Nebula3 test viewer app.

    (C) 2006 Radon Labs GmbH
*/
#include "apprender/viewerapplication.h"
#include "graphics/spotlightentity.h"
#include "graphics/modelentity.h"
#include "graphics/globallightentity.h"

//------------------------------------------------------------------------------
namespace Tools
{
class TestViewerApplication : public App::ViewerApplication
{
public:
    /// constructor
    TestViewerApplication();
    /// destructor
    virtual ~TestViewerApplication();
    /// open the application
    virtual bool Open();
    /// close the application
    virtual void Close();

private:
    /// process input (called before rendering)
    virtual void OnProcessInput();
    /// render current frame
    virtual void OnUpdateFrame();

    Ptr<Graphics::ModelEntity> ground;
    Util::Array<Ptr<Graphics::ModelEntity>> models;
    Ptr<Graphics::GlobalLightEntity> globalLight;
    Ptr<Graphics::SpotLightEntity> localLight0;

    bool renderDebug;
};

} // namespace Test
//------------------------------------------------------------------------------
#endif
