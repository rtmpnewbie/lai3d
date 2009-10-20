#ifndef NETWORKFEATURE_NETWORKFEATUREUNIT_H
#define NETWORKFEATURE_NETWORKFEATUREUNIT_H
//------------------------------------------------------------------------------
/**
    @class NetworkFeature::NetworkFeatureUnitUnit
    
    (C) 2008 Radon Labs GmbH
*/
#include "game/featureunit.h"
#include "multiplayer/multiplayermanager.h"

//------------------------------------------------------------------------------
namespace NetworkFeature
{
class NetworkFeatureUnit : public Game::FeatureUnit    
{
    __DeclareClass(NetworkFeatureUnit);
    __DeclareSingleton(NetworkFeatureUnit);   
public:
    /// constructor
    NetworkFeatureUnit();
    /// destructor
    virtual ~NetworkFeatureUnit();
    /// set optional client handler
    void SetClientHandler(const Ptr<Multiplayer::NetMessageHandler>& handler);
    /// set optional server handler
    void SetServerHandler(const Ptr<Multiplayer::NetMessageHandler>& handler);

    /// called from GameServer::ActivateProperties()
    virtual void OnActivate();
    /// called from GameServer::DeactivateProperties()
    virtual void OnDeactivate();   
    /// called on begin of frame
    virtual void OnBeginFrame();
    
protected:
    Ptr<Multiplayer::MultiplayerManager> mulitplayerManager;
};

} // namespace NetworkFeatureUnit
//------------------------------------------------------------------------------
#endif