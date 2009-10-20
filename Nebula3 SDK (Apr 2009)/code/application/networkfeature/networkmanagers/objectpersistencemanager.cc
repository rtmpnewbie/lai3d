//------------------------------------------------------------------------------
//  networkmanagers/ObjectPersistenceManager.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "networkmanagers/objectpersistencemanager.h"

namespace NetworkFeature
{
__ImplementClass(ObjectPersistenceManager, 'OPEM', Game::Manager);
__ImplementSingleton(ObjectPersistenceManager);

//------------------------------------------------------------------------------
/**
*/
ObjectPersistenceManager::ObjectPersistenceManager()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
ObjectPersistenceManager::~ObjectPersistenceManager()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
ObjectPersistenceManager::OnFrame()
{
}

}; // namespace Blob
