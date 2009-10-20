#pragma once
#ifndef AUDIO_AUDIOSERVER_H
#define AUDIO_AUDIOSERVER_H
//------------------------------------------------------------------------------
/**
    @class Audio::AudioServer

    Client-side front-end of the Audio subsystem. Initializes the audio 
    subsystem and manages sound banks.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "coreaudio/audiodevice.h"
#include "audio/audioemitter.h"
#include "audio/audiolistener.h"

//------------------------------------------------------------------------------
namespace Audio
{
class AudioServer : public Core::RefCounted
{
    __DeclareClass(AudioServer);
    __DeclareSingleton(AudioServer);
public:
    /// the default adapter index
    static const IndexT DefaultAdapter = CoreAudio::AudioDevice::DefaultAdapter;

    /// constructor
    AudioServer();
    /// destructor
    virtual ~AudioServer();
    
    /// get info about installed audio adapters (waits for completion)
    Util::Array<CoreAudio::AudioAdapterInfo> GetAdapterInfos() const;
    /// select an audio adapter (use default adapter if this method is not called)
    void SetAdapterIndex(IndexT adapterIndex);
    /// get adapter index to use for audio output
    IndexT GetAdapterIndex() const;
    
    /// set URI of global settings file
    void SetGlobalSettingsURI(const IO::URI& uri);
    /// get URI of global settings file
    const IO::URI& GetGlobalSettingsURI() const;

    /// open the audio subsystem (waits for completion)
    void Open();
    /// close the audio subsystem (waits for completion)
    void Close();
    /// return true if the audio subsystem is open
    bool IsOpen() const;
    /// called per-frame 
    void OnFrame();

    /// load a sound bank
    bool LoadSoundBank(const Resources::ResourceId& resId, bool streaming);
    /// unload a sound bank
    void UnloadSoundBank(const Resources::ResourceId& resId);

private:
    friend class AudioEmitter;

    /// register an AudioEmitter (called from AudioEmitter::Setup)
    void RegisterAudioEmitter(const Ptr<AudioEmitter>& audioEmitter);
    /// unregister an AudioEmitter (called from AudioEmitter::Discard)
    void UnregisterAudioEmitter(const Ptr<AudioEmitter>& audioEmitter);

    bool isOpen;
    IndexT adapterIndex;
    IO::URI globalSettingsURI;
    Util::Array<Ptr<AudioEmitter>> audioEmitters;
    Ptr<AudioListener> audioListener;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
AudioServer::IsOpen() const
{
    return this->isOpen;
}

//------------------------------------------------------------------------------
/**
*/
inline void
AudioServer::SetGlobalSettingsURI(const IO::URI& uri)
{
    this->globalSettingsURI = uri;
}

//------------------------------------------------------------------------------
/**
*/
inline const IO::URI&
AudioServer::GetGlobalSettingsURI() const
{
    return this->globalSettingsURI;
}

//------------------------------------------------------------------------------
/**
*/
inline void
AudioServer::SetAdapterIndex(IndexT i)
{
    this->adapterIndex = i;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT
AudioServer::GetAdapterIndex() const
{
    return this->adapterIndex;
}

} // namespace Audio
//------------------------------------------------------------------------------
#endif
    