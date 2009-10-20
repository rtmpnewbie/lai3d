#pragma once
//------------------------------------------------------------------------------
/**
    @class Timing::MasterTime
    
    This master time uses a simple timer to get the "global" time. Only the main 
    thread should have a reference to this object, and no other thread.

    The time of this timer is used to send sync messages to all other threads,
    to synchronize their times. 

    Trigger this timer at first in a frame in your application.

    (C) 2009 Radon Labs GmbH
*/
#include "timing/timer.h"
#include "core/refcounted.h"
#include "core/singleton.h"

//------------------------------------------------------------------------------
namespace Timing
{
class MasterTime : public Core::RefCounted
{
    __DeclareClass(MasterTime);
    __DeclareSingleton(MasterTime);

public:
    /// constructor
    MasterTime();
    /// deconstructor
    virtual ~MasterTime();

    /// start/continue the timer
    void Start();
    /// stop the timer
    void Stop();
    /// reset the timer
    void Reset();
    /// return true if currently running
    bool Running() const;

    /// update time
    void Update();

    /// get current time in seconds
    Timing::Time GetTime() const;
    /// get current time in ticks
    Timing::Tick GetTicks() const;

private:
    /// simple timer object
    Timing::Timer masterTimer;
    Timing::Time time;
    Timing::Tick timeInTicks;
};

//------------------------------------------------------------------------------
/**
    Simply delegate to timer object
*/
inline void
MasterTime::Start()
{
    this->masterTimer.Start();
}

//------------------------------------------------------------------------------
/**
    Simply delegate to timer object
*/
inline void
MasterTime::Stop()
{
    this->masterTimer.Stop();
}

//------------------------------------------------------------------------------
/**
    Simply delegate to timer object
*/
inline void
MasterTime::Reset()
{
    this->masterTimer.Reset();
}

//------------------------------------------------------------------------------
/**
    Simply delegate to timer object
*/
inline bool
MasterTime::Running() const
{
    return this->masterTimer.Running();
}

//------------------------------------------------------------------------------
/**
*/
inline void
MasterTime::Update()
{
    this->timeInTicks = this->masterTimer.GetTicks();
    this->time = TicksToSeconds(this->timeInTicks);
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Time
MasterTime::GetTime() const
{
    return this->time;
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Tick
MasterTime::GetTicks() const
{
    return this->timeInTicks;
}

} // namespace Timing
//------------------------------------------------------------------------------