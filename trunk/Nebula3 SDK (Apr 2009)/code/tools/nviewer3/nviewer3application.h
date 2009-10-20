#pragma once
//------------------------------------------------------------------------------
/**
    @class Tools::TestViewer
    
    Nebula3 test viewer app.

    (C) 2006 Radon Labs GmbH
*/
#include "apprender/viewerapplication.h"
#include "graphics/globallightentity.h"

//------------------------------------------------------------------------------
namespace Tools
{
class NViewer3Application : public App::ViewerApplication
{
public:
    /// constructor
    NViewer3Application();
    /// destructor
    virtual ~NViewer3Application();
    /// open the application
    virtual bool Open();
    /// close the application
    virtual void Close();

private:

    Ptr<Graphics::GlobalLightEntity> globalLight;
};

} // namespace Test
//------------------------------------------------------------------------------
