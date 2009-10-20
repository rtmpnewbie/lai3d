//------------------------------------------------------------------------------
/**
*/
#include "stdneb.h"
#include "corenetwork/netstream.h"

namespace CoreNetwork
{
#if __WIN32__
    __ImplementClass(CoreNetwork::NetStream, 'NEST', RakNet::RaknetStream);
#else
#error "Socket class not implemented on this platform!"
#endif
}