//------------------------------------------------------------------------------
//  transformdevice.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/transformdevice.h"

namespace CoreGraphics
{
#if (__WIN32__ || __XBOX360__)
__ImplementClass(CoreGraphics::TransformDevice, 'TRDV', Win360::D3D9TransformDevice);
__ImplementSingleton(CoreGraphics::TransformDevice);
#elif __WII__
__ImplementClass(CoreGraphics::TransformDevice, 'TRDV', Wii::WiiTransformDevice);
__ImplementSingleton(CoreGraphics::TransformDevice);
#else
#error "TransformDevice class not implemented on this platform!"
#endif

//------------------------------------------------------------------------------
/**
*/
TransformDevice::TransformDevice()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
TransformDevice::~TransformDevice()
{
    __DestructSingleton;
}

} // namespace CoreGraphics
