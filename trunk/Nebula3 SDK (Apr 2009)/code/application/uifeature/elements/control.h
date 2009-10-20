#ifndef UI_CONTROL_H
#define UI_CONTROL_H
//------------------------------------------------------------------------------
/**
    @class UI::Control

    Base class for interactive UI elements.

    (C) 2005 Radon Labs GmbH
*/
#include "uifeature/elements/element.h"

//------------------------------------------------------------------------------
namespace UI
{

class Control : public Element
{
	__DeclareClass(Control);
public:
    /// constructor
    Control();
    /// destructor
    virtual ~Control(); 
};

}; // namespace UI
//------------------------------------------------------------------------------
#endif
