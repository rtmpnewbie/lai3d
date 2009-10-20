#pragma once
#ifndef COREAUDIO_CUE_H
#define COREAUDIO_CUE_H
//------------------------------------------------------------------------------
/**
    @class CoreAudio::Cue
    
    An audio cue is a "highlevel sound effect" and is actually a wrapper
    around one or several waves. See the XACT docs for more details.
    It is important to note that cues are one-use-only. After a cue has
    stopped playing it cannot be reused to play for another time. Instead
    everytime a cue is played, a new cue object will be instantiated.
    
    (C) 2008 Radon Labs GmbH
*/
#if (__WIN32__ || __XBOX360__)
#include "coreaudio/win360/xactcue.h"
namespace CoreAudio
{
class Cue : public Win360::XACTCue
{
    __DeclareClass(Cue);
};
}
#else
#error "Cue not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif