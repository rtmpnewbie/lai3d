#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::VertexBuffer
  
    A resource which holds an array of vertices.
    
    (C) 2007 Radon Labs GmbH
*/    
#if (__WIN32__ || __XBOX360__)
#include "coregraphics/win360/d3d9vertexbuffer.h"
namespace CoreGraphics
{
class VertexBuffer : public Win360::D3D9VertexBuffer
{
    __DeclareClass(VertexBuffer);
};
}
#elif __WII__
#include "coregraphics/wii/wiivertexbuffer.h"
namespace CoreGraphics
{
class VertexBuffer : public Wii::WiiVertexBuffer
{
    __DeclareClass(VertexBuffer);
};
}
#else
#error "VertexBuffer class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
