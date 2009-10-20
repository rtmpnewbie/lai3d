//------------------------------------------------------------------------------
//  soundbank.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coreaudio/soundbank.h"

#if (__WIN32__ || __XBOX360__)
namespace CoreAudio
{
__ImplementClass(CoreAudio::SoundBank, 'SNDB', Win360::XACTSoundBank);
}
#else
#error "SoundBank not implemented on this platform!"
#endif
