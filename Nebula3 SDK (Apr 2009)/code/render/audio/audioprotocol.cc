//------------------------------------------------------------------------------
//  MACHINE GENERATED, DON'T EDIT!
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "audioprotocol.h"

namespace Audio
{
    __ImplementClass(Audio::SetupAudio, 'saud', Messaging::Message);
    __ImplementMsgId(SetupAudio);
    __ImplementClass(Audio::GetAudioAdapterInfos, 'gaad', Messaging::Message);
    __ImplementMsgId(GetAudioAdapterInfos);
    __ImplementClass(Audio::LoadSoundBank, 'lsbk', Messaging::Message);
    __ImplementMsgId(LoadSoundBank);
    __ImplementClass(Audio::UnloadSoundBank, 'ulsk', Messaging::Message);
    __ImplementMsgId(UnloadSoundBank);
    __ImplementClass(Audio::PlayCue, 'pcue', Messaging::Message);
    __ImplementMsgId(PlayCue);
    __ImplementClass(Audio::UpdateCue, 'ucue', Messaging::Message);
    __ImplementMsgId(UpdateCue);
    __ImplementClass(Audio::StopCue, 'scue', Messaging::Message);
    __ImplementMsgId(StopCue);
    __ImplementClass(Audio::PauseCue, 'paus', Messaging::Message);
    __ImplementMsgId(PauseCue);
    __ImplementClass(Audio::ResumeCue, 'resu', Messaging::Message);
    __ImplementMsgId(ResumeCue);
    __ImplementClass(Audio::StopCueInstances, 'scui', Messaging::Message);
    __ImplementMsgId(StopCueInstances);
    __ImplementClass(Audio::PlayWave, 'pwav', Messaging::Message);
    __ImplementMsgId(PlayWave);
    __ImplementClass(Audio::StopWaveInstances, 'swav', Messaging::Message);
    __ImplementMsgId(StopWaveInstances);
    __ImplementClass(Audio::UpdateListener, 'updl', Messaging::Message);
    __ImplementMsgId(UpdateListener);
    __ImplementClass(Audio::SetGlobalVariable, 'sgva', Messaging::Message);
    __ImplementMsgId(SetGlobalVariable);
    __ImplementClass(Audio::GetGlobalVariable, 'ggva', Messaging::Message);
    __ImplementMsgId(GetGlobalVariable);
} // Audio

namespace Commands
{
} // namespace Commands
//------------------------------------------------------------------------------
