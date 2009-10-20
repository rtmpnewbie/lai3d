#pragma once
#ifndef BASE_AUDIODEVICEBASE_H
#define BASE_AUDIODEVICEBASE_H
//------------------------------------------------------------------------------
/**
    @class Base::AudioDeviceBase
  
    Central class of the core audio subsystem. Prepares the audio subsystem
    for use, manages sound banks and offers sound play back. Applications
    should not use the CoreAudio subsystem directly (since it's running
    in its own thread) but instead use the front end classes in the 
    Audio subsystem!
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "io/uri.h"
#include "resources/resourceid.h"
#include "coreaudio/audioadapterinfo.h"
#include "coreaudio/soundbank.h"
#include "math/vector.h"
#include "math/matrix44.h"
#include "coreaudio/cue.h"
#include "coreaudio/wave.h"
#include "util/stringatom.h"

//------------------------------------------------------------------------------
namespace Base
{
class AudioDeviceBase : public Core::RefCounted
{
    __DeclareClass(AudioDeviceBase);
    __DeclareSingleton(AudioDeviceBase);
public:
    /// the default adapter index
    static const IndexT DefaultAdapter = 127;

    /// constructor
    AudioDeviceBase();
    /// destructor
    virtual ~AudioDeviceBase();
    /// test if audio output is supported on this machine
    static bool CanCreate();
    
    /// get number of audio adapters
    SizeT GetNumAdapters() const;
    /// get info about an audio adapter
    CoreAudio::AudioAdapterInfo GetAdapterInfo(IndexT adapterIndex) const;
    /// select adapter for audio output, if this method is not called, use default adapter
    void SetAdapter(IndexT adapterIndex);
    /// get adapter currently selected for audio output
    IndexT GetAdapter() const;

    /// set URI of global settings file
    void SetGlobalSettingsURI(const IO::URI& uri);
    /// get URI of global settings file
    const IO::URI& GetGlobalSettingsURI() const;

    /// open the audio device
    bool Open();
    /// close the audio device
    void Close();
    /// return true if currently open
    bool IsOpen() const;
    /// do per-frame work
    void OnFrame();

    /// load a sound bank
    bool LoadSoundBank(const Resources::ResourceId& resId, bool streaming);
    /// discard a loaded sound bank
    void UnloadSoundBank(const Resources::ResourceId& resId);
    /// get number of loaded sound bank
    SizeT GetNumSoundBanks() const;
    /// get pointer to sound bank by index
    const Ptr<CoreAudio::SoundBank>& GetSoundBankByIndex(IndexT i) const;
    /// return true if sound bank exists
    bool HasSoundBank(const Resources::ResourceId& resId) const;
    /// get pointer to sound bank by name
    const Ptr<CoreAudio::SoundBank>& GetSoundBankByName(const Resources::ResourceId& resId) const;

    /// update the 3d listener properties
    void UpdateListener(const Math::matrix44& transform, const Math::vector& velocity);
    /// get listener transform
    const Math::matrix44& GetListenerTransform() const;
    /// get listener velocity
    const Math::vector& GetListenerVelocity() const;

    /// play or prepare a 2D or 3D cue
    Ptr<CoreAudio::Cue> PlayCue(const CoreAudio::CueId& cueId, bool play, bool enable3D, const Math::matrix44& tranform, const Math::vector& velocity);
    /// stop playback of a cue all matching cue instances, allow release and transitions
    void StopCue(const CoreAudio::CueId& cueId, bool immediate);
    /// get array of currently active cues 
    const Util::Array<Ptr<CoreAudio::Cue> >& GetActiveCues() const;

    /// play or prepare a 2D or 3D wave
    Ptr<CoreAudio::Wave> PlayWave(const CoreAudio::WaveId& waveId, bool play, SizeT loopCount, float volume, float pitch, bool enable3D, const Math::matrix44& tranform, const Math::vector& velocity);
    /// stop playback of all matching wave instances
    void StopWave(const CoreAudio::WaveId& waveId, bool immediate);
    /// get array of currently active waves 
    const Util::Array<Ptr<CoreAudio::Wave> >& GetActiveWaves() const;

    /// set volume for a category
    //void SetCategoryVolume(const Util::StringAtom& category, float v);
    /// pause playback of a category
    //void PauseCategory(const Util::StringAtom& category);
    /// resume playback of a paused category
    //void ResumeCategory(const Util::StringAtom& category);
    /// stop playback of a category
    //void StopCategory(const Util::StringAtom& category);

    /// find sound bank and cue index for given cue id, return InvalidIndex if not found
    IndexT FindCue(const CoreAudio::CueId& cueId, Ptr<CoreAudio::SoundBank>& outSoundBank) const;
    /// find sound bank and wave index for given wave id, return InvalidIndex if not found
    IndexT FindWave(const CoreAudio::WaveId& waveId, Ptr<CoreAudio::SoundBank>& outSoundBank) const;

    /// set a global variable 
    void SetGlobalVariable(const Util::StringAtom& varName, float varValue);
    /// get global variable value
    float GetGlobalVariable(const Util::StringAtom& varName) const;

protected:
    /// garbage-collect stopped cues
    void GarbageCollectStoppedCues();
    /// garbage-collect stopped waves
    void GarbageCollectStoppedWaves();

    bool isOpen;
    IndexT adapterIndex;
    IO::URI globalSettingsUri;
    Util::Dictionary<Resources::ResourceId, Ptr<CoreAudio::SoundBank> > soundBanks;
    Util::Array<Ptr<CoreAudio::Cue>> activeCues;
    Util::Array<Ptr<CoreAudio::Wave>> activeWaves;
    Math::matrix44 listenerTransform;
    Math::vector listenerVelocity;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
AudioDeviceBase::IsOpen() const
{
    return this->isOpen;
}

//------------------------------------------------------------------------------
/**
*/
inline void
AudioDeviceBase::SetAdapter(IndexT i)
{
    this->adapterIndex = i;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT
AudioDeviceBase::GetAdapter() const
{
    return adapterIndex;
}

//------------------------------------------------------------------------------
/**
*/
inline void
AudioDeviceBase::SetGlobalSettingsURI(const IO::URI& uri)
{
    this->globalSettingsUri = uri;
}

//------------------------------------------------------------------------------
/**
*/
inline const IO::URI&
AudioDeviceBase::GetGlobalSettingsURI() const
{
    return this->globalSettingsUri;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
AudioDeviceBase::GetNumSoundBanks() const
{
    return this->soundBanks.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreAudio::SoundBank>&
AudioDeviceBase::GetSoundBankByIndex(IndexT i) const
{
    return this->soundBanks.ValueAtIndex(i);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
AudioDeviceBase::HasSoundBank(const Resources::ResourceId& resId) const
{
    return this->soundBanks.Contains(resId);
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreAudio::SoundBank>&
AudioDeviceBase::GetSoundBankByName(const Resources::ResourceId& resId) const
{
    return this->soundBanks[resId];
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
AudioDeviceBase::GetListenerTransform() const
{
    return this->listenerTransform;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::vector&
AudioDeviceBase::GetListenerVelocity() const
{
    return this->listenerVelocity;
}

} // namespace Base
//------------------------------------------------------------------------------
#endif
