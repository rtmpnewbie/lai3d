//------------------------------------------------------------------------------
//  cuebase.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coreaudio/base/cuebase.h"

namespace Base
{
__ImplementClass(Base::CueBase, 'CUEB', Core::RefCounted);

using namespace CoreAudio;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
CueBase::CueBase() :
    cueIndex(InvalidIndex),
    is3DEnabled(false),
    transform(matrix44::identity()),
    velocity(vector::nullvec())
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
CueBase::~CueBase()
{
    n_assert(!this->IsValid());
}

//------------------------------------------------------------------------------
/**
*/
void
CueBase::Setup(const Ptr<SoundBank>& sb, IndexT index, bool start, bool enable3D)
{
    n_assert(!this->IsValid());
    n_assert(sb.isvalid());
    n_assert(InvalidIndex != index);
    this->soundBank = sb;
    this->cueIndex = index;
    this->is3DEnabled = enable3D;
}

//------------------------------------------------------------------------------
/**
*/
void
CueBase::Discard()
{
    n_assert(this->IsValid());
    this->soundBank = 0;
    this->cueIndex = InvalidIndex;
}

//------------------------------------------------------------------------------
/**
*/
const CueInfo&
CueBase::GetCueInfo() const
{
    n_assert(this->IsValid());
    return this->soundBank->GetCueInfo(this->cueIndex);
}

//------------------------------------------------------------------------------
/**
*/
void
CueBase::Play()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
CueBase::Stop()
{
    // empty, override in subclass
}

//------------------------------------------------------------------------------
/**
*/
void
CueBase::StopImmediate()
{
    // empty, override in subclass
}

//------------------------------------------------------------------------------
/**
*/
void
CueBase::Pause()
{
    // empty, override in subclass
}

//------------------------------------------------------------------------------
/**
*/
void
CueBase::Resume()
{
    // empty, override in subclass
}

//------------------------------------------------------------------------------
/**
*/
void
CueBase::Update()
{
    // empty, override in subclass
}

//------------------------------------------------------------------------------
/**
*/
bool
CueBase::IsPreparing() const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
CueBase::IsPrepared() const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
CueBase::IsPlaying() const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
CueBase::IsStopping() const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
CueBase::IsStopped() const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
CueBase::IsPaused() const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
SoundState::Mask
CueBase::GetState() const
{
    return 0;
}

} // namespace Base