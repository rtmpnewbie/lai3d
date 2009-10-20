//------------------------------------------------------------------------------
//  animclip.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coreanimation/animclip.h"

namespace CoreAnimation
{
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
AnimClip::AnimClip() :
    numKeys(0),
    keyStride(0),
    keyDuration(0),
    preInfinityType(InfinityType::Constant),
    postInfinityType(InfinityType::Constant),
    inBeginEvents(false),
    startKeyIndex(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
AnimClip::SetNumCurves(SizeT numCurves)
{
    this->curves.SetSize(numCurves);
}

//------------------------------------------------------------------------------
/**
*/
SizeT
AnimClip::GetNumCurves() const
{
    return this->curves.Size();
}

//------------------------------------------------------------------------------
/**
*/
AnimCurve&
AnimClip::CurveByIndex(IndexT curveIndex) const
{
    return this->curves[curveIndex];
}

//------------------------------------------------------------------------------
/**
*/
void
AnimClip::BeginEvents(SizeT numEvents)
{
    n_assert(!this->inBeginEvents);
    n_assert(this->events.IsEmpty());
    this->inBeginEvents = true;
    this->events.Reserve(numEvents);
    this->eventIndexMap.Reserve(numEvents);
}

//------------------------------------------------------------------------------
/**
*/
void
AnimClip::AddEvent(const AnimEvent& animEvent)
{
    n_assert(this->inBeginEvents);
    n_assert(animEvent.GetName().Value().IsValid());
    this->events.Append(animEvent);
}

//------------------------------------------------------------------------------
/**
*/
void
AnimClip::EndEvents()
{
    n_assert(this->inBeginEvents);
    this->inBeginEvents = false;

    // sort event array by event time
    this->events.Sort();

    // build name-lookup table
    IndexT i;
    this->eventIndexMap.BeginBulkAdd();
    for (i = 0; i < this->events.Size(); i++)
    {
        this->eventIndexMap.Add(this->events[i].GetName(), i);
    }
    this->eventIndexMap.EndBulkAdd();
}

//------------------------------------------------------------------------------
/**
*/
bool
AnimClip::HasEvent(const StringAtom& name) const
{
    return this->eventIndexMap.Contains(name);
}

//------------------------------------------------------------------------------
/**
*/
const AnimEvent&
AnimClip::GetEventByName(const StringAtom& name) const
{
    return this->events[this->eventIndexMap[name]];
}

//------------------------------------------------------------------------------
/**
    Get events in a specific time range. Return the number of events in the
    time range, and the index of the start event. This does a linear search
    on the event array.
*/
SizeT
AnimClip::GetEventsInRange(Timing::Tick startTime, Timing::Tick endTime, IndexT& outStartEventIndex) const
{
    outStartEventIndex = InvalidIndex;

    // find start index
    SizeT numEvents = this->events.Size();

    IndexT i = 0;
    while ((i < numEvents) && (startTime > this->events[i].GetTime()))
    {
        i++;
    }
    if (i < numEvents)
    {
        outStartEventIndex = i;
        SizeT numEventsInRange = 0;
        while ((i < numEvents) && (endTime > this->events[i].GetTime()))
        {
            numEventsInRange++;
            i++;
        }
        return numEventsInRange;
    }
    return 0;
}

} // namespace CoreAnimation
