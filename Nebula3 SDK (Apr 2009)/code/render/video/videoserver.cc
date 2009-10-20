//------------------------------------------------------------------------------
//  videoserver.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "video/videoserver.h"

namespace Video
{
#if __WIN32__
__ImplementClass(Video::VideoServer, 'VIDS', Win32::Win32VideoServer);
__ImplementSingleton(Video::VideoServer);
#elif __XBOX360__
// FIXME!
__ImplementClass(Video::VideoServer, 'VIDS', Xbox360::Xbox360VideoServer);
__ImplementSingleton(Video::VideoServer);
#elif __WII__
__ImplementClass(Video::VideoServer, 'VIDS', Wii::WiiVideoServer);
__ImplementSingleton(Video::VideoServer);
#else
#error "InputServer class not implemented on this platform!"
#endif

//------------------------------------------------------------------------------
/**
*/
VideoServer::VideoServer()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
VideoServer::~VideoServer()
{
    __DestructSingleton;
}

} // namespace Video
