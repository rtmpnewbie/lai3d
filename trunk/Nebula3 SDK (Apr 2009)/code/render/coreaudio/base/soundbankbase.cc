//------------------------------------------------------------------------------
//  soundbankbase.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coreaudio/base/soundbankbase.h"

namespace Base
{
__ImplementClass(Base::SoundBankBase, 'SBKB', Core::RefCounted);

using namespace Util;
using namespace Resources;

//------------------------------------------------------------------------------
/**
*/
SoundBankBase::SoundBankBase() :
    isValid(false),
    isStreaming(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
SoundBankBase::~SoundBankBase()
{
    n_assert(!this->IsValid());
}

//------------------------------------------------------------------------------
/**
*/
bool
SoundBankBase::Setup(const ResourceId& resId, bool streaming)
{
    n_assert(!this->IsValid());
    this->isValid = true;
    this->resourceId = resId;
    this->isStreaming = streaming;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
SoundBankBase::Discard()
{
    n_assert(this->IsValid());
    this->resourceId.Clear();
    this->cueInfos.Clear();
    this->cueIndexMap.Clear();
    this->waveInfos.Clear();
    this->waveIndexMap.Clear();
    this->isValid = false;
}

//------------------------------------------------------------------------------
/**
*/
void
SoundBankBase::StopCue(IndexT cueIndex, bool immediate)
{
    // empty, override in subclass!
}

//------------------------------------------------------------------------------
/**
*/
void
SoundBankBase::StopWave(IndexT waveIndex, bool immediate)
{
    // empty, override in subclass!
}

} // namespace Base