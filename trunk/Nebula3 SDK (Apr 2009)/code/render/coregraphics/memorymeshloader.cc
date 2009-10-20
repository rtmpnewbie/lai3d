//------------------------------------------------------------------------------
//  streammeshloader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/memorymeshloader.h"

#if (__WIN32__ || __XBOX360__)
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::MemoryMeshLoader, 'MMLD', Win360::D3D9MemoryMeshLoader);
}
#elif __WII__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::MemoryMeshLoader, 'MMLD', Resources::ResourceLoader);
}
#else
#error "MemoryMeshLoader class not implemented on this platform!"
#endif
