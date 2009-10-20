#pragma once
#ifndef COREAUDIO_WAVE_H
#define COREAUDIO_WAVE_H
//------------------------------------------------------------------------------
/**
    @class CoreAudio::Wave
    
    A wave object represents a low level sound effect.
    
    (C) 2008 Radon Labs GmbH
*/
#if (__WIN32__ || __XBOX360__)
#include "coreaudio/win360/xactwave.h"
namespace CoreAudio
{
class Wave : public Win360::XACTWave
{
    __DeclareClass(Wave);
};
}
#else
#error "Wave not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
    