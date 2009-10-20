#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::MemoryMeshLoader
    
    Resource loader to setup a Mesh object from a memory.
    
    (C) 2008 Radon Labs GmbH
*/
#if (__WIN32__ || __XBOX360__)
#include "coregraphics/win360/d3d9memorymeshloader.h"
namespace CoreGraphics
{
class MemoryMeshLoader : public Win360::D3D9MemoryMeshLoader
{
    __DeclareClass(MemoryMeshLoader);
};
}
#elif __WII__
// not implemented on Wii
#include "resources/resourceloader.h"
namespace CoreGraphics
{
class MemoryMeshLoader : public Resources::ResourceLoader
{
    __DeclareClass(MemoryMeshLoader);
};
}
#else
#error "MemoryMeshLoader class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------

    