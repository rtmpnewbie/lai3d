//------------------------------------------------------------------------------
//  cue.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coreaudio/cue.h"

#if (__WIN32__ || __XBOX360__)
namespace CoreAudio
{
__ImplementClass(CoreAudio::Cue, 'CACE', Win360::XACTCue);
}
#else
#error "Cue not implemented on this platform!"
#endif