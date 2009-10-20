//------------------------------------------------------------------------------
//  vertexbuffer.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/vertexbuffer.h"
#if (__WIN32__ || __XBOX360__)
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::VertexBuffer, 'VTXB', Win360::D3D9VertexBuffer);
}
#elif __WII__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::VertexBuffer, 'VTXB', Wii::WiiVertexBuffer);
}
#else
#error "VertexBuffer class not implemented on this platform!"
#endif

