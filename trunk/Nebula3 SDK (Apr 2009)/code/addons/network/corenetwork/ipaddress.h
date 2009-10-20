#pragma once
#ifndef CORENETWORK_IPADDRESS_H
#define CORENETWORK_IPADDRESS_H
//------------------------------------------------------------------------------
/**
@class Net::IpAddress

(C) 2008 Radon Labs GmbH
*/
#if __WIN32__
#include "corenetwork/raknet/raknetipaddress.h"
namespace CoreNetwork
{
    class IpAddress : public RakNet::RakNetIpAddress
    {
        __DeclareClass(IpAddress);
    public:
        /// constructor
        IpAddress();
        /// destructor
        virtual ~IpAddress();
    };
}
#else
#error "IpAddress class not implemented on this platform"
#endif
//------------------------------------------------------------------------------
#endif