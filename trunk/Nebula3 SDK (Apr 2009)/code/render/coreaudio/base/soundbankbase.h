#pragma once
#ifndef BASE_SOUNDBANKBASE_H
#define BASE_SOUNDBANKBASE_H
//------------------------------------------------------------------------------
/**
    @class Base::SoundBankBase

    This is an abstract wrapper for XACT's WaveBanks/SoundBanks. The sound
    bank allows to lookup sound properties without actually creating
    a Cue or Wave object.

    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "resources/resourceid.h"
#include "coreaudio/cueinfo.h"
#include "coreaudio/waveinfo.h"

//------------------------------------------------------------------------------
namespace Base
{
class SoundBankBase : public Core::RefCounted
{
    __DeclareClass(SoundBankBase);
public:
    /// constructor
    SoundBankBase();
    /// destructor
    virtual ~SoundBankBase();
    
    /// setup the sound bank
    bool Setup(const Resources::ResourceId& resId, bool streaming);
    /// discard the sound bank
    void Discard();
    /// return true if the sound bank has been setup
    bool IsValid() const;

    /// get resource id of sound bank
    const Resources::ResourceId& GetResourceId() const;
    /// return true if this is a streaming bank
    bool IsStreaming() const;
    
    /// get number of cues in the sound bank
    SizeT GetNumCues() const;
    /// get info about a cue in the sound bank
    const CoreAudio::CueInfo& GetCueInfo(IndexT cueIndex) const;
    /// check if sound bank contains a cue
    bool HasCue(const CoreAudio::CueId& cueId) const;
    /// find cue index by cue id, returns InvalidIndex if not found
    IndexT FindCueIndexByName(const CoreAudio::CueId& cueId) const;

    /// get number of waves in the sound bank
    SizeT GetNumWaves() const;
    /// get info about a wave in the sound bank
    const CoreAudio::WaveInfo& GetWaveInfo(IndexT waveIndex) const;
    /// check if sound bank contains a wave
    bool HasWave(const CoreAudio::WaveId& waveId) const;
    /// find wave index by wave id, returns InvalidIndex if not found
    IndexT FindWaveIndexByName(const CoreAudio::WaveId& waveId) const;

    /// stop all matching cue instances
    void StopCue(IndexT cueIndex, bool immediate);
    /// stop all matching wave instances
    void StopWave(IndexT waveIndex, bool immediate);

protected:
    Resources::ResourceId resourceId;
    bool isValid;
    bool isStreaming;
    Util::Array<CoreAudio::CueInfo> cueInfos;
    Util::Array<CoreAudio::WaveInfo> waveInfos;
    Util::Dictionary<CoreAudio::CueId, IndexT> cueIndexMap;
    Util::Dictionary<CoreAudio::WaveId, IndexT> waveIndexMap;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
SoundBankBase::IsValid() const
{
    return this->isValid;
}

//------------------------------------------------------------------------------
/**
*/
inline const Resources::ResourceId&
SoundBankBase::GetResourceId() const
{
    return this->resourceId;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
SoundBankBase::IsStreaming() const
{
    return this->isStreaming;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
SoundBankBase::GetNumCues() const
{
    return this->cueInfos.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline const CoreAudio::CueInfo&
SoundBankBase::GetCueInfo(IndexT cueIndex) const
{
    return this->cueInfos[cueIndex];
}

//------------------------------------------------------------------------------
/**
*/
inline bool
SoundBankBase::HasCue(const CoreAudio::CueId& cueId) const
{
    return this->cueIndexMap.Contains(cueId);
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT
SoundBankBase::FindCueIndexByName(const CoreAudio::CueId& cueId) const
{
    IndexT mapIndex = this->cueIndexMap.FindIndex(cueId);
    if (InvalidIndex != mapIndex)
    {
        return this->cueIndexMap.ValueAtIndex(mapIndex);
    }
    else
    {
        return InvalidIndex;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
SoundBankBase::GetNumWaves() const
{
    return this->waveInfos.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline const CoreAudio::WaveInfo&
SoundBankBase::GetWaveInfo(IndexT waveIndex) const
{
    return this->waveInfos[waveIndex];
}

//------------------------------------------------------------------------------
/**
*/
inline bool
SoundBankBase::HasWave(const CoreAudio::WaveId& waveId) const
{
    return this->waveIndexMap.Contains(waveId);
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT
SoundBankBase::FindWaveIndexByName(const CoreAudio::WaveId& waveId) const
{
    IndexT mapIndex = this->waveIndexMap.FindIndex(waveId);
    if (InvalidIndex == mapIndex)
    {
        return this->waveIndexMap.ValueAtIndex(mapIndex);
    }
    else
    {
        return InvalidIndex;
    }
}

} // namespace Base
//------------------------------------------------------------------------------
#endif