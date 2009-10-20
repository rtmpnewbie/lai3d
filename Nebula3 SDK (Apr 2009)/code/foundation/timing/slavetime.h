#pragma once
//------------------------------------------------------------------------------
/**
    @class Timing::SlaveTime
    
    This slave time has to be update regularly from the thread wich contains 
    the master time object at the beginning of a thread cylce and just one time 
    within it.

    The slave timer is the global time setter for the thread and its subsystems.

    If a thread cycle is finished, and no master time sync has been done, the timer
    will approximate the time wich has been synced and simulates a master sync by
    it self and the time differences from the last frames. (Dead Reckoning)

    The master time sync always overwrites the simulated time.    

    (C) 2009 Radon Labs GmbH
*/
#include "timing/time.h"
#include "timing/timingtype.h"
#include "core/refcounted.h"
#include "core/singleton.h"
//------------------------------------------------------------------------------
namespace Timing
{
class SlaveTime : public Core::RefCounted
{
    __DeclareClass(SlaveTime);
    __DeclareSingleton(SlaveTime);

public:
    /// constructor
    SlaveTime();
    /// deconstructor
    virtual ~SlaveTime();

    /// sync the time (call this method if the thread got a SyncTime message)
    void SyncTime(Timing::Time time, TimingType::Code timeType);
    /// check sync (call this message at the beginning of each thread cycle)
    void UpdateTimes();

    /// return synced flag
    bool IsSynced(TimingType::Code timeType) const;
    
    /// get current time in seconds
    Timing::Time GetTime(TimingType::Code timeType) const;
    /// get current time in ticks
    Timing::Tick GetTicks(TimingType::Code timeType) const;
    /// get the last frame time
    Timing::Time GetFrameTime(TimingType::Code timeType) const;

private:
    // public accessible values
    Util::FixedArray<Timing::Time> times;
    Util::FixedArray<Timing::Tick> timesInTicks;
    Util::FixedArray<Timing::Tick> lastSyncTicks;

    Util::FixedArray<Timing::Time> lastFrameTimes;

    bool synced;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
SlaveTime::IsSynced(TimingType::Code timeType) const
{
    return this->timesInTicks[timeType] == this->timesInTicks[timeType];
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Time
SlaveTime::GetTime(TimingType::Code timeType) const
{
    return this->times[timeType];
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Tick
SlaveTime::GetTicks(TimingType::Code timeType) const
{
    return this->timesInTicks[timeType];
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Time
SlaveTime::GetFrameTime(TimingType::Code timeType) const
{
    // ignore frametimes below zero, might be synchronized just in that frame and 
    // the dead reckoning for time might have been to fast!    
    return this->lastFrameTimes[timeType] > 0 ? this->lastFrameTimes[timeType] : 0;
}
} // namespace Timing
//------------------------------------------------------------------------------