//------------------------------------------------------------------------------
//  wave.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coreaudio/wave.h"

#if (__WIN32__ || __XBOX360__)
namespace CoreAudio
{
__ImplementClass(CoreAudio::Wave, 'CAWV', Win360::XACTWave);
}
#else
#error "Wave not implemented on this platform!"
#endif