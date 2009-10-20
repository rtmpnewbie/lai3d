#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "messaging/message.h"
#include "timing/timingtype.h"
#include "timing/time.h"
#include "util/dictionary.h"

//------------------------------------------------------------------------------
namespace Timing
{
//------------------------------------------------------------------------------
class SyncTime : public Messaging::Message
{
    __DeclareClass(SyncTime);
    __DeclareMsgId;
public:
    SyncTime() 
    { };
public:
    void SetTimes(const Util::Dictionary<Timing::TimingType::Code, Timing::Time>& val)
    {
        n_assert(!this->handled);
        this->times = val;
    };
    const Util::Dictionary<Timing::TimingType::Code, Timing::Time>& GetTimes() const
    {
        return this->times;
    };
private:
    Util::Dictionary<Timing::TimingType::Code, Timing::Time> times;
};
} // namespace Timing
//------------------------------------------------------------------------------
