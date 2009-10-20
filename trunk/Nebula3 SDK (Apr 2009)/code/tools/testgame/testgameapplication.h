#pragma once
#ifndef TOOLS_TESTGAMEAPPLICATION_H
#define TOOLS_TESTGAMEAPPLICATION_H
//------------------------------------------------------------------------------
/**
    @class Tools::TestGameApplication
    
    Nebula3 Application layer test app.

    (C) 2006 Radon Labs GmbH
*/
#include "appgame/gameapplication.h"

#if __USE_PHYSICS__
#include "physicsfeature/physicsfeatureunit.h"
#include "stateobjectfeature/stateobjectfeatureunit.h"
#endif

//------------------------------------------------------------------------------
namespace Tools
{
class TestGameApplication : public App::GameApplication
{
    __DeclareSingleton(TestGameApplication);   
public:
    /// constructor
    TestGameApplication();
    /// destructor
    virtual ~TestGameApplication();
    
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
