#pragma once
#ifndef AUDIO_WAVEHANDLE_H
#define AUDIO_WAVEHANDLE_H
//------------------------------------------------------------------------------
/**
    @class Audio::WaveHandle
    
    A private handle to a CoreAudio Wave object.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"

//------------------------------------------------------------------------------
namespace Audio
{
typedef Ptr<Core::RefCounted> WaveHandle;
} // namespace Audio
//------------------------------------------------------------------------------
#endif
    