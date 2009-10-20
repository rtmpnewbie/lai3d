#pragma once
//------------------------------------------------------------------------------
/**
    @class Video::VideoServer
    
    The VideoServer is the central object of the Video subsystem. 

    (C) 2009 Radon Labs GmbH
*/
#include "core/singleton.h"
#if __WIN32__
#include "video/win32/win32videoserver.h"
namespace Video
{
class VideoServer : public Win32::Win32VideoServer
{
    __DeclareClass(VideoServer);
    __DeclareSingleton(VideoServer);
public:
    /// constructor
    VideoServer();
    /// destructor
    virtual ~VideoServer();
};
} // namespace Video
#elif __XBOX360__
#include "video/xbox360/xbox360videoserver.h"
namespace Video
{
class VideoServer : public Xbox360::Xbox360VideoServer
{
    __DeclareClass(VideoServer);
    __DeclareSingleton(VideoServer);
public:
    /// constructor
    VideoServer();
    /// destructor
    virtual ~VideoServer();
};
} // namespace Video
//#elif __WII__
//#include "video/wii/wiivideoserver.h"
//namespace Video
//{
//class VideoServer : public Wii::WiiVideoServer
//{
//    __DeclareClass(VideoServer);
//    __DeclareSingleton(VideoServer);
//public:
//    /// constructor
//    VideoServer();
//    /// destructor
//    virtual ~VideoServer();
//};
//} // namespace Video
#else
#error "InputServer class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------