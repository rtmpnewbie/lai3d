//------------------------------------------------------------------------------
//  shadervariable.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/shadervariable.h"

#if __WIN32__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::ShaderVariable, 'SHDV', Direct3D9::D3D9ShaderVariable);
}
#elif __XBOX360__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::ShaderVariable, 'SHDV', Xbox360::Xbox360ShaderVariable);
}
#elif __WII__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::ShaderVariable, 'SHDV', Wii::WiiShaderVariable);
}
#else
#error "ShaderVariable class not implemented on this platform!"
#endif
