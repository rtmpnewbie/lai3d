#pragma once
#ifndef WIN360_XACTAUDIODEVICE_H
#define WIN360_XACTAUDIODEVICE_H
//------------------------------------------------------------------------------
/**
    @class Win360::XACTAudioDevice
    
    XACT implementation of CoreAudio::AudioDevice.
    
    (C) 2008 Radon Labs GmbH
*/
#include "coreaudio/base/audiodevicebase.h"
#include "threading/event.h"

//------------------------------------------------------------------------------
namespace Win360
{
class XACTAudioDevice : public Base::AudioDeviceBase
{
    __DeclareClass(XACTAudioDevice);
    __DeclareSingleton(XACTAudioDevice);
public:
    /// constructor
    XACTAudioDevice();
    /// destructor
    virtual ~XACTAudioDevice();
    /// test if audio output is supported on this machine
    static bool CanCreate();

    /// get number of audio adapters
    SizeT GetNumAdapters() const;
    /// get info about an audio adapter, pass DefaultAdapter to query the default adapter
    CoreAudio::AudioAdapterInfo GetAdapterInfo(IndexT adapterIndex) const;

    /// open the audio device
    bool Open();
    /// close the audio device
    void Close();
    /// do per-frame work
    void OnFrame();

    /// get pointer to XACT engine
    IXACT3Engine* GetXACTEngine() const;
    /// wait until the wave bank is prepared
    void WaitForWaveBankPrepared();

    /// set a global variable 
    void SetGlobalVariable(const Util::StringAtom& varName, float varValue);
    /// get global variable value
    float GetGlobalVariable(const Util::StringAtom& varName) const;

private:
    static const SizeT MaxDstChannels = 16;
    friend class XACTCue;

    /// open XACT3
    static void OpenXACT();
    /// close XACT3
    void CloseXACT();
    /// XACT notification callback
    static void WINAPI NotificationCallback(const XACT_NOTIFICATION* notification);
    /// get index of default adapter
    IndexT GetDefaultAdapterIndex() const;
    /// load global settings file
    bool LoadGlobalSettings();
    /// unload global settings file
    void UnloadGlobalSettings();
    /// read/write access to the shared X3DAUDIO_DSP_SETTINGS structure
    X3DAUDIO_DSP_SETTINGS& X3DAudioDspSettings();

    static IXACT3Engine* xactEngine;
    void* globalSettingsBuffer;
    DWORD globalSettingsBufferSize;
    X3DAUDIO_DSP_SETTINGS dspSettings;
    FLOAT32 dspMatrixCoefficients[MaxDstChannels];
    FLOAT32 dspDelayTimes[MaxDstChannels];
    X3DAUDIO_HANDLE x3dAudioHandle;
    bool no3DSupport;
    Threading::Event waveBankPreparedEvent;
};

//------------------------------------------------------------------------------
/**
    NOTE: XACTCue may directly mess around with our embedded dsp settings
    structure. This is used directly as the result buffer for XACT3DCalculate!
*/
inline X3DAUDIO_DSP_SETTINGS&
XACTAudioDevice::X3DAudioDspSettings()
{
    return this->dspSettings;
}

} // namespace XACTAudioDevice    
//------------------------------------------------------------------------------
#endif

    