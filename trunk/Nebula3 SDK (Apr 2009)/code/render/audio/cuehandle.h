#pragma once
#ifndef AUDIO_CUEHANDLE_H
#define AUDIO_CUEHANDLE_H
//------------------------------------------------------------------------------
/**
    @class Audio::CueHandle
    
    A private handle to a CoreAudio Cue object.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"

//------------------------------------------------------------------------------
namespace Audio
{
typedef Ptr<Core::RefCounted> CueHandle;
} // namespace Audio
//------------------------------------------------------------------------------
#endif
    