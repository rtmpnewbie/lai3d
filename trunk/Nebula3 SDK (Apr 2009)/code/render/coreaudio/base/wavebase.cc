//------------------------------------------------------------------------------
//  wavebase.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coreaudio/base/wavebase.h"

namespace Base
{
__ImplementClass(Base::WaveBase, 'WAVB', Core::RefCounted);

using namespace CoreAudio;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
WaveBase::WaveBase() :
    waveIndex(InvalidIndex),
    is3DEnabled(false),
    transform(matrix44::identity()),
    velocity(vector::nullvec())
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
WaveBase::~WaveBase()
{
    n_assert(!this->IsValid());
}

//------------------------------------------------------------------------------
/**
*/
void
WaveBase::Setup(const Ptr<SoundBank>& sb, IndexT index, bool startPlayback, SizeT loopCnt, bool enable3D)
{
    n_assert(!this->IsValid());
    n_assert(sb.isvalid());
    n_assert(InvalidIndex != index);
    this->soundBank = sb;
    this->waveIndex = index;
    this->is3DEnabled = enable3D;
}

//------------------------------------------------------------------------------
/**
*/
void
WaveBase::Discard()
{
    n_assert(this->IsValid());
    this->soundBank = 0;
    this->waveIndex = InvalidIndex;
}

//------------------------------------------------------------------------------
/**
*/
const WaveInfo&
WaveBase::GetWaveInfo() const
{
    n_assert(this->IsValid());
    return this->soundBank->GetWaveInfo(this->waveIndex);
}

//------------------------------------------------------------------------------
/**
    Set wave volume. 0 means silence, 1 means original volume.
*/
void
WaveBase::SetVolume(float v)
{
    // empty, override in subclass
}

//------------------------------------------------------------------------------
/**
    Set wave pitch between -1.0 and 1.0.
*/
void
WaveBase::SetPitch(float p)
{
    // empty, override in subclass
}    

//------------------------------------------------------------------------------
/**
*/
void
WaveBase::Play()
{
    // empty, override in subclass
}

//------------------------------------------------------------------------------
/**
*/
void
WaveBase::Stop()
{
    // empty, override in subclass
}

//------------------------------------------------------------------------------
/**
*/
void
WaveBase::StopImmediate()
{
    // empty, override in subclass
}

//------------------------------------------------------------------------------
/**
*/
void
WaveBase::Pause()
{
    // empty, override in subclass
}

//------------------------------------------------------------------------------
/**
*/
void
WaveBase::Resume()
{
    // empty, override in subclass
}

//------------------------------------------------------------------------------
/**
*/
bool
WaveBase::IsPreparing() const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
WaveBase::IsPrepared() const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
WaveBase::IsPlaying() const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
WaveBase::IsStopping() const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
WaveBase::IsStopped() const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
WaveBase::IsPaused() const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
SoundState::Mask
WaveBase::GetState() const
{
    return 0;
}

} // namespace Base