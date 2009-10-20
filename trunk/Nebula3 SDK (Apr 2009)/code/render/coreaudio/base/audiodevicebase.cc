//------------------------------------------------------------------------------
//  audiodevicebase.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coreaudio/base/audiodevicebase.h"
#include "coreaudio/soundbank.h"

namespace Base
{
__ImplementClass(Base::AudioDeviceBase, 'AUDB', Core::RefCounted);
__ImplementSingleton(Base::AudioDeviceBase);

using namespace Resources;
using namespace CoreAudio;
using namespace Math;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
AudioDeviceBase::AudioDeviceBase() :
    isOpen(false),
    adapterIndex(DefaultAdapter),
    activeCues(128, 128),
    activeWaves(128, 128),
    listenerTransform(matrix44::identity()),
    listenerVelocity(vector::nullvec())
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
AudioDeviceBase::~AudioDeviceBase()
{
    n_assert(!this->IsOpen());
}

//------------------------------------------------------------------------------
/**
*/
bool
AudioDeviceBase::CanCreate()
{
    // override in subclass!
    return false;
}

//------------------------------------------------------------------------------
/**
*/
SizeT
AudioDeviceBase::GetNumAdapters() const
{
    // override in subclass!
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
AudioAdapterInfo
AudioDeviceBase::GetAdapterInfo(IndexT adapterIndex) const
{
    // override in subclass!
    return AudioAdapterInfo();
}

//------------------------------------------------------------------------------
/**
*/
bool
AudioDeviceBase::Open()
{
    n_assert(!this->isOpen);
    n_assert(this->soundBanks.IsEmpty());
    this->isOpen = true;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
AudioDeviceBase::Close()
{
    n_assert(this->isOpen);
    
    // discard all active cues and waves
    IndexT i;
    for (i = 0; i < this->activeCues.Size(); i++)
    {
        this->activeCues[i]->Discard();
    }
    for (i = 0; i < this->activeWaves.Size(); i++)
    {
        this->activeWaves[i]->Discard();
    }
    this->activeCues.Clear();
    this->activeWaves.Clear();

    // discard loaded sound banks
    for (i = 0; i < this->soundBanks.Size(); i++)
    {
        this->soundBanks.ValueAtIndex(i)->Discard();        
    }
    this->soundBanks.Clear();

    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
void
AudioDeviceBase::OnFrame()
{
    n_assert(this->IsOpen());
    this->GarbageCollectStoppedCues();
    this->GarbageCollectStoppedWaves();

    // update playing cues (to apply the 3d effect)
    IndexT i;
    for (i = 0; i < this->activeCues.Size(); i++)
    {
        this->activeCues[i]->Update();
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
AudioDeviceBase::LoadSoundBank(const ResourceId& resId, bool streaming)
{
    n_assert(!this->HasSoundBank(resId));
    Ptr<SoundBank> newSoundBank = SoundBank::Create();
    if (newSoundBank->Setup(resId, streaming))
    {
        this->soundBanks.Add(resId, newSoundBank);
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
AudioDeviceBase::UnloadSoundBank(const ResourceId& resId)
{
    n_assert(this->HasSoundBank(resId));
    this->soundBanks[resId]->Discard();
    this->soundBanks.Erase(resId);
}

//------------------------------------------------------------------------------
/**
    NOTE: we could optimize this by building a global dictionary, but
    this would cost additional memory...
*/
IndexT
AudioDeviceBase::FindCue(const CueId& cueId, Ptr<SoundBank>& outSoundBank) const
{
    IndexT soundBankIndex;
    for (soundBankIndex = 0; soundBankIndex < this->soundBanks.Size(); soundBankIndex++)
    {
        const Ptr<SoundBank>& curSoundBank = this->soundBanks.ValueAtIndex(soundBankIndex);
        IndexT cueIndex = curSoundBank->FindCueIndexByName(cueId);
        if (InvalidIndex != cueIndex)
        {
            // success
            outSoundBank = curSoundBank;
            return cueIndex;
        }
    }
    // fallthrough: not found
    return InvalidIndex;
}

//------------------------------------------------------------------------------
/**
    NOTE: we could optimize this by building a global dictionary, but
    this would cost additional memory...
*/
IndexT
AudioDeviceBase::FindWave(const WaveId& waveId, Ptr<SoundBank>& outSoundBank) const
{
    IndexT soundBankIndex;
    for (soundBankIndex = 0; soundBankIndex < this->soundBanks.Size(); soundBankIndex++)
    {
        const Ptr<SoundBank>& curSoundBank = this->soundBanks.ValueAtIndex(soundBankIndex);
        IndexT waveIndex = curSoundBank->FindWaveIndexByName(waveId);
        if (InvalidIndex != waveIndex)
        {
            // success
            outSoundBank = curSoundBank;
            return waveIndex;
        }
    }
    // fallthrough: not found
    return InvalidIndex;
}

//------------------------------------------------------------------------------
/**
*/
void
AudioDeviceBase::GarbageCollectStoppedCues()
{
    // start at the end, so we don't mess up the index
    IndexT i;
    for (i = this->activeCues.Size() - 1; i != InvalidIndex; i--)
    {
        if (this->activeCues[i]->IsStopped())
        {
            this->activeCues[i]->Discard();
            this->activeCues.EraseIndex(i);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
AudioDeviceBase::GarbageCollectStoppedWaves()
{
    // start at the end, so we don't mess up the index
    IndexT i;
    for (i = this->activeWaves.Size() - 1; i != InvalidIndex; i--)
    {
        if (this->activeWaves[i]->IsStopped())
        {
            this->activeWaves[i]->Discard();
            this->activeWaves.EraseIndex(i);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
AudioDeviceBase::UpdateListener(const matrix44& transform, const vector& velocity)
{
    this->listenerTransform = transform;
    this->listenerVelocity = velocity;
}

//------------------------------------------------------------------------------
/**
    Create a new Cue instance, start playing or prepare it and add it to the
    array of active cues. If this is a true fire-and-forget sound, the client
    can ignore the returned pointer, otherwise the pointer can be used to 
    manipulate the cue during playback. Clients must not call Discard() on the
    cue, stopped cues will be discarded automatically by the AudioDevice.
*/
Ptr<Cue>
AudioDeviceBase::PlayCue(const CueId& cueId, bool play, bool enable3D, const Math::matrix44& transform, const Math::vector& velocity)
{
    n_assert(this->IsOpen());
    Ptr<SoundBank> soundBank;
    IndexT cueIndex = this->FindCue(cueId, soundBank);
    if (InvalidIndex != cueIndex)
    {
        // create and setup a new cue instance
        Ptr<Cue> newCue = Cue::Create();
        if (enable3D)
        {
            newCue->SetTransform(transform);
            newCue->SetVelocity(velocity);
        }
        newCue->Setup(soundBank, cueIndex, play, enable3D);
        this->activeCues.Append(newCue);
        return newCue;
    }
    else
    {
        n_error("PlayCue(): cue '%s' not found in any loaded sound bank!", cueId.Value().AsCharPtr());
        return Ptr<Cue>();
    }
}

//------------------------------------------------------------------------------
/**
    This stops ALL playing instances of a cue allowing for release phase and
    transitions. To stop an individual cue, call the cue's Stop() method.
*/
void
AudioDeviceBase::StopCue(const CueId& cueId, bool immediate)
{
    n_assert(this->IsOpen());
    Ptr<SoundBank> soundBank;
    IndexT cueIndex = this->FindCue(cueId, soundBank);
    if (InvalidIndex != cueIndex)
    {
        soundBank->StopCue(cueIndex, immediate);
    }
    else
    {
        n_error("StopCue(): cue '%s' not found in any loaded sound bank!", cueId.Value().AsCharPtr());
    }
}

//------------------------------------------------------------------------------
/**
    Create a new Wave instance, start playing or prepare it and add it to the
    array of active waves. If this is a true fire-and-forget sound, the client
    can ignore the returned pointer, otherwise the pointer can be used to 
    manipulate the wave during playback. Clients must not call Discard() on the
    wave, stopped waves will be discarded automatically by the AudioDevice.
*/
Ptr<Wave>
AudioDeviceBase::PlayWave(const CoreAudio::WaveId& waveId, bool play, SizeT loopCount, float volume, float pitch, bool enable3D, const Math::matrix44& transform, const Math::vector& velocity)
{
    n_assert(this->IsOpen());
    Ptr<SoundBank> soundBank;
    IndexT waveIndex = this->FindWave(waveId, soundBank);
    if (InvalidIndex != waveIndex)
    {
        // create and setup a new cue instance
        Ptr<Wave> newWave = Wave::Create();
        if (enable3D)
        {
            newWave->SetTransform(transform);
            newWave->SetVelocity(velocity);
        }
        newWave->SetVolume(volume);
        newWave->SetPitch(pitch);
        newWave->Setup(soundBank, waveIndex, play, loopCount, enable3D);
        this->activeWaves.Append(newWave);
        return newWave;
    }
    else
    {
        n_error("PlayWave(): wave '%s' not found in any loaded sound bank!", waveId.Value().AsCharPtr());
        return Ptr<Wave>();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
AudioDeviceBase::StopWave(const WaveId& waveId, bool immediate)
{
    n_assert(this->IsOpen());
    Ptr<SoundBank> soundBank;
    IndexT waveIndex = this->FindWave(waveId, soundBank);
    if (InvalidIndex != waveIndex)
    {
        soundBank->StopWave(waveIndex, immediate);
    }
    else
    {
        n_error("StopWave(): wave '%s' not found in any loaded sound bank!", waveId.Value().AsCharPtr());
    }
}

//------------------------------------------------------------------------------
/**
    Returns an array of currently active cues. Note that the content of the
    array may change after the next OnFrame() method due to garbage collection
    of stopped cues!
*/
const Array<Ptr<Cue> >&
AudioDeviceBase::GetActiveCues() const
{
    return this->activeCues;
}

//------------------------------------------------------------------------------
/**
    Returns an array of currently active waves. Note that the content of the
    array may change after the next OnFrame() method due to garbage collection
    of stopped waves!
*/
const Array<Ptr<Wave> >&
AudioDeviceBase::GetActiveWaves() const
{
    return this->activeWaves;
}

//------------------------------------------------------------------------------
/**
    No functionality in base
*/
void
AudioDeviceBase::SetGlobalVariable(const Util::StringAtom& varName, float varValue)
{
    // empty
}

//------------------------------------------------------------------------------
/**
    No functionality in base
*/
float
AudioDeviceBase::GetGlobalVariable(const Util::StringAtom& varName) const
{
    // empty
    return 0;
}
} // namespace Base