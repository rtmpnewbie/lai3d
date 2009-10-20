#pragma once
#ifndef COREAUDIO_SOUNDSTATE_H
#define COREAUDIO_SOUNDSTATE_H
//------------------------------------------------------------------------------
/**
    @class CoreAudio::SoundState
    
    Defines state bits which represent the current state of a Cue or Wave
    object.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/types.h"

//------------------------------------------------------------------------------
namespace CoreAudio
{
class SoundState
{
public:
    /// sound state bits
    enum Bits
    {
        Initial = (1<<0),           // sound has been created but nothing else
        Preparing = (1<<1),         // sound is preparing to play
        Prepared = (1<<2),          // sound is ready to play
        Playing = (1<<3),           // sound is currently playing
        Stopping = (1<<4),          // sound is currently stopping
        Stopped = (1<<5),           // sound is stopped
        Paused = (1<<6),            // sound is currently paused
    };
    typedef ushort Mask;
};

} // namespace CoreAudio
//------------------------------------------------------------------------------
#endif
    