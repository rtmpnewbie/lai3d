//------------------------------------------------------------------------------
//  slavetime.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "timing/slavetime.h"

namespace Timing
{
__ImplementClass(Timing::SlaveTime, 'TIST', Core::RefCounted);
__ImplementSingleton(Timing::SlaveTime);

//------------------------------------------------------------------------------
/**
*/
SlaveTime::SlaveTime() : synced(false),
                         lastSyncTicks(Timing::TimingType::NumTimingTypes),
                         times(Timing::TimingType::NumTimingTypes),
                         timesInTicks(Timing::TimingType::NumTimingTypes),
                         lastFrameTimes(Timing::TimingType::NumTimingTypes)
{
    IndexT index;
    for (index = 0; index < Timing::TimingType::NumTimingTypes; index++)
    {
        this->lastSyncTicks[index] = 0;
        this->times[index] = 0;
        this->timesInTicks[index] = 0;
        this->lastFrameTimes[index] = 0;
    }
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
SlaveTime::~SlaveTime()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
SlaveTime::UpdateTimes()
{
    // if time is synced
    if (this->synced)
    {
        //n_printf("->>>> %f synced (frameTime: %f)\n", this->times[TimingType::MasterTime], this->lastFrameTimes[TimingType::MasterTime]);
        this->synced = false; 
    }
    else
    {
        // approximate current times by last sync interval
        IndexT index;
        for (index = 0; index < Timing::TimingType::NumTimingTypes; index++)
        {
            this->times[index] = this->times[index] + this->lastFrameTimes[index];
            this->timesInTicks[index] = SecondsToTicks(this->times[index]);
        }
        //n_printf("->>>> %f unsynced (frameTime: %f)\n", this->times[TimingType::MasterTime], this->lastFrameTimes[TimingType::MasterTime]);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
SlaveTime::SyncTime(Timing::Time time, TimingType::Code timeType)
{
    // set all stuff
    this->lastFrameTimes[timeType] = time - this->times[timeType];
    
    this->times[timeType] = time;
    this->timesInTicks[timeType] = SecondsToTicks(time);
    
    this->synced = true;
    
    this->lastSyncTicks[timeType] = this->timesInTicks[timeType];
}
} // namespace Timing
