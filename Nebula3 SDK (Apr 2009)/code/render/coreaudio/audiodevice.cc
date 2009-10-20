//------------------------------------------------------------------------------
//  audiodevice.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coreaudio/audiodevice.h"

namespace CoreAudio
{
#if (__WIN32__ || __XBOX360__)
__ImplementClass(CoreAudio::AudioDevice, 'AUDD', Win360::XACTAudioDevice);
__ImplementSingleton(CoreAudio::AudioDevice);
#else
#error "AudioDevice class not implemented on this platform!"
#endif

//------------------------------------------------------------------------------
/**
*/
AudioDevice::AudioDevice()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
AudioDevice::~AudioDevice()
{
    __DestructSingleton;
}

} // namespace CoreAudio
