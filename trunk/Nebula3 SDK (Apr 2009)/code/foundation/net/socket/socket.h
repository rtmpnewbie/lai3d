#pragma once
#ifndef NET_SOCKET_H
#define NET_SOCKET_H
//------------------------------------------------------------------------------
/**
    @class Net::Socket
    
    Platform independent wrapper class for the Sockets API.
    
    (C) 2006 Radon Labs GmbH
*/
#if (__WIN32__ || __XBOX360__)
#include "net/win360/win360socket.h"
namespace Net
{
class Socket : public Win360::Win360Socket
{
    __DeclareClass(Socket);
};
}
#elif __WII__
// class doesn't exist on Wii
#else
#error "Socket class not implemented on this platform"
#endif
//------------------------------------------------------------------------------
#endif
