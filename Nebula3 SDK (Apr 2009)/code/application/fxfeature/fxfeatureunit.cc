//------------------------------------------------------------------------------
//  FxFeature/FxFeatureUnit.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "fxfeature/fxfeatureunit.h"

namespace FxFeature
{
__ImplementClass(FxFeature::FxFeatureUnit, 'FXFU' , Game::FeatureUnit);
__ImplementSingleton(FxFeature::FxFeatureUnit);

//------------------------------------------------------------------------------
/**
*/
FxFeatureUnit::FxFeatureUnit()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
FxFeatureUnit::~FxFeatureUnit()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
FxFeatureUnit::OnActivate()
{
    FeatureUnit::OnActivate();

    // create fxmanager and enable camera shake mixer
    this->fxManager = FxManager::Create();
    this->fxManager->Open();
    this->fxManager->SetupCameraShakeMixer(Math::vector(0.1f, 0.1f, 0.0f), Math::vector(0.0f, 0.0f, 0.0f));
}

//------------------------------------------------------------------------------
/**
*/
void
FxFeatureUnit::OnDeactivate()
{
    this->fxManager->Close();
    this->fxManager = 0;

    FeatureUnit::OnDeactivate();    
}

//------------------------------------------------------------------------------
/**
*/
void
FxFeatureUnit::OnLoad()
{
    //empty
}

//------------------------------------------------------------------------------
/**
*/
void
FxFeatureUnit::OnBeginFrame()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
FxFeatureUnit::OnEndFrame()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
FxFeatureUnit::OnFrame()
{
    // empty
}
};