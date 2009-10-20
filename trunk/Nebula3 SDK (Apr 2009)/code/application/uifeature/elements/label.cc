//------------------------------------------------------------------------------
//  uifeature/elements/label.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "uifeature/elements/label.h"
#include "uifeature/elements/window.h"
#include "uifeature/uimanager.h"

namespace UI
{
__ImplementClass(Label, 'UILA', Element);

//------------------------------------------------------------------------------
/**
*/
Label::Label() 
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Label::~Label()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This method is called when the element has been attached to a parent.
*/
void
Label::OnCreate()
{
    // call parent class
    Element::OnCreate();
}
}; // namespace UI
