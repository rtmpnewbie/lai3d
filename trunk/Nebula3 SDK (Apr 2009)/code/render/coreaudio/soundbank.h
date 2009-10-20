#pragma once
#ifndef COREAUDIO_SOUNDBANK_H
#define COREAUDIO_SOUNDBANK_H
//------------------------------------------------------------------------------
/**
    @class CoreAudio::SoundBank
    
    A sound bank is a collection of Cues and Waves under a single name.
    
    (C) 2008 Radon Labs GmbH
*/
#if (__WIN32__ || __XBOX360__)
#include "coreaudio/win360/xactsoundbank.h"
namespace CoreAudio
{
class SoundBank : public Win360::XACTSoundBank
{
    __DeclareClass(SoundBank);
};
}
#else
#error "SoundBank not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
