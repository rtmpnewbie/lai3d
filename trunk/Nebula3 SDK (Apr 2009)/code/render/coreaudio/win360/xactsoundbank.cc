//------------------------------------------------------------------------------
//  xactsoundbank.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coreaudio/win360/xactsoundbank.h"
#include "coreaudio/win360/xactaudiodevice.h"
#include "io/ioserver.h"
#include "io/win360/win360fswrapper.h"
#include "timing/time.h"

namespace Win360
{
__ImplementClass(Win360::XACTSoundBank, 'XSBK', Base::SoundBankBase);

using namespace IO;
using namespace Util;
using namespace Resources;
using namespace Base;
using namespace CoreAudio;

//------------------------------------------------------------------------------
/**
*/
XACTSoundBank::XACTSoundBank() :
    xactSoundBank(0),
    xactWaveBank(0),
    soundBankData(0),
    waveBankData(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
XACTSoundBank::~XACTSoundBank()
{
    n_assert(!this->IsValid());
}

//------------------------------------------------------------------------------
/**
*/
bool
XACTSoundBank::Setup(const ResourceId& resId, bool streaming)
{
    n_assert(!this->IsValid());
    n_assert(this->cueInfos.IsEmpty());
    n_assert(this->cueIndexMap.IsEmpty());
    n_assert(this->waveInfos.IsEmpty());
    n_assert(this->waveIndexMap.IsEmpty());

    // call parent class
    SoundBankBase::Setup(resId, streaming);

    // setup XACT sound and wave banks
    if (!this->SetupWaveBank())
    {
        return false;
    }
    if (!this->SetupSoundBank())
    {
        return false;
    }

    // populate cue and wave info arrays
    HRESULT hr;
    XACTINDEX numCues = 0;
    hr = this->xactSoundBank->GetNumCues(&numCues);
    n_assert(SUCCEEDED(hr));
    if (numCues > 0)
    {
        this->cueInfos.Reserve(numCues);
        this->cueIndexMap.Reserve(numCues);
        IndexT cueIndex;
        for (cueIndex = 0; cueIndex < numCues; cueIndex++)
        {
            XACT_CUE_PROPERTIES xactCueProps;
            hr = this->xactSoundBank->GetCueProperties((XACTINDEX)cueIndex, &xactCueProps);
            n_assert(SUCCEEDED(hr));

            CueInfo cueInfo;
            cueInfo.SetCueId(CueId(xactCueProps.friendlyName));
            cueInfo.SetInteractive(TRUE == xactCueProps.interactive);
            cueInfo.SetNumVariations(xactCueProps.numVariations);
            cueInfo.SetMaxNumInstances(xactCueProps.maxInstances);

            this->cueInfos.Append(cueInfo);
            this->cueIndexMap.Add(cueInfo.GetCueId(), this->cueInfos.Size() - 1);
        }
    }

    XACTINDEX numWaves = 0;
    hr = this->xactWaveBank->GetNumWaves(&numWaves);
    n_assert(SUCCEEDED(hr));
    if (numWaves > 0)
    {
        this->waveInfos.Reserve(numWaves);
        this->waveIndexMap.Reserve(numWaves);
        IndexT waveIndex;
        for (waveIndex = 0; waveIndex < numWaves; waveIndex++)
        {
            XACT_WAVE_PROPERTIES xactWaveProps;
            hr = this->xactWaveBank->GetWaveProperties((XACTINDEX)waveIndex, &xactWaveProps);
            n_assert(SUCCEEDED(hr));

            WaveInfo waveInfo;
            waveInfo.SetWaveId(WaveId(xactWaveProps.friendlyName));
            switch (xactWaveProps.format.wFormatTag)
            {
                case WAVEBANKMINIFORMAT_TAG_PCM:    waveInfo.SetFormat(WaveInfo::FormatPCM); break;
                case WAVEBANKMINIFORMAT_TAG_XMA:    waveInfo.SetFormat(WaveInfo::FormatXMA); break;
                case WAVEBANKMINIFORMAT_TAG_ADPCM:  waveInfo.SetFormat(WaveInfo::FormatADPCM); break;
                case WAVEBANKMINIFORMAT_TAG_WMA:    waveInfo.SetFormat(WaveInfo::FormatWMA); break;
            }
            if (WAVEBANKMINIFORMAT_BITDEPTH_8 == xactWaveProps.format.wBitsPerSample)
            {
                waveInfo.SetBitsPerSample(8);
            }
            else
            {
                waveInfo.SetBitsPerSample(16);
            }
            waveInfo.SetSamplesPerSecond(xactWaveProps.format.nSamplesPerSec);
            waveInfo.SetNumChannels(xactWaveProps.format.nChannels);

            this->waveInfos.Append(waveInfo);
            this->waveIndexMap.Add(waveInfo.GetWaveId(), this->waveInfos.Size() - 1);
        }   
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
XACTSoundBank::Discard()
{
    n_assert(this->IsValid());
    this->DiscardSoundBank();
    this->DiscardWaveBank();
    SoundBankBase::Discard();
}

//------------------------------------------------------------------------------
/**
*/
bool
XACTSoundBank::SetupWaveBank()
{
    n_assert(0 == this->xactWaveBank);
    n_assert(0 == this->waveBankData);
    n_assert(this->resourceId.IsValid());
    IXACT3Engine* xactEngine = XACTAudioDevice::Instance()->GetXACTEngine();
    n_assert(0 != xactEngine);
    HRESULT hr;

    // different setup for streaming and non-streaming wavebanks
    if (this->isStreaming)
    {
        // for streaming wave banks, we need to open the file with Win32 file functions directly!
        String path;
        path.Format("stream:%s.xwb", this->resourceId.Value().AsCharPtr());
        String absPath = AssignRegistry::Instance()->ResolveAssigns(path).LocalPath();

        XACT_STREAMING_PARAMETERS xactStreamParams = { 0 };
        xactStreamParams.file = Win360::Win360FSWrapper::OpenFile(absPath, Stream::ReadAccess, Stream::Random, FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING);
        if (0 == xactStreamParams.file)
        {
            n_error("XACTSoundBank: Failed to open streaming wavebank '%s'!", absPath.AsCharPtr());
            return false;
        }
        xactStreamParams.offset     = 0;
        xactStreamParams.flags      = 0;
        xactStreamParams.packetSize = 64;   // this is >100ms for CD quality
        hr = xactEngine->CreateStreamingWaveBank(&xactStreamParams, &this->xactWaveBank);
        n_assert(SUCCEEDED(hr));
        n_assert(0 != this->xactWaveBank);
    }
    else
    {
        // for in-memory-wavebanks, read the file into a memory buffer conventionally
        String path;
        path.Format("audio:%s.xwb", this->resourceId.Value().AsCharPtr());
        Ptr<Stream> stream = IoServer::Instance()->CreateStream(path);
        stream->SetAccessMode(Stream::ReadAccess);
        stream->SetAccessPattern(Stream::Sequential);
        if (stream->Open())
        {
            // read wave bank file into memory buffer
            SizeT size = stream->GetSize();
            #if __XBOX360__
                void* scratchBuffer = Memory::Alloc(Memory::ScratchHeap, size); 
                this->waveBankData = Memory::Alloc(Memory::Xbox360AudioHeap, size);             
                SizeT bytesRead = stream->Read(scratchBuffer, size);
                Memory::CopyToGraphicsMemory(scratchBuffer, this->waveBankData, size);
                Memory::Free(Memory::ScratchHeap, scratchBuffer);
                scratchBuffer = 0;
            #else
                this->waveBankData = Memory::Alloc(Memory::ResourceHeap, size);
                SizeT bytesRead = stream->Read(this->waveBankData, size);
            #endif
            n_assert(bytesRead == size);
            stream->Close();
            stream = 0;

            // create XACT wave bank
            hr = xactEngine->CreateInMemoryWaveBank(this->waveBankData,     // pvBuffer
                                                    size,                   // dwSize
                                                    0,                      // dwFlags
                                                    0,                      // dwAllocAttributes
                                                    &(this->xactWaveBank)); // ppWaveBank
            n_assert(SUCCEEDED(hr));
            n_assert(0 != this->xactWaveBank);
        }
        else
        {
            n_error("XACTSoundBank: failed to open in-memory wavebank '%s'!", path.AsCharPtr());
            return false;
        }
    }

    // we need to wait until the wave bank is prepared before calling methods on it...
    XACTAudioDevice::Instance()->WaitForWaveBankPrepared();
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
XACTSoundBank::DiscardWaveBank()
{
    n_assert(0 != this->xactWaveBank);

    // destroy the wave bank
    this->xactWaveBank->Destroy();
    this->xactWaveBank = 0;

    // if this is a non-streaming wave bank, also release the in-memory buffer
    if (0 != this->waveBankData)
    {
        #if __XBOX360__
            Memory::Free(Memory::Xbox360AudioHeap, this->waveBankData); 
        #else
            Memory::Free(Memory::ResourceHeap, this->waveBankData);
        #endif
        this->waveBankData = 0;
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
XACTSoundBank::SetupSoundBank()
{
    n_assert(0 == this->xactSoundBank);
    n_assert(0 == this->soundBankData);
    n_assert(this->resourceId.IsValid());
    IXACT3Engine* xactEngine = XACTAudioDevice::Instance()->GetXACTEngine();
    n_assert(0 != xactEngine);
    HRESULT hr;

    // read sound-bank file into a memory buffer
    String path;
    path.Format("audio:%s.xsb", this->resourceId.Value().AsCharPtr());
    Ptr<Stream> stream = IoServer::Instance()->CreateStream(path);
    stream->SetAccessMode(Stream::ReadAccess);
    stream->SetAccessPattern(Stream::Sequential);
    if (stream->Open())
    {
        // read wave bank file into memory buffer
        SizeT size = stream->GetSize();
        this->soundBankData = Memory::Alloc(Memory::ResourceHeap, size);
        SizeT bytesRead = stream->Read(this->soundBankData, size);
        n_assert(bytesRead == size);
        stream->Close();
        stream = 0;

        // create XACT sound bank
        hr = xactEngine->CreateSoundBank(this->soundBankData,       // pvBuffer
                                         size,                      // dwSize
                                         0,                         // dwFlags
                                         0,                         // dwAllocAttributes
                                         &(this->xactSoundBank));   // ppSoundBank
        n_assert(SUCCEEDED(hr));
        n_assert(0 != this->xactSoundBank);
        
        return true;
    }
    else
    {
        n_error("XACTSoundBank: failed to open soundbank '%s'!", path.AsCharPtr());
        return false;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
XACTSoundBank::DiscardSoundBank()
{
    n_assert(0 != this->xactSoundBank);
    n_assert(0 != this->soundBankData);

    // destroy XACT sound bank object
    this->xactSoundBank->Destroy();
    this->xactSoundBank = 0;

    // free sound bank data buffer
    Memory::Free(Memory::ResourceHeap, this->soundBankData);
    this->soundBankData = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
XACTSoundBank::StopCue(IndexT cueIndex, bool immediate)
{
    n_assert(0 != this->xactSoundBank);
    DWORD dwFlags = immediate ? XACT_FLAG_SOUNDBANK_STOP_IMMEDIATE : 0;
    HRESULT hr = this->xactSoundBank->Stop((XACTINDEX)cueIndex, dwFlags);
    n_assert(SUCCEEDED(hr));
}

//------------------------------------------------------------------------------
/**
*/
void
XACTSoundBank::StopWave(IndexT waveIndex, bool immediate)
{
    n_assert(0 != this->xactWaveBank);

    // hmm, does flags=0 mean anything valid? the docs are not clear there...
    DWORD dwFlags = immediate ? XACT_FLAG_STOP_IMMEDIATE : XACT_FLAG_STOP_RELEASE;
    HRESULT hr = this->xactWaveBank->Stop((XACTINDEX)waveIndex, dwFlags);
    n_assert(SUCCEEDED(hr));
}

} // namespace Win360
