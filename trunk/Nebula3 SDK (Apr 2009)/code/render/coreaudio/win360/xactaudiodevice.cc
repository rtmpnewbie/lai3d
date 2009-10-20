//------------------------------------------------------------------------------
//  xactaudiodevice.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coreaudio/win360/xactaudiodevice.h"
#include "coreaudio/config.h"
#include "io/stream.h"
#include "io/ioserver.h"

namespace Win360
{
__ImplementClass(Win360::XACTAudioDevice, 'XAUD', Core::RefCounted);
__ImplementSingleton(Win360::XACTAudioDevice);

IXACT3Engine* XACTAudioDevice::xactEngine = 0;

using namespace Util;
using namespace IO;
using namespace CoreAudio;

//------------------------------------------------------------------------------
/**
*/
XACTAudioDevice::XACTAudioDevice() :
    globalSettingsBuffer(0),
    globalSettingsBufferSize(0),
    no3DSupport(false)
{
    __ConstructSingleton;
    this->globalSettingsUri = "audio:globals.xgs";
    Memory::Clear(&this->x3dAudioHandle, sizeof(this->x3dAudioHandle));
    Memory::Clear(&this->dspSettings, sizeof(this->dspSettings));
    Memory::Clear(&this->dspMatrixCoefficients, sizeof(this->dspMatrixCoefficients));
    Memory::Clear(&this->dspDelayTimes, sizeof(this->dspDelayTimes));
    this->OpenXACT();
}

//------------------------------------------------------------------------------
/**
*/
XACTAudioDevice::~XACTAudioDevice()
{
    if (this->IsOpen())
    {
        this->Close();
    }
    this->CloseXACT();
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
    Get a pointer to the XACT engine. Creates the XACT engine object if
    it doesn't exist yet.
*/
IXACT3Engine*
XACTAudioDevice::GetXACTEngine() const
{
    n_assert(0 != this->xactEngine);
    return this->xactEngine;
}

//------------------------------------------------------------------------------
/**
    Test if this machine provides audio output.
*/
bool
XACTAudioDevice::CanCreate()
{
    #if __XBOX360__
        // audio always works on the 360
        return true;
    #else
        // Win32: check if at least one valid audio renderer exists
        OpenXACT();
        if (0 != xactEngine)
        {
            XACTINDEX rendererCount = 0;
            HRESULT hr = xactEngine->GetRendererCount(&rendererCount);
            n_assert(SUCCEEDED(hr));
            if (rendererCount > 0)
            {
                return true;
            }
        }
        // fallthrough: no valid audio renderers
        return false;
    #endif
}

//------------------------------------------------------------------------------
/**
    Creates the XACT engine object which will already happen in the 
    constructor of XACTAudioDevice.. This will not actually initialize the
    engine for use, as this happens inside Open(). Before Open() the application
    has the chance to query and select an audio adapter to use (on Xbox360
    there are no multiple audio adapters, so selecting an audio adapter
    does nothing).
*/
void
XACTAudioDevice::OpenXACT()
{
    if (0 == xactEngine)
    {
        HRESULT hr;            
    #if __WIN32__
    	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    #endif
        DWORD creationFlags = 0;
        #if NEBULA3_XACT_AUDITIONMODE
            creationFlags |= XACT_FLAG_API_AUDITION_MODE;
        #endif
        #if NEBULA3_XACT_DEBUGMODE
            creationFlags |= XACT_FLAG_API_DEBUG_MODE;
        #endif
        hr = XACT3CreateEngine(creationFlags, &xactEngine);
        n_assert(SUCCEEDED(hr));
        n_assert(0 != xactEngine);
    }
}

//------------------------------------------------------------------------------
/**
    Close the XACT engine. This method is exclusively called by the destructor.
*/
void
XACTAudioDevice::CloseXACT()
{
    n_assert(!this->IsOpen());
    if (0 != xactEngine)
    {
        xactEngine->Release();
        xactEngine = 0;
    }
}

//------------------------------------------------------------------------------
/**
    This method performs per-frame-work in the audio system and must
    be called in regular intervalls (like once per frame or so).
*/
void
XACTAudioDevice::OnFrame()
{
    // call parent class
    AudioDeviceBase::OnFrame();

    // perform own per-frame-stuff
    n_assert(0 != this->xactEngine);
    HRESULT hr = this->xactEngine->DoWork();
    n_assert(SUCCEEDED(hr));
}

//------------------------------------------------------------------------------
/**
    Returns number of audio devices in the system. This method may be
    called before the Open() method to query the audio adapters
    in the system and optionally to select a different audio adapter.
*/
SizeT
XACTAudioDevice::GetNumAdapters() const
{
    n_assert(0 != this->xactEngine);
    XACTINDEX rendererCount = 0;
    HRESULT hr = this->xactEngine->GetRendererCount(&rendererCount);
    n_assert(SUCCEEDED(hr));
    return rendererCount;
}

//------------------------------------------------------------------------------
/**
    Returns the index of the default audio adapter. IF there is no default
    adapter in the system, the method returns InvalidIndex (this
    shouldn't happen though).
*/
IndexT
XACTAudioDevice::GetDefaultAdapterIndex() const
{
    n_assert(0 != this->xactEngine);
    n_assert(this->GetNumAdapters() > 0);
    XACT_RENDERER_DETAILS xactRendererDetails;
    IndexT i;
    SizeT num = this->GetNumAdapters();
    for (i = 0; i < num; i++)
    {
        Memory::Clear(&xactRendererDetails, sizeof(xactRendererDetails));
        HRESULT hr = this->xactEngine->GetRendererDetails((XACTINDEX)i, &xactRendererDetails);
        n_assert(SUCCEEDED(hr));
        if (xactRendererDetails.defaultDevice)
        {
            return i;
        }
    }
    return InvalidIndex;
}

//------------------------------------------------------------------------------
/**
    Returns an AudioAdapterInfo object with details about an audio 
    device in the system. DefaultAdapter can be passed to this method
    to query the adapter id and name of the default adapter.
*/
AudioAdapterInfo
XACTAudioDevice::GetAdapterInfo(IndexT adpIndex) const
{
    n_assert(0 != this->xactEngine);
    n_assert(this->GetNumAdapters() > 0);
    HRESULT hr;

    // if the default adapter is requested we need to find it first
    if (DefaultAdapter == adpIndex)
    {
        adpIndex = this->GetDefaultAdapterIndex();
        n_assert(InvalidIndex != adpIndex);
    }
    n_assert(adpIndex < this->GetNumAdapters());

    XACT_RENDERER_DETAILS xactRendererDetails = { 0 };
    hr = this->xactEngine->GetRendererDetails((XACTINDEX)adpIndex, &xactRendererDetails);
    n_assert(SUCCEEDED(hr));

    // convert names from wide to ANSI
    char rendererId[XACT_RENDERER_ID_LENGTH + 1];
    char displayName[XACT_RENDERER_NAME_LENGTH + 1];
    WideCharToMultiByte(CP_ACP, 0, xactRendererDetails.rendererID, -1, rendererId, sizeof(rendererId), NULL, NULL);
    WideCharToMultiByte(CP_ACP, 0, xactRendererDetails.displayName, -1, displayName, sizeof(displayName), NULL, NULL);

    // fill AudioAdapterInfo object
    AudioAdapterInfo info;
    info.SetAdapterId(rendererId);
    info.SetDisplayName(displayName);
    info.SetDefaultAdapter(TRUE == xactRendererDetails.defaultDevice);
    return info;
}

//------------------------------------------------------------------------------
/**
    Load the global settings file into a private buffer. Failing to load
    the settings is not considered a hard error (since XACT works fine
    without them).
*/
bool
XACTAudioDevice::LoadGlobalSettings()
{
    n_assert(this->globalSettingsUri.IsValid());
    n_assert(0 == this->globalSettingsBuffer);

    Ptr<Stream> stream = IoServer::Instance()->CreateStream(this->globalSettingsUri);
    stream->SetAccessMode(Stream::ReadAccess);
    if (stream->Open())
    {
        this->globalSettingsBufferSize = stream->GetSize();
        n_assert(this->globalSettingsBufferSize > 0);
        this->globalSettingsBuffer = Memory::Alloc(Memory::ResourceHeap, this->globalSettingsBufferSize);
        SizeT bytesRead = stream->Read(this->globalSettingsBuffer, this->globalSettingsBufferSize);
        n_assert(bytesRead == (SizeT)this->globalSettingsBufferSize);
        stream->Close();
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
    Unload the global settings buffer.
*/
void
XACTAudioDevice::UnloadGlobalSettings()
{
    if (0 != this->globalSettingsBuffer)
    {
        Memory::Free(Memory::ResourceHeap, this->globalSettingsBuffer);
        this->globalSettingsBuffer = 0;
        this->globalSettingsBufferSize = 0;
    }
}

//------------------------------------------------------------------------------
/**
    Initialize the XACT audio engine using the selected audio adapter.
*/
bool
XACTAudioDevice::Open()
{
    n_assert(!this->IsOpen());
    n_assert(0 != this->xactEngine);
    HRESULT hr;

    // call parent class
    AudioDeviceBase::Open();

    // first read the global settings
    if (!this->LoadGlobalSettings())
    {
        // if settings could not be loaded we can still continue with default settings!
        n_warning("XACTAudioDevice failed to load global settings from '%s'!\n", 
            this->globalSettingsUri.AsString().AsCharPtr());
    }

    // get renderer details for audio renderer to use
    IndexT adpIndex = this->adapterIndex;
    if (DefaultAdapter == adpIndex)
    {
        adpIndex = this->GetDefaultAdapterIndex();
    }    
    XACT_RENDERER_DETAILS xactRendererDetails = { 0 };
    hr = this->xactEngine->GetRendererDetails((XACTINDEX)adpIndex, &xactRendererDetails);
    n_assert(SUCCEEDED(hr));

    // initialize the XACT engine
    XACT_RUNTIME_PARAMETERS xactRuntimeParams = { 0 };
    xactRuntimeParams.lookAheadTime = XACT_ENGINE_LOOKAHEAD_DEFAULT;
    xactRuntimeParams.pGlobalSettingsBuffer = this->globalSettingsBuffer;
    xactRuntimeParams.globalSettingsBufferSize = this->globalSettingsBufferSize;
    xactRuntimeParams.pRendererID = xactRendererDetails.rendererID;
    xactRuntimeParams.fnNotificationCallback = NotificationCallback;
    hr = this->xactEngine->Initialize(&xactRuntimeParams);
    if (FAILED(hr))
    {
        n_error("XACTAudioDevice: Failed to initialize XACT engine!");
        return false;
    }

    // setup the dsp settings for 3d audio
    Memory::Clear(&(this->dspSettings), sizeof(this->dspSettings));
    WAVEFORMATEXTENSIBLE wfxFinalMixFormat = { 0 };
    hr = this->xactEngine->GetFinalMixFormat(&wfxFinalMixFormat);
    n_assert(SUCCEEDED(hr));
    n_assert(wfxFinalMixFormat.Format.nChannels <= MaxDstChannels);
    this->dspSettings.pMatrixCoefficients = this->dspMatrixCoefficients;
    this->dspSettings.pDelayTimes = this->dspDelayTimes;
    this->dspSettings.SrcChannelCount = 1;
    this->dspSettings.DstChannelCount = wfxFinalMixFormat.Format.nChannels;

    // initialize the XACT3D functions (3D only works if global settings are provided)
    this->no3DSupport = false;
    hr = XACT3DInitialize(this->xactEngine, this->x3dAudioHandle);
    if (FAILED(hr))
    {
        this->no3DSupport = true;
    }

    // register notifications
    XACT_NOTIFICATION_DESCRIPTION notifyDesc = { 0 };
    notifyDesc.flags = XACT_FLAG_NOTIFICATION_PERSIST;
    notifyDesc.type  = XACTNOTIFICATIONTYPE_WAVEBANKPREPARED;
    notifyDesc.pvContext = this;
    hr = this->xactEngine->RegisterNotification(&notifyDesc);
    n_assert(SUCCEEDED(hr));

    // done
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
XACTAudioDevice::Close()
{
    n_assert(this->IsOpen());
    n_assert(0 != this->xactEngine);

    // first call parent, this will discard all sound banks
    AudioDeviceBase::Close();

    // shutdown the XACT engine (it is actually released in the destructor!)
    this->xactEngine->ShutDown();
}

//------------------------------------------------------------------------------
/**
    This method waits for the waveBankPrepared event to be set from
    within the NotificationCallback method.
*/
void
XACTAudioDevice::WaitForWaveBankPrepared()
{
    while (!this->waveBankPreparedEvent.WaitTimeout(10))
    {
        this->xactEngine->DoWork();
    }
}

//------------------------------------------------------------------------------
/**
    The notification call back is called by XACT to signal various events.
*/
void
XACTAudioDevice::NotificationCallback(const XACT_NOTIFICATION* notification)
{
    if ((0 != notification) && (0 != notification->pvContext))
    {
        XACTAudioDevice* self = (XACTAudioDevice*) notification->pvContext;
        if (notification->type == XACTNOTIFICATIONTYPE_WAVEBANKPREPARED)
        {
            self->waveBankPreparedEvent.Signal();
        }
    }
}

//------------------------------------------------------------------------------
/**
    Set a global variable
*/
void
XACTAudioDevice::SetGlobalVariable(const Util::StringAtom& varName, float varValue)
{
    // first find variable
    XACTVARIABLEINDEX varIndex = this->xactEngine->GetGlobalVariableIndex(varName.Value().AsCharPtr());

    // set the value
    this->xactEngine->SetGlobalVariable(varIndex, varValue);
}

//------------------------------------------------------------------------------
/**
    Get a global variable
*/
float
XACTAudioDevice::GetGlobalVariable(const Util::StringAtom& varName) const
{
    // first find variable
    XACTVARIABLEINDEX varIndex = this->xactEngine->GetGlobalVariableIndex(varName.Value().AsCharPtr());

    // get the value
    XACTVARIABLEVALUE varValue;
    this->xactEngine->GetGlobalVariable(varIndex, &varValue);

    return (float)varValue;
}
} // namespace Win360
