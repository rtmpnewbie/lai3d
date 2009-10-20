#pragma once
//------------------------------------------------------------------------------
/**
    @class SUI::SUIServerBase

    Central SUI server on the render-thread-side.

    (C) 2009 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "util/stringatom.h"
#include "resources/resourceid.h"
#include "sui/rt/suilayout.h"
#include "sui/suievent.h"
#include "input/inputevent.h"
#include "timing/timer.h"

//------------------------------------------------------------------------------
namespace Base
{
class SUIServerBase : public Core::RefCounted
{
    __DeclareClass(SUIServerBase);
    __DeclareSingleton(SUIServerBase);
public:
    /// constructor
    SUIServerBase();
    /// destructor
    virtual ~SUIServerBase();

    /// setup the server
    void Setup();
    /// discard the server
    void Discard();
    /// return true if server has been setup
    bool IsValid() const;

    /// create a layout object
    Ptr<SUI::SUILayout> CreateLayout(const Util::StringAtom& layoutId, const Resources::ResourceId& resId);
    /// destroy a layout object
    void DestroyLayout(const Util::StringAtom& layoutId);
    /// show a layout object
    void ShowLayout(const Util::StringAtom& layoutId);
    /// hide a layout object
    void HideLayout(const Util::StringAtom& layoutId);
    
    /// return true if layout exists
    bool HasLayout(const Util::StringAtom& layoutId) const;
    /// get layout by layout id
    Ptr<SUI::SUILayout> GetLayoutById(const Util::StringAtom& layoutId) const;
    /// get all layouts 
    Util::Array<Ptr<SUI::SUILayout> > GetLayouts() const;

    /// invoke an action on the UI
    void InvokeAction(const Ptr<SUI::SUIAction>& action);
    /// process input events on all visible layout
    Util::Array<SUI::SUIEvent> ProcessInputEvents(const Util::Array<Input::InputEvent>& inputEvents);
    /// update the SUI system, called exactly once per frame
    void Update();
    /// render SUI system, may be called several times per frame
    void Render();
    /// get current SUI time
    Timing::Time GetTime() const;
    /// get current SUI time ticks
    Timing::Tick GetTicks() const;

private:
    Util::Dictionary<Util::StringAtom, Ptr<SUI::SUILayout> > layouts;
    bool isValid;
    Timing::Timer timer;
    Timing::Time curTime;
    Timing::Tick curTicks;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
SUIServerBase::IsValid() const
{
    return this->isValid;
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Time
SUIServerBase::GetTime() const
{
    return this->curTime;
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Tick
SUIServerBase::GetTicks() const
{
    return this->curTicks;
}

} // namespace SUI
//------------------------------------------------------------------------------
