#ifndef NetworkFeature_OBJECTPERSISTENCEMANAGER_H
#define NetworkFeature_OBJECTPERSISTENCEMANAGER_H
//------------------------------------------------------------------------------
/**
    @class Blob::ObjectPersistenceManager

    (C) 2008 Radon Labs GmbH
*/
#include "game/manager.h"
#include "game/entity.h"

//------------------------------------------------------------------------------
namespace NetworkFeature
{
class ObjectPersistenceManager : public Game::Manager
{
	__DeclareClass(ObjectPersistenceManager);
    __DeclareSingleton(ObjectPersistenceManager);

public:
    /// constructor
    ObjectPersistenceManager();
    /// destructor
    virtual ~ObjectPersistenceManager();

    /// trigger 
    virtual void OnFrame();
	
protected:

};

}; // namespace Managers
//------------------------------------------------------------------------------
#endif

