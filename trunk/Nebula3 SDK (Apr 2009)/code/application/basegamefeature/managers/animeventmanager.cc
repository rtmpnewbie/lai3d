//------------------------------------------------------------------------------
//  animeventmanager.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "managers/animeventmanager.h"
#include "basegametiming/gametimesource.h"
#include "game/entity.h"

namespace BaseGameFeature
{
__ImplementClass(AnimEventManager, 'AEMA', Game::Manager);
__ImplementSingleton(AnimEventManager);

using namespace Timing;

//------------------------------------------------------------------------------
/**
*/
AnimEventManager::AnimEventManager()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
AnimEventManager::~AnimEventManager()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
AnimEventManager::OnActivate()
{
    Manager::OnActivate();
}

//------------------------------------------------------------------------------
/**
*/
void
AnimEventManager::RegisterEntity(const Ptr<Game::Entity>& entity, const Util::StringAtom& category)
{
    n_assert(entity.isvalid());
    
    // check if entity is already registered
    IndexT index = this->animEventData.FindIndex(entity);
    if (InvalidIndex != index)
    {
        AnimEventData& existingData = this->animEventData.ValueAtIndex(index);
        // check if category exists
        if (InvalidIndex == existingData.registredCategories.FindIndex(category))
        {
            existingData.registredCategories.Append(category);
            existingData.animEvents.Add(category, Util::Array<Animation::AnimEventInfo>());
        }
        return;
    }    

    // create data ...
    AnimEventData data;
    data.animEvents.Add(category, Util::Array<Animation::AnimEventInfo>());
    data.keyDuration = -1;
    data.lastCheckEndTime = -1;
    data.registredCategories.Append(category);
    data.getEventsMsg = Graphics::GetActiveAnimEvents::Create();
    data.getEventsMsg->SetCategories(data.registredCategories);     // categories will also be set each time before sending
    data.getEventsMsg->SetUseJustDominatingClip(true);
    data.getEventsMsg->SetOneKeyDuration(false);
    data.getMaxDurMsg = Graphics::GetMaximumKeyDuration::Create();    

    // ... get duration ...
    entity->SendSync(data.getMaxDurMsg.cast<Messaging::Message>());

    // ... and add 
    this->animEventData.Add(entity, data);
}

//------------------------------------------------------------------------------
/**
*/
void
AnimEventManager::UnregisterEntity(const Ptr<Game::Entity>& entity, const Util::StringAtom& category)
{
    IndexT index = this->animEventData.FindIndex(entity);
    // check if more than one category
    if (InvalidIndex != index)
    {
        AnimEventData& data = this->animEventData.ValueAtIndex(index);
        IndexT catIndex = data.registredCategories.FindIndex(category);
        if (InvalidIndex != catIndex)
        {
            data.registredCategories.EraseIndex(catIndex);
        }
        else
        {
            n_error("AnimEventManager::UnregisterEntity() category '%s' not found!\n", category.Value().AsCharPtr());
        }

        // check if this was last category
        if (data.registredCategories.Size() == 0)
        {
            this->animEventData.EraseAtIndex(index);
        }
    }
    else
    {
        n_error("AnimEventManager::UnregisterEntity() entity (with id '%s') not found!\n", entity->GetString(Attr::Id));
    }
}

//------------------------------------------------------------------------------
/**
*/
void
AnimEventManager::OnBeginFrame()
{    
    // check if its time to check for new anim events
    Tick time = GameTimeSource::Instance()->GetTicks();

    // do for each registered entity
    IndexT index;
    for (index = 0; index < this->animEventData.Size(); index++)
    {
        const Ptr<Game::Entity> curEntity = this->animEventData.KeyAtIndex(index);
        AnimEventData& curData = this->animEventData.ValueAtIndex(index);

        // ------------------------ first check if duration already set
        if (curData.getMaxDurMsg.isvalid())
        {
            // check if message is already returned
            if (curData.getMaxDurMsg->Handled())
            {
                if (curData.getMaxDurMsg->GetDuration() == 0)
                {
                    // resend
                    curData.getMaxDurMsg->SetHandled(false);
                    curEntity->SendSync(curData.getMaxDurMsg.cast<Messaging::Message>());
                }
                else
                {
                    curData.keyDuration = curData.getMaxDurMsg->GetDuration();
                    curData.getMaxDurMsg = 0;
                }
            }
            else
            {
                // skip
                continue;
            }
        }

        // ------------------------ check event msg
        if(curData.getEventsMsg->Handled() || curData.lastCheckEndTime == -1)
        {        
            // check if its time
            if(time > curData.lastCheckEndTime)
            {
                // first save old data if just handled
                if (curData.getEventsMsg->Handled())
                {
                    curData.animEvents = curData.getEventsMsg->GetAnimEvents();
                }

                // now request new one
                Tick endTime = time + curData.keyDuration;
                curData.getEventsMsg->SetHandled(false);
                curData.getEventsMsg->SetStartTime(curData.lastCheckEndTime);
                curData.getEventsMsg->SetEndTime(endTime);
                curData.getEventsMsg->SetApplicationTime(time);
                curData.getEventsMsg->SetCategories(curData.registredCategories);
                curEntity->SendSync(curData.getEventsMsg.cast<Messaging::Message>());
                curData.lastCheckEndTime = endTime;                
            }            
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
AnimEventManager::OnEndFrame()
{
    // clear the data
    IndexT entityIndex;
    for (entityIndex = 0; entityIndex < this->animEventData.Size(); entityIndex++)
    {
        IndexT categoryIndex;
        for (categoryIndex = 0; categoryIndex < this->animEventData.ValueAtIndex(entityIndex).animEvents.Size(); categoryIndex++)
        {
            this->animEventData.ValueAtIndex(entityIndex).animEvents.ValueAtIndex(categoryIndex).Clear();
        }        
    }
}

//------------------------------------------------------------------------------
/**
*/
const Util::Array<Animation::AnimEventInfo>&
AnimEventManager::GetAnimEvents(const Ptr<Game::Entity>& entity, const Util::StringAtom& category)
{
    IndexT entityIndex = this->animEventData.FindIndex(entity);
    n_assert(InvalidIndex != entityIndex);
    IndexT categoryIndex = this->animEventData.ValueAtIndex(entityIndex).animEvents.FindIndex(category);
    n_assert(InvalidIndex != categoryIndex);
    
    return this->animEventData.ValueAtIndex(entityIndex).animEvents.ValueAtIndex(categoryIndex);
}
}