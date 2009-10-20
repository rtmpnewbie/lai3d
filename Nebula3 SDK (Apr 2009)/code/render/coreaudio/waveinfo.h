#pragma once
#ifndef COREAUDIO_WAVEINFO_H
#define COREAUDIO_WAVEINFO_H
//------------------------------------------------------------------------------
/**
    @class CoreAudio::WaveInfo
  
    Cotains static properties of a Wave.
    
    (C) 2008 Radon Labs GmbH
*/    
#include "coreaudio/waveid.h"

//------------------------------------------------------------------------------
namespace CoreAudio
{
class WaveInfo
{
public:
    /// format enum
    enum Format
    {
        FormatPCM,
        FormatXMA,
        FormatADPCM,
        FormatWMA,

        InvalidFormat,
    };
    
    /// constructor
    WaveInfo();
    /// set wave id
    void SetWaveId(const WaveId& waveId);
    /// get wave id
    const WaveId& GetWaveId() const;
    /// set format
    void SetFormat(Format f);
    /// get format
    Format GetFormat() const;
    /// set bit depth
    void SetBitsPerSample(SizeT d);
    /// get bit depth
    SizeT GetBitsPerSample() const;
    /// set samples-per-second
    void SetSamplesPerSecond(SizeT s);
    /// get samples-per-second
    SizeT GetSamplesPerSecond() const;
    /// set number of channels
    void SetNumChannels(SizeT n);
    /// get number of channels
    SizeT GetNumChannels() const;
    /// convert format code to string
    static Util::String FormatToString(Format f);

private:
    SizeT samplesPerSecond;
    SizeT numChannels;
    SizeT bitsPerSample;
    WaveId waveId;
    Format format;
};

//------------------------------------------------------------------------------
/**
*/
inline
WaveInfo::WaveInfo() :
    samplesPerSecond(0),
    numChannels(0),
    bitsPerSample(0),
    waveId(0),
    format(InvalidFormat)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline void
WaveInfo::SetWaveId(const WaveId& id)
{
    this->waveId = id;
}

//------------------------------------------------------------------------------
/**
*/
inline const WaveId&
WaveInfo::GetWaveId() const
{
    return this->waveId;
}

//------------------------------------------------------------------------------
/**
*/
inline void
WaveInfo::SetFormat(Format f)
{
    this->format = f;
}

//------------------------------------------------------------------------------
/**
*/
inline WaveInfo::Format
WaveInfo::GetFormat() const
{
    return this->format;
}

//------------------------------------------------------------------------------
/**
*/
inline void
WaveInfo::SetBitsPerSample(SizeT d)
{
    this->bitsPerSample = d;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
WaveInfo::GetBitsPerSample() const
{
    return this->bitsPerSample;
}

//------------------------------------------------------------------------------
/**
*/
inline void
WaveInfo::SetSamplesPerSecond(SizeT s)
{
    this->samplesPerSecond = s;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
WaveInfo::GetSamplesPerSecond() const
{
    return this->samplesPerSecond;
}

//------------------------------------------------------------------------------
/**
*/
inline void
WaveInfo::SetNumChannels(SizeT n)
{
    this->numChannels = n;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
WaveInfo::GetNumChannels() const
{
    return this->numChannels;
}

//------------------------------------------------------------------------------
/**
*/
inline Util::String
WaveInfo::FormatToString(Format f)
{
    switch (f)
    {
        case FormatPCM:     return "PCM";
        case FormatXMA:     return "XMA";
        case FormatADPCM:   return "ADPCM";
        case FormatWMA:     return "WMA";
        default:            return "Unknown";
    }
}

} // namespace WaveInfo
//------------------------------------------------------------------------------
#endif
