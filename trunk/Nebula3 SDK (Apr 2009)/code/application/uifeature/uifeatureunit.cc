//------------------------------------------------------------------------------
//  uifeature/uifeatureunit.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "uifeature/uifeatureunit.h"
#include "timing/mastertime.h"

namespace UIFeature
{
__ImplementClass(UIFeatureUnit, 'UIFU' , Game::FeatureUnit);
__ImplementSingleton(UIFeatureUnit);

//------------------------------------------------------------------------------
/**
*/
UIFeatureUnit::UIFeatureUnit()
{	
	this->uiServer = UI::UIManager::Create();
	this->uiRenderer = UI::UIRenderer::Create();
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
UIFeatureUnit::~UIFeatureUnit()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
UIFeatureUnit::OnActivate()
{
    FeatureUnit::OnActivate();
    this->uiServer->Open();	
    if (this->HasLoadingScreenReourceId())
    {
        this->uiServer->ShowLoadingScreen(this->loadingWindowFilename);
        this->uiServer->Trigger();
        this->uiServer->Render();
        this->uiRenderer->OnFrame();
    }    
}

//------------------------------------------------------------------------------
/**
*/
void
UIFeatureUnit::OnDeactivate()
{
    this->uiServer->Close();
    this->uiServer = 0;

	this->uiRenderer = 0;

    FeatureUnit::OnDeactivate();    
}

//------------------------------------------------------------------------------
/**
*/
void
UIFeatureUnit::OnRenderDebug()
{
	this->uiServer->RenderDebug();

	FeatureUnit::OnRenderDebug();
}

//------------------------------------------------------------------------------
/**
*/
void
UIFeatureUnit::SetUIEventHandler(const Ptr<UI::UIEventHandler>& handler)
{
	n_assert(handler.isvalid());
	n_assert(this->uiServer.isvalid());
	this->uiServer->SetEventHandler(handler);
}

//------------------------------------------------------------------------------
/**
*/
void
UIFeatureUnit::OnFrame()
{    
    // update time in ui system
    Timing::Time time = Timing::MasterTime::Instance()->GetTime();
	this->uiServer->SetTime(time);

    // trigger ui
    this->uiServer->Trigger();
	this->uiServer->Render();

	// now render the stuff	
	this->uiRenderer->OnFrame();

    FeatureUnit::OnFrame();
}
}; // namespace Game
