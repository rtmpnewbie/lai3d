//------------------------------------------------------------------------------
//  uifeature/uieventhandler.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "uifeature/uieventhandler.h"
#include "uifeature/uievent.h"

namespace UI
{
__ImplementClass(UIEventHandler, 'UIEH', Core::RefCounted);

//------------------------------------------------------------------------------
/**
*/
UIEventHandler::UIEventHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
UIEventHandler::~UIEventHandler ()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    The HandleEvent() method is the front end for your UI logic implementation.
    All user interface events emitted by the current UI will be routed to
    this method.
*/
void
UIEventHandler::HandleEvent(const Ptr<UIEvent>& e)
{
    // empty (implement in subclass)
}

}; // namespace UI