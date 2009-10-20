//------------------------------------------------------------------------------
//  game/networkfeature.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "networkfeature/networkfeatureunit.h"
#include "game/gameserver.h"

namespace NetworkFeature
{
__ImplementClass(NetworkFeatureUnit, 'NEFU' , Game::FeatureUnit);
__ImplementSingleton(NetworkFeatureUnit);

using namespace Multiplayer;

//------------------------------------------------------------------------------
/**
*/
NetworkFeatureUnit::NetworkFeatureUnit()
{
    __ConstructSingleton;
    this->mulitplayerManager = MultiplayerManager::Create();
}

//------------------------------------------------------------------------------
/**
*/
NetworkFeatureUnit::~NetworkFeatureUnit()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
NetworkFeatureUnit::OnActivate()
{
    FeatureUnit::OnActivate();
    this->mulitplayerManager->Open();
}

//------------------------------------------------------------------------------
/**
*/
void
NetworkFeatureUnit::OnDeactivate()
{
    this->mulitplayerManager->Close();
    FeatureUnit::OnDeactivate();
}

//------------------------------------------------------------------------------
/**
*/
void
NetworkFeatureUnit::OnBeginFrame()
{  
    this->mulitplayerManager->OnFrame();
}

//------------------------------------------------------------------------------
/**
*/
void 
NetworkFeatureUnit::SetClientHandler(const Ptr<Multiplayer::NetMessageHandler>& handler)
{
    n_assert(this->mulitplayerManager.isvalid());

    this->mulitplayerManager->AttachClientHandler(handler);
}

//------------------------------------------------------------------------------
/**
*/
void 
NetworkFeatureUnit::SetServerHandler(const Ptr<Multiplayer::NetMessageHandler>& handler)
{
    n_assert(this->mulitplayerManager.isvalid());

    this->mulitplayerManager->AttachServerHandler(handler);
}
} // namespace Game
