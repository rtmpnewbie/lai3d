#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "messaging/message.h"
#include "corefx/effects/effect.h"
#include "corefx/mixer/effectmixer.h"

//------------------------------------------------------------------------------
namespace CoreFX
{
//------------------------------------------------------------------------------
class AttachEffect : public Messaging::Message
{
    __DeclareClass(AttachEffect);
    __DeclareMsgId;
public:
    AttachEffect() 
    { };
public:
    void SetEffect(const Ptr<CoreFX::Effect>& val)
    {
        n_assert(!this->handled);
        this->effect = val;
    };
    const Ptr<CoreFX::Effect>& GetEffect() const
    {
        return this->effect;
    };
private:
    Ptr<CoreFX::Effect> effect;
};
//------------------------------------------------------------------------------
class AttachEffectMixer : public Messaging::Message
{
    __DeclareClass(AttachEffectMixer);
    __DeclareMsgId;
public:
    AttachEffectMixer() 
    { };
public:
    void SetEffectMixer(const Ptr<CoreFX::EffectMixer>& val)
    {
        n_assert(!this->handled);
        this->effectmixer = val;
    };
    const Ptr<CoreFX::EffectMixer>& GetEffectMixer() const
    {
        return this->effectmixer;
    };
private:
    Ptr<CoreFX::EffectMixer> effectmixer;
};
} // namespace CoreFX
//------------------------------------------------------------------------------
