#pragma once
#ifndef TOOLS_TESTGAMEAPPLICATION_H
#define TOOLS_TESTGAMEAPPLICATION_H
//------------------------------------------------------------------------------
/**
    @class Tools::TestNetGameApplication
    
    Nebula3 Application layer test app.

    (C) 2006 Radon Labs GmbH
*/
#include "appgame/gameapplication.h"

#if __USE_PHYSICS__
#include "physicsfeature/physicsfeatureunit.h"
#endif

//------------------------------------------------------------------------------
namespace Tools
{
class TestNetGameApplication : public App::GameApplication
{
    DeclareSingleton(TestNetGameApplication);   
public:
    /// constructor
    TestNetGameApplication();
    /// destructor
    virtual ~TestNetGameApplication();
    
protected:
    /// setup application state handlers
    virtual void SetupStateHandlers();
    /// setup game features
    virtual void SetupGameFeatures();
    /// cleanup game features
    virtual void CleanupGameFeatures(); 

private:
#if __USE_PHYSICS__
    Ptr<PhysicsFeature::PhysicsFeatureUnit> physicsFeature;
#endif    
};

} // namespace Test
//------------------------------------------------------------------------------
#endif
