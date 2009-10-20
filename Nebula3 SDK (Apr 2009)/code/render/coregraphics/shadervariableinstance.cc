//------------------------------------------------------------------------------
//  shadervariableinstance.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/shadervariableinstance.h"

#if __WIN32__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::ShaderVariableInstance, 'SDVI', Base::ShaderVariableInstanceBase);
}
#elif __XBOX360__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::ShaderVariableInstance, 'SDVI', Base::ShaderVariableInstanceBase);
}
#elif __WII__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::ShaderVariableInstance, 'SDVI', Base::ShaderVariableInstanceBase);
}
#else
#error "ShaderVariableInstance class not implemented on this platform!"
#endif
