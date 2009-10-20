//------------------------------------------------------------------------------
//  lightserver.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "lighting/lightserver.h"

namespace Lighting
{
#if (__WIN32__ || __XBOX360__)
__ImplementClass(Lighting::LightServer, 'LISV', Lighting::SM30LightServer);
#elif __WII__
__ImplementClass(Lighting::LightServer, 'LISV', Wii::WiiLightServer);
#else
#error "LightServer class not implemented on this platform!"
#endif
__ImplementSingleton(Lighting::LightServer);

//------------------------------------------------------------------------------
/**
*/
LightServer::LightServer()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
LightServer::~LightServer()
{
    __DestructSingleton;
}

} // namespace Lighting


