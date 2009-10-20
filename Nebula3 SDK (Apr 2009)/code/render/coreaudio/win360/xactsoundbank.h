#pragma once
#ifndef WIN360_XACTSOUNDBANK_H
#define WIN360_XACTSOUNDBANK_H
//------------------------------------------------------------------------------
/**
    @class Win360::XACTSoundBank
    
    Encapsulates an XACT wave bank and an associated XACT sound bank.
    
    (C) 2008 Radon Labs GmbH
*/
#include "coreaudio/base/soundbankbase.h"

//------------------------------------------------------------------------------
namespace Win360
{
class XACTSoundBank : public Base::SoundBankBase
{
    __DeclareClass(XACTSoundBank);
public:
    /// constructor
    XACTSoundBank();
    /// destructor
    virtual ~XACTSoundBank();
    
    /// setup the sound bank
    bool Setup(const Resources::ResourceId& resId, bool streaming);
    /// discard the sound bank
    void Discard();

    /// stop all matching cue instances
    void StopCue(IndexT cueIndex, bool immediate);
    /// stop all matching wave instances
    void StopWave(IndexT waveIndex, bool immediate);

    /// get pointer to XACT sound bank
    IXACT3SoundBank* GetXACTSoundBank() const;
    /// get pointer to XACT wave bank
    IXACT3WaveBank* GetXACTWaveBank() const;

private:
    /// setup the XACT wave bank (called from Setup)
    bool SetupWaveBank();
    /// setup the sound bank (called from Setup)
    bool SetupSoundBank();
    /// discard the XACT wave bank (called from Discard)
    void DiscardWaveBank();
    /// discard the XACT sound bank (called from Discard)
    void DiscardSoundBank();

    IXACT3SoundBank* xactSoundBank;
    IXACT3WaveBank* xactWaveBank;
    void* soundBankData;
    void* waveBankData;
};

//------------------------------------------------------------------------------
/**
*/
inline IXACT3SoundBank*
XACTSoundBank::GetXACTSoundBank() const
{
    n_assert(0 != this->xactSoundBank);
    return this->xactSoundBank;
}

//------------------------------------------------------------------------------
/**
*/
inline IXACT3WaveBank*
XACTSoundBank::GetXACTWaveBank() const
{
    n_assert(0 != this->xactWaveBank);
    return this->xactWaveBank;
}

} // namespace Win360
//------------------------------------------------------------------------------
#endif
