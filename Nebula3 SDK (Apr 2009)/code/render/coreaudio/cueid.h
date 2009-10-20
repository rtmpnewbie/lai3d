#pragma once
#ifndef COREAUDIO_CUEID_H
#define COREAUDIO_CUEID_H
//------------------------------------------------------------------------------
/**
    @class CoreAudio::CueId
  
    Defines a cue identifier (name of an audio cue in a SoundBank).
    
    (C) 2008 Radon Labs GmbH
*/    
#include "util/string.h"
#include "util/atom.h"

//------------------------------------------------------------------------------
namespace CoreAudio
{
typedef Util::Atom<Util::String> CueId;
} // namespace Resources
//------------------------------------------------------------------------------
#endif
