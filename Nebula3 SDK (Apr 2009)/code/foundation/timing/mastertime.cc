//------------------------------------------------------------------------------
//  mastertimer.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "timing/mastertime.h"

namespace Timing
{
__ImplementClass(Timing::MasterTime, 'TIMT', Core::RefCounted);
__ImplementSingleton(Timing::MasterTime);

//------------------------------------------------------------------------------
/**
*/
MasterTime::MasterTime() :
    time(0.0),
    timeInTicks(0)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
MasterTime::~MasterTime()
{
    __DestructSingleton;
}
} // namespace Timing
