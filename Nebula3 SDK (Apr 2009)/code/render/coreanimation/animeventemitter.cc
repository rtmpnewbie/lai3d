//------------------------------------------------------------------------------
//  animeventemitter.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coreanimation/animeventemitter.h"

namespace CoreAnimation
{
//------------------------------------------------------------------------------
/**
    collects all animevents from given clip
*/
Util::Array<AnimEvent> 
CoreAnimation::AnimEventEmitter::EmitAnimEvents(const AnimClip& clip, Timing::Tick start, Timing::Tick end, bool isInfitine)
{
    Util::Array<AnimEvent> events;

    if (start <= end)
    {
        IndexT startIdx = 0;
        SizeT numEvents = clip.GetEventsInRange(start, end, startIdx);
        IndexT i;
        IndexT endIndex = startIdx + numEvents;
        for (i = startIdx; startIdx != -1 && i < endIndex; ++i)
        {
    	    events.Append(clip.GetEventByIndex(i));
        }
    }
    else
    {
        Timing::Tick clipDuration = clip.GetNumKeys() * clip.GetKeyDuration();
        events.AppendArray(AnimEventEmitter::EmitAnimEvents(clip, start, clipDuration, false));
        if (isInfitine)
        {
            events.AppendArray(AnimEventEmitter::EmitAnimEvents(clip, 0, end, false));
        }
    }
    return events;
}
}