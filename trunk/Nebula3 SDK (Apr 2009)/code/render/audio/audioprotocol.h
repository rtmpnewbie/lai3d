#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "messaging/message.h"
#include "io/uri.h"
#include "util/array.h"
#include "coreaudio/audioadapterinfo.h"
#include "resources/resourceid.h"
#include "coreaudio/cueid.h"
#include "coreaudio/waveid.h"
#include "coreaudio/soundstate.h"
#include "math/matrix44.h"
#include "math/vector.h"
#include "audio/cuehandle.h"
#include "audio/wavehandle.h"
#include "audio/audioserver.h"

//------------------------------------------------------------------------------
namespace Audio
{
//------------------------------------------------------------------------------
class SetupAudio : public Messaging::Message
{
    __DeclareClass(SetupAudio);
    __DeclareMsgId;
public:
    SetupAudio() :
        adapterindex(AudioServer::DefaultAdapter)
    { };
public:
    void SetAdapterIndex(IndexT val)
    {
        n_assert(!this->handled);
        this->adapterindex = val;
    };
    IndexT GetAdapterIndex() const
    {
        return this->adapterindex;
    };
private:
    IndexT adapterindex;
public:
    void SetGlobalSettingsURI(const IO::URI& val)
    {
        n_assert(!this->handled);
        this->globalsettingsuri = val;
    };
    const IO::URI& GetGlobalSettingsURI() const
    {
        return this->globalsettingsuri;
    };
private:
    IO::URI globalsettingsuri;
};
//------------------------------------------------------------------------------
class GetAudioAdapterInfos : public Messaging::Message
{
    __DeclareClass(GetAudioAdapterInfos);
    __DeclareMsgId;
public:
    GetAudioAdapterInfos() 
    { };
public:
    void SetResult(const Util::Array<CoreAudio::AudioAdapterInfo>& val)
    {
        n_assert(!this->handled);
        this->result = val;
    };
    const Util::Array<CoreAudio::AudioAdapterInfo>& GetResult() const
    {
        n_assert(this->handled);
        return this->result;
    };
private:
    Util::Array<CoreAudio::AudioAdapterInfo> result;
};
//------------------------------------------------------------------------------
class LoadSoundBank : public Messaging::Message
{
    __DeclareClass(LoadSoundBank);
    __DeclareMsgId;
public:
    LoadSoundBank() :
        streaming(false),
        result(false)
    { };
public:
    void SetResourceId(const Resources::ResourceId& val)
    {
        n_assert(!this->handled);
        this->resourceid = val;
    };
    const Resources::ResourceId& GetResourceId() const
    {
        return this->resourceid;
    };
private:
    Resources::ResourceId resourceid;
public:
    void SetStreaming(bool val)
    {
        n_assert(!this->handled);
        this->streaming = val;
    };
    bool GetStreaming() const
    {
        return this->streaming;
    };
private:
    bool streaming;
public:
    void SetResult(bool val)
    {
        n_assert(!this->handled);
        this->result = val;
    };
    bool GetResult() const
    {
        return this->result;
    };
private:
    bool result;
};
//------------------------------------------------------------------------------
class UnloadSoundBank : public Messaging::Message
{
    __DeclareClass(UnloadSoundBank);
    __DeclareMsgId;
public:
    UnloadSoundBank() 
    { };
public:
    void SetResourceId(const Resources::ResourceId& val)
    {
        n_assert(!this->handled);
        this->resourceid = val;
    };
    const Resources::ResourceId& GetResourceId() const
    {
        return this->resourceid;
    };
private:
    Resources::ResourceId resourceid;
};
//------------------------------------------------------------------------------
class PlayCue : public Messaging::Message
{
    __DeclareClass(PlayCue);
    __DeclareMsgId;
public:
    PlayCue() :
        startplaying(true),
        enable3d(false),
        transform(Math::matrix44::identity()),
        velocity(Math::vector::nullvec())
    { };
public:
    void SetCueId(const CoreAudio::CueId& val)
    {
        n_assert(!this->handled);
        this->cueid = val;
    };
    const CoreAudio::CueId& GetCueId() const
    {
        return this->cueid;
    };
private:
    CoreAudio::CueId cueid;
public:
    void SetStartPlaying(bool val)
    {
        n_assert(!this->handled);
        this->startplaying = val;
    };
    bool GetStartPlaying() const
    {
        return this->startplaying;
    };
private:
    bool startplaying;
public:
    void SetEnable3D(bool val)
    {
        n_assert(!this->handled);
        this->enable3d = val;
    };
    bool GetEnable3D() const
    {
        return this->enable3d;
    };
private:
    bool enable3d;
public:
    void SetTransform(const Math::matrix44& val)
    {
        n_assert(!this->handled);
        this->transform = val;
    };
    const Math::matrix44& GetTransform() const
    {
        return this->transform;
    };
private:
    Math::matrix44 transform;
public:
    void SetVelocity(const Math::vector& val)
    {
        n_assert(!this->handled);
        this->velocity = val;
    };
    const Math::vector& GetVelocity() const
    {
        return this->velocity;
    };
private:
    Math::vector velocity;
public:
    void SetCueHandle(const Audio::CueHandle& val)
    {
        n_assert(!this->handled);
        this->cuehandle = val;
    };
    const Audio::CueHandle& GetCueHandle() const
    {
        n_assert(this->handled);
        return this->cuehandle;
    };
private:
    Audio::CueHandle cuehandle;
};
//------------------------------------------------------------------------------
class UpdateCue : public Messaging::Message
{
    __DeclareClass(UpdateCue);
    __DeclareMsgId;
public:
    UpdateCue() :
        statemask(0)
    { };
public:
    void SetCueHandle(const Audio::CueHandle& val)
    {
        n_assert(!this->handled);
        this->cuehandle = val;
    };
    const Audio::CueHandle& GetCueHandle() const
    {
        return this->cuehandle;
    };
private:
    Audio::CueHandle cuehandle;
public:
    void SetTransform(const Math::matrix44& val)
    {
        n_assert(!this->handled);
        this->transform = val;
    };
    const Math::matrix44& GetTransform() const
    {
        return this->transform;
    };
private:
    Math::matrix44 transform;
public:
    void SetVelocity(const Math::vector& val)
    {
        n_assert(!this->handled);
        this->velocity = val;
    };
    const Math::vector& GetVelocity() const
    {
        return this->velocity;
    };
private:
    Math::vector velocity;
public:
    void SetStateMask(const CoreAudio::SoundState::Mask& val)
    {
        n_assert(!this->handled);
        this->statemask = val;
    };
    const CoreAudio::SoundState::Mask& GetStateMask() const
    {
        n_assert(this->handled);
        return this->statemask;
    };
private:
    CoreAudio::SoundState::Mask statemask;
};
//------------------------------------------------------------------------------
class StopCue : public Messaging::Message
{
    __DeclareClass(StopCue);
    __DeclareMsgId;
public:
    StopCue() 
    { };
public:
    void SetCueHandle(const Audio::CueHandle& val)
    {
        n_assert(!this->handled);
        this->cuehandle = val;
    };
    const Audio::CueHandle& GetCueHandle() const
    {
        return this->cuehandle;
    };
private:
    Audio::CueHandle cuehandle;
};
//------------------------------------------------------------------------------
class PauseCue : public Messaging::Message
{
    __DeclareClass(PauseCue);
    __DeclareMsgId;
public:
    PauseCue() 
    { };
public:
    void SetCueHandle(const Audio::CueHandle& val)
    {
        n_assert(!this->handled);
        this->cuehandle = val;
    };
    const Audio::CueHandle& GetCueHandle() const
    {
        return this->cuehandle;
    };
private:
    Audio::CueHandle cuehandle;
};
//------------------------------------------------------------------------------
class ResumeCue : public Messaging::Message
{
    __DeclareClass(ResumeCue);
    __DeclareMsgId;
public:
    ResumeCue() 
    { };
public:
    void SetCueHandle(const Audio::CueHandle& val)
    {
        n_assert(!this->handled);
        this->cuehandle = val;
    };
    const Audio::CueHandle& GetCueHandle() const
    {
        return this->cuehandle;
    };
private:
    Audio::CueHandle cuehandle;
};
//------------------------------------------------------------------------------
class StopCueInstances : public Messaging::Message
{
    __DeclareClass(StopCueInstances);
    __DeclareMsgId;
public:
    StopCueInstances() :
        stopimmediate(false)
    { };
public:
    void SetCueId(const CoreAudio::CueId& val)
    {
        n_assert(!this->handled);
        this->cueid = val;
    };
    const CoreAudio::CueId& GetCueId() const
    {
        return this->cueid;
    };
private:
    CoreAudio::CueId cueid;
public:
    void SetStopImmediate(bool val)
    {
        n_assert(!this->handled);
        this->stopimmediate = val;
    };
    bool GetStopImmediate() const
    {
        return this->stopimmediate;
    };
private:
    bool stopimmediate;
};
//------------------------------------------------------------------------------
class PlayWave : public Messaging::Message
{
    __DeclareClass(PlayWave);
    __DeclareMsgId;
public:
    PlayWave() :
        startplaying(true),
        loopcount(1),
        volume(1.0f),
        pitch(0.0f),
        enable3d(false),
        transform(Math::matrix44::identity()),
        velocity(Math::vector::nullvec())
    { };
public:
    void SetWaveId(const CoreAudio::WaveId& val)
    {
        n_assert(!this->handled);
        this->waveid = val;
    };
    const CoreAudio::WaveId& GetWaveId() const
    {
        return this->waveid;
    };
private:
    CoreAudio::WaveId waveid;
public:
    void SetStartPlaying(bool val)
    {
        n_assert(!this->handled);
        this->startplaying = val;
    };
    bool GetStartPlaying() const
    {
        return this->startplaying;
    };
private:
    bool startplaying;
public:
    void SetLoopCount(SizeT val)
    {
        n_assert(!this->handled);
        this->loopcount = val;
    };
    SizeT GetLoopCount() const
    {
        return this->loopcount;
    };
private:
    SizeT loopcount;
public:
    void SetVolume(float val)
    {
        n_assert(!this->handled);
        this->volume = val;
    };
    float GetVolume() const
    {
        return this->volume;
    };
private:
    float volume;
public:
    void SetPitch(float val)
    {
        n_assert(!this->handled);
        this->pitch = val;
    };
    float GetPitch() const
    {
        return this->pitch;
    };
private:
    float pitch;
public:
    void SetEnable3D(bool val)
    {
        n_assert(!this->handled);
        this->enable3d = val;
    };
    bool GetEnable3D() const
    {
        return this->enable3d;
    };
private:
    bool enable3d;
public:
    void SetTransform(const Math::matrix44& val)
    {
        n_assert(!this->handled);
        this->transform = val;
    };
    const Math::matrix44& GetTransform() const
    {
        return this->transform;
    };
private:
    Math::matrix44 transform;
public:
    void SetVelocity(const Math::vector& val)
    {
        n_assert(!this->handled);
        this->velocity = val;
    };
    const Math::vector& GetVelocity() const
    {
        return this->velocity;
    };
private:
    Math::vector velocity;
public:
    void SetWaveHandle(const Audio::WaveHandle& val)
    {
        n_assert(!this->handled);
        this->wavehandle = val;
    };
    const Audio::WaveHandle& GetWaveHandle() const
    {
        n_assert(this->handled);
        return this->wavehandle;
    };
private:
    Audio::WaveHandle wavehandle;
};
//------------------------------------------------------------------------------
class StopWaveInstances : public Messaging::Message
{
    __DeclareClass(StopWaveInstances);
    __DeclareMsgId;
public:
    StopWaveInstances() :
        stopimmediate(false)
    { };
public:
    void SetWaveId(const CoreAudio::WaveId& val)
    {
        n_assert(!this->handled);
        this->waveid = val;
    };
    const CoreAudio::WaveId& GetWaveId() const
    {
        return this->waveid;
    };
private:
    CoreAudio::WaveId waveid;
public:
    void SetStopImmediate(bool val)
    {
        n_assert(!this->handled);
        this->stopimmediate = val;
    };
    bool GetStopImmediate() const
    {
        return this->stopimmediate;
    };
private:
    bool stopimmediate;
};
//------------------------------------------------------------------------------
class UpdateListener : public Messaging::Message
{
    __DeclareClass(UpdateListener);
    __DeclareMsgId;
public:
    UpdateListener() 
    { };
public:
    void SetTransform(const Math::matrix44& val)
    {
        n_assert(!this->handled);
        this->transform = val;
    };
    const Math::matrix44& GetTransform() const
    {
        return this->transform;
    };
private:
    Math::matrix44 transform;
public:
    void SetVelocity(const Math::vector& val)
    {
        n_assert(!this->handled);
        this->velocity = val;
    };
    const Math::vector& GetVelocity() const
    {
        return this->velocity;
    };
private:
    Math::vector velocity;
};
//------------------------------------------------------------------------------
class SetGlobalVariable : public Messaging::Message
{
    __DeclareClass(SetGlobalVariable);
    __DeclareMsgId;
public:
    SetGlobalVariable() 
    { };
public:
    void SetVariableName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->variablename = val;
    };
    const Util::StringAtom& GetVariableName() const
    {
        return this->variablename;
    };
private:
    Util::StringAtom variablename;
public:
    void SetVaribaleValue(float val)
    {
        n_assert(!this->handled);
        this->varibalevalue = val;
    };
    float GetVaribaleValue() const
    {
        return this->varibalevalue;
    };
private:
    float varibalevalue;
};
//------------------------------------------------------------------------------
class GetGlobalVariable : public Messaging::Message
{
    __DeclareClass(GetGlobalVariable);
    __DeclareMsgId;
public:
    GetGlobalVariable() 
    { };
public:
    void SetVariableName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->variablename = val;
    };
    const Util::StringAtom& GetVariableName() const
    {
        return this->variablename;
    };
private:
    Util::StringAtom variablename;
public:
    void SetVaribaleValue(float val)
    {
        n_assert(!this->handled);
        this->varibalevalue = val;
    };
    float GetVaribaleValue() const
    {
        n_assert(this->handled);
        return this->varibalevalue;
    };
private:
    float varibalevalue;
};
} // namespace Audio
//------------------------------------------------------------------------------
