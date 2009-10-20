#pragma once
#ifndef COREAUDIO_AUDIODEVICE_H
#define COREAUDIO_AUDIODEVICE_H
//------------------------------------------------------------------------------
/**
    @class CoreAudio::AudioDevice
  
    Central class of the core audio subsystem. Prepares the audio subsystem
    for use and manages sound banks. Actual audio playback happens through
    the AudioPlayer class.
    
    (C) 2008 Radon Labs GmbH
*/    
#if (__WIN32__ || __XBOX360__)
#include "coreaudio/win360/xactaudiodevice.h"
namespace CoreAudio
{
class AudioDevice : public Win360::XACTAudioDevice
{
    __DeclareClass(AudioDevice);
    __DeclareSingleton(AudioDevice);
public:
    /// constructor
    AudioDevice();
    /// destructor
    virtual ~AudioDevice();
};
}
#else
#error "AudioDevice not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
     