#ifndef UIFEATURE_UIFEATUREUNIT_H
#define UIFEATURE_UIFEATUREUNIT_H
//------------------------------------------------------------------------------
/**
    @class Game::UIFeatureUnit
    
    The UIFeatureUnit deliveres a Server and the needed Factory Stuff, to create
	and handle a complete UserInterface

	// TODO more desc

    (C) 2008 Radon Labs GmbH
*/
#include "game/featureunit.h"
#include "uifeature/uimanager.h"
#include "coreui/uirenderer.h"

//------------------------------------------------------------------------------
namespace UIFeature
{

class UIFeatureUnit : public Game::FeatureUnit    
{
    __DeclareClass(UIFeatureUnit);
    __DeclareSingleton(UIFeatureUnit);   

public:
    /// constructor
    UIFeatureUnit();
    /// destructor
    virtual ~UIFeatureUnit();

    /// called from GameServer::ActivateProperties()
    virtual void OnActivate();
    /// called from GameServer::DeactivateProperties()
    virtual void OnDeactivate();
    
    /// called in the middle of the feature trigger cycle
    virtual void OnFrame();

	/// called when game debug visualization is on
	virtual void OnRenderDebug();

	/// sets the handler vor the UI
    virtual void SetUIEventHandler(const Ptr<UI::UIEventHandler>& handler);

    /// set loading screen resource
    void SetLoadingScreenFilename(const Util::String& res);
    /// has loading resource id
    bool HasLoadingScreenReourceId() const;

protected:
    Ptr<UI::UIManager> uiServer;
    Ptr<UI::UIRenderer> uiRenderer;
    Util::String loadingWindowFilename;
};

//------------------------------------------------------------------------------
/**
*/
inline void 
UIFeatureUnit::SetLoadingScreenFilename(const Util::String& res)
{
    this->loadingWindowFilename = res;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
UIFeatureUnit::HasLoadingScreenReourceId() const
{
    return this->loadingWindowFilename.IsValid();
}
}; // namespace PhysicsFeature
//------------------------------------------------------------------------------
#endif