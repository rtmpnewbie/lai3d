//------------------------------------------------------------------------------
//  streamtexturesaver.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/streamtexturesaver.h"

#if (__WIN32__ || __XBOX360__)
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::StreamTextureSaver, 'STXS', Win360::D3D9StreamTextureSaver);
}
#elif __WII__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::StreamTextureSaver, 'STXS', Base::StreamTextureSaverBase);
}
#else
#error "StreamTextureSaver class not implemented on this platform!"
#endif
