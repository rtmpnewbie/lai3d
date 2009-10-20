//------------------------------------------------------------------------------
//  uifeature/uievent.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "uifeature/uievent.h"

namespace UI
{
__ImplementClass(UIEvent, 'UIEV', Messaging::Message);
__ImplementMsgId(UIEvent);

//------------------------------------------------------------------------------
/**
*/
UIEvent::UIEvent() :	eventType(Input::InputEvent::InvalidType),
						mouseButton(Input::MouseButton::InvalidMouseButton),
						key(Input::Key::InvalidKey)
{
    // empty
}

};
