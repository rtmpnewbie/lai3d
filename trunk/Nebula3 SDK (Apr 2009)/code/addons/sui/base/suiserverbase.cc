//------------------------------------------------------------------------------
//  suiserverbase.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "sui/base/suiserverbase.h"

namespace Base
{
__ImplementClass(Base::SUIServerBase, 'SUSB', Core::RefCounted);
__ImplementSingleton(Base::SUIServerBase);

using namespace Util;
using namespace Resources;
using namespace SUI;
using namespace Input;

//------------------------------------------------------------------------------
/**
*/
SUIServerBase::SUIServerBase() :
    isValid(false),
    curTime(0.0),
    curTicks(0)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
SUIServerBase::~SUIServerBase()
{
    n_assert(!this->IsValid());
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
SUIServerBase::Setup()
{
    n_assert(!this->IsValid());
    this->isValid = true;
}

//------------------------------------------------------------------------------
/**
*/
void
SUIServerBase::Discard()
{
    n_assert(this->IsValid());

    // discard existing layouts
    IndexT i;
    for (i = 0; i < this->layouts.Size(); i++)
    {
        this->layouts.ValueAtIndex(i)->Discard();
    }
    this->layouts.Clear();

    this->isValid = false;
}

//------------------------------------------------------------------------------
/**
    Create a new layout object and start loading it. The loading usually
    is asynchronously (also see SUIServerBase::Update() for details).
*/
Ptr<SUILayout>
SUIServerBase::CreateLayout(const StringAtom& layoutId, const ResourceId& resId)
{
    n_assert(!this->HasLayout(layoutId));

    Ptr<SUILayout> layout = SUILayout::Create();
    layout->Setup(layoutId, resId);
    layout->Load();
    this->layouts.Add(layoutId, layout);
    return layout;
}

//------------------------------------------------------------------------------
/**
    Destroy an existing layout object.
*/
void
SUIServerBase::DestroyLayout(const StringAtom& layoutId)
{
    n_assert(this->HasLayout(layoutId));
    this->layouts[layoutId]->Discard();
    this->layouts.Erase(layoutId);
}

//------------------------------------------------------------------------------
/**
*/
void
SUIServerBase::ShowLayout(const StringAtom& layoutId)
{
    n_assert(this->HasLayout(layoutId));
    this->layouts[layoutId]->Show();
}

//------------------------------------------------------------------------------
/**
*/
void
SUIServerBase::HideLayout(const StringAtom& layoutId)
{
    n_assert(this->HasLayout(layoutId));
    this->layouts[layoutId]->Hide();
}

//------------------------------------------------------------------------------
/**
*/
bool
SUIServerBase::HasLayout(const StringAtom& layoutId) const
{
    return this->layouts.Contains(layoutId);
}

//------------------------------------------------------------------------------
/**
*/
Ptr<SUILayout>
SUIServerBase::GetLayoutById(const StringAtom& layoutId) const
{
    return this->layouts[layoutId];
}

//------------------------------------------------------------------------------
/**
*/
Array<Ptr<SUILayout> >
SUIServerBase::GetLayouts() const
{
    return this->layouts.ValuesAsArray();
}

//------------------------------------------------------------------------------
/**
    This calls the ProcessInputEvents() on ALL layouts which are not pending. 
    A layout should return any resulting SUIEvents in an array, which will then 
    be merged to one big array by this method.
*/
Array<SUIEvent>
SUIServerBase::ProcessInputEvents(const Array<InputEvent>& inputEvents)
{
    Array<SUIEvent> result;
    IndexT i;
    for (i = 0; i < this->layouts.Size(); i++)
    {
        const Ptr<SUILayout>& curLayout = this->layouts.ValueAtIndex(i);
        if (curLayout->IsShowable())
        {        
            curLayout->ProcessInputEvents(inputEvents, result);
        }
    }
    return result;
}

//------------------------------------------------------------------------------
/**
    This method will be called exactly once per frame. Layouts should
    realize animation and other effects in this method. The method will
    also call the Load() method on all layouts which are still pending.
*/
void
SUIServerBase::Update()
{
    // first update times, do a clean timer start in the first frame
    if (!this->timer.Running())
    {
        this->timer.Start();
    }
    this->curTime = this->timer.GetTime();
    this->curTicks = this->timer.GetTicks();

    IndexT i;
    for (i = 0; i < this->layouts.Size(); i++)
    {
        const Ptr<SUILayout>& curLayout = this->layouts.ValueAtIndex(i);
        if (curLayout->LoadFailed())
        {
            // loading the layout has failed
            n_error("SUIServer: loading layout '%s' from res '%s' failed!\n", 
                curLayout->GetLayoutId().Value().AsCharPtr(),
                curLayout->GetResourceId().Value().AsCharPtr());
        }
        else if (curLayout->IsPending())
        {
            // layout still loading... continue to call Load()
            curLayout->Load();
        }
        else
        {
            // layout is ready, call its update method
            curLayout->Update();
        }
    }
}

//------------------------------------------------------------------------------
/**
    This method will call the Render() method on all showable layouts. Note
    that Render() will even be called on layouts which are not in the 
    shown state, the layout object needs to decide itself whether it
    wants to render something!
*/
void
SUIServerBase::Render()
{
    IndexT i;
    for (i = 0; i < this->layouts.Size(); i++)
    {
        const Ptr<SUILayout>& curLayout = this->layouts.ValueAtIndex(i);
        if (curLayout->IsShowable())
        {
            curLayout->Render();
        }
    }
}

//------------------------------------------------------------------------------
/**
    Invokes an action on the UI. This is the only way to manipulate the state 
    of the GUI from the outside.
*/
void
SUIServerBase::InvokeAction(const Ptr<SUIAction>& action)
{
    // distribute the action to the right layout object
    IndexT layoutIndex = this->layouts.FindIndex(action->GetLayoutId());
    if (InvalidIndex != layoutIndex)
    {
        this->layouts.ValueAtIndex(layoutIndex)->InvokeAction(action);
    }
    else
    {
        n_printf("SUIServer::InvokeAction() WARNING: layout doesn't exist: '%s'\n", action->GetLayoutId().Value().AsCharPtr());
    }
}

} // namespace SUI
