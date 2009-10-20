#pragma once
#ifndef COREAUDIO_WAVEID_H
#define COREAUDIO_WAVEID_H
//------------------------------------------------------------------------------
/**
    @class CoreAudio::WaveId
    
    Defines a wave id (name of a wave in a sound bank).
    
    (C) 2008 Radon Labs GmbH
*/
#include "util/string.h"
#include "util/atom.h"

//------------------------------------------------------------------------------
namespace CoreAudio
{
typedef Util::Atom<Util::String> WaveId;
} // namespace Resources
//------------------------------------------------------------------------------
#endif
