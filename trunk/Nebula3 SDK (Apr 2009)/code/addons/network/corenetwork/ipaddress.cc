//------------------------------------------------------------------------------
//  ipaddress.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "corenetwork/ipaddress.h"

namespace CoreNetwork
{
#if __WIN32__
    __ImplementClass(CoreNetwork::IpAddress, 'IPAD', RakNet::RakNetIpAddress);
#else
#error "IpAddress class not implemented on this platform!"
#endif

//------------------------------------------------------------------------------
/**
*/
IpAddress::IpAddress()
{
}

//------------------------------------------------------------------------------
/**
*/
IpAddress::~IpAddress()
{
}

} // namespace CoreNetwork
