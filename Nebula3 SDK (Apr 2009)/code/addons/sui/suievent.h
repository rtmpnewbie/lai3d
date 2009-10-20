#pragma once
//------------------------------------------------------------------------------
/**
    @class SUI::SUIEvent

    An SUIEvent defines what and where some action has happened in a
    layout (for instance, a button has been pressed).

    (C) 2009 Radon Labs GmbH
*/
#include "core/types.h"
#include "util/stringatom.h"

//------------------------------------------------------------------------------
namespace SUI
{
class SUIEvent
{
public:
    /// event action enums (bit mask)
    enum Action
    {
        NoAction  = 0,
        MouseDown = (1<<0),
        MouseUp   = (1<<1),
        MouseOver = (1<<2),
    };

    /// default constructor
    SUIEvent();
    /// constructor
    SUIEvent(const Util::StringAtom& layoutId, const Util::StringAtom& elementId, uint actions);
    /// equality operator (NOTE: "any match" is true for action bits)
    bool operator==(const SUIEvent& rhs) const;
    /// inequality operator (NOTE: "no match" on action bits!)
    bool operator!=(const SUIEvent& rhs) const;
    /// setup the event
    void Setup(const Util::StringAtom& layoutId, const Util::StringAtom& elementId, uint actions);
    /// get layout id
    const Util::StringAtom& GetLayoutId() const;
    /// get element id
    const Util::StringAtom& GetElementId() const;
    /// get action mask
    uint GetActions() const;

private:
    Util::StringAtom layoutId;
    Util::StringAtom elementId;
    uint actions;
};

//------------------------------------------------------------------------------
/**
*/
inline
SUIEvent::SUIEvent() :
    actions(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
SUIEvent::SUIEvent(const Util::StringAtom& lid, const Util::StringAtom& eid, uint actns) :
    layoutId(lid),
    elementId(eid),
    actions(actns)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline bool
SUIEvent::operator==(const SUIEvent& rhs) const
{
    return (this->layoutId == rhs.layoutId) &&
           (this->elementId == rhs.elementId) &&
           ((this->actions & rhs.actions) != 0);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
SUIEvent::operator!=(const SUIEvent& rhs) const
{
    return *this != rhs;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom&
SUIEvent::GetLayoutId() const
{
    return this->layoutId;
}

//------------------------------------------------------------------------------
/**
*/
inline void
SUIEvent::Setup(const Util::StringAtom& lid, const Util::StringAtom& eid, uint actns)
{
    this->layoutId = lid;
    this->elementId = eid;
    this->actions = actns;   
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom&
SUIEvent::GetElementId() const
{
    return this->elementId;
}

//------------------------------------------------------------------------------
/**
*/
inline uint
SUIEvent::GetActions() const
{
    return this->actions;
}

} // namespace SUI
//------------------------------------------------------------------------------
